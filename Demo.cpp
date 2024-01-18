#include <iostream>
#include <string>
#include <stdbool.h>
#include "API.h"

#define MAZE_SIZE 16

void initQueue(Queue *q);
void enqueue(Queue *q, Cell);
Cell dequeue(Queue *q);
bool isQEmpty(Queue q);
void floodFill(Cell);

Maze maze;

//int temp_value = 20;

char dir_chars[4] = {'n', 'e', 's', 'w'};
int dir_mask[4] = {0b1000, 0b0100, 0b0010, 0b0001};

int main(int argc, char* argv[]) 
{
    maze.mouse_pos = (Coord){0, 0};
    maze.mouse_dir = NORTH;

    // 4. POINTER DEMO
    // pointer_demo(temp_value);
    //std::cerr << temp_value << std::endl;

    // 1. FILL THIS IN
    for(int x = 0; x < MAZE_SIZE; x++) {
        for(int y = 0; y < MAZE_SIZE; y++) {
            maze.distances[y][x] = x + y;
        }
    }

    while (true) {
        scanWalls(&maze);
        CellList* adjacentCells = getNeighborCells(&maze);
        for(int i = 0; i < adjacentCells->size; i++) {
            std::cerr << adjacentCells->cells[i].pos.x << ", " << adjacentCells->cells[i].pos.y << std::endl;
        }
        
        free(adjacentCells->cells);
        free(adjacentCells);


        updateSimulator(maze);

        std::cerr << "(" << maze.mouse_pos.x << ", " << maze.mouse_pos.y << ")" << std::endl;

        // Left Wall Follow Codes
        // if (!API::wallLeft()) 
        // {
        //     API::turnLeft();
        //     maze.mouse_dir = (Direction)((maze.mouse_dir + 3) % 4);
        // }
        // while (API::wallFront()) 
        // {
        //     API::turnRight();
        //     maze.mouse_dir = (Direction)((maze.mouse_dir + 1) % 4);
        // }

        API::moveForward();
    
        // 3. UPDATE THIS WITH POINTERS updateMousePos
        updateMousePos(&maze.mouse_pos, maze.mouse_dir);

        // 5. MOVE TO updateSimulator() + scanWalls() FUNCTION
    }
}

void log(const std::string& text) 
{
    std::cerr << text << std::endl;
}

void pointer_demo(int* var)
{
    *var = 42;  //asterick is a derference
}

enum Direction {
    NORTH = 0,
    EAST = 1,
    SOUTH = 2,
    WEST = 3
};

enum DirectionBitmask {
    NORTH_MASK = 0b1000,
    EAST_MASK  = 0b0100,
    SOUTH_MASK = 0b0010,
    WEST_MASK  = 0b0001
};

struct Coord {
    int x;
    int y;
};

struct Cell {
    Coord pos;
    Direction dir;
    bool blocked;
};

struct CellList {
    int size;
    Cell* cells;
};

struct Queue {
    Cell cell[MAZE_SIZE * MAZE_SIZE];
    int head, tail;
};

struct Maze {
    Coord mouse_pos;
    Direction mouse_dir;

    int distances[MAZE_SIZE][MAZE_SIZE];
    int cellWalls[MAZE_SIZE][MAZE_SIZE];

    Coord* goalPos;
};

//Queue Functions
void initQueue(Queue *q) {
    q->head = -1;
    q->tail = -1;
}

void enqueue(Queue *q, Cell cell) {
    if(q->tail == MAZE_SIZE * MAZE_SIZE - 1)
        return;
    if(q->head == -1)
        q->head = 0;
    q->tail++;
    q->cell[q->tail] = cell;
}

// Cell dequeue(Queue *q) {
//     Cell cell;
//     if(isQEmpty(*q)) {
//         cell.pos = -1;
//         cell.dir = -1;
//     }
// }

bool isQEmpty(Queue q) {
    return q.head == -1;
}

// 2. FILL THIS IN
void updateSimulator(Maze maze) // redraws the maze in simulator after each loop in main
{
    for(int y = 0; y < MAZE_SIZE; y++) {
        for(int x = 0; x < MAZE_SIZE; x++) {
            API::setText(x, y, std::to_string(maze.distances[y][x]));
            if(maze.cellWalls[y][x] & NORTH_MASK) {
                API::setWall(x, y, 'n');
            }
            if(maze.cellWalls[y][x] & EAST_MASK) {
                API::setWall(x, y, 'e');
            }
            if(maze.cellWalls[y][x] & SOUTH_MASK) {
                API::setWall(x, y, 's');
            }
            if(maze.cellWalls[y][x] & WEST_MASK) {
                API::setWall(x, y, 'w');
            }
        }
    }
}

// 5. FILL THIS IN
void scanWalls(Maze* maze)
{
    if (API::wallFront())
        maze->cellWalls[maze->mouse_pos.y][maze->mouse_pos.x] |= dir_mask[maze->mouse_dir];
    if (API::wallRight())
        maze->cellWalls[maze->mouse_pos.y][maze->mouse_pos.x] |= dir_mask[(maze->mouse_dir + 1) % 4];
    if (API::wallLeft())
        maze->cellWalls[maze->mouse_pos.y][maze->mouse_pos.x] |= dir_mask[(maze->mouse_dir + 3) % 4];
}

// 3. FILL THIS IN
void updateMousePos(Coord* pos, Direction dir)
{
    if (dir == NORTH)
        pos->y++;
    if (dir == SOUTH)
        pos->y--;
    if (dir == WEST)
        pos->x--;
    if (dir == EAST)
        pos->x++;
    //std::cerr << "inside function: (" << pos->x << ", " << pos->y << ")" << std::endl;
}

// 6. FILL THIS IN
CellList* getNeighborCells(Maze* maze)
{
    CellList* cellList = (CellList*)malloc(sizeof(CellList));
    

    //some logic to set the cell list size
    //cellList->size = 3;
    cellList->cells = (Cell*)malloc(cellList->size * sizeof(Cell));  //malloc reserves amt of memory for ptr var

    // check if north cell
    // if(true){
    //     cellList->cells[i] = (Cell){(Coord){24, 567}, NORTH, true};
    //     i++;
    // }
    cellList->size = 2;
    cellList->cells = (Cell*)malloc(cellList->size * sizeof(Cell));

    cellList->cells[0] = (Cell){(Coord){24, 25}, NORTH, false};
    cellList->cells[1] = (Cell){(Coord){4, 5}, WEST, true};
    // check if south cell, west, and east

    return cellList;
};

// void floodFill(Cell destination) {
    
// }