//
// Created by heeve on 22.09.20.
//

#include <iostream>
#include "../headers/parsers.h"
#include <filesystem>
#include "../headers/wildcard.hpp"
#include <unistd.h>

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


void
parse_input(const char *inp, std::vector<std::vector<std::string>> &args, std::vector<std::pair<int, int>> &pipes) {
    std::string input(inp);
    input = input.substr(0, input.find('#'));
    std::vector<std::string> temp;
    if (input.find("=$") != std::string::npos) {
        auto command_start = input.find('(') + 1;
        auto command_end = input.find(')');
        if (command_end - command_start <= 0){
            std::cerr << "User debil" << std::endl;
            exit(EXIT_FAILURE);
        }
        auto cmd = input.substr(command_start, command_end-command_start);
        std::string mexport_w = "mexport_wrapper";
        wildcard(cmd, temp);
        args.push_back(std::move(temp));
        temp.clear();
        temp.emplace_back(mexport_w);
        temp.emplace_back(input.substr(0, input.find("=$")));
        args.push_back(temp);
    }
    else {
        size_t initialPos = 0;
        size_t pos = input.find(' ');
        std::string to_put;
        while (pos != std::string::npos) {
            to_put = input.substr(initialPos, pos - initialPos);
            if (to_put == "|" && !temp.empty()) {
                args.push_back(std::move(temp));
                temp.clear();
            } else if (to_put.find_first_not_of(' ') != std::string::npos)
                wildcard(to_put, temp);
            initialPos = pos + 1;
            pos = input.find(' ', initialPos);
        }
        to_put = input.substr(initialPos, std::min(pos, input.size()) - initialPos + 1);

        if (to_put.find_first_not_of(' ') != std::string::npos)
            wildcard(to_put, temp);
    }
    // create pipes


    if (!temp.empty()) {
        args.push_back(std::move(temp));
        int pfd[2];
        for (int i = 0; i < args.size() - 1; ++i) {
            if (pipe(pfd) == -1) {
                std::cerr << "Failed to pipe" << std::endl;
                exit(EXIT_FAILURE);
            }
            pipes.emplace_back(pfd[0], pfd[1]);
        }
    }
}
