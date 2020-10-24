#include <string>
#include <unistd.h>
#include <vector>
#include "../headers/parsers.h"
#include "../headers/utils.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <boost/filesystem.hpp>
#include <sys/wait.h>
#include <iostream>

#define MAX_PATH 128
extern BuiltIns builtIns;

int __thread merrno_num;

int main(int argc, char **argv) {
    merrno_num = 0;

    setup_path();

    if (argc > 1 && boost::filesystem::extension(std::string(argv[1])) == ".msh")
        return builtIns.get_func(".")(std::vector<std::string>(argv, argv + argc), false);
    char *input;
    char cwd[MAX_PATH];
    std::vector<std::vector<std::string>> args;
    std::vector<std::pair<int, int>> pipes;
    int status = 0;
    std::vector<int> statuses;
    std::string parent_args;
    std::pair<bool, bool> crutch;

    while (true) {
        if (getcwd(cwd, sizeof(cwd)) == nullptr) {
            return 1;
        }
        strcat(cwd, " $ ");
        input = readline(cwd);
        add_history(input);


        crutch = parse_input(input, args, pipes, parent_args);

        if (!args.empty()) {
            for (int i = 0; i < args.size(); ++i) {
                status = launch(args[i], pipes, i, i == args.size() && crutch.second);
                statuses.emplace_back(status);
            }
        }

        if (crutch.first) {
            init_var_by_pipe(parent_args, pipes);
        }

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

        if (crutch.second) {
            signal(SIGCHLD, SIG_IGN);
        } else if (status != 0) {
            for (int st : statuses) {
                if (waitpid(st, &status, 0) == -1)
                    exit(EXIT_FAILURE);
            }
        }

        merrno_num = status;
        args.clear();
        pipes.clear();
        statuses.clear();
        parent_args.clear();
        free(input);
    }
}
