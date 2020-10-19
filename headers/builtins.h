//
// Created by heeve on 22.09.20.
//

#ifndef MYSHELL_BUILTINS_H
#define MYSHELL_BUILTINS_H


#include <unistd.h>
#include <unordered_map>
#include <vector>

extern int __thread merrno_num;

typedef int (*myFunc)(const std::vector<std::string> &, bool);

typedef std::unordered_map<std::string, myFunc> myMap;

int mcd(const std::vector<std::string> &args, bool bHelp);

int script_runner(const std::vector<std::string> &args, bool);

int mexport(const std::vector<std::string> &args, bool);

int mpwd(const std::vector<std::string> &args, bool bHelp);

int mexit(const std::vector<std::string> &args, bool bHelp);

int mecho(const std::vector<std::string> &args, bool bHelp);

int merrno(const std::vector<std::string> &args, bool bHelp);

struct BuiltIns {


    BuiltIns();

    BuiltIns(BuiltIns &other) = delete;

    void operator=(const BuiltIns &) = delete;

    void insert(const std::string &command, myFunc);

    bool find(const std::string &command);

    myFunc get_func(const std::string &command);

private:
    myMap m;
};


#endif //MYSHELL_BUILTINS_H
