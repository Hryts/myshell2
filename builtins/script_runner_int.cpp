//
// Created by heeve on 29.09.20.
//

#include <fstream>
#include <vector>
#include <iostream>
#include "../headers/parsers.h"
#include "../headers/utils.h"
#include <sys/wait.h>

int script_runner(const std::vector<std::string> &args, bool bHelp) {
    if (bHelp) {
        std::cout << "myshell: . usage: . <scriptname.msh> [-h|--help]\n";
        return 0;
    }
    if (args.size() > 2) {
        std::cout << "myshell: . : Too many arguments\n";
        return 1;
    } else if (args.size() < 2) {
        std::cout << "myshell: . : Argument required\n";
        return 2;
    }

    std::ifstream script(args[1]);
    if (!script.is_open()) {
        script.close();
        return -1;
    }
    std::string line;
    std::vector<std::vector<std::string>> arg_sh;
    std::vector<std::pair<int, int>> pipes;
    int status;

    while (getline(script, line)) {
        if (line.empty() || line[0] == '#')
            continue;
        parse_input(line.c_str(), arg_sh, pipes);
        if (!arg_sh.empty()) {
            for (int i = 0; i < arg_sh.size(); ++i) {
                launch(arg_sh[i], pipes[i], pipes[i + 1]);
            }
        }
        for (int i = 1; i < pipes.size() - 1; ++i)
            if (close(pipes[i].first) == -1 || close(pipes[i].second == -1))
                exit(EXIT_FAILURE);

        for (int i = 0; i < args.size(); ++i)
            if (wait(&status) == -1)
                exit(EXIT_FAILURE);

        arg_sh.clear();
        pipes.clear();
    }
    script.close();
    return status;
}