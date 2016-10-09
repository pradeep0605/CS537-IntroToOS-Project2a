#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include <pthread.h>
#include "linked_list.h"

#define true 1
#define false 0
#define HASH_SIZE (1000)
#define mysh_printf(format, ...) \
  sprintf(output, format, ##__VA_ARGS__); \
  if (write(STDOUT_FILENO, output, strlen(output)) == -1) \
    perror("Error in writing to STDOUT\n");

#define mysh_perror(format, ...) \
  sprintf(output, format, ##__VA_ARGS__); \
  if (write(STDOUT_FILENO, output, strlen(output)) == -1) \
    perror("Error in writing to STDERR\n");

typedef struct job {
  int jid;
  int pid;
  linked_list_t command;
} job_t;

linked_list_t job_hash[HASH_SIZE + 1];

/* Globals */
char output[1000] = {0};
char *shell = "mysh> ";
volatile int global_jid = 0; 
pthread_mutex_t jobs_mutex = PTHREAD_MUTEX_INITIALIZER;


void read_line(char *s) {
  char ch;
  int index = 0;
  while((ch = getchar()) != '\n') {
    if (ch == EOF) {
      exit(0);
    }
    s[index++] = ch;
  }
  s[index] = '\0';
}

void trim_line(char *s) {
  int index = 0;
  /* Remove unwanted space in the begginning */
  while ((s[index] == ' ' || s[index] == '\t') && s[index] != '\0') {
    index++;
  }
  strcpy(s, &s[index]);
  /* Remove unwanted space in the end */
  index = strlen(s) - 1;
  while (index >= 0 && (s[index] == ' ' || s[index] == '\t')) {
    index--;
  }
  s[++index] = '\0';
}

void prune_line(char *s) {
  trim_line(s);
}

int process_inbuild_command(char *s) {
  if (strcmp(s, "j") == 0) {
    
    return true;
  } else if (strcmp(s, "exit") == 0) {
    exit(0);
  } else if (strcmp(s, "myw") == 0) {
    return true;
  }
  return false;;
}

int hash_func(int jid) {
  return jid % (HASH_SIZE + 1);
}

void inc_global_jid() {
  global_jid++;
}

void process_line(char *s) {
  char *token;
  //int hash_idx;
  job_t *new_job = (job_t *) malloc(sizeof(job_t));

  /* Increment the global job counter */
  inc_global_jid();
  new_job->jid = global_jid;
  /* Initialize the comamnd-args list */
  ll_initialize(&new_job->command);

  /* Process the tokens and add then to the list */
  token = strtok(s, " \t");
  while (token != NULL) {
    ll_insert_tail(&new_job->command, (void *)strdup(token));
    token = strtok(NULL, " \t");
  }

  node_t *head = new_job->command.head;
  while (head != NULL) {  
    mysh_printf("%s ", (char *) head->data); 
    head = head->next;
  }
}

int main(int argc, char *argv[])
{
  char input[2000] = {0};
  if (argc == 2) {
    
    /* Work on batch mode */
    goto success;
  } else if (argc > 2) {
    mysh_printf("Usage: mysh [batchFile]\n");
    goto fail;
  }

  while (1) {
    mysh_printf("%s", shell);
    read_line(input);
    if (strlen(input) > 512) {
      mysh_perror("Command more than 512 characters\n");
      continue;
    }
    process_line(input);
    /* if in built command then process and return true */
  }

success:
  exit(0);
fail:
  exit(1);
}
