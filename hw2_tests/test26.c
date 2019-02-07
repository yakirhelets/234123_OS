#include "test.h"

int main() {
    int retval;
    restart_cond();
    /* stress tests */
    int i = 0;
    for (i = 0 ; i < 100 ; i++) {
        TASSERT(retval = start_lottery_scheduler() == 0 ,
        "Stress tests: start_lottery_scheduler failed");
        TASSERT(retval = start_orig_scheduler() == 0 ,
        "Stress tests: start_orig_scheduler failed");
    }

    start_lottery_scheduler();
    for (i = 0 ; i < 100000 ; i++) {
        int pid = fork();
        if (pid == 0) {
            nice(20);
            sched_yield();
            exit(0);
        }
    }
    int status = 0;
    while(wait(&status) != -1) {
        status = WEXITSTATUS(status);
        TASSERT(status == 0, "error in a forked process (status=%d)", status);
    }
    start_orig_scheduler();
    printf("test 26: [SUCCESS]\n");
    return 0;
}
