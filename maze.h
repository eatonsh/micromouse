#pragma once
#include <string>
#include <stdbool.h>
#include "API.h"

#define MAZE_SIZE 16
#define MAX_CELL MAZE_SIZE*MAZE_SIZE

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

struct Coordlist {
    int size;
    Coord* coords;
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

struct Maze {
    Coord mouse_pos;
    Direction mouse_dir;

    int distances[MAZE_SIZE][MAZE_SIZE];
    int cellWalls[MAZE_SIZE][MAZE_SIZE]; //list of all walls in maze
        // how maze remembers discovered walls
    bool explored[MAZE_SIZE][MAZE_SIZE];

    Coord* goalPos;

    std::string commands;
};

void initCenterGoalPos(Maze* maze);
void initStartGoalPos(Maze* maze);

void scanWalls(Maze* maze);
void updateSimulator(Maze maze);
void updateMousePos(Coord* pos, Direction dir);
bool onGoal(Maze maze);
bool isCoordSame(Coord c1, Coord c2);

CellList* getNeighborCells(Maze* maze, Coord c);
void floodFill(Maze* maze, bool to_start);