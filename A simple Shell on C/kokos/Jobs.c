
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>

typedef struct process
{
  struct process *next;       /* next process in pipeline */
  char **argv;                /* for exec */
  pid_t pid;                  /* process ID */
  char completed;             /* true if process has completed */
  char stopped;               /* true if process has stopped */
  int status;                 /* reported status value */
} process;

/* The active processes are linked into a list.  This is its head.   */

process *first_process = NULL;


/* Find the active process with the indicated pid.  */
process *find_process (pid_t active_pid){
  process *j;

  for (j = first_process; j; j = j->next)
    if (j->pid == active_pid)
      return j;
  return NULL;
}

/* Return true if a process have stopped or completed.  */
int process_is_stopped (process *j){
  process *p;

  for (p = first_process; p; p = p->next)
    if (!p->completed && !p->stopped)
      return 0;
  return 1;
}

/* Return true if all processes in the process have completed.  */
int process_is_completed (process *j){
  process *p;

  for (p = first_process; p; p = p->next)
    if (!p->completed)
      return 0;
  return 1;
}

void launch_process (process *p){

  /* Exec the new process.  Make sure we exit.  */
  execvp (p->argv[0], p->argv);
  perror ("execvp");
  exit (1);
}


/* Put job j in the foreground.  If cont is nonzero,
   restore the saved terminal modes and send the process group a
   SIGCONT signal to wake it up before we block.  */

void put_process_in_foreground (process j, int cont){
  /* Put the job into the foreground.  */

  /* Send the job a continue signal, if necessary.  */
  if (cont)
    {
      if (kill (- (j->pid), SIGCONT) < 0)
        perror ("kill (SIGCONT)");
    }

  /* Wait for it to report.  */
  wait_for_job (j);
}

/* Put a job in the background.  If the cont argument is true, send
   the process group a SIGCONT signal to wake it up.  */
void put_process_in_background (process *j, int cont){
  /* Send the job a continue signal, if necessary.  */
  if (cont)
    if (kill (-j->pid, SIGCONT) < 0)
      perror ("kill (SIGCONT)");
}


/* Store the status of the process pid that was returned by waitpid.
   Return 0 if all went well, nonzero otherwise.  */

int mark_process_status (pid_t pid, int status){

  process *p;

  if (pid > 0)
    {
      /* Update the record for the process.  */
      
	      
        for (p = first_process; p; p = p->next)
          if (p->pid == pid){
              p->status = status;
              if (WIFSTOPPED (status))
                p->stopped = 1;
              else
                {
                  p->completed = 1;
                  if (WIFSIGNALED (status))
                    fprintf (stderr, "%d: Terminated by signal %d.\n",
                             (int) pid, WTERMSIG (p->status));
                }
              return 0;
          }
      fprintf (stderr, "No child process %d.\n", pid);
      return -1;
    }

  else if (pid == 0 || errno == ECHILD)
    /* No processes ready to report.  */
    return -1;
  else {
    /* Other weird errors.  */
    perror ("waitpid");
    return -1;
  }
}


/* Check for processes that have status information available,
   without blocking.  */

void update_status (void){
  int status;
  pid_t pid;

  do
    pid = waitpid (WAIT_ANY, &status, WUNTRACED|WNOHANG);
  while (!mark_process_status (pid, status));
}

/* Check for processes that have status information available,
   blocking until all processes in the given job have reported.  */

void wait_for_job (process *j){
  int status;
  pid_t pid;

  do
    pid = waitpid (WAIT_ANY, &status, WUNTRACED);
  while (!mark_process_status (pid, status)
         && !job_is_stopped (j)
         && !job_is_completed (j));
}


/* Format information about job status for the user to look at.  */

void format_job_info (process *j, const char *status){
  fprintf (stderr, "%ld (%s): \n", (long)j->pid, status);
}

/* Notify the user about stopped or terminated jobs.
   Delete terminated jobs from the active job list.  */

void do_job_notification (void){
   
  process *j,*jlast, *jnext;

  /* Update status information for child processes.  */
  update_status ();

  jlast = NULL;
  for (j = first_process; j; j = jnext)
    {
      jnext = j->next;

      /* If all processes have completed, tell the user the job has
         completed and delete it from the list of active jobs.  */
      if (job_is_completed (j)) {
        format_job_info (j, "completed");
        if (jlast)
          jlast->next = jnext;
        else
          first_process = jnext;
        free_job (j);
      }

      /* Notify the user about stopped jobs,
         marking them so that we won’t do this more than once.  */
      else if (job_is_stopped (j) && !j->notified) {
        format_job_info (j, "stopped");
        
        jlast = j;
      }

      /* Don’t say anything about jobs that are still running.  */
      else
        jlast = j;
    }
}

/* Mark a stopped job J as being running again.  */

void mark_process_as_running (process *j){
	Process *p;

	for (p = first_process; p; p = p->next){
		if(p->pid == j->pid){
			p->stopped = 0;
		}
	}
}

/* Continue the job J.  */

void continue_process (process *j, int foreground){
  mark_process_as_running (j);
  if (foreground)
    put_process_in_foreground (j, 1);
  else
    put_process_in_background (j, 1);
}