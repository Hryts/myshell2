//
// Created by heeve on 29.09.20.
//

#include "../headers/builtins.h"
#include <iostream>
#include <vector>


int mexit(const std::vector<std::string> &args, bool bHelp) {
    if (bHelp) {
        std::cout << "myshell: mexit usage: mexit [exit code] [-h|--help]\n";
        return 0;
    }
    if (args.size() > 2) {
        std::cout << "myshell: mexit: Too many arguments\n";
        return 1;
    }
    if (args.size() == 2){
        exit(std::stoi(args[1]));
    }
    exit(0);
}

