//
// Created by heeve on 22.09.20.
//

#include "../headers/utils.h"
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <boost/filesystem.hpp>

extern char **environ;
BuiltIns builtIns;


static std::pair<bool, int> check_for_builtins(const std::vector<std::string> &args) {
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

int launch(std::vector<std::string> &args, const std::vector<std::pair<int, int>> &pipes, int indx) {
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

        // if pipes are used - set up descriptors and check if command is a built-in
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
            for (auto &p: pipes){
                if ((close(p.first) == -1)) {
                    std::cerr << "Closing pipe" << std::endl;
                    exit(EXIT_FAILURE);
                }

                if ((close(p.second) == -1)) {
                    std::cerr << "Closing pipe" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }

            std::tie(builtin, status) = check_for_builtins(args);
            if (builtin)
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