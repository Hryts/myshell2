//
// Created by heeve on 29.09.20.
//

#include "../headers/builtins.h"
#include <iostream>
#include <vector>
#include <cstdlib>


int mecho(const std::vector<std::string> &args, bool bHelp) {
    if (bHelp) {
        std::cout
                << "myshell: mecho usage: mecho [-h|--help] [text|$<var_name>] "
                   "[text|$<var_name>]  [text|$<var_name>] ...\n";
        return 0;
    }
    std::string output;
    for (auto s = args.begin() + 1; s != args.end(); ++s){
        if (s->find('$') != std::string::npos){
            char *env = getenv(s->substr(s->find('$') + 1, s->size()).data());
            output += s->substr(0, s->find('$'));
            if (env){
                output += env;
            }
        }
        else{
            output += *s;
        }
        output += ' ';
    }
    std::cout << output << std::endl;
    return 0;
}

