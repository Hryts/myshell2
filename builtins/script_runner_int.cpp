//
// Created by heeve on 29.09.20.
//

#include <fstream>
#include <vector>
#include <iostream>
#include "../headers/parsers.h"
#include "../headers/utils.h"

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
    std::vector<std::string> arg_sh;
    int status;

    while (getline(script, line)) {
        if (line.empty() || line[0] == '#')
            continue;
        parse_input(line.c_str(), arg_sh);
        if (!arg_sh.empty())
            status = launch(arg_sh);
        arg_sh.clear();
    }
    script.close();
    return status;
}