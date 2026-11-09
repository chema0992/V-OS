#include "../commands.h"

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

void cmd_ls(int argc, char **argv) {
    const char *path = ".";   // 기본값: 현재 디렉토리

    // 인자가 있으면 그 경로 사용
    if (argc >= 2) {
        path = argv[1];
    }

    DIR *dir = opendir(path);
    if (!dir) {
        printf("ls: cannot open directory '%s'\n", path);
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // . 과 .. 은 기본적으로 숨김 (원하면 주석 해제)
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        // 디렉토리면 / 붙여서 표시
        char fullpath[512];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);

        struct stat st;
        if (stat(fullpath, &st) == 0 && S_ISDIR(st.st_mode)) {
            printf("%s/\n", entry->d_name);
        } else {
            printf("%s\n", entry->d_name);
        }
    }

    closedir(dir);
}
