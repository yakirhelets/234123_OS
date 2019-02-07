#include "test.h"

int main() {
    restart_cond();

    int pid = fork();
    if (pid == 0) {
        enable_logging(1);
        sched_yield();
        disable_logging();
        cs_log logs[1];
        int n = get_logger_records(logs);
        TASSERT(n > 0, "Interactive processes behaviour: The logger didn't recored any record");
        int init_prio = logs[0].prev_priority;

        sleep(10);
        enable_logging(1);
        sched_yield();
        disable_logging();
        n = get_logger_records(logs);
        TASSERT(n > 0, "Interactive processes behaviour: The logger didn't recored any record");
        int new_prio = logs[0].prev_priority;
        TASSERT(new_prio < init_prio, "Interactive processes behaviour: A sleeper process didn't gain extra priority");
        exit(0);
    }
    int status = 0;
    wait(&status);
    status = WEXITSTATUS(status);
    TASSERT(status == 0, "error in a forked process (status=%d)", status);
    printf("test 29: [SUCCESS]\n");
    return 0;
}
