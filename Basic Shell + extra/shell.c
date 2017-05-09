#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<ctype.h>

#define MAX_INPUT 1024

int main (int argc, char ** argv, char **envp){

    int finished = 0;
    char cmd[MAX_INPUT];

    int is_running_interactively = 1;

    signal (SIGINT, SIG_IGN);
    signal (SIGQUIT, SIG_IGN);
    signal (SIGTSTP, SIG_IGN);
    signal (SIGTTIN, SIG_IGN);
    signal (SIGTTOU, SIG_IGN);
    signal (SIGCHLD, SIG_IGN);

    char *scriptInputFileName = malloc(MAX_INPUT*sizeof(char*));
    scriptInputFileName = argv[1];
    if(scriptInputFileName != NULL){
        is_running_interactively = 0;
    }

    FILE *file_pointer;
    char *current_line = NULL;
    int len = 0;
    int read_;

    file_pointer = fopen(scriptInputFileName, "r");
    if(is_running_interactively == 0 & file_pointer == NULL){
        perror("INVALID FILE");
        exit(3);
    }

    char **envPath= malloc((MAX_INPUT) * sizeof(char*));
    int environmentPathCount = 0; //WHY USE THIS?
    char *path;
    path = getenv("PATH");
    char *path_copy = malloc(strlen(path)*sizeof(char*));
    strcpy(path_copy, path);

    char colonSeparator[] = ":";
    char *result = NULL;
    result = strtok(path_copy,colonSeparator);
    while(result != NULL ) {
        envPath[environmentPathCount] = result;
        environmentPathCount++;
        result = strtok(NULL, colonSeparator);
    }

    if(is_running_interactively){
        while (!finished) {
            char *cursor;
            char last_char;
            int rv;
            int count;


            // Print the prompt
            rv = fancyCommandPrompt();
            if (!rv) {
                finished = 1;
                break;
            }

            // read and parse the input
            for(rv = 1, count = 0,
                cursor = cmd, last_char = 1;
                rv
                && (++count < (MAX_INPUT-1))
                && (last_char != '\n');
                cursor++) {

                rv = read(0, cursor, 1);
                last_char = *cursor;
            }
            *cursor = '\0';
            cmd[count - 2] = '\0';
            if (!rv) {
                finished = 1;
                break;
            }
                execute(cmd, envPath, environmentPathCount);

        }
    }
    else{

        while((read_ = getline(&current_line, &len, file_pointer)) != -1){
            char newlineDelimiter[] = "\n";
            char *cmd = NULL;
            cmd = strtok(current_line, newlineDelimiter);
            char *comment_args = malloc(strlen(cmd)*sizeof(char*));
            strncpy(comment_args, cmd, 1);
            if(strcmp(comment_args, "#") > 0){
                    execute(current_line, envPath, environmentPathCount);
            }
        }

    }

    return 0;
}

int execute(char *cmd, char **environment_paths, int environmentPathCount){
    if(environmentPathCount > 0){
        int current_pipefd[2];
        int former_fd = 0;

        char *fullcmd;
        fullcmd = cmd;

        char **cmd_args;
        int cmdArgCount;

        char pipeSeparator[] = "|";
        char *pipeResult = NULL;
        char *end_pipeResult;
        pipeResult = strtok_r(fullcmd,pipeSeparator, &end_pipeResult);
        while(pipeResult != NULL ) {

            cmd_args = malloc((MAX_INPUT) * sizeof(char*));
            cmdArgCount = 0;

            char* currentPipedArgs = malloc(strlen(pipeResult)*sizeof(char*));
            strcpy(currentPipedArgs, pipeResult);

            char* currentCommandArgs = malloc(strlen(pipeResult)*sizeof(char*));

            int redirectCount = 0;
            char redirectSeparator[] = ">\'<";
            char *redirectResult = NULL;
            char *end_redirectResult;
            redirectResult = strtok_r(pipeResult,redirectSeparator, &end_redirectResult);
            int redirectResultLen = 0;

            char *redirectIn = malloc(strlen(pipeResult)*sizeof(char*));
            char *redirectOut = malloc(strlen(pipeResult)*sizeof(char*));
            char *redirectOutIdentifier = malloc(strlen(currentPipedArgs)*sizeof(char*));

            int redirect_in_flag = 0;
            int redirect_out_flag = 0;


            while(redirectResult != NULL ) {

                if (redirectCount == 0) {
                    strcpy(currentCommandArgs, redirectResult);
                    redirectResultLen += strlen(redirectResult);
                } else {
                    char r_arg = currentPipedArgs[redirectResultLen];
                    if( r_arg == '<') {
                        strcpy(redirectIn, redirectResult);
                        redirect_in_flag = 1;
                    } else if(r_arg == '>'){
                        if(isdigit(currentPipedArgs[redirectResultLen-1])){
                            int index = 1;
                            while(isdigit(currentPipedArgs[redirectResultLen-index])){
                                index++;
                            }
                            for(int i = 0; i < index-1; i++){
                                redirectOutIdentifier[i] = currentPipedArgs[redirectResultLen-index+(i+1)];
                            }
                        } else {
                            redirectOutIdentifier = "1";
                        }
                        strcpy(redirectOut, redirectResult);
                        redirect_out_flag = 1;
                    } else {
                        perror("invalid re-direction symbol");
                    }
                    redirectResultLen += strlen(redirectResult)+1;
                }
                redirectCount++;
                redirectResult = strtok_r(NULL, redirectSeparator, &end_redirectResult);
            }

            char spaceSeparator[] = " ";
            char *commandResult = NULL;
            char *end_commandResult;
            commandResult = strtok_r(currentCommandArgs, spaceSeparator, &end_commandResult);
            while(commandResult != NULL ) {
                cmd_args[cmdArgCount] = commandResult;
                cmdArgCount++;
                commandResult = strtok_r(NULL, spaceSeparator, &end_commandResult);
            }
            cmd_args[cmdArgCount] = NULL;

            pipeResult = strtok_r(NULL, pipeSeparator, &end_pipeResult);

            char fullFilePath[MAX_INPUT]= "";
            int fileExistsFlag = doesFileExist(cmd_args[0], environment_paths, environmentPathCount, fullFilePath);
            if(fileExistsFlag == 1){

                pipe(current_pipefd);

                int pid = fork();

                if (pid == -1) {
                    perror("FORK FAILED");
                }

                else if (pid > 0) {

                    int status = (int) malloc(sizeof(int)*5);
                    waitpid(pid, &status, 0);
                    close(current_pipefd[1]);
                    former_fd = current_pipefd[0];
                }
                else {
                    dup2(former_fd, 0);
                    if(pipeResult != NULL){
                        dup2(current_pipefd[1], 1);
                    }
                    close(current_pipefd[0]);
                    if(redirect_in_flag){
                        int fd_in = open(redirectIn, 0);
                        dup2(fd_in, STDIN_FILENO);
                        close(fd_in);
                    }
                    if(redirect_out_flag){
                        int fd_out = creat(redirectOut, 0644);
                        dup2(fd_out, atoi(redirectOutIdentifier));
                        close(fd_out);
                    }
                    if(strcmp(cmd_args[0], "echo") == 0){
                        BuiltInCommand(cmd_args, cmdArgCount, fullFilePath);
                    } else {
                        execCmd(fullcmd, cmd_args, cmdArgCount, fullFilePath);
                    }
                }
            }else {

                char *script_args = malloc(strlen(cmd)*sizeof(char*));
                strncpy(script_args, cmd, 2);

                if(strcmp(script_args, "#") == 0){
                    execv(cmd_args[0], cmd_args);
                } else {
                    int didRun = BuiltInCommand(cmd_args, cmdArgCount, fullFilePath);
                    if(didRun == 1){
                        if(cmd_args[0][0] == '#'){
                            continue;
                        }
                        else {
                            write(1, "command not found", strlen("command not found"));
                        }
                    }
                }
            }
        }
    } else {
        perror("ENVIRONMENT PATH NOT FOUND");
    }
    return 0;
}

int BuiltInCommand(char **cmd_args, int cmdArgCount, char *fullFilePath) {

    if (strcmp(cmd_args[0], "exit") == 0) {
        exit(3);
    }

    else if (strcmp(cmd_args[0], "echo") == 0) {
        echoCmd(cmd_args[0], cmd_args, cmdArgCount, fullFilePath);
        return 0;
    }
    else if(!strcmp(cmd_args[0], "jobs")){
        return 0;
    }
    else if (strcmp(cmd_args[0], "cd") == 0) {
        if (cmd_args[1] == NULL) {
            chdir(getenv("HOME"));
        }
        else {
            chdir(cmd_args[1]);
        }
        return 0;
    } else if(strcmp(cmd_args[0], "set") == 0){
        char **variable_args = malloc((2) * sizeof(char*));
        char equalsSeparator[] = "=";
        char *result = NULL;
        result = strtok(cmd_args[1],equalsSeparator);
        int variableArgsIndex = 0;
        while(result != NULL ) {
            variable_args[variableArgsIndex] = result;
            variableArgsIndex++;
            result = strtok(NULL, equalsSeparator);
        }
        setenv(variable_args[0], variable_args[1], 1);
        return 0;
    }

    if (!strcmp(cmd_args[0], "&"))	
        return 1;
    return 1;
}

int doesFileExist(char *fileName, char **environment_paths, int environmentPathsCount, char *fullPath){
    struct stat buf;
    for (int i = 0; i < environmentPathsCount; i++) {
        strcpy(fullPath, environment_paths[i]);
        strcat(fullPath, "/");
        strcat(fullPath, fileName);
        if (stat(fullPath, &buf)==0 && strcmp(fileName, "cd") != 0) {
            int fullFilePathLength = strlen(fileName)+strlen(environment_paths[i]);
            fullPath[fullFilePathLength+1]='\0';
            return 1;
        }
    }
    return 0;
}

int execCmd(char *cmd, char **cmd_args, int cmdArgCount, char *fullFilePath){
    if(strcmp(cmd,"echo") == 0){
        BuiltInCommand(cmd_args, cmdArgCount, fullFilePath);
    } else {
        cmd_args[0] = fullFilePath;
        execv(cmd_args[0], cmd_args);
    }
    return -1;
}

echoCmd(char *cmd, char **cmd_args, int cmdArgCount, char *fullFilePath){

    for(int i = 1; i < cmdArgCount; i++){
        char *echo_arg = malloc(strlen(cmd_args[i])*sizeof(char)+2);
        echo_arg = cmd_args[i];
        int variableFoundFlag = 0;

        if(echo_arg[0] == '$'){
            for(int j = 0; j < strlen(echo_arg); j++){
                echo_arg[j] = echo_arg[j+1];
            }
            char *environment_variable = malloc(MAX_INPUT*sizeof(char));
            environment_variable = getenv(echo_arg);
            if(environment_variable!=NULL){
                variableFoundFlag = 1;
                write(1, environment_variable, strlen(environment_variable));
            }
        }
        if(variableFoundFlag == 0){
            write(1, echo_arg, strlen(echo_arg));
        }
    }
    write(1, "\n", strlen("\n"));
}

int fancyCommandPrompt(){
    char *currentDirectory;
    currentDirectory = getcwd (0,0);
    if (currentDirectory != NULL) {
        char *directoryPrompt = malloc(MAX_INPUT*sizeof(char*));
        strcat(directoryPrompt, "[");
        strcat(directoryPrompt, currentDirectory);
        strcat(directoryPrompt, "]");
        strcat(directoryPrompt, " ");
        strcat(directoryPrompt, " thsh> ");
        free(currentDirectory);
        return write(1, directoryPrompt, strlen(directoryPrompt));
    }
    else {
        return write(1, " thsh> ", strlen(" thsh> "));
    }
}