#ifndef COMMANDS_H
#define COMMANDS_H

typedef struct {
    const char *name;                           // 명령어 이름
    void (*func)(int argc, char **argv);        // 실행 함수
    const char *desc;                           // help에 나올 설명
} command_t;

// 테이블 (commands.c에서 정의)
extern const command_t commands[];

// 유틸 함수
const command_t *find_command(const char *name);
void list_commands(void);                       // help에서 사용

#endif
