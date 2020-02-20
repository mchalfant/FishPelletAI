//client
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHSIZE 10000

void move();
void moveL();
void moveR();
void moveo();
int pFish= 4;

int (*grid)[12][20];

const int FISH = 2;
const int EAT_PELLET = 3;
const int BOTTOM_ROW = 10;
const int MAX_APP_UPTIME = 200;

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

   int fish = 1;
   *grid[BOTTOM_ROW][pFish]=fish;
   
   for(int i = 0; i < MAX_APP_UPTIME; i++)
   {
	  move();
	  usleep(250000);
   }
   return 0;
}

bool IsMoveAllowed(int offset)
{
   bool results = false;
   if(*grid[BOTTOM_ROW - offset][pFish - (1 + offset)] == 2)
   {
	   moveL();
	   results = true;
   }
   else if(*grid[BOTTOM_ROW - offset][pFish + (1 + offset)] == 2)
   {
	   moveR();
	   results = true;
   }
   return results;
}

void move() {
   int i = 0; 
   bool canMove = false;

   while(canMove == false || i = 5)
   {
	   canMove = IsMoveAllowed(i);
	   i++;
   }
}

void moveL()
{
	if(pFish < BOTTOM_ROW & pFish > 0)
	{
		*grid[10][pFish]= (*grid[10][pFish]) -1;
		pFish = pFish - 1;	
		*grid[10][pFish]= *grid[10][pFish] + 1;
 	}
}

void moveR()
{
	if(pFish < (BOTTOM_ROW - 1) & pFish >= 0)
	{
		*grid[10][pFish]= (*grid[10][pFish]) -1;
		pFish = pFish + 1;	
		*grid[10][pFish]= *grid[10][pFish] + 1;
 	}
}