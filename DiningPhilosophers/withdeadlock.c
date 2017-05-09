#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h> //importing semaphores header
#include <sys/shm.h> //for shared memory


#define LEFT i
#define RIGHT (i+1)%5
#define TIME 5

typedef int semaphore;
struct sembuf pop, vop;

semaphore* semFork;
semaphore * mutex;
int mutexID;
int semForkID;
int rscGraphID;

struct sembuf pop, vop ;

#define P(s) semop(s, &pop, 1)  /* pop is the structure we pass for doing the P(s) operation */
#define V(s) semop(s, &vop, 1)  /* vop is the structure we pass for doing the V(s) operation */

void take_left_fork(int i, int * rscGraph);
int take_right_fork(int i, int * rscGraph);
void put_forks(int i, int * rscGraph);

//Simulates behaviour of philosopher i
void philosopher(int i)
{
	int * rscGraph = shmat(rscGraphID, 0, 0);
	int think = rand()%TIME+1; //random thinking time
	int eat = rand()%TIME+1; // random eating time
	int wait = rand()%TIME+1; // random waiting time

	while(1)
	{
		sleep(think);

		int hasBothForks = 0;
		while(1){
			take_left_fork(i, rscGraph);
			sleep(wait);
	
			P(*mutex);
			if(rscGraph[i*5+i]==0)
			{
				V(*mutex);
				continue;
			}
			V(*mutex);

			int hasBothForks = take_right_fork(i, rscGraph);

			if(hasBothForks==1){
			 printf("philosopher %d hasBothForks = %d\n", i,hasBothForks);
				break;
			}
		}

		printf("Philosopher %d starts eating\n",i);
		sleep(eat);
		printf("Philosopher %d ends eating and releases forks %d (left) and %d (right)\n",i,LEFT,RIGHT);
		printf("Philosopher %d starts thinking\n",i);
		put_forks(i, rscGraph);
	}

}

void take_left_fork(int i, int * rscGraph)
{
	P(semFork[LEFT]);
	P(*mutex);
	rscGraph[i*5+i] = 1;
	V(*mutex);
	printf("Philosopher %d grabs fork %d (left)\n",i,LEFT);
}

int take_right_fork(int i, int * rscGraph)
{
	P(semFork[RIGHT]);

	P(*mutex);//Shared memory accessed
	if(rscGraph[i*5+i]==0)
	{
		V(semFork[RIGHT]);
		V(*mutex);
		return 0;
	}
	else
	{
		rscGraph[RIGHT*5 + RIGHT] = 0;
		rscGraph[i*5+(RIGHT)] = 1;
		V(*mutex);
		printf("Philosopher %d grabs fork %d (right)\n",i,RIGHT);
		return 1;
	}
}

//Philosopher puts down both left and right forks after eating
void put_forks(int i, int * rscGraph)
{
	V(semFork[LEFT]);
	P(*mutex);
	rscGraph[i*5+i] = 0;
	V(*mutex);
	V(semFork[RIGHT]);
	P(*mutex);
	rscGraph[i*5+RIGHT] = 0;
	V(*mutex);	
}

void main()
{
	int i, j;

	//getting shared memory and attaching it
	//reference on shmget used from here: http://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/shm/shmget.html
	rscGraphID = shmget(IPC_PRIVATE, 5*5*sizeof(int), 0777|IPC_CREAT); //0777 refers to octal representation
	int * rscGraph = shmat(rscGraphID, 0, 0);

	semForkID = shmget(IPC_PRIVATE, 5*sizeof(semaphore), 0777|IPC_CREAT);
	semFork = (semaphore *)shmat(semForkID, 0, 0);

	mutexID = shmget(IPC_PRIVATE, 1*sizeof(semaphore), 0777|IPC_CREAT);
	mutex = (semaphore *)shmat(mutexID, 0, 0);	

	// initialising resource graph elements to 0
	for(i=0;i<5;++i)
	{
		for(j=0;j<5;++j)
		{
			rscGraph[i*5+j] = 0;
		}
	}

	for(i=0;i<5;i++)
	{
		semFork[i] = semget(IPC_PRIVATE, 1, 0777|IPC_CREAT);
		semctl(semFork[i], 0, SETVAL, 1);
	}

	*mutex = semget(IPC_PRIVATE, 1, 0777|IPC_CREAT);
	semctl(*mutex, 0, SETVAL, 1);

	pop.sem_num = vop.sem_num = 0;
	pop.sem_flg = vop.sem_flg = 0;
	pop.sem_op = -1 ; vop.sem_op = 1 ;

	for(i=0;i<5;i++)
	{
		if(fork()==0)
		{
			printf("Philosopher %d starts thinking\n",i);
			philosopher(i);
			exit(1);
		}
	}

	//Parent process
	while(1)
	{
		P(*mutex);
		for(i=0;i<5;++i)
		{
			if(rscGraph[i*5+i]==0)//Philosopher i does not have left fork, So no deadlock
				break;
		}
		
		if(i==5)//All philosophers holding left forks, So deadlock detected
		{
			printf("Parent detects deadlock, going to initiate recovery\n");
			int toPreempt = rand()%5;
			printf("Parent preempts Philosopher %d\n", toPreempt);
			rscGraph[toPreempt*5+toPreempt] = 0;
			V(semFork[toPreempt]);	
		}
		V(*mutex);
		sleep(1);
	}
	
}
