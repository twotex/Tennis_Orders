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

class Order
{
    private:
        static const int ORDERSIZE = 5;
        int index;
        int itemsOrdered[ORDERSIZE];

    public:
        Order(int theIndex, int theItemsOrdered[])
        {
            index = theIndex;

            for (int count = 0; count < ORDERSIZE; count++)
            {
                itemsOrdered[count] = theItemsOrdered[count];
            }
        }

        int getIndex()
        {
            return index;
        }

        int* getItemsOrdered()
        {
            return itemsOrdered;
        }
};

    int calculateOrder(SEMAPHORE &sem, int* sharedMemory, int index);
    int handleOrders(SEMAPHORE &sem, int* sharedMemory, int size);
    const int numOfItems = 5;
    const int parentId = 10;
    const int restockId = 11;
    int* theInventory;

int main()
{
    int theShipment[numOfItems];;

    int shmid;
	int *shmBUF;
    const int BUFFSIZE = 10;

    shmid = shmget(IPC_PRIVATE, sizeof(int) * numOfItems * BUFFSIZE, PERMS);
	shmBUF = (int*)shmat(shmid, 0, SHM_RND);

    SEMAPHORE sem(12);
    
    for (int count = 0; count < 10; count++)
    {
        sem.V(count);
    }

    sem.V(restockId);
    
    if (fork() != 0)
    {
        handleOrders(sem, shmBUF, BUFFSIZE);
    }

    else
    {
        int index = 0;

        for (int count = 1; count <= 10; count++)
        {
            if (fork() == 0)
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
            for (int count = 0; count < 5; count++)
            {
                theInventory[count] = 3000;
            }

            int* restock;

            while(true)
            {
                sem.P(restockId);
                restock = shipment_arrival(theInventory);
                theInventory = restock;
                sem.V(restockId);
            }
        }
    }

    return 0;
}


int handleOrders(SEMAPHORE &sem, int* sharedMemory, int size)
{
    vector <Order> allOrders;
    int theItems[5];

    while (true)
    {
        sem.P(parentId);
        
        for (int count = 0; count < 10; count++)
        {
            for (int zCount = 0; zCount <= 4; zCount++)
            {
                theItems[zCount] = *(sharedMemory + (count * numOfItems * sizeof(int)) + (zCount * sizeof(int)));
                cout << "Child " << count + 1 << ":   ";

                while (theItems[zCount] > theInventory[zCount]);
                cout << "Inventory " << theInventory[zCount] << " - " << theItems[zCount] << " ------ ";
                sem.P(restockId);
                theInventory[zCount] = theInventory[zCount] - theItems[zCount];
                sem.V(restockId);
                
                switch(zCount)
                {
                    case 0: cout << "Overgrips";
                    break;
                    case 1: cout << "Racket Strings";
                    break;
                    case 2: cout << "Tennis Shoes";
                    break;
                    case 3: cout << "Tennis Balls";
                    break;
                    case 4: cout << "Tennis Accessories";
                    break;
                }

                cout << endl;
            }

            Order newOrder(count, theItems);
            allOrders.push_back(newOrder);
            sem.V(count);
        }
    }
}

int calculateOrder(SEMAPHORE &sem, int* sharedMemory, int index)
{
        srand(time(0));
        const int numOvergrips = 1;

        int itemQuantity[5];
        itemQuantity[0] = 1;

        for (int count = 0; count < 100; count++)
        {
            for (int tCount = 1; tCount <= 4; tCount++)
            {
                switch (tCount)
                {
                    case 1: itemQuantity[tCount] = rand() % 2;
                    break;
                    case 2: itemQuantity[tCount] = rand() % 2;
                    break;
                    case 3: itemQuantity[tCount] = rand() % 2;
                    break;
                    case 4: itemQuantity[tCount] = rand() % 2;
                    break;
                }
            }

            sem.P(index);
            sem.V(parentId);

            for (int zCount = 0; zCount <= 4; zCount++)
            {
                *(sharedMemory + (sizeof(int) * numOfItems * index) + (zCount * sizeof(int))) = itemQuantity[zCount]; 
            }
        } 
}