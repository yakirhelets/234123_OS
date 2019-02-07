#include "test.h"

int main() {
    restart_cond();
    int retval;
    /* ~~~~~~~~~~~~ start_orig_scheduler failures test ~~~~~~~~~~~~~~~~~~~ */

    TASSERT(retval = start_orig_scheduler() == -1,
    "start_orig_scheduler: should return -1 when already enabled (retval=%d)",retval);
    TASSERT(errno == EINVAL,
    "start_orig_scheduler: should set errno=EINVAL when size is negative (errno=%d)",errno);

    /* ~~~~~~~~~~~~ start_lottery_scheduler failures tests ~~~~~~~~~~~~~~~~ */

    start_lottery_scheduler();
    TASSERT(retval = start_lottery_scheduler() == -1,
    "start_lottery_scheduler: should return -1 when already enabled (retval=%d)",retval);
    TASSERT(errno == EINVAL,
    "start_lottery_scheduler: should set errno=EINVAL when size is negative (errno=%d)",errno);
    /* ~~~~~~~~~~~~ start_orig_scheduler failures test ~~~~~~~~~~~~~~~~~~~ */

    start_orig_scheduler();
    TASSERT(retval = start_orig_scheduler() == -1,
    "start_orig_scheduler: should return -1 when already enabled (retval=%d)",retval);
    TASSERT(errno == EINVAL,
    "start_orig_scheduler: should set errno=EINVAL when size is negative (errno=%d)",errno);
    printf("test 24: [SUCCESS]\n");
    return 0;
}
