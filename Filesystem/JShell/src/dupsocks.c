#include <stdio.h>
#include <fcntl.h>

void dupsockin(int fd)
{
  if (dup2(fd, 0) < 0) { perror("dupsockin:dup2"); exit(1); }
}

void dupsockout(int fd){
if(dup2(fd, 1) <0) { perror("dupsockout:dup2"); exit(1);}
if(dup2(fd, 2) <0) { perror("dupsockout:dup2"); exit(1);}
}