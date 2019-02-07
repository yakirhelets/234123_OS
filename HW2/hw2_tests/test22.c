#include "test.h"

#define LOG_SIZE 100

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
int main()
{
    restart_cond();
    int retval;
    cs_log logs1[LOG_SIZE];
    /* ~~~~~~~~~~~~~~~~~~~ enable_logging testing ~~~~~~~~~~~~~~~~~~~~~ */
    TASSERT(retval = enable_logging(10) == 0,
    "enable_logging:  should return 0 when size is positive (retval=%d)",retval);
    disable_logging();
    TASSERT(retval = enable_logging(0) == 0,
    "enable_logging:  should return 0 when size == 0 (retval=%d)",retval);
    disable_logging();

    /* ~~~~~~~~~~~~~~~~~~~ disable_logging testing ~~~~~~~~~~~~~~~~~~~~~ */
    enable_logging(10);
    TASSERT(retval = disable_logging() == 0,
    "disable_logging: should return 0 on success (retval=%d)",retval);

    /* ~~~~~~~~~~~~~~~~~~~ get_logger_records testing ~~~~~~~~~~~~~~~~~~~~~ */
    enable_logging(LOG_SIZE);
    int k;
    for (k = 0; k < 2*LOG_SIZE ; k++) {
        int pid = fork();
        TASSERT(pid >= 0,
        "error while forking (pid=%d)", pid);
        if (pid == 0)
            exit(0);
    }
    int status = 0;
    while(wait(&status) != -1) {
        status = WEXITSTATUS(status);
        TASSERT(status == 0, "get_logger_records: error in a forked process (status=%d)", status);
    }
    disable_logging();
    TASSERT(retval = get_logger_records(logs1) == LOG_SIZE,
    "get_logger_records: should return the number of copied records on success (retval=%d < %d)", retval, LOG_SIZE);
    TASSERT(retval = get_logger_records(logs1) == 0,
    "get_logger_records: should empty the logger (retval=%d)", retval);
    test_logger_validity(logs1);
    printf("test 22: [SUCCESS]\n");
    return 0;
}
