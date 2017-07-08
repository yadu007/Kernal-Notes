#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

void Parse(char* cmd, char** params);
int Run(char** params);

#define LENGTH 100
#define PARAMS 10

int main()
{
    char cmd[LENGTH + 1];
    char* params[PARAMS + 1];

    int cmdCount = 0;

    while(1) {

        printf(">>");

    
        if(fgets(cmd, sizeof(cmd), stdin) == NULL) break;

        
      if(cmd[strlen(cmd)-1] == '\n') {
            cmd[strlen(cmd)-1] = '\0';
        }

    
        Parse(cmd, params);

    
        if(strcmp(params[0], "exit") == 0) break;

      
        if(Run(params) == 0) break;
    }

    return 0;
}


void Parse(char* cmd, char** params)
{       
    for(int i = 0; i < PARAMS; i++) {
        params[i] = strsep(&cmd, " ");
        if(params[i] == NULL) break;
    }
}

int Run(char** params)
{

    pid_t pid = fork();

 
    if (pid == -1) {
       
        printf("fork:Error");
        return 1;
    }

 
    else if (pid == 0) {
     
        execvp(params[0], params);  

   
        printf("shell:Error Not found\n");
        return 0;
    }

  
    
}