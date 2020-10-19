#include <string>
#include <unistd.h>
#include <vector>
#include "../headers/parsers.h"
#include "../headers/utils.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <boost/filesystem.hpp>

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
    std::vector<std::string> args;

    while (true) {
        if (getcwd(cwd, sizeof(cwd)) == nullptr) {
            return 1;
        }
        strcat(cwd, " $ ");
        input = readline(cwd);
        add_history(input);

        parse_input(input, args);
        if (!args.empty())
            launch(args);
        args.clear();
        free(input);
    }
}
