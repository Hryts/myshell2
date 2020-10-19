//
// Created by heeve on 08.09.20.
//

#include "mycat.h"
#include <cerrno>

void write_literal(int fd, const std::string &str) {
    write(fd, str.c_str(), str.size());
}


static void convert_to_hex(std::vector<char> &buffer, int n) {
    std::vector<char> newBuffer;
    newBuffer.reserve(BUF_SIZE);
    char hx[5];
    for (int i = 0; i < n; ++i) {
        if (!isspace(buffer[i]) && !isprint(buffer[i])) {
            sprintf(hx, "\\x%2X", HEX_COMPLEMENT + buffer[i]);
            newBuffer.insert(newBuffer.end(), hx, hx + 4);
            continue;
        }
        newBuffer.push_back(buffer[i]);
    }
    buffer.swap(newBuffer);
}


int mycat(std::vector<int> &descriptors, bool bHex) {
    std::vector<char> buffer(BUF_SIZE);
    int bytes_read;
    for (auto &inputFd: descriptors) {
        while (!((bytes_read = read(inputFd, &buffer[0], buffer.size())) <= 0 && errno != EINTR)) {
            int bytes_written = 0;
            int wr;
            if (bHex) {
                convert_to_hex(buffer, bytes_read);
                bytes_read = buffer.size();
            }
            while (bytes_written < bytes_read) {
                if ((wr = write(STDOUT_FILENO, &buffer[bytes_written], bytes_read - bytes_written)) < 1 &&
                    errno != EINTR) {
                    write_literal(STDERR_FILENO, "Error while writing to STDOUT\n");
                    return -3;
                }
                bytes_written += wr;
            }
        }

        if (bytes_read == -1) {
            write_literal(STDERR_FILENO, "Error while reading input file\n");
            return -4;
        }
    }


    return 0;
}
