#include "../commands.h"
#include <stdio.h>
#include <unistd.h>

void cmd_pwd(int argc, char **argv) {
    (void)argc;
    (void)argv;

    char buf[512];
    if (getcwd(buf, sizeof(buf)) == NULL) {
        printf("pwd: failed\n");
        return;
    }
    printf("%s\n", buf);
}
