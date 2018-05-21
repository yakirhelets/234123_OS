
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
	FILE *stream = fopen("stream.txt", "w");
	if (!stream) {
	   printf("Cannot open stream.txt!\n");
	   return false;
	}
	FILE *stream_log_pdf = fopen("stream_log_pdf.txt", "w");
	if (!stream_log_pdf) {
	   printf("Cannot open stream_log_pdf.txt!");
	}
  FILE *stream_log_csv = fopen("stream_log_csv.txt", "w");
	if (!stream_log_csv) {
	   printf("Cannot open stream_log_csv.txt!");
	}
  FILE *graph = fopen("graph.txt", "w");
  if (!graph) {
     printf("Cannot open graph.txt!");
  }
  float p1_time=0;
  float p2_time=0;
  fprintf(stream, "p1 pid: %d\n", parent_pid);
  fprintf(stream, "p2 pid: %d\n", child_pid);
	for (i = 0; i < logger_records_size; i++) {
	  fprintf(stream, "Log size: %d\n", logger_records_size);
	  fprintf(stream_log_pdf, "log[%d]: prev = %d, next = %d, prev_priority = %d, next_priority = %d, prev_policy = %d, next_policy = %d, n_tickets = %d, switch_time = %d \n\n", i, log[i].prev, log[i].next, log[i].prev_priority, log[i].next_priority, log[i].prev_policy, log[i].next_policy, log[i].n_tickets, log[i].switch_time);
    fprintf(stream_log_csv, "%d, %d, %d, %d, %d, %d, %d, %d \n", log[i].prev, log[i].next, log[i].prev_priority, log[i].next_priority, log[i].prev_policy, log[i].next_policy, log[i].n_tickets, log[i].switch_time);
    fprintf(stream, "%d \n", log[i].next_priority);
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
	fclose(stream);
	fclose(stream_log_pdf);
  fclose(stream_log_csv);
  fclose(graph);
	stream = fopen("stream.txt", "r");
	if (!stream) {
	   printf("Cannot open stream.txt!\n");
	   return false;
	}
	char line[3];
	int count = 0;
	while (fgets(line, MAXSTRING, stream)) {
	   if (!strcmp(line, "90 \n")) {

	      count++;
	   }
	}
	float final = ((float)count / LOG_SIZE);
  printf("%d\n", TEST_SIZE);
	if (!(final <= 0.33 && final >= 0.27)) {
	   return false;
	}
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
