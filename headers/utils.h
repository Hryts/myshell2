//
// Created by heeve on 22.09.20.
//

#ifndef MYSHELL_UTILS_H
#define MYSHELL_UTILS_H

#include <string>
#include <vector>
#include "builtins.h"

int launch(std::vector<std::string> &args, const std::pair<int, int> &left, const std::pair<int, int> &right);

void setup_path();


#endif //MYSHELL_UTILS_H
