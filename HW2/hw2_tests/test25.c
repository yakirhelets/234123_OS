#include "test.h"

int main() {
    restart_cond();
    int retval;

    /* start_lottery_scheduler basic invocation */

    TASSERT(retval = start_lottery_scheduler() == 0,
    "start_lottery_scheduler: should return 0 (retval=%d)",retval);

    /* start_orig_scheduler basic invocation */

    TASSERT(retval = start_orig_scheduler() == 0,
    "start_orig_scheduler: should return 0 (retval=%d)",retval);

    /* test set_max_tickets */

    set_max_tickets(1);
    struct sched_param param = {30};
    sched_setscheduler(getpid(), SCHED_RR, &param);
    start_lottery_scheduler();
    enable_logging(10);
    busy_wait_by_sec(6);
    disable_logging();

    cs_log logs[10];
    get_logger_records(logs);
    int i;
    for (i = 0; i < 10 ; i++) {
        TASSERT(logs[i].prev == logs[i].next,
        "testing basic functionality: prev and next should be equals, but prev (%d) != next (%d)", logs[i].prev, logs[i].next);
    }
    for (i = 1; i < 10 ; i++) {
        TASSERT(logs[i - 1].next == logs[i].next,
        "testing basic functionality: only one process is able to run, but (%d) and (%d) runs", logs[i - 1].next, logs[i].next);
    }

    int pid = fork();
    set_max_tickets(-1);
    enable_logging(1000);
    if (pid == 0) {
        busy_wait_by_sec(10);
        set_max_tickets(1); // Only the father is able to run after the son dies
        exit(0);
    }
    busy_wait_by_sec(10);
    int status = 0;
    wait(&status);
    status = WEXITSTATUS(status);
    TASSERT(status == 0, "testing basic functionality: error in a forked process (status=%d)", status);
    disable_logging();

    start_orig_scheduler();

    cs_log logs2[1000];
    int n = get_logger_records(logs2);

    int has_seen_son = 0;
    int has_seen_father = 0;

    for (i = 0 ; i < n ; i++) {
        if (logs2[i].next == pid)
            has_seen_son = 1;
        if (logs2[i].next == getpid())
            has_seen_father = 1;
    }
    TASSERT(has_seen_father == 1 && has_seen_son == 1,
    "set_max_tickets was set with negative value, but the limit still exists");
    printf("test 25: [SUCCESS]\n");
    return 0;
}
