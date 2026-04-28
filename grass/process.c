/*
 * (C) 2026, Cornell University
 * All rights reserved.
 *
 * Description: helper functions for process management
 */

#include "process.h"

extern struct process proc_set[MAX_NPROCESS + 1];

static void proc_set_status(int pid, enum proc_status status) {
  for (uint i = 0; i < MAX_NPROCESS; i++)
    if (proc_set[i].pid == pid)
      proc_set[i].status = status;
}

void proc_set_ready(int pid) { proc_set_status(pid, PROC_READY); }
void proc_set_running(int pid) { proc_set_status(pid, PROC_RUNNING); }
void proc_set_runnable(int pid) { proc_set_status(pid, PROC_RUNNABLE); }
void proc_set_pending(int pid) { proc_set_status(pid, PROC_PENDING_SYSCALL); }

int proc_alloc() {
  static uint curr_pid = 0;
  for (uint i = 1; i <= MAX_NPROCESS; i++)
    if (proc_set[i].status == PROC_UNUSED) {
      proc_set[i].pid = ++curr_pid;
      proc_set[i].status = PROC_LOADING;
      proc_set[i].level = 0;
      proc_set[i].mlfq_time_remaining = MLFQ_LEVEL_RUNTIME(0);
      proc_set[i].cpu_run_time = 0;
      proc_set[i].timer_interrupt_count = 0;
      proc_set[i].response_time = 0;
      proc_set[i].turn_around_time = 0;
      proc_set[i].run_start_timestamp = 0;
      /* Student's code goes here (Preemptive Scheduler | System Call). */
      proc_set[i].creation_timestamp = mtime_get();
      /* Initialization of lifecycle statistics, MLFQ or process sleep. */

      /* Student's code ends here. */
      return curr_pid;
    }

  FATAL("proc_alloc: reach the limit of %d processes", MAX_NPROCESS);
}

void proc_free(int pid) {
  /* Student's code goes here (Preemptive Scheduler). */

  /* Print the lifecycle statistics of the terminated process or processes. */
  if (pid != GPID_ALL) {
    earth->mmu_free(pid);
    for (uint i = 0; i < MAX_NPROCESS; i++)
      if (proc_set[i].pid == pid) {
        proc_set[i].turn_around_time =
            mtime_get() - proc_set[i].creation_timestamp;
        INFO("process %d terminated after %d timer interrupts, turnaround "
             "time: %dms, response time: %dms, CPU time: %dms",
             pid, proc_set[i].timer_interrupt_count,
             (int)(proc_set[i].turn_around_time / 10000),
             (int)(proc_set[i].response_time / 10000),
             (int)(proc_set[i].cpu_run_time / 10000));
      }
    proc_set_status(pid, PROC_UNUSED);
  } else {
    /* Free all user processes. */
    for (uint i = 0; i < MAX_NPROCESS; i++)
      if (proc_set[i].pid >= GPID_USER_START &&
          proc_set[i].status != PROC_UNUSED) {
        earth->mmu_free(proc_set[i].pid);
        proc_set[i].status = PROC_UNUSED;
      }
  }
  /* Student's code ends here. */
}

void mlfq_update_level(struct process *p, ulonglong runtime) {
  /* Student's code goes here (Preemptive Scheduler). */

  /* Update the MLFQ-related fields in struct process* p after this
   * process has run on the CPU for another runtime microseconds. */
  if (runtime < p->mlfq_time_remaining) {
    p->mlfq_time_remaining -= runtime;
    return;
  } else {
    p->level = p->level >= 4 ? 4 : p->level + 1;
    p->mlfq_time_remaining = MLFQ_LEVEL_RUNTIME(p->level);
  }
  /* Student's code ends here. */
}

void mlfq_reset_level() {
  /* Student's code goes here (Preemptive Scheduler). */
  if (!earth->tty_input_empty()) {
    for (uint i = 0; i < MAX_NPROCESS; i++)
      if (proc_set[i].pid == GPID_SHELL) {
        proc_set[i].level = 0;
        proc_set[i].mlfq_time_remaining = MLFQ_LEVEL_RUNTIME(0);
      }
  }

  static ulonglong MLFQ_last_reset_time = 0;
  if (mtime_get() - MLFQ_last_reset_time > MLFQ_RESET_PERIOD) {
    for (uint i = 0; i < MAX_NPROCESS; i++)
      if (proc_set[i].status != PROC_UNUSED) {
        proc_set[i].level = 0;
        proc_set[i].mlfq_time_remaining = MLFQ_LEVEL_RUNTIME(0);
      }
    MLFQ_last_reset_time = mtime_get();
  }
  /* Student's code ends here. */
}

void proc_sleep(int pid, uint usec) {
  /* Student's code goes here (System Call & Protection). */

  /* Update the sleep-related fields in the struct process for process pid. */

  /* Student's code ends here. */
}

void proc_coresinfo() {
  /* Student's code goes here (Multicore & Locks). */

  /* Print out the pid of the process running on each CPU core. */

  /* Student's code ends here. */
}
