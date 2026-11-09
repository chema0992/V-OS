#include "../commands.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/reboot.h>   // RB_AUTOBOOT, RB_POWER_OFF 등

void cmd_reboot(int argc, char **argv) {
    (void)argc;
    (void)argv;

    printf("Syncing disks...\n");
    sync();                 // 디스크 버퍼 내리기 (중요)

    printf("Rebooting...\n");
    fflush(stdout);

    // 실제 재부팅
    if (reboot(RB_AUTOBOOT) < 0) {
        // 실패했을 때만 여기 옴 (권한 부족 등)
        perror("reboot failed");
    }

    // reboot()가 성공하면 여기까지 오지 않음
}
