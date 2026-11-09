#include "../commands.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

void cmd_id(int argc, char **argv) {
    (void)argc;
    (void)argv;

    uid_t uid  = getuid();
    uid_t euid = geteuid();
    gid_t gid  = getgid();
    gid_t egid = getegid();

    printf("uid=%d gid=%d", (int)uid, (int)gid);

    if (uid != euid || gid != egid) {
        printf(" euid=%d egid=%d", (int)euid, (int)egid);
    }

    printf("\n");
}
