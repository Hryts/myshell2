//
// Created by heeve on 22.09.20.
//

#include "../headers/builtins.h"
#include <unistd.h>
#include <iostream>
#include <vector>

int mcd(const std::vector<std::string> &args, bool bHelp) {
    if (bHelp){
        std::cout << "myshell: mcd usage: mcd <path> [-h|--help]\n";
        return 0;
    }
    if (args.size() > 2) {
        std::cout << "myshell: mcd: Too many arguments\n";
        return 1;
    } else if (args.size() < 2) {
        std::cout << "myshell: mcd: Argument required\n";
        return 2;
    }
    int status;
    if ((status = chdir(args[1].c_str())) != 0){
        std::cout << "myshell: mcd: No such file or directory\n";
    }
    return status;
}

