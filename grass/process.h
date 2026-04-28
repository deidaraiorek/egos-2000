#pragma once

#include "egos.h"
#include "syscall.h"

enum proc_status {
  PROC_UNUSED,
  PROC_LOADING,
  PROC_READY,
  PROC_RUNNING,
  PROC_RUNNABLE,
  PROC_PENDING_SYSCALL
};

struct process {
  int pid;
  struct syscall syscall;
  enum proc_status status;
  uint mepc, saved_registers[32];
  /* Student's code goes here (Preemptive Scheduler | System Call). */
  ulonglong creation_timestamp;
  ulonglong first_scheduled_timestamp;
  ulonglong response_time;
  ulonglong turn_around_time;
  ulonglong run_start_timestamp;
  ulonglong cpu_run_time;
  int timer_interrupt_count;
  int level;
  ulonglong mlfq_time_remaining;
  /* Add new fields for lifecycle statistics, MLFQ or process sleep. */

  /* Student's code ends here. */
};
#define MAX_NPROCESS 16
#define MLFQ_NLEVELS 5
#define MLFQ_RESET_PERIOD 10000000
#define MLFQ_LEVEL_RUNTIME(x) (((x) + 1) * 100000)

ulonglong mtime_get();

int proc_alloc();
void proc_free(int);
void proc_set_ready(int);
void proc_set_running(int);
void proc_set_runnable(int);
void proc_set_pending(int);

void mlfq_reset_level();
void mlfq_update_level(struct process *p, ulonglong runtime);
void proc_sleep(int pid, uint usec);
void proc_coresinfo();

extern uint core_to_proc_idx[NCORES];
