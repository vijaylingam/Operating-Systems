#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for fork *
#include <sys/types.h> // for pid_t 


int main(int argc, char * argv[]){
    if(argc == 2){
        //argv[1] is 'n' 
        char *arg[] = {"xterm", "-e", "./paramax", argv[1], NULL};
        
        execvp(arg[0], arg);
        return 0;
    }else{
        printf("Invalid Arguments \n");
    }
}