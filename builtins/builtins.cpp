//
// Created by heeve on 22.09.20.
//

#include "../headers/builtins.h"


BuiltIns::BuiltIns() {
    insert("mcd", mcd);
    insert("mpwd", mpwd);
    insert("merrno", merrno);
    insert("mexit", mexit);
    insert("mecho", mecho);
    insert(".", script_runner);
    insert("mexport", mexport);
}

void BuiltIns::insert(const std::string &command, myFunc f) {
    m.insert(std::pair<std::string, myFunc>(command, f));
}

bool BuiltIns::find(const std::string &command) {
    auto itr = m.find(command);
    return (itr != m.end());
}

myFunc BuiltIns::get_func(const std::string &command) {
    if (find(command))
        return m[command];
    return nullptr;
}
