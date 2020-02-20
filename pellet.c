//client
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHSIZE 10000

void drop();
void create();

int (*grid)[12][20];
int main(int argc, char *argv[])
{
   int shmid;
   key_t key;
   char *shm;
   char *s;

   key = 9876;
   shmid = shmget(key, SHSIZE, 0666);

   if(shmid < 0)
   {
 	    perror("shmget");
  	  exit(1);
   }
 
  shm = shmat(shmid, NULL, 0);
  grid = (int(*)[12][20])shmat(shmid, NULL, 0);
 
  for(int i=0; i < 200; i++)
  { 
	   create();
	   drop();
	   usleep(250000);
  }
  return 0;
}

void create()
{ 
   int pellet;
   pellet = rand() % 10;
   *grid[0][pellet] = 2; //writes 2 on the array
}

void drop()
{
  int q, w;
  for (q=11; q>=0; q = q--)
	{
		for (w=10; w>=0; w--)
		{
			if(*grid[q][w]==2) //if its a pellet
			{
				*grid[q+1][w]=(*grid[q+1][w])+2;
				*grid[q][w]=0;
			}
		}
	}
}