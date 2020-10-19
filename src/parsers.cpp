//
// Created by heeve on 22.09.20.
//

#include <iostream>
#include "../headers/parsers.h"
#include <filesystem>
#include "../headers/wildcard.hpp"

namespace fs = std::filesystem;


void wildcard(std::string &path, std::vector<std::string> &args) {
    int added = 0;
    if (fs::exists(fs::path(path).parent_path())) {
        for (const auto &entry : fs::directory_iterator(fs::path(path).parent_path())) {
            if (stlplus::wildcard(path, entry.path())) {
                args.emplace_back(std::string(fs::absolute(entry.path())));
                ++added;
            }
        }
        // if no files were matched - add the original path for other code to handle it
        if (!added)
            args.emplace_back(path);
    } else { args.emplace_back(path); }
}


void parse_input(const char *inp, std::vector<std::string> &args) {
    std::string input(inp);
    input = input.substr(0, input.find('#'));
    size_t initialPos = 0;
    size_t pos = input.find(' ');
    std::string to_put;
    while (pos != std::string::npos) {
        to_put = input.substr(initialPos, pos - initialPos);
        if (to_put.find_first_not_of(' ') != std::string::npos)
            wildcard(to_put, args);
        initialPos = pos + 1;
        pos = input.find(' ', initialPos);
    }
    to_put = input.substr(initialPos, std::min(pos, input.size()) - initialPos + 1);

    if (to_put.find_first_not_of(' ') != std::string::npos)
        wildcard(to_put, args);

    for (auto& s: args){
        std::cout << s<< "\n";
    }
}