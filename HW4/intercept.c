#include <linux/module.h>
#include <linux/kernel.h>
// more #include statements
#include <asm-i386/unistd.h>
#include <linux/syscall.h>
#include <linux/utsname.h>
#include <linux/string.h>
#include <linux/signal.h>

MODULE_LICENSE("GPL");

// command-line arguments
void** sys_call_table = NULL;
char* program_name = NULL;
int scan_range = 100;
MODULE_PARM(program_name,"s");
asmlinkage long (*original_sys_kill)(void);

// new syscall
static int kill_something_info(int sig, struct siginfo *info, int pid)
{
	if (!pid) {
		return kill_pg_info(sig, info, current->pgrp);
	} else if (pid == -1) {
		int retval = 0, count = 0;
		struct task_struct * p;

		read_lock(&tasklist_lock);
		for_each_task(p) {
			if (p->pid > 1 && p != current && thread_group_leader(p)) {
				int err = send_sig_info(sig, info, p);
				++count;
				if (err != -EPERM)
					retval = err;
			}
		}
		read_unlock(&tasklist_lock);
		return count ? retval : -ESRCH;
	} else if (pid < 0) {
		return kill_pg_info(sig, info, -pid);
	} else {
		return kill_proc_info(sig, info, pid);
	}
}

asmlinkage long our_sys_kill(int pid, int sig)
{
  if (program_name) {
    printk("kill interrupted\n");
    if (sig==SIGKILL && (strcmp(program_name,find_task_by_pid(pid)->comm)==0)) {
      printk("kill interrupted\n");
      return (-EPERM);
    }
  }
	struct siginfo info;

	info.si_signo = sig;
	info.si_errno = 0;
	info.si_code = SI_USER;
	info.si_pid = current->pid;
	info.si_uid = current->uid;

	return kill_something_info(sig, &info, pid);
}

void find_sys_call_table(int scan_range) {
  // looking for syscalls table address;
   sys_call_table = (void*)(&system_utsname);//low address
   while (*sys_call_table != (&sys_read)) {//sys_read is on higher address
     sys_call_table++;
   }//should be by now sys_call_table[__NR_read]
   sys_call_table-=(__NR_read);//should be by now sys_call_table address
}

int init_module(void) {
   find_sys_call_table(scan_range);
   // store a reference to the original sys_kill
   original_sys_kill = sys_call_table[__NR_kill];
   // hijack the sys_kill
   sys_call_table[__NR_kill] = &our_sys_kill;
   return 0;
}

void cleanup_module(void) {
   // restore the original sys_kill
   sys_call_table[__NR_kill] = original_sys_kill;
}
