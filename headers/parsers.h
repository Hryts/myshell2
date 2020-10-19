//
// Created by heeve on 22.09.20.
//

#ifndef MYSHELL_PARSERS_H
#define MYSHELL_PARSERS_H

#include <string>
#include <vector>
#include <ctype.h>

void
parse_input(const char *inp, std::vector<std::vector<std::string>> &args, std::vector<std::pair<int, int>> &pipes);

#endif //MYSHELL_PARSERS_H
