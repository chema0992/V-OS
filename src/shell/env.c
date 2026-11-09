#include "env.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *environ_local[ENV_MAX + 1];
static int env_count;

static int find_index(const char *key) {
    size_t klen = strlen(key);
    for (int i = 0; i < env_count; i++) {
        if (strncmp(environ_local[i], key, klen) == 0 &&
            environ_local[i][klen] == '=')
            return i;
    }
    return -1;
}

void env_init(void) {
    env_count = 0;
    env_set("PATH", "/bin");
    env_set("HOME", "/");
    env_set("USER", "root");
    env_set("SHELL", "/bin/vsh");
}

const char *env_get(const char *key) {
    int i = find_index(key);
    if (i < 0) return NULL;
    const char *eq = strchr(environ_local[i], '=');
    return eq ? eq + 1 : NULL;
}

int env_set(const char *key, const char *value) {
    char buf[256];
    snprintf(buf, sizeof(buf), "%s=%s", key, value);

    int i = find_index(key);
    if (i >= 0) {
        free(environ_local[i]);
        environ_local[i] = strdup(buf);
        return environ_local[i] ? 0 : -1;
    }
    if (env_count >= ENV_MAX) return -1;
    environ_local[env_count] = strdup(buf);
    if (!environ_local[env_count]) return -1;
    env_count++;
    environ_local[env_count] = NULL;
    return 0;
}

char **env_get_envp(void) {
    environ_local[env_count] = NULL;
    return environ_local;
}

void env_print_all(void) {
    for (int i = 0; i < env_count; i++)
        printf("%s\n", environ_local[i]);
}
