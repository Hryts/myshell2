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
    std::vector<int> statuses;

    std::vector<std::string> parent_args;

    auto parent_behaviour = std::function<void(const std::vector<std::string> &p_a,
                                               std::vector<std::pair<int, int>> &pipes)>(
            [](const std::vector<std::string> &p_a,
               std::vector<std::pair<int, int>> &_pipes) {});

    while (getline(script, line)) {
        if (line.empty() || line[0] == '#')
            continue;

        parse_input(line.c_str(), arg_sh, pipes, parent_behaviour, parent_args);
        if (!arg_sh.empty()) {
            for (int i = 0; i < arg_sh.size(); ++i) {
                status = launch(arg_sh[i], pipes, i);
                statuses.emplace_back(status);
            }
        }
        parent_behaviour(parent_args, pipes);
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
        arg_sh.clear();
        pipes.clear();
        statuses.clear();
        parent_args.clear();

    }
    script.close();
    return status;
}