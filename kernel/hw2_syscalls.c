#include <linux/sched.h>
#include <linux/errno.h>
#include <asm/uaccess.h>
#include <linux/slab.h>

int sys_enable_logging(int size) {
	if (size<0) {
		return (-EINVAL);
	}
	if(logger) {
		if(logger->logger_status==1) {
			return (-EINVAL);
		} else {
			if (size>0) {
				kfree(logger->array);
				logger->array=kmalloc((sizeof(cs_log))*size, GFP_KERNEL);
			}
			if(size>0 && !logger->array) {
				return (-ENOMEM);
			}
			logger->size=size;
			logger->logger_status=1;
			logger->index=0;
		}
	} else {
		logger=initLogger(size);
		if (!logger) {
			return (-ENOMEM);
		}
	}
	return 0;
}

int sys_disable_logging() {
	if (!logger) {
		return (-ENOMEM);
	}
	if (logger->logger_status==0) {
		return (-EINVAL);
	}
	logger->logger_status=0;
	return 0;
}

int sys_get_logger_records(cs_log* user_mem) {
	if (!logger || !user_mem) {
		logger->index=0;
		return (-ENOMEM);
	}
	if (logger->size == 0) {
		return 0;
	}
	if (copy_to_user((user_mem), (logger->array), sizeof(cs_log)*(logger->index)) != 0) {
		logger->index=0;
		return (-ENOMEM);
	}
	int numOfRecords=(logger->index);
	logger->index=0;
	return numOfRecords;
}

int sys_start_lottery_scheduler() {

	return 0;
}

int sys_start_orig_scheduler() {

	return 0;
}

void sys_set_max_tickets(int max_tickets) {

}
