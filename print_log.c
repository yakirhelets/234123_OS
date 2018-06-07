#include <limits.h>
#include <stdlib.h>
#include <sched.h>
#include <unistd.h>
#include "hw2_syscalls.h"
#include "test_utilities.h"
#include <stdlib.h>
#include <string.h>

#define SCHED_OTHER		0
#define SCHED_FIFO		1
#define SCHED_RR		2
#define SCHED_LOTTERY	3

#define MAXSTRING 255

#define LOG_SIZE 500
#define MAX_TICKETS 140

/* a function that prints the scheuler log */
bool print_log(int TEST_SIZE) {
  disable_logging();
	start_orig_scheduler();
	cs_log log[LOG_SIZE];
	int parent_pid = getpid();
	struct sched_param param;
	param.sched_priority = 29; //prio = 70, num_of_tickets = 70
	sched_setscheduler(parent_pid, SCHED_FIFO, &param);
	set_max_tickets(MAX_TICKETS);
	int child_pid = fork();
	param.sched_priority = 29; //prio = 70, num_of_tickets = 70
	sched_setscheduler(child_pid, SCHED_FIFO, &param);
	enable_logging(LOG_SIZE);
	start_lottery_scheduler();
	//just a triple loop to make the processes run and perform context switches between them :)
	int i, j, k;
	for (i = 0; i < TEST_SIZE; ++i) {
	   for (j = 0; j < TEST_SIZE; ++j) {
	      for (k = 0; k < LOG_SIZE; ++k) {

	      }
	   }
	}
	if (child_pid == 0) {
	   exit(0);
	}
	wait(NULL);
	ASSERT_TEST(disable_logging() == 0);
	ASSERT_TEST(start_orig_scheduler() == 0);
	int logger_records_size = get_logger_records(log);
  FILE *graph = fopen("graph.txt", "w");
  if (!graph) {
     printf("Cannot open graph.txt!");
  }
  float p1_time=0;
  float p2_time=0;

	for (i = 0; i < logger_records_size; i++) {
    if (parent_pid==(int)log[i].next) {
      if (p1_time<log[i].switch_time) {
        p1_time=log[i].switch_time;
      }
    }
    if (child_pid==(int)log[i].next) {
      if (p2_time<log[i].switch_time) {
        p2_time=log[i].switch_time;
      }
    }
	}
  fprintf(graph, "%d , %lf \n", TEST_SIZE,(float)p1_time/(float)p2_time);
  fclose(graph);
  printf("%d\n", TEST_SIZE);
	return true;
}

int main() {
int i;
  for (i=1000 ; i<10001 ; i=i+10) {
    disable_logging();
  	start_orig_scheduler();
  	print_log(i);
  }

	return 0;
}
