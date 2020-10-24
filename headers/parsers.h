//
// Created by heeve on 22.09.20.
//

#ifndef MYSHELL_PARSERS_H
#define MYSHELL_PARSERS_H

#include <string>
#include <vector>
#include <ctype.h>
#include <functional>

//void parse_input(const char *inp,
//        std::vector<std::vector<std::string>> &args,
//        std::vector<std::pair<int, int>> &pipes,
//        void (*parent_behaviour)(const std::vector<std::string>&, std::vector<std::pair<int, int>>&),
//        std::vector<std::string>& p_args);


void parse_input(const char *inp, std::vector<std::vector<std::string>> &args, std::vector<std::pair<int, int>> &pipes,
                 std::function<void(const std::vector<std::string> &p_a,
                                    std::vector<std::pair<int, int>> &pipes)>& parent_function,
                 std::vector<std::string> &p_args);
#endif //MYSHELL_PARSERS_H
