//
// Created by heeve on 22.09.20.
//

#ifndef MYSHELL_UTILS_H
#define MYSHELL_UTILS_H

#include <string>
#include <vector>
#include "builtins.h"

int launch(std::vector<std::string> &args, const std::vector<std::pair<int, int>> &pipes, int indx, bool isBackProc);
void init_var_by_pipe(const std::string &p_a, std::vector<std::pair<int, int>> &_pipes);
void setup_path();


#endif //MYSHELL_UTILS_H
