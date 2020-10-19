//
// Created by heeve on 29.09.20.
//

#include "../headers/builtins.h"
#include <unistd.h>
#include <iostream>
#include <vector>

#define MAX_PATH 128

int mpwd(const std::vector<std::string> &args, bool bHelp) {
    if (bHelp) {
        std::cout << "myshell: mpwd usage: mpwd [-h|--help]\n";
        return 0;
    }
    if (args.size() > 1) {
        std::cout << "myshell: mpwd: Too many arguments\n";
        return 1;
    }
    char cwd[MAX_PATH];
    if (getcwd(cwd, sizeof(cwd)) == nullptr) {
        return 2;
    }
    std::cout << cwd << std::endl;

    return 0;
}

