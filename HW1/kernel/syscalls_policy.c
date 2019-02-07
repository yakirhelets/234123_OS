#include <linux/sched.h>
#include <linux/slab.h>
#include <asm/uaccess.h>

int sys_enable_policy (pid_t pid, int size, int password) {
	if (pid < 0) {
		return -ESRCH;
	}	
	task_t* p = find_task_by_pid(pid);
	if (p == NULL) { //a process with pid = pid isn't exsits
		return -ESRCH;
	}
	if (password != 234123) {
		return -EINVAL;
	}
	if (p->policyFeature == 1) {
		return -EINVAL;
	}
	if (size < 0) {
		return -EINVAL;
	}

	p->policyFeature = 1;
	p->privilegeLevel = 2;
	p->numOffrobiddenActivities = 0;
	return 0;
}

int sys_disable_policy (pid_t pid, int password) {
	if (pid < 0) {
		return -ESRCH;
	}	
	task_t* p = find_task_by_pid(pid);
	if (p == NULL) { //a process with pid = pid isn't exsits
		return -ESRCH;
	}
	if (password != 234123) {
		return -EINVAL;
	}
	if (p->policyFeature == 0) {
		return -EINVAL;
	}
	
	p->policyFeature = 0;
	p->privilegeLevel = 2;
	processLogs* info = NULL;
	struct list_head* p1, *p2;
	list_for_each_safe(p1, p2, &p->frobiddenActivities) {
		info = list_entry(p1, processLogs, my_ptr);
		list_del(&info->my_ptr);
		kfree(info);
		p->numOffrobiddenActivities--;
	}
	return 0;
}

int sys_set_process_capabilities (pid_t pid, int new_level, int password) {
	if (pid < 0) {
		return -ESRCH;
	}	
	task_t* p = find_task_by_pid(pid);
	if (p == NULL) { //a process with pid = pid isn't exsits
		return -ESRCH;
	}
	if (new_level != 0 && new_level != 1 && new_level != 2) {
		return -EINVAL;
	}
	if (password != 234123) {
		return -EINVAL;
	}
	if (p->policyFeature == 0) {
		return -EINVAL;
	}
	p->privilegeLevel = new_level;
	return 0;
	
}

int sys_get_process_log (pid_t pid, int size, struct forbidden_activity_info* user_mem) {
	if (pid < 0) {
		return -ESRCH;
	}	
	task_t* p = find_task_by_pid(pid);
	if (p == NULL) { //a process with pid = pid isn't exsits
		return -ESRCH;
	}
	if (p->numOffrobiddenActivities < size) {
		return -EINVAL;
	}
	if (size < 0) {
		return -EINVAL;
	}
	if (p->policyFeature == 0) {
		return -EINVAL;
	}

	int count=0;
	processLogs* info = NULL;
	struct list_head* p1, *p2;
	list_for_each_safe(p1, p2, &p->frobiddenActivities) {
		if (count == size) {
			break;
		}
		info = list_entry(p1, processLogs, my_ptr);
		list_del(&info->my_ptr);
		p->numOffrobiddenActivities--;
		if (copy_to_user(&(user_mem[count]), &(info->my_frobidden_activities), sizeof(info->my_frobidden_activities)) != 0) {
			return -EINVAL;
		}
		count++;
		kfree(info);
	}
	return 0;
}
