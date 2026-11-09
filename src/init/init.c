#include "../shell/reset.h"   /* src/shell/reset.h 경로에 맞게 */
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main(void) {
    reset_system();   /* 마운트 + 스플래시 + hide */

    for (;;) {
        pid_t pid = fork();
        if (pid < 0) {
            sleep(1);
            continue;
        }
        if (pid == 0) {
            execl("/bin/vsh", "vsh", (char *)NULL);
            _exit(1);
        }
        int status;
        while (wait(&status) != pid)
            ;
        printf("\n[init] shell exited, restarting...\n");
        sleep(1);
    }
    return 0;
}
