#include "../commands.h"
#include "../env.h"
#include <stdio.h>
#include <string.h>

void cmd_export(int argc, char **argv) {
    if (argc < 2) {
        env_print_all();
        return;
    }
    for (int i = 1; i < argc; i++) {
        char *eq = strchr(argv[i], '=');
        if (!eq) {
            printf("export: invalid: %s (use KEY=VALUE)\n", argv[i]);
            continue;
        }
        *eq = '\0';
        if (env_set(argv[i], eq + 1) < 0)
            printf("export: failed: %s\n", argv[i]);
        *eq = '=';
    }
}
