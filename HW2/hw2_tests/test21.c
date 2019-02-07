/* checking some basic return values of all system calls */
#include "test.h"

int main()
{
    restart_cond();
    int retval;
    /* ~~~~~~~~~~~~~~~ enable_logging errors testing ~~~~~~~~~~~~~~~~~ */
    TASSERT(retval = enable_logging(-1) == -1,
    "enable_logging: should return -1 when size is negative (retval=%d)",retval);
    TASSERT(errno == EINVAL,
    "enable_logging: should set errno=EINVAL when size is negative (errno=%d)",errno);
    enable_logging(0);
    TASSERT(enable_logging(0) == -1,
    "enable_logging: should return -1 when size is negative (retval=%d)",retval);
    TASSERT(errno == EINVAL,
    "enable_logging: should set errno=EINVAL when size is negative (errno=%d)",errno);
    disable_logging();

    /* ~~~~~~~~~~~~~~~ disable_logging errors testing ~~~~~~~~~~~~~~~~~ */

    TASSERT(retval = disable_logging() == -1,
    "disable_logging: should return -1 when already disabled (retval=%d)",retval);
    TASSERT(errno == EINVAL,
    "disable_logging: should set errno=EINVAL when already disabled (errno=%d)",errno);
    enable_logging(0);
    disable_logging();
    TASSERT(retval = disable_logging() == -1,
    "disable_logging: should return -1 when already disabled (retval=%d)",retval);
    TASSERT(errno == EINVAL,
    "disable_logging: should set errno=EINVAL when already disabled (errno=%d)",errno);

    /* ~~~~~~~~~~~~~~~ get_logger_records errors testing ~~~~~~~~~~~~~~~~~ */

    TASSERT(retval = get_logger_records(NULL) == -1,
    "get_logger_records: should return -1 when user_mem == NULL (retval=%d)",retval);
    TASSERT(errno == ENOMEM,
    "get_logger_records: should set errno=ENOMEM when user_mem == NULL (errno=%d)",errno);
    enable_logging(10);
    busy_wait_by_sec(5);
    disable_logging();
    TASSERT(retval = get_logger_records(0xf0000000) == -1,
    "get_logger_records: should return -1 when user_mem == 0xf0000000 (kernel space) (retval=%d)",retval);
    TASSERT(errno == ENOMEM,
    "get_logger_records: should set errno=ENOMEM when user_mem == 0xf0000000 (kernel space) (errno=%d)",errno);
    printf("test 21: [SUCCESS]\n");
    return 0;
}
