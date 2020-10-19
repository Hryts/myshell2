//
// Created by heeve on 29.09.20.
//

#include "../headers/builtins.h"
#include <iostream>
#include <vector>


int merrno(const std::vector<std::string> &args, bool bHelp) {
    if (bHelp) {
        std::cout << "myshell: merrno usage: merrno [-h|--help]\n";
        return 0;
    }
    if (args.size() > 1) {
        std::cout << "myshell: merrno: Too many arguments\n";
        return 1;
    }

    std::cout << merrno_num << std::endl;

    return 0;
}

