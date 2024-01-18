#include "queue.h"

void initQueue(Queue *q) {
    q->head = 0;
    q->tail = 0;
}

void enqueue(Queue *q, Coord coord) {
    if(q->tail == MAX_CELL - 1)
        return;
    q->coords[q->tail] = coord;
    q->tail++;
}

Coord dequeue(Queue *q) {
    Coord coord;
    coord = q->coords[q->head];
    q->head++;
    return coord; //return the dequeued coord
}

bool isQEmpty(Queue q) {
    return q.head == q.tail;
}