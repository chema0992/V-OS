#include "../commands.h"
#include <stdio.h>

void cmd_help(int argc, char **argv) {
    (void)argc; (void)argv;   // 안 쓰면 경고 방지
    list_commands();
}
