#include <stdio.h>
#include <stdlib.h>
#include "queue.h"


int main(void){

    Queue* q = createQueue();

    addQueue(q, 10, 3);
    addQueue(q, 1, 1);
    addQueue(q, 2, 3);
    addQueue(q, 1, 4);
    addQueue(q, 5, 2);

    addQueue(q, 10, 3);
    addQueue(q, 29, 4);
    addQueue(q, 3, 1);
    addQueue(q, 7, 3);
    addQueue(q, 12, 2);

    // printf("\n");
    // printQueue(q);
    // printf("FCFS: %.2f", FCFS(q));
    // printf("\n");
    // printf("STJ: %.2f", SJF(q));
    // printf("\n");
    // printf("Priority: %.2f", priority(q));
    // printf("\n");
    printf("Round Robin: %.2f", roundRobin(q, 10));
    printf("\n");


    return 0;
}