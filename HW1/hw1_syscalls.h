#include <errno.h>
#include <sys/types.h>
#include <stdio.h>


struct forbidden_activity_info {
	int syscall_req_level;
	int proc_level;
	int time;
};
typedef struct forbidden_activity_info forbidden_activity_info;

int enable_policy (pid_t pid, int size, int password)
{
	int res;
	__asm__(
			"int $0x80;"
			: "=a" (res)
			: "0" (243) , "b" (pid) , "c" (size), "d" (password)
			: "memory"
	);

	if (res >= (unsigned long)(-125)) {
		errno = -res;
		res = -1;
	}
	return (int)res;
}


int disable_policy (pid_t pid, int password)
{
	int res;
	__asm__(
			"int $0x80;"
			: "=a" (res)
			: "0" (244) , "b" (pid), "c" (password)
			: "memory"
	);


	if (res >= (unsigned long)(-125)) {
		errno = -res;
		res = -1;
	}
	return (int)res;
}

int set_process_capabilities (pid_t pid, int new_level, int password)
{
    int res;
    __asm__(
            "int $0x80;"
            : "=a" (res)
            : "0" (245) , "b" (pid), "c" (new_level), "d" (password)
            : "memory"
    );
    
    
    if (res >= (unsigned long)(-125)) {
		errno = -res;
		res = -1;
	}
	return (int)res;
}

int get_process_log (pid_t pid, int size, struct forbidden_activity_info* user_mem)
{
    int res;
    __asm__(
            "int $0x80;"
            : "=a" (res)
            : "0" (246) , "b" (pid), "c" (size), "d" (user_mem)
            : "memory"
    );
    
    
    if (res >= (unsigned long)(-125)) {
		errno = -res;
		res = -1;
	}
	return (int)res;
}
