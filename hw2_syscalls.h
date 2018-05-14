#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

typedef struct {
	pid_t prev;
	pid_t next;
	int prev_priority;
	int next_priority;
	int prev_policy;
	int next_policy;
  long switch_time;
} cs_log;

int enable_logging (int size) {
 unsigned int res;
 __asm__(
 "int $0x80;"
 : "=a" (res)
 : "0" (243) ,"b" (size)
 : "memory"
);
 if (res>= (unsigned long)(-125))
 {
errno = -res;
res = -1;
}
return (int) res;
}




int disable_logging() {
 unsigned int res;
 __asm__(
 "int $0x80;"
 : "=a" (res)
 : "0" (244)
 : "memory"
);
 if (res>= (unsigned long)(-125))
 {
errno = -res;
res = -1;
}
return (int) res;
}





int get_logger_records(cs_log* user_mem) {
 unsigned int res;
 __asm__(
 "int $0x80;"
 : "=a" (res)
 : "0" (245) ,"b" (user_mem)
 : "memory"
);
 if (res>= (unsigned long)(-125))
 {
errno = -res;
res = -1;
}
return (int) res;
}





int start_lottery_scheduler() {
 unsigned int res;
 __asm__(
 "int $0x80;"
 : "=a" (res)
 : "0" (246)
 : "memory"
);
 if (res>= (unsigned long)(-125))
 {
errno = -res;
res = -1;
}
return (int) res;
}





int start_orig_scheduler() {
 unsigned int res;
 __asm__(
 "int $0x80;"
 : "=a" (res)
 : "0" (247) ,"b" (user_mem)
 : "memory"
);
 if (res>= (unsigned long)(-125))
 {
errno = -res;
res = -1;
}
return (int) res;
}






void set_max_tickets(int max_tickets) {
 unsigned int res;
 __asm__(
 "int $0x80;"
 : "=a" (res)
 : "0" (248) ,"b" (user_mem)
 : "memory"
);
}
