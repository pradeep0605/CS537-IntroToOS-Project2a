#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>
#include<fcntl.h>

int
main(int argc, char *argv[])
{
  int a;
  int readfd, writefd;
  close(0);
  readfd = open("input.txt", O_RDONLY);
  printf("Readfd = %d\n", readfd);
  close(1);
  writefd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC);
  printf("Writefd = %d\n", writefd);
  scanf("%d", &a);
  printf("a = %d\n", a);

  return 0;   
}
