#include <cstring>
#include "../mycat/mycat.h"


int main(int argc, char **argv) {
    std::vector<int> descriptors;
    int inputFd, result = 0;
    bool bHelp = false, bHex = false;

    for (int i = 1; i < argc; ++i) {
        if ((strcmp(argv[i], "--help") == 0) || (strcmp(argv[i], "-h") == 0)) {
            bHelp = true;
            write_literal(STDOUT_FILENO, "Usage: ./mycat [-h|--help] [-A] <file1> <file2> ... <fileN>\n");
            break;
        }
        if (strcmp(argv[i], "-A") == 0) {
            bHex = true;
            continue;
        }
        if ((inputFd = open(argv[i], O_RDONLY)) == -1) {
            write_literal(STDERR_FILENO, "Error while opening input file\n");
            return -1;
        }
        descriptors.push_back(inputFd);
    }

    if (!bHelp)
        result = mycat(descriptors, bHex);


    for (auto &fd: descriptors) {
        if (close(fd) != 0) {
            write_literal(STDERR_FILENO, "Error while closing input file\n");
            return -2;
        }
    }


    return result;
}
