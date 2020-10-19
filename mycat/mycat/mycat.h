//
// Created by heeve on 08.09.20.
//

#ifndef MYCAT_MYCAT_H

#define MYCAT_MYCAT_H
#define BUF_SIZE 4096
#define HEX_COMPLEMENT 256

#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <string>


int mycat(std::vector<int> &descriptors, bool bHex);

void write_literal(int fd, const std::string &str);


#endif //MYCAT_MYCAT_H
