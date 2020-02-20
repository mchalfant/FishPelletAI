#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHSIZE 10000

int (*grid)[12][20]; 
void display();
void show();
void create();

int space = 0;
int pFish= 4;
int missed = 0;
int ate = 0;

const int EMPTY_CELL = 0;
const int FISH = 1;
const int PELLET 2;
const int EAT_PELLET = 3;
const int BOTTOM_ROW = 10;
const int MAX_APP_UPTIME = 20;

int main(int argc, char *argv[])
{
   int shmid;
   key_t key;
   char *shm;
   char *s;
   
   key = 9876;
   shmid = shmget(key, SHSIZE, IPC_CREAT | 0666);

   if(shmid < 0)
   {
 	  perror("shmget");
 	  exit(1);
   }

   shm = shmat(shmid, NULL, 0);
   if(shm == (char *) -1)
   {
 	  perror("shmat");
 	  exit(1);
   }

   grid = (int(*)[12][20])shmat(shmid, NULL, 0);

   if(grid == (int(*)[12][20]) -1)
   {	
		perror("shmget");
		exit(1);
   }
	
   printf("hello\n");

   intializeArray();
   show();

   sleep(1);
   printf("\n");

   pid_t Pfish = fork();

   if (Pfish < 0)
   {
	   perror("fork failed).");
	   exit(0);
   }
   else if (Pfish == 0)
   {
	   char *args[2] = {"./pellet.out", NULL};
	   execvp(args[0], args);
   }
	
   pid_t pellet = fork();
   if (pellet < 0) 
   {
	   perror("fork failed.");
	   exit(0);
   }
   else if (pellet == 0)
   {
	   char *args[2] = {"./fish.out", NULL};
	   execvp(args[0], args);
   }

   int loop;
   for(loop = 0; loop < 60; loop++)
   {	
	   show();
	   usleep(250000);
   }

   shmdt((void *)grid);
   shmctl(shmid, IPC_RMID, NULL); //terminate shared memory;
   return 0;
}

void intializeArray()
{
   for (int j = 0; j < 12; j = j++)
   {
	   for (int k = 0; k < 20; k = k++)
	   {
		   *grid[j][k]=0;
	   }		
   }
}

void display(){
   for (int j = 0; j < BOTTOM_ROW; j++)
   {
      for (int k = 0; k < BOTTOM_ROW; k++){
         if (*grid[j][k] == EMPTY_CELL)
            printf(". ");	
         else if (*grid[j][k] == FISH)
            printf("f "); 	
         else if (*grid[j][k] == PELLET)
            printf("0 "); 	
         else if (*grid[j][k] == EAT_PELLET)
            printf("F ");   
		 }
	  printf("\n");
	}
}

void show()
{
	//missed count
	for(int i = 0; i < MAX_APP_UPTIME; i++)
	{
		if (*grid[BOTTOM_ROW + 1][i]==2)
		{
			*grid[BOTTOM_ROW][i] = 0;
			missed = missed + 1;
		}
	}	

   printf(" \n");
   for ( j = 1; j < BOTTOM_ROW + 1; j= j++)
   {
		for (k = 0; k < BOTTOM_ROW; k = k++)
		{
			if (*grid[j][k] == EMPTY_CELL)
				printf(". ");	
			else if (*grid[j][k] == FISH)
				printf("f "); 	
			else if (*grid[j][k] == PELLET)
				printf("0 "); 	
			else if (*grid[j][k] == EAT_PELLET)
				printf("F ");   
			space++;
			if (space == BOTTOM_ROW){
				printf(" \n");
				space = 0;
			}
		}
	}

	for(int q = 0 ; q < BOTTOM_ROW ; q = q++)
	{
		if (*grid[BOTTOM_ROW][q]==3)
		{
			*grid[BOTTOM_ROW][q] = 0;
			ate = ate + 1;
			*grid[BOTTOM_ROW][q] = 1;
		}
	}

	printf("missed: %d\n", missed);
	printf("ate: %d\n\n", ate);
}