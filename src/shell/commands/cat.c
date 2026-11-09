#include "../commands.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

void cmd_cat(int argc, char **argv) {
    if (argc < 2) {
        printf("usage: cat <file>\n");
        return;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        printf("cat: cannot open '%s'\n", argv[1]);
        return;
    }

    char buf[256];
    ssize_t n;
    while ((n = read(fd, buf, sizeof(buf))) > 0) {
        write(1, buf, (size_t)n);
    }
    close(fd);
}
