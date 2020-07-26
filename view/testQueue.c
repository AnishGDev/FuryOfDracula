#include <stdio.h>
#include <stdlib.h>
#include "Queue.h"
int main(void) {
    Queue q = newQueue();
    QueueJoin(q, 10);
    QueueJoin(q,120);
    QueueJoin(q,500);
    showQueue(q);
    return EXIT_SUCCESS;
}