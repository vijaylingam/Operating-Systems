#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void CsignalHandler(int sig){
    if (sig == SIGUSR1)
    {
        printf("C says I am the winner.\n");
    }
    else if (sig == SIGUSR2)
    {
        printf("C says I am the loser.\n");
    }
    return;
}

void DsignalHandler(int sig){
    if (sig == SIGUSR1)
    {
        printf("D says I am the winner.\n");
    }
    else if (sig == SIGUSR2)
    {
        printf("D says I am the loser.\n");
    }
    return;
}

int main()
{
    int pid_C, pid_D;
    int fd[2], fd1[2];

    char number1[100], number2[100];

    if(pipe(fd) == -1){
        printf("Pipe failed between P and C\n");
        return 1;
    }
    if(pipe(fd1) == -1){
        printf("Pipe failed between P and D\n");
        return 1;
    }
    //Child C
    if ((pid_C=fork()) == 0)
    {
        srand(pid_C);
        close(fd1[0]);
        close(fd1[1]);
        close(fd[0]);

        signal(SIGUSR1, CsignalHandler);
        signal(SIGUSR2, CsignalHandler);

        while(1)
        {
            int n = rand()%200; //limiting random numbers generated to upper bound of 200

            sprintf(number1, "%d", n);
            write(fd[1], number1, 100);
            sleep(1);
        }
    }
    //Child D
    else if((pid_D = fork()) == 0)
    {

        close(fd[0]);
        close(fd[1]);
        close(fd1[0]);

        signal(SIGUSR1, DsignalHandler);
        signal(SIGUSR2, DsignalHandler);

        while(1)
        {
            int n = rand()%200;
            sprintf(number2, "%d", n);	      //writing to string
            write(fd1[1], number2, 100);     //writing to pipe
            sleep(1);
        }
    }
    else
    {
        close(fd[1]);//close write end
        close(fd1[1]);//close write end

        int C_score = 0;
        int D_score = 0;
        int round = 1;

        while(C_score != 10 && D_score != 10)
        {
            int flag = rand()%2;
            printf("\nRound %d:\nP's choice of Flag: ",round);
            round++;
            if (flag==0){
                printf("MIN\n");
            }else{
                printf("MAX\n");
            }
            read(fd[0], number1, 100);
            int numberC = atoi(number1);
            read(fd1[0], number2, 100);
            int numberD = atoi(number2);

            printf("Integer received from C: %d\n", numberC);
            printf("Integer received from D: %d\n", numberD);

            if(flag == 0){
                if(numberC < numberD)
                {
                    C_score++;
                    printf("C gets 1 point\n");
                }
                else if(numberC > numberD)
                {
                    D_score++;
                    printf("D gets 1 point\n");
                }
            }
            else
            {
                if(numberC > numberD)
                {
                    C_score++;
                    printf("C gets 1 point\n");
                }
                else if(numberC < numberD)
                {
                    D_score++;
                    printf("D gets 1 point\n");
                }
            }

            printf("Current Score of C : %d\n", C_score);
            printf("Current Score of D : %d\n", D_score);
        }
        if(C_score == 10){
            printf("\nP declares C is the winner\n");
            kill(pid_C, SIGUSR1);
            kill(pid_D, SIGUSR2);
        }else
        {
            printf("\nP declares D is the winner\n");
            kill(pid_D, SIGUSR1);            //process D killed
            kill(pid_C, SIGUSR2);            //process C killed
        }
        return 1;
    }
}