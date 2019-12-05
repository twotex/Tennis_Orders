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

int calculateOrder(SEMAPHORE &sem, int* sharedMemory, int index);
int handleOrder(SEMAPHORE &sem, int* sharedMemory, int size);
enum {OVERGRIPS, RACKET_STRINGS, TENNIS_SHOES, TENNIS_BALLS, TENNIS_ACCESSORIES};
const int numOfItems = 5;

int main()
{
    
    
    int theShipment[numOfItems];

    int* theReturnValue;
    theReturnValue = shipment_arrival(theShipment);

    int shmid;
	int *shmBUF;
    const int BUFFSIZE = 10;

    shmid = shmget(IPC_PRIVATE, sizeof(int) * numOfItems * BUFFSIZE, PERMS);
	shmBUF = (int*)shmat(shmid, 0, SHM_RND);

    SEMAPHORE sem(12);
    
    for (int count = 0; count < 12; count++)
    {
        sem.V(count);
    }
    
    if (fork() != 0)
    {
        handleOrder(sem, shmBUF, BUFFSIZE);
    }

    else
    {
        int index = 0;

        for (int count = 1; count <= 10; count++)
        {
            if (fork == 0)
            {
                index = count;
                break;
            }
        }

        if (index != 10)
        {
            calculateOrder(sem, shmBUF, index);
        }

        else
        {
            //Add header file here
        }
    }

    return 0;
}


int handleOrder(SEMAPHORE &sem, int* sharedMemory, int size)
{
    int inventory[size];

    for (int count = 0; count < size; count++)
    {
        inventory[count] = 1000;
    }

    vector <Order> allOrders;

    int theItems[5];

    while (true)
    {
        for (int count = 0; count < 10; count++)
        {
            for (int zCount = 0; zCount <= 4; zCount++)
            {
                theItems[zCount] = *(sharedMemory + (count * numOfItems * sizeof(int) + (zCount + sizeof(int))));
            }
        }
    }
}

int calculateOrder(SEMAPHORE &sem, int* sharedMemory, int index)
{
        srand(time(0));
        customer dummy;
        const int numOvergrips = 1;

        int itemQuantity[5];
        itemQuantity[0] = 1;

        for (int count = 0; count < 100; count++)
        {
            for (int count = 1; count <= 4; count++)
            {
                switch (count)
                {
                    case 1: itemQuantity[count] = rand() % 2;
                    break;
                    case 2: itemQuantity[count] = rand() % 2;
                    break;
                    case 3: itemQuantity[count] = rand() % 2;
                    break;
                    case 4: itemQuantity[count] = rand() % 2;
                    break;
                }
            }

            for (int count = 0; count <= 4; count++)
            {
                *(sharedMemory + (sizeof(int) * numOfItems * index) + (count * sizeof(int))) = itemQuantity[count]; 
            }

            sem.P(index);
        } 
}

class Order
{
    private:
        static const int ORDERSIZE = 5;
        string name;
        int itemsOrdered[ORDERSIZE];

        Order(string theName, int theItemsOrdered[])
        {
            name = theName;

            for (int count = 0; count < ORDERSIZE; count++)
            {
                itemsOrdered[count] = theItemsOrdered[count];
            }
        }

    public:
        string getName()
        {
            return name;
        }

        int* getItemsOrdered()
        {
            return itemsOrdered;
        }
};



/*
    while (*(sharedMemory + OVERGRIPS * sizeof(int)) < 1);
            sem.P(OVERGRIPS);
            (*(sharedMemory + OVERGRIPS * sizeof(int)))--;
            sem.V(OVERGRIPS);


            if (numRacketStrings != 0)
            {
                sem.P(RACKET_STRINGS);
                while (*(sharedMemory + RACKET_STRINGS * sizeof(int)) < 1);
                (*(sharedMemory + RACKET_STRINGS * sizeof(int)))--;
                sem.V(RACKET_STRINGS);
            }

            if (numTennisShoes != 0)
            {
                sem.P(TENNIS_SHOES);
                while (*(sharedMemory + TENNIS_SHOES * sizeof(int)) < 1);
                (*(sharedMemory + TENNIS_SHOES * sizeof(int)))--;
                sem.V(TENNIS_SHOES);
                
            }

            if (numTennisBalls != 0)
            {
                sem.P(TENNIS_BALLS);
                while (*(sharedMemory + TENNIS_BALLS * sizeof(int)) < 1);
                (*(sharedMemory + TENNIS_BALLS * sizeof(int)))--;
                sem.V(TENNIS_BALLS);
            }

            if (numTennisAccessories != 0)
            {
                sem.P(TENNIS_ACCESSORIES);

                while (*(sharedMemory + TENNIS_ACCESSORIES * sizeof(int)) < 1);
                (*(sharedMemory + TENNIS_ACCESSORIES * sizeof(int)))--;

                sem.V(TENNIS_ACCESSORIES);
            }
*/
