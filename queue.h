#pragma once

#include <iostream>
#include <string>
#include <stdbool.h>
#include <cstdlib>
#include "maze.h"

struct Queue {
    Coord coords[MAX_CELL];
    int head, tail;
};

void initQueue(Queue *q);
void enqueue(Queue *q, Coord coord);
Coord dequeue(Queue *q);
bool isQEmpty(Queue q);

void printQueue(Queue q);