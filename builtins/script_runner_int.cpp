//
// Created by heeve on 29.09.20.
//

#include <fstream>
#include <vector>
#include <iostream>
#include "../headers/parsers.h"
#include "../headers/utils.h"
#include <sys/wait.h>

//int script_runner(const std::vector<std::string> &args, bool bHelp) {
//    if (bHelp) {
//        std::cout << "myshell: . usage: . <scriptname.msh> [-h|--help]\n";
//        return 0;
//    }
//    if (args.size() > 2) {
//        std::cout << "myshell: . : Too many arguments\n";
//        return 1;
//    } else if (args.size() < 2) {
//        std::cout << "myshell: . : Argument required\n";
//        return 2;
//    }
//
//    std::ifstream script(args[1]);
//    if (!script.is_open()) {
//        script.close();
//        return -1;
//    }
//    std::string line;
//    std::vector<std::vector<std::string>> arg_sh;
//    std::vector<std::pair<int, int>> pipes;
//    int status;
//    std::vector<int> statuses;
//
//    while (getline(script, line)) {
//        if (line.empty() || line[0] == '#')
//            continue;
//
//        void (*parent_behaviour)(std::vector<std::string>, std::vector<std::pair<int, int>> &pipes);
//        std::vector<std::string> parent_args;
//
//        // Preserving
//        int stdin_copy = dup(STDIN_FILENO);
//        int stdout_copy = dup(STDOUT_FILENO);
//        int stderr_copy = dup(STDERR_FILENO);
//
//
//        parse_input(line.c_str(), args, pipes, parent_behaviour, parent_args);
//        if (!arg_sh.empty()) {
//            for (int i = 0; i < args.size(); ++i) {
//                status = launch(arg_sh[i], pipes, i);
//                statuses.emplace_back(status);
//            }
//        }
//        for (auto & pipe : pipes) {
//            if (close(pipe.first) == -1)
//                exit(EXIT_FAILURE);
//            if (close(pipe.second) == -1)
//                exit(EXIT_FAILURE);
//        }
//
//
//        if (status != 0)
//            for (int st : statuses) {
//                if (waitpid(st, &status, 0) == -1)
//                    exit(EXIT_FAILURE);
//            }
//
//        arg_sh.clear();
//        pipes.clear();
//        statuses.clear();
//    }
//    script.close();
//    return status;
//}