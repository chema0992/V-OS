#include "shell.h"
#include "commands.h"
#include "env.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#define INPUT_SIZE 256
#define MAX_ARGS   64
#define PATH_SIZE  512

static void print_prompt(void) {
    char cwd[PATH_SIZE];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        strcpy(cwd, "/");
    }

    const char *user = env_get("USER");
    if (!user || !user[0])
        user = "root";

    char suffix = (geteuid() == 0) ? '#' : '$';
    printf("%s@V-OS:%s%c ", user, cwd, suffix);
    fflush(stdout);
}

static int parse_input(char *input, char *argv[]) {
    int argc = 0;
    char *p = input;

    while (*p && argc < MAX_ARGS - 1) {
        while (*p == ' ' || *p == '\t')
            p++;
        if (*p == '\0')
            break;

        argv[argc++] = p;

        while (*p && *p != ' ' && *p != '\t')
            p++;
        if (*p)
            *p++ = '\0';
    }
    argv[argc] = NULL;
    return argc;
}

/* PATH=:/bin:/usr/bin 형태를 순회하며 execve 시도 */
static void exec_with_path(char *argv[], char **envp) {
    if (strchr(argv[0], '/')) {
        execve(argv[0], argv, envp);
        return;
    }

    const char *path_env = env_get("PATH");
    if (!path_env || !path_env[0])
        path_env = "/bin";

    char pathbuf[PATH_SIZE];
    char tmp[PATH_SIZE];
    strncpy(tmp, path_env, sizeof(tmp) - 1);
    tmp[sizeof(tmp) - 1] = '\0';

    char *save = NULL;
    for (char *dir = strtok_r(tmp, ":", &save);
         dir != NULL;
         dir = strtok_r(NULL, ":", &save)) {

        snprintf(pathbuf, sizeof(pathbuf), "%s/%s", dir, argv[0]);
        execve(pathbuf, argv, envp);
        /* 실패하면 다음 경로 */
    }
}

static void execute_external(int argc, char *argv[]) {
    (void)argc;

    pid_t pid = fork();
    if (pid < 0) {
        perror("vsh: fork");
        return;
    }

    if (pid == 0) {
        char **envp = env_get_envp();
        exec_with_path(argv, envp);

        fprintf(stderr, "vsh: command not found: %s\n", argv[0]);
        _exit(127);
    }

    int status;
    while (waitpid(pid, &status, 0) < 0) {
        if (errno == EINTR)
            continue;
        perror("vsh: waitpid");
        break;
    }
}

void shell_loop(void) {
    env_init();

    printf("\033[?25h");
    printf("\033[?12h");
    printf("\033[2J\033[H");
    printf("==========================================\n");
    printf("       Welcome to V-OS (ㅸ-OS) v0.1\n");
    printf("==========================================\n\n");

    char input[INPUT_SIZE];
    char *argv[MAX_ARGS];

    while (1) {
        print_prompt();

        if (fgets(input, sizeof(input), stdin) == NULL) {
            if (feof(stdin)) {
                printf("\n");
                break;
            }
            clearerr(stdin);
            continue;
        }

        input[strcspn(input, "\n")] = '\0';

        int argc = parse_input(input, argv);
        if (argc == 0)
            continue;

        const command_t *cmd = find_command(argv[0]);
        if (cmd != NULL) {
            cmd->func(argc, argv);
        } else {
            execute_external(argc, argv);
        }
    }
}
