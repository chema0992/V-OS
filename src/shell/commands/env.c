#include "../commands.h"
#include "../env.h"
#include <stdio.h>

void cmd_env(int argc, char **argv) {
    (void)argc;
    (void)argv;
    env_print_all();
}
