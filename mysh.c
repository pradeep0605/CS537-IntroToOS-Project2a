#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include "linked_list.h"
#include<string.h>

#define true 1
#define false 0
#define mysh_printf(format, ...) \
  sprintf(output, format, ##__VA_ARGS__); \
  if (write(STDOUT_FILENO, output, strlen(output)) == -1) \
    perror("Error in writing to STDOUT\n");

#define mysh_perror(format, ...) \
  sprintf(output, format, ##__VA_ARGS__); \
  if (write(STDOUT_FILENO, output, strlen(output)) == -1) \
    perror("Error in writing to STDERR\n");

char output[1000] = {0};
char *shell = "mysh> ";

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

/*
void remove_redundant_spaces(char *s) {
  int index = 0;
  int i = 0;
  int spacecopied = false;
  char *temp = malloc(strlen(s) + 1);
  
  trim_line(s);

  while (s[i] != '\0') {
    if (s[i] == '\t') {
      s[i] = ' ';
    }
    i++;
  }

  for (i = 0; i < strlen(s); ++i) {
    if ((s[i] == ' ' || s[i] == '\t') && !spacecopied) {
      temp[index] = s[i];
      index++;
      spacecopied = true;
    }  
    if (s[i] != ' ' || s[i] != '\t') {
      temp[index] = s[i];
      index++;
      spacecopied = false;
    }
  }

  temp[index] = '\0';
  strcpy(s, temp);
  free(temp);
}
*/
void prune_line(char *s) {
  trim_line(s);
  //remove_redundant_spaces(s);
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

void process_line(char *s) {
}

int main(int argc, char *argv[])
{
  char input[2000] = {0};
  if (argc == 2) {
    /* Work on batch mode */
    goto success;
  } else {
    mysh_printf("Usage: mysh [batchFile]\n");
  }

  while (1) {
    mysh_printf("%s", shell);
    read_line(input);
    if (strlen(intput) > 512) {
      mysh_printf("Command more than 512 characters\n");
      continue;
    }
    process_line(input);
    
    /* if in built command then process and return true */
    if (process_inbuild_command(input)) {
      continue;
    }
  }

success:
  exit(0);
  exit(1);
}
