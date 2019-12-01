#include <iostream>
#include <random>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "shipment.h"
#include "semaphore.h"

using namespace std;

class customer
{
    const static int SIZE = 5;

    private:
    string customerName;
    int theOrder[SIZE];
    bool notified;
};

int calculateOrder();
enum {OVERGRIPS, RACKET_STRINGS, TENNIS_SHOES, TENNIS_BALLS, TENNIS_ACCESSORIES};
const int numOfItems = 5;

int main()
{
    
    int theShipment[numOfItems];

    
    string theNames[10] = 
    {
        "Patrick Antonious", "Phil Ivey", "Daniel Negreanu", "Justin Bonomo", "Erik Seidel",
        "Fedor Holz", "Tom Dwan", "Antonio Esfandiari", "Phil Hellmuth", "Jason Mercier"
    };

    int* theReturnValue;
    theReturnValue = shipment_arrival(theShipment);

    int shmid;
	int *shmBUF;
    const int BUFFSIZE = 5;

    shmid = shmget(IPC_PRIVATE, BUFFSIZE*sizeof(int), PERMS);
	shmBUF = (int*)shmat(shmid, 0, SHM_RND);

    SEMAPHORE sem(5);



    if (fork() != 0)
    {

    }

    else
    {
        
    }

    return 0;
}


int calculateOrder()
{
        customer dummy;
        const int numOvergrips = 1;
        int numRacketStrings = 0;
        int numTennisShoes = 0;
        int numTennisBalls = 0;
        int numTennisAccessories = 0;
        for (int count = 0; count < 100; count++)
        {
            for (int count = 0; count < 4; count++)
            {
                switch (count)
                {
                    case 0: numRacketStrings = rand() % 2;
                    break;
                    case 1: numTennisShoes = rand() % 2;
                    break;
                    case 2: numTennisBalls = rand() % 2;
                    break;
                    case 3: numTennisAccessories = rand() % 2;
                    break;
                }
            }

            while (*(shmBUF + OVERGRIPS * sizeof(int)) < 1);
            sem.P(OVERGRIPS);
            (*(shmBUF + OVERGRIPS * sizeof(int)))--;
            sem.V(OVERGRIPS);


            if (numRacketStrings != 0)
            {
                sem.P(RACKET_STRINGS);
                while (*(shmBUF + RACKET_STRINGS * sizeof(int)) < 1);
                (*(shmBUF + RACKET_STRINGS * sizeof(int)))--;
                sem.V(RACKET_STRINGS);
            }

            if (numTennisShoes != 0)
            {
                sem.P(TENNIS_SHOES);
                while (*(shmBUF + TENNIS_SHOES * sizeof(int)) < 1);
                (*(shmBUF + TENNIS_SHOES * sizeof(int)))--;
                sem.V(TENNIS_SHOES);
                
            }

            if (numTennisBalls != 0)
            {
                sem.P(TENNIS_BALLS);
                while (*(shmBUF + TENNIS_BALLS * sizeof(int)) < 1);
                (*(shmBUF + TENNIS_BALLS * sizeof(int)))--;
                sem.V(TENNIS_BALLS);
            }

            if (numTennisAccessories != 0)
            {
                sem.P(TENNIS_ACCESSORIES);

                while (*(shmBUF + TENNIS_ACCESSORIES * sizeof(int)) < 1);
                (*(shmBUF + TENNIS_ACCESSORIES * sizeof(int)))--;

                sem.V(TENNIS_ACCESSORIES);
            }
        } 
}


