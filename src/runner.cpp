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
    int status;
    while (true) {
        if (getcwd(cwd, sizeof(cwd)) == nullptr) {
            return 1;
        }
        strcat(cwd, " $ ");
        input = readline(cwd);
        add_history(input);

        parse_input(input, args, pipes);
        if (!args.empty()) {
            for (int i = 0; i < args.size(); ++i) {
                status = launch(args[i], pipes[i], pipes[i + 1]);
            }
        }
        for (int i = 1; i < pipes.size() - 1; ++i)
            if (close(pipes[i].first) == -1 || close(pipes[i].second == -1))
                exit(EXIT_FAILURE);

        for (int i = 0; i < args.size(); ++i)
            if (status != 0 && wait(&status) == -1)
                exit(EXIT_FAILURE);

        merrno_num = status;
        args.clear();
        pipes.clear();
        free(input);
    }
}
