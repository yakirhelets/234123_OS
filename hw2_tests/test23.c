#include "test.h"

#define LOG_SIZE 10

void test_logger_validity(cs_log* logs) {
    int i;
    for (i = 0 ; i < LOG_SIZE ; i++) {
        TASSERT(logs[i].next >= 0,
        "get_logger_records: logger contains next < 0 (next=%d)", logs[i].next);
        TASSERT(logs[i].prev_priority >= 0 && logs[i].prev_priority <= 139,
         "get_logger_records: logger contains prev_priority out of bounds (prev_priority=%d)", logs[i].prev_priority);
        TASSERT(logs[i].next_priority >= 0 && logs[i].next_priority <= 139,
         "get_logger_records: logger contains next_priority out of bounds (prev_priority=%d)", logs[i].next_priority);
        if (i == 0)
            continue;
        TASSERT(logs[i].switch_time >= logs[i-1].switch_time,
        "get_logger_records: switch_time is descending (logger[%d].switch_time: %d > logger[%d].switch_time: %d",
         i-1, logs[i-1].switch_time, i, logs[i].switch_time);
    }
}

int main() {
    restart_cond();
    int retval;
    cs_log logs1[LOG_SIZE];
    int i = 0;
    /* ~~~~~~~~~~~ Cross processes testing ~~~~~~~~~~~~ */
    int pid = fork();
    TASSERT(pid >= 0,
        "error while forking (pid=%d)", pid);
    if (pid == 0) {
        TASSERT(retval = enable_logging(LOG_SIZE) == 0,
        "enable_logging has failed in the forked process (pid=%d, retval=%d)", getpid(), retval);
        exit(0);
    }
    int status = 0;
    wait(&status);
    status = WEXITSTATUS(status);
    TASSERT(status == 0, "error in a forked process (status=%d)", status);
    for (i = 0 ; i < LOG_SIZE; i++) {
        int fpid = fork();
        TASSERT(fpid >= 0,
        "error while forking (pid=%d)", fpid);
        if (fpid == 0)
            exit(0);
    }
    while(wait(&status) != -1) {
        status = WEXITSTATUS(status);
        TASSERT(status == 0, "error in a forked process (status=%d)", status);
    }
    pid = fork();
    TASSERT(pid >= 0,
        "error while forking (pid=%d)", pid);
    if (pid == 0) {
        TASSERT(retval = disable_logging() == 0,
        "disable_logging has failed in the forked process (pid=%d, retval=%d)", getpid(), retval);
        exit(0);
    }
    wait(&status);
    status = WEXITSTATUS(status);
    TASSERT(status == 0, "error in a forked process (status=%d)", status);
    TASSERT(retval = get_logger_records(logs1) == LOG_SIZE,
    "get_logger_records failed (retval=%d != %d)", retval, LOG_SIZE);
    test_logger_validity(logs1);

    /* ~~~~~~~~~~~~ Stress tests ~~~~~~~~~~~~~~~ */
    for (i = 0; i < 10000000 ; i++) {
        TASSERT(retval = enable_logging(100) == 0,
        "Stress tests: enable_logging has failed (retval=%d)", retval);
        TASSERT(retval = disable_logging() == 0,
        "Stress tests: disable_logging has failed (retval=%d)", retval);
    }
    printf("test 23: [SUCCESS]\n");
    return 0;
}
