#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>


int main(int argc, char * argv[])
{
    if(argc == 2){
        int randomArray[atoi(argv[1])];
        int n, i, status, childPid;
        n = atoi(argv[1]);

        printf("Value of n is: %d \n", n);

        for(i=0;i<n;++i)
            randomArray[i] = rand()%128;

        printf("Unsorted array: ");
        for(i=0;i<n;++i)
            printf("%d ",randomArray[i]);
        printf("\n");


        if((childPid = fork()) == 0)//Child process
        {
            paramax(randomArray, 0, n-1);
        }
        else
        {
            waitpid(childPid, &status, 0);//wait for child to complete
            int max = status >> 8;
            printf("The maximum value in the array is : %d\n",max);
        }
    }else{
        printf("Invalid arguments\n");
    }

}

int paramax(int randomArray[], int left, int right)
{

    int status;
    // process computes minimum element in the chunk becase there are less than 10 elements in the array
    if(right - left < 9)
    {
        int max = -1;
        int i;
        for(i = left; i <= right; ++i)
        {
            //1st level of bubble sort
            if(randomArray[i] > max)
                max = randomArray[i];
        }

        printf("Max = %d \t PID = %d \t parent PID = %d \n", max, getpid(), getppid());
        exit(max);
    }

    else
    {
        //break the array into two halves
        int child1Pid, child2Pid;
        if((child1Pid = fork()) == 0)
        {
            right = (left + right)/2;
            paramax(randomArray, left, right);
        }
        if((child2Pid = fork()) == 0)
        {
            left = (left + right)/2 + 1;
            paramax(randomArray, left, right);
        }
        else
        {
            waitpid(child1Pid, &status, 0);
            int leftmax = status >> 8;
            waitpid(child2Pid, &status, 0);
            int rightmax = status >> 8;

            int max = leftmax>rightmax?leftmax:rightmax; //ternary operator

            printf("Max = %d \t PID = %d \t parent PID = %d \n", max, getpid(), getppid());
            exit(max);
        }
    }
}
