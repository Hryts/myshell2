//
// Created by heeve on 29.09.20.
//
#include <iostream>
#include "../headers/builtins.h"

int mexport(const std::vector<std::string> &args, bool bHelp) {

    if (bHelp){
        std::cout << "myshell: mexport usage: mexport <var_name=VAL> [-h|--help]\n";
        return 0;
    }
    if (args.size() > 2) {
        std::cout << "myshell: mexport: Too many arguments\n";
        return 1;
    } else if (args.size() < 2) {
        std::cout << "myshell: mexport: Argument required\n";
        return 2;
    }
    if (args[1].find('=') == std::string::npos){
        std::cout << "myshell: mexport usage: mexport <var_name=VAL> [-h|--help]\n";
        return 3;
    }
    std::string var = args[1].substr(0, args[1].find('='));
    std::string val = args[1].substr(args[1].find('=') + 1, args[1].size());

    int status = setenv(var.c_str(), val.c_str(), 1);
    return status;
}

int mexport_wrapper(const std::vector<std::string> &a_little_bit_needed_args, bool totally_unneeded_bHelp) {
    // bad bad !
    std::string res;

    std::string line;
    while (std::getline(std::cin, line)) {
        res += line + ' ';
    }
    std::cout << res << "\n";

    res = a_little_bit_needed_args[1] + '=' + res;
    std::cout << res << "\n";

    // A bit of crutches here
    std::vector<std::string> actually_needed_args;
    actually_needed_args.emplace_back("mexport");

    actually_needed_args.emplace_back(res);
    totally_unneeded_bHelp = false;
    return mexport(actually_needed_args, totally_unneeded_bHelp);
}
