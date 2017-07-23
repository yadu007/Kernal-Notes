#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

int Parse(char* cmd, char** params);
int Run(char** params);
int execpipe (char ** argv1, char ** argv2);
int Global=0;
#define LENGTH 100
#define PARAMS 10

int main()
{
 char * argv1[PARAMS+1] = {0};    
 char * argv2[PARAMS+1] = {0};    
 char cmd[LENGTH + 1];
 char* params[PARAMS + 1];
 int Red=1,y,red1=0,red2=0;
 int k,x;
 int cmdCount = 0;
while(1) {

  printf(">>");
  if(fgets(cmd, sizeof(cmd), stdin) == NULL) break;
  if(cmd[strlen(cmd)-1] == '\n') 
   {
  cmd[strlen(cmd)-1] = '\0';
   }
 
  int count=Parse(cmd, params);
 
  if(strcmp(params[0], "exit") == 0) break;
 
  for (k=0; k <count; k++) 
   { 
   if (strcmp(params[k], "|") == 0) 
     {    
     Red=0;y=k;
     break;
     }
   if( strcmp(params[k], ">") == 0)
     {
     red1=1;Red=0;Global=1;
     break;
     }
   if(strcmp(params[k], "<") == 0)
     {
   red2=1;Red=0; break;
     }
  }
  if (Red==0)
   {
    for (x=0; x<k; x++) 
     {    
       argv1[x]=params[x];
     }     
    int z = 0;     
    for (x=k+1; x< count; x++) 
     {     
      argv2[z]=params[x];
      z++;
     } 
    if(red1){execred(argv1, argv2); break;}
    if(red2){execred(argv1, argv2); break;} 
    if(execpipe(argv1, argv2)==0) break;
           
   } 
  else if (Red==1)
   {     
    if (Run(params) == 0) break;
   }
    }
    return 0;
}

int Parse(char* cmd, char** params)
{     
  int n=-1;  
    for(int i = 0; i < PARAMS; i++) {
        params[i] = strsep(&cmd, " ");
        n++;
        if(params[i] == NULL) break;
    }
    return(n);
}
int execred(char ** argv1, char ** argv2)
{

    pid_t pid = fork();
    if (pid == -1) { 
        char *error = strerror(errno);
        printf("error fork!!\n");
        return 1;
    } 
    if (pid == 0) { 
        if (Global==0) { 
        int fd = open(*argv2, O_RDONLY, 0);
      dup2(fd, STDIN_FILENO);
      Global = 0;
       int cur = dup(0); 
       }

    else if(Global==1) { 
    int fd = creat(*argv2, 0644);
    dup2(fd, STDOUT_FILENO);
    Global = 0;
    int curr = dup(1);
       }
       Run(argv1);
    }
}
int execpipe (char ** argv1, char ** argv2) {
    int fds[2];
    pipe(fds);
    int i;
    pid_t pid = fork();
    if (pid == -1) { 
        char *error = strerror(errno);
        printf("error\n");
        return 1;
    } 
    if (pid == 0) { 
        close(fds[1]);
        dup2(fds[0], 0);
     
        execvp(argv2[0], argv2); 
        char *error = strerror(errno);
        printf("errer\n");
        return 0;
    } else { 
        close(fds[0]);
        dup2(fds[1], 1);
       
        execvp(argv1[0], argv1); 
        char *error = strerror(errno);
        printf("error\n");
     
    }
    return 0;
}
int Run(char** params)
{
    pid_t pid = fork();
    if (pid == -1) 
     {
      printf("error\n");
      return 1;
     }
    else if (pid == 0) 
     {
     execvp(params[0], params);  
     printf("error\n");
     return 0;
     }
}
