//
// Created by heeve on 22.09.20.
//

#include "../headers/utils.h"
#include <unistd.h>
#include <iostream>
#include <boost/filesystem.hpp>
#include <fcntl.h>

extern char **environ;
BuiltIns builtIns;




void mdup(size_t fd1, size_t fd2) {
    if (dup2(fd1, fd2) == -1) {
        std::cerr << "Failed: dup2 " << fd2 << std::endl;
        exit(EXIT_FAILURE);
    }
}

int mopen(const std::string &path, int flags) {
    int fd = open(path.c_str(), flags, 0600);
    if (fd < 0) {
        exit(EXIT_FAILURE);
    }
    return fd;
}

void redirect(std::vector<std::string> &inp) {
    auto arrow_sign = inp.begin() - 1;
    auto predicate = [](const std::string &str) {
        return str.find('>') != std::string::npos || str.find('<') != std::string::npos;
    };

    while (arrow_sign != inp.end()) {
        arrow_sign = std::find_if(arrow_sign + 1, inp.end(), predicate);
        if (arrow_sign == inp.end())
            return;
        auto arrow_ind = std::distance(inp.begin(), arrow_sign);
        auto red_command = inp[arrow_ind];
        int tmp;
        if (red_command == ">") {
            auto open_fd = mopen(inp[arrow_ind + 1], O_WRONLY | O_CREAT | O_TRUNC);
            mdup(open_fd, STDOUT_FILENO);
            close(open_fd);
            inp.erase(arrow_sign + 1);
            inp.erase(arrow_sign--);
        } else if (red_command == "<") {
            auto open_fd = mopen(inp[arrow_ind + 1], O_RDONLY);
            mdup(open_fd, STDIN_FILENO);
            close(open_fd);
            inp.erase(arrow_sign + 1);
            inp.erase(arrow_sign--);
        } else if (red_command == "&>") {
            auto open_fd = mopen(inp[arrow_ind + 1], O_WRONLY | O_CREAT | O_TRUNC);
            mdup(open_fd, STDOUT_FILENO);
            mdup(open_fd, STDERR_FILENO);
            close(open_fd);
            inp.erase(arrow_sign + 1);
            inp.erase(arrow_sign--);
        } else if ((tmp = red_command.find(">&") != std::string::npos)) {
            int fd1 = std::stoi(red_command.substr(tmp + 2, red_command.size()));  // after >& (until the end)
            int fd2 = std::stoi(red_command.substr(0, tmp));  // before >&
            mdup(fd1, fd2);
            inp.erase(arrow_sign--);
        } else if ((tmp = red_command.find(">") != std::string::npos)) {
            auto fd1 = mopen(inp[arrow_ind + 1], O_WRONLY | O_CREAT | O_TRUNC);
            int fd2 = std::stoi(red_command.substr(0, tmp));  // before >
            mdup(fd1, fd2);
            close(fd1);
            inp.erase(arrow_sign + 1);
            inp.erase(arrow_sign--);
        }
    }
}


static std::pair<bool, int> check_for_builtins(std::vector<std::string> &args) {
    redirect(args);
    myFunc cmd;
    int status;
    if ((cmd = builtIns.get_func(args[0])) != nullptr) {
        bool bHelp = false;
        for (int i = 1; i < args.size(); ++i) {
            if (args[i] == "--help" || args[i] == "-h") {
                bHelp = true;
            }
        }
        status = cmd(args, bHelp);
        merrno_num = status;
        return std::make_pair(true, status);
    }
    return std::make_pair(false, 0);
}

void init_var_by_pipe(const std::string &p_a, std::vector<std::pair<int, int>> &_pipes) {
    int stdin_copy = dup(_pipes[_pipes.size() - 1].first);

    for (auto &p: _pipes) {
        if ((close(p.first) == -1)) {
            std::cerr << "Closing pipe" << std::endl;
            exit(EXIT_FAILURE);
        }

        if ((close(p.second) == -1)) {
            std::cerr << "Closing pipe" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    _pipes.clear();

    std::string res = "=";
    char buffer[4096];
    int bytes_read;
    std::string read_pipe;
//    std::cout << "dad " << _pipes[_pipes.size() - 1].first << " " << _pipes[_pipes.size() - 1].second << std::endl;
    while (!((bytes_read = read(stdin_copy, &buffer[0], 4096)) <= 0 && errno != EINTR)) {
        read_pipe = buffer;
        res += read_pipe.substr(0, bytes_read - 1);
    }
    close(stdin_copy);
    res = p_a + res;

// A bit of crutches here
    std::vector<std::string> args;
    args.emplace_back("mexport");

    args.push_back(res);
    if (mexport(args, false))
        exit(EXIT_FAILURE);
}


//template <typename Function, typename... Args>
int launch(std::vector<std::string> &args, const std::vector<std::pair<int, int>> &pipes, int indx, bool isBackProc) {
    int status;
    bool builtin;

    // use built-in in current process only if no pipes are used
    if (pipes.empty()) {
        std::tie(builtin, status) = check_for_builtins(args);
        if (builtin) {
            merrno_num = status;
            return 0;
        }
    }

    pid_t pid = fork();

    if (pid == -1) {
        std::cerr << "Failed to fork()" << std::endl;
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        if (!(pipes.empty())) {
            if (indx != 0) {
                if (dup2(pipes[indx - 1].first, STDIN_FILENO) == -1) {
                    std::cerr << "Dup2 stdin" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
            if (indx != pipes.size()) {
                if (dup2(pipes[indx].second, STDOUT_FILENO) == -1) {
                    std::cerr << "Dup2 stdout" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }

            // close all pipes
            for (auto &p: pipes) {
                if ((close(p.first) == -1)) {
                    std::cerr << "Closing pipe" << std::endl;
                    exit(EXIT_FAILURE);
                }

                if ((close(p.second) == -1)) {
                    std::cerr << "Closing pipe" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
        }

        redirect(args);

        if (isBackProc)
            if (close(STDOUT_FILENO) == -1)
            {
                std::cerr << "Closing stdout error" << std::endl;
                exit(EXIT_FAILURE);
            }

        std::tie(builtin, status) = check_for_builtins(args);
        if (builtin) {
            exit(status);
        }


        if (boost::filesystem::extension(args[0]) == ".msh") {
            args.insert(args.begin(), ".");
            status = builtIns.get_func(".")(args, false);
            merrno_num = status;
            exit(status);
        }

        std::string victim = args[0];
        std::vector<const char *> arg_for_c;
        arg_for_c.reserve(args.size() + 1);

        for (const auto &s: args)
            arg_for_c.push_back(s.c_str());
        arg_for_c.push_back(nullptr);
        execvp(victim.c_str(), const_cast<char *const *>(arg_for_c.data()));

        std::cerr << victim << ": command not found " << std::endl;
        exit(errno);

    }
    return pid;
}

void setup_path() {
    char cwd[128];
    getcwd(cwd, sizeof(cwd));
    auto path_ptr = getenv("PATH");
    std::string path_str;
    if (path_ptr != nullptr)
        path_str = path_ptr;
    ((path_str += ":") += cwd) += "/utils";
    setenv("PATH", path_str.c_str(), 1);
}
