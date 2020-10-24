//
// Created by heeve on 22.09.20.
//

#include <iostream>
#include "../headers/parsers.h"
#include <filesystem>
#include "../headers/wildcard.hpp"
#include <unistd.h>
#include <functional>
#include <sys/wait.h>
#include "../headers/builtins.h"

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


std::pair<bool, bool> parse_input(const char *inp,
                                  std::vector<std::vector<std::string>> &args,
                                  std::vector<std::pair<int, int>> &pipes,
                                  std::string &p_args) {
    std::string input(inp);
    input = input.substr(0, input.find('#'));
    std::vector<std::string> temp;
    std::string cmd;
    bool isInitVar = input.find("=$") != std::string::npos, isBackProc = false;
    if (isInitVar) {
        // Seek for command
        auto command_start = input.find("(") + 1;
        auto command_end = input.find(")");
        if (command_end - command_start <= 0) {
            std::cerr << "Invalid input" << std::endl;
            exit(EXIT_FAILURE);
        }
        auto var_name = input.substr(0, input.find("=$"));
        p_args = var_name;
        cmd = input.substr(command_start, command_end - command_start);

        // Create a pipe
        int pfd[2];
        if (pipe(pfd) == -1) {
            std::cerr << "Failed to pipe" << std::endl;
            exit(EXIT_FAILURE);
        }
        pipes.emplace_back(pfd[0], pfd[1]);
    }
    if ((!cmd.empty()))
        input = cmd;
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

    isBackProc = to_put == "&";

    if (to_put.find_first_not_of(' ') != std::string::npos)
        wildcard(to_put, temp);

    if (!temp.empty())
        args.push_back(std::move(temp));


    // create pipes
    int pfd[2];
    for (int i = 0; i < args.size() - 1; ++i) {
        if (pipe(pfd) == -1) {
            std::cerr << "Failed to pipe" << std::endl;
            exit(EXIT_FAILURE);
        }
        pipes.emplace_back(pfd[0], pfd[1]);
    }

    return std::make_pair(isInitVar, isBackProc);

}
