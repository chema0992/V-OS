#include "commands.h"
#include <string.h>
#include <stdio.h>

// 각 명령어 함수 선언 (commands/*.c에 구현되어 있음)
void cmd_help(int argc, char **argv);
void cmd_clear(int argc, char **argv);
void cmd_info(int argc, char **argv);
void cmd_reboot(int argc, char **argv);
void cmd_ls(int argc, char **argv);
void cmd_echo(int argc, char **argv);
void cmd_uname(int argc, char **argv);
void cmd_cat(int argc, char **argv);
void cmd_cd(int argc, char **argv);
void cmd_pwd(int argc, char **argv);
void cmd_id(int argc, char **argv);
void cmd_env(int argc, char **argv);
void cmd_export(int argc, char **argv);

// ★ 명령어 테이블 (여기에만 등록하면 끝)
const command_t commands[] = {
    { "help",   cmd_help,   "Show available commands" },
    { "clear",  cmd_clear,  "Clear the screen" },
    { "info",   cmd_info,   "Show system information" },
    { "reboot", cmd_reboot, "Reboot the system" },
    { "exit",   cmd_reboot, "Exit / shutdown" },   // exit도 같은 함수로
    { "ls",     cmd_ls,     "List directory contents" },
    { "echo",  cmd_echo,  "Print arguments" },
    { "uname", cmd_uname, "Show system name" },
    { "cat",   cmd_cat,   "Show file contents" },
    { "cd",  cmd_cd,  "Change directory" },
    { "pwd", cmd_pwd, "Print working directory" },
    { "id", cmd_id, "Show user and group ids" },
    { "env",    cmd_env,    "Show environment" },
    { "export", cmd_export, "Set environment variable" },
    { NULL, NULL, NULL }   // 끝 표시 (중요)
};

const command_t *find_command(const char *name) {
    for (int i = 0; commands[i].name != NULL; i++) {
        if (strcmp(commands[i].name, name) == 0) {
            return &commands[i];
        }
    }
    return NULL;
}

void list_commands(void) {
    printf("Available commands:\n");
    for (int i = 0; commands[i].name != NULL; i++) {
        printf("  %-10s - %s\n", commands[i].name, commands[i].desc);
    }
}
