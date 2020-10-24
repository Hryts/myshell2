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
    std::vector<std::string> parent_args;

    auto parent_behaviour = std::function<void(const std::vector<std::string> &p_a,
                                               std::vector<std::pair<int, int>> &pipes)>(
            [](const std::vector<std::string> &p_a,
               std::vector<std::pair<int, int>> &_pipes) {});

    while (true) {
        if (getcwd(cwd, sizeof(cwd)) == nullptr) {
            return 1;
        }
        strcat(cwd, " $ ");
        input = readline(cwd);
        add_history(input);


        // Preserving
//        int stdin_copy = dup(STDIN_FILENO);
//        int stdout_copy = dup(STDOUT_FILENO);
//        int stderr_copy = dup(STDERR_FILENO);


        parse_input(input, args, pipes, parent_behaviour, parent_args);
        if (!args.empty()) {
            for (int i = 0; i < args.size(); ++i) {
                status = launch(args[i], pipes, i);
                statuses.emplace_back(status);
            }
        }

        parent_behaviour(parent_args, pipes);


//        dup2(stdin_copy, STDIN_FILENO);
//        dup2(stdout_copy, STDOUT_FILENO);
//        dup2(stderr_copy, STDERR_FILENO);
//        close(stdin_copy);
//        close(stdin_copy);
//        close(stderr_copy);


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
        if (status != 0)
            for (int st : statuses) {
                if (waitpid(st, &status, 0) == -1)
                    exit(EXIT_FAILURE);
            }

        parent_behaviour = [](const std::vector<std::string> &p_a,
                              std::vector<std::pair<int, int>> &_pipes) {};

        merrno_num = status;
        args.clear();
        pipes.clear();
        statuses.clear();
        parent_args.clear();
        free(input);
    }
}
