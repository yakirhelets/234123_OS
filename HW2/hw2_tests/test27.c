#include "test.h"


int main() {
    restart_cond();
    int retval;
    struct sched_param param = {30};
    /* Test sched_setscheduler*/

    /* Testing the original scheduler */
    int pid = fork();
    if (pid == 0) {
        TASSERT(retval = sched_setscheduler(getpid(),SCHED_RR, &param) == 0,
        "sched_setscheduler (orig): changing policy to SCHED_RR should succeed (retval=%d)", retval);
        TASSERT(retval = sched_setscheduler(getpid(),SCHED_FIFO, &param) == 0,
        "sched_setscheduler (orig): changing policy to SCHED_FIFO should succeed (retval=%d)", retval);
        TASSERT(retval = sched_setscheduler(getpid(),3, &param) < 0,
        "sched_setscheduler (orig): changing policy to SCHED_LOTTERY should fail (retval=%d)", retval);
        exit(0);
    }
    int status = 0;
    wait(&status);
    status = WEXITSTATUS(status);
    TASSERT(status == 0, "error in a forked process (status=%d)", status);

    /* Testing the lottery scheduler */
    start_lottery_scheduler();
    pid = fork();
    if (pid == 0) {
        TASSERT(retval = sched_setscheduler(getpid(),SCHED_RR, &param) < 0,
        "sched_setscheduler (lottery): changing policy to SCHED_RR should fail (retval=%d)", retval);
        TASSERT(retval = sched_setscheduler(getpid(),SCHED_FIFO, &param) < 0,
        "sched_setscheduler (lottery): changing policy to SCHED_FIFO should fail (retval=%d)", retval);
        TASSERT(retval = sched_setscheduler(getpid(),3, &param) < 0,
        "sched_setscheduler (lottery): changing policy to SCHED_LOTTERY should fail (retval=%d)", retval);
        exit(0);
    }
    wait(&status);
    status = WEXITSTATUS(status);
    TASSERT(status == 0, "error in a forked process (status=%d)", status);
    printf("status 0\n");

    pid = fork();
    if (pid == 0) {
        retval = sched_setscheduler(getpid(),-1, &param);
        TASSERT(retval == 0,
        "sched_setscheduler (lottery): changing priority should succeed (retval=%d)", retval);
        exit(0);
    }
    wait(&status);
    status = WEXITSTATUS(status);
    printf("statues = %d\n", status);
    TASSERT(status == 0, "error in a forked process (status=%d)", status);

    printf("test 27: [SUCCESS]\n");
    return 0;
}
