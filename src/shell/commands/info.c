#include "../commands.h"
#include "../../../ver.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <sys/utsname.h>
#include <pwd.h>

// 코드 내부에 고정된 OS 및 쉘 정보
#define OS_NAME    "V-OS (ㅸ-OS)"
#define INIT_SHELL "C Shell"

void cmd_info(int argc, char **argv) {
    // 사용하지 않는 인자 경고 방지
    (void)argc; 
    (void)argv;

    struct sysinfo s_info;
    struct utsname u_info;
    char hostname[256];
    struct passwd *pw;
    char *tty_name;

    // 1. 리눅스 커널 시스템 콜 및 POSIX 함수로 정보 수집 (실패 시 안전장치 포함)
    if (sysinfo(&s_info) < 0) return;
    if (uname(&u_info) < 0) return;
    if (gethostname(hostname, sizeof(hostname)) < 0) {
        snprintf(hostname, sizeof(hostname), "unknown");
    }
    
    uid_t uid = getuid();
    pw = getpwuid(uid);
    tty_name = ttyname(STDIN_FILENO); // 표준입력(0번) 터미널 식별

    // 2. 가동 시간(Uptime) 계산
    long uptime_secs = s_info.uptime;
    long days = uptime_secs / 86400;
    long hours = (uptime_secs % 86400) / 3600;
    long mins = (uptime_secs % 3600) / 60;

    // 3. 메모리 계산 (단위: MB, mem_unit 배수 고려)
    unsigned long total_mem = (s_info.totalram * s_info.mem_unit) / (1024 * 1024);
    unsigned long free_mem = (s_info.freeram * s_info.mem_unit) / (1024 * 1024);
    unsigned long used_mem = total_mem - free_mem;
    double mem_percent = total_mem > 0 ? ((double)used_mem / total_mem) * 100.0 : 0.0;

    // 4. 요청하신 레이아웃 포맷 출력
    printf("==================================================\n");
    printf("[ OS Information ]\n");
    printf("  OS Name      : %s\n", OS_NAME);
    printf("  OS Version   : %s\n", OS_VERSION);
    printf("  Built on     : %s %s\n", __DATE__, __TIME__);
    printf("\n");
    
    printf("[ Kernel & Hardware ]\n");
    printf("  Kernel       : %s %s\n", u_info.sysname, u_info.release); // 호스트 커널 정보 반영
    printf("  Arch         : %s\n", u_info.machine);                  // x86_64, aarch64 등 자동 감지
    printf("  Uptime       : %ld days, %ld hours, %ld mins\n", days, hours, mins);
    printf("  Memory       : %lu MB / %lu MB (%.1f%%)\n", used_mem, total_mem, mem_percent);
    printf("\n");
    
    printf("[ Shell Session ]\n");
    printf("  Init / Shell : %s\n", INIT_SHELL);
    printf("  User         : %s (UID: %d)\n", pw ? pw->pw_name : "unknown", uid);
    printf("  Hostname     : %s\n", hostname);
    printf("  TTY          : %s\n", tty_name ? tty_name : "N/A");
    printf("==================================================\n");
}

