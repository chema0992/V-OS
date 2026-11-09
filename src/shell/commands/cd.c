#include "../commands.h"
#include <stdio.h>
#include <unistd.h>

void cmd_cd(int argc, char **argv) {
    const char *path = "/";   // 인자 없으면 루트

    if (argc >= 2) {
        path = argv[1];
    }

    if (chdir(path) < 0) {
        printf("cd: no such directory: %s\n", path);
    }
}
