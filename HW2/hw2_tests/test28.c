#include "test.h"

int main() {
   restart_cond();
    struct sched_param param = {30};
    set_max_tickets(168);

    sched_setscheduler(getpid(), SCHED_FIFO, &param);
    int pid = fork();
    if (pid == 0) {
        exit(0);
    }
    enable_logging(100); // The father is FIFO, hence no other process will run, scheduler shouldn't be called.
    start_lottery_scheduler();
    /* Now only one of the father/son (probably father) should run, but when the quantum is ending
    the other should run with probabilty of 0.5 for each context switch*/
    busy_wait_by_sec(10);
    disable_logging();
    int status = 0;
    wait(&status);
    status = WEXITSTATUS(status);
    TASSERT(status == 0, "error in a forked process (status=%d)", status);

    cs_log logs[100];
    int n = get_logger_records(logs);
    int i;
    int fh = 0;
    int sn = 0;
    int fpid = getpid();
    for (i = 0 ; i < n ; i++) {
        if (logs[i].prev == fpid && logs[i].next == pid)
            fh= 1;
        if (logs[i].prev == pid && logs[i].next == fpid)
            sn = 1;
    }
    TASSERT(fh == 1 || sn == 1, "FIFO behavior checks: Original FIFO process doesn't has a quantum");
    printf("test 28: [SUCCESS]\n");
    return 0;
}
