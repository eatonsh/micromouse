#include "maze.h"
#include "API.h"
#include "queue.h"

char dir_chars[4] = {'n', 'e', 's', 'w'};
int dir_mask[4] = {0b1000, 0b0100, 0b0010, 0b0001};

/*void loopGoal() {
    if (mouse_pos.x == 7) {
        goright
        if y == 7 {
            goup
        } else {
            godown
        }
        goleft
        go opposite 
    } else {
        go left
        if y == 7 {
            goup
        } else {
            godown
        }
        goright
        go opposite
    }
}*/

bool isCoordSame(Coord c1, Coord c2) {
    return (c1.x == c2.x && c1.y == c2.y);
}

bool onGoal(Maze maze) {
    for (int i = 0; i < 4; i++) {
        // std::cerr << maze.mouse_pos.x << maze.mouse_pos.y << std::endl;
        // std::cerr << maze.goalPos[i].x << maze.goalPos[i].y << std::endl;
        if (isCoordSame(maze.mouse_pos, maze.goalPos[i])) {
            return true;
        }
    }
    return false;
}

void initCenterGoalPos(Maze* maze) {
    maze->goalPos[0].x = 7;
    maze->goalPos[0].y = 7;
    maze->goalPos[1].x = 8;
    maze->goalPos[1].y = 7;
    maze->goalPos[2].x = 7;
    maze->goalPos[2].y = 8;
    maze->goalPos[3].x = 8;
    maze->goalPos[3].y = 8;
}

void initStartGoalPos(Maze* maze) {
    maze->goalPos[0].x = 0;
    maze->goalPos[0].y = 0;
    maze->goalPos[1].x = 0;
    maze->goalPos[1].y = 0;
    maze->goalPos[2].x = 0;
    maze->goalPos[2].y = 0;
    maze->goalPos[3].x = 0;
    maze->goalPos[3].y = 0;
}

void scanWalls(Maze* maze) {
    if (API::wallFront()) {
        maze->cellWalls[maze->mouse_pos.y][maze->mouse_pos.x] |= dir_mask[maze->mouse_dir];
        
            // bitwise operation to detect wall in front (8)
            // maze.cellwalls[mouseposy][mouseposx] or operation against 
        //std::cerr << "FRONT Wall Detected: " << maze->cellWalls[maze->mouse_pos.y][maze->mouse_pos.x] << std::endl;
    }
    if (API::wallRight()) {
        maze->cellWalls[maze->mouse_pos.y][maze->mouse_pos.x] |= dir_mask[(maze->mouse_dir + 1) % 4];  
        //std::cerr << "RIGHT Wall Detected: " << maze->cellWalls[maze->mouse_pos.y][maze->mouse_pos.x] << std::endl;
        // printing added to wall front
        //BITWISE OR is ADDITION
    }
    if (API::wallLeft()) {
        maze->cellWalls[maze->mouse_pos.y][maze->mouse_pos.x] |= dir_mask[(maze->mouse_dir + 3) % 4];
        //std::cerr << "LEFT Wall Detected: " << maze->cellWalls[maze->mouse_pos.y][maze->mouse_pos.x] << std::endl;
    }
}

void updateSimulator(Maze maze) { // redraws the maze in simulator after each loop in main
    for(int x = 0; x < MAZE_SIZE; x++) 
    {
        for(int y = 0; y < MAZE_SIZE; y++) 
        {
            API::setText(x, y, std::to_string(maze.distances[y][x]));
            //previously print statement caused error causing updatesimulator to crash everytime it ran
            if (maze.cellWalls[y][x] & NORTH_MASK)
                API::setWall(x, y, 'n');
                // sets color of cells walls if NORTH MASK DISCOVERED
                // BITWISE AND is MULTIPLICATION
                // When sum of cell walls is multiplied to each individual mask, you get all sides individually
            if (maze.cellWalls[y][x] & EAST_MASK)
                API::setWall(x, y, 'e');
            if (maze.cellWalls[y][x] & SOUTH_MASK)
                API::setWall(x, y, 's');
            if (maze.cellWalls[y][x] & WEST_MASK)
                API::setWall(x, y, 'w');
        }
    }
}

void updateMousePos(Coord* pos, Direction dir) {
    //depending on the mouse direction, increment position by one
    if (dir == NORTH)
        pos->y++;
    if (dir == SOUTH)
        pos->y--;
    if (dir == WEST)
        pos->x--;
    if (dir == EAST)
        pos->x++;
    
    // std::cerr << "inside function: (" << pos.x << ", " << pos.y << ")" << std::endl;
}

CellList* getNeighborCells(Maze* maze, Coord c) { //to be called in a while loop within Floodfill when setting each cell
    CellList* cellList = (CellList*)malloc(sizeof(CellList));  
    //CellList named cellList allocating memory for all cells in cellList  

    // Directions: W E S N[]
    int dirX[] = {-1, 1, 0, 0}; //possible directions away from center WEST, EAST
    int dirY[] = {0, 0, -1, 1}; //SOUTH, NORTH
    int count = 0;
    int newX[4];
    int newY[4];
    int newDirs[4]; //direction of the neighbor cell from cur pos

    for(int i = 0; i < 4; i++) { //for loops creates position offset from current cell for 4 adjacent cells
        int x = c.x + dirX[i]; //left right depending on index
        int y = c.y + dirY[i]; //up down
        //std::cerr << "neighbor" << i << "is" << x << y << std::endl;
        //check boundaries for cell needs to include blocked or not
        if(x >= 0 && y >= 0 && x < MAZE_SIZE && y < MAZE_SIZE) {
            newX[count] = x;
            newY[count] = y;
            if (i == 0) {
                newDirs[count] = WEST_MASK; //setting the direction of the neighbor cell respective to cur pos
            }
            if (i == 1) {
                newDirs[count] = EAST_MASK;
            }
            if (i == 2) {
                newDirs[count] = SOUTH_MASK;
            }
            if (i == 3) {
                newDirs[count] = NORTH_MASK;
            }
            count++;
        }
    }

    cellList->size = count;
    cellList->cells = (Cell*)malloc(cellList->size * sizeof(Cell)); 

    for (int i = 0; i < count; i++) {
        bool cur_blocked = false; //intialize to not blocked
        //check what direction newDirs[i] corresponds to check direction_mask
        //direction matters for floodfill to check isBlocked
        if (newDirs[i] == WEST_MASK) {
            if (maze->cellWalls[c.y][c.x] & WEST_MASK || maze->cellWalls[c.y][c.x-1] & EAST_MASK) { 
                //condition makes it so when get neighbor is called by floodfill, it checks that the other side is also blocked or not
                //if no OR condition, cell walls is  screwed up so it thinks theres a wall from one dir but not the other cuz different cell
                cur_blocked = true;
            }
        }
        if (newDirs[i] == EAST_MASK) {
            if (maze->cellWalls[c.y][c.x] & EAST_MASK || maze->cellWalls[c.y][c.x+1] & WEST_MASK) {
                cur_blocked = true;
            }
        }
        if (newDirs[i] == SOUTH_MASK) {
            if (maze->cellWalls[c.y][c.x] & SOUTH_MASK || maze->cellWalls[c.y-1][c.x] & NORTH_MASK) {
                cur_blocked = true;
            }
        }
        if (newDirs[i] == NORTH_MASK) {
            if (maze->cellWalls[c.y][c.x] & NORTH_MASK || maze->cellWalls[c.y+1][c.x] & SOUTH_MASK) {
                cur_blocked = true;
            }
        }
        //assigning each cell in cellList with the new coords
        cellList->cells[i] = (Cell){(Coord){newX[i], newY[i]}, NORTH, cur_blocked}; 
        //Coordinate should be X, Y, not Y, X, even though everything else is Y, X
    }

    return cellList;
};

void floodFill(Maze* maze, bool to_start) {
    Queue q; //INIT QUEUE

    //intialize maze distances to max
    for(int x = 0; x < MAZE_SIZE; x++) {
        for(int y = 0; y < MAZE_SIZE; y++) {
            maze->distances[y][x] = MAX_CELL;
        }
    }

    int b; 
    if (to_start == true) {
        b = 1;
    } else {
        b = 4;
    }

    //set goalPos to 0
    for(int i = 0; i < b; i++) {
        Coord g = maze->goalPos[i];
        maze->distances[g.y][g.x] = 0;
    }

    //init q head and tail to 0/-1
    initQueue(&q);

    //add goalPos to queue
    for(int i = 0 ; i < b; i++) {
        //std::cerr << maze->goalPos[i].x << ", " << maze->goalPos[i].y << std::endl;
        enqueue(&q, maze->goalPos[i]);
        //std::cerr << q.coords[i].x << ", " << q.coords[i].y << std::endl;
    }

    //while queue has more than 0 cells
    while(isQEmpty(q) == 0) {
        //set current position as head in queue
        Coord cur_pos = dequeue(&q);

        //set the new distances as the current distance++
        int newDist = maze->distances[cur_pos.y][cur_pos.x] + 1;

        //list of cells from getneightbors of current position
        CellList* neighbors = getNeighborCells(maze, cur_pos);

        for(int i = 0; i < neighbors->size; i++) { //for all cell in list
            // Useful logs: 
            //std::cerr << "CUR= " << cur_pos.y << ", " << cur_pos.x << " NEIGHBOR= " << cur.pos.y << ", " << cur.pos.x << " BLCOKED= " << cur.blocked << std::endl;
            //std::cerr << cur.pos.x << "," << cur.pos.y << std::endl;
            //std::cerr << "NIEGHBORS SIZE" << neighbors->size << std::endl;

            // Operate on the current cell's neighbors
            Cell cur = neighbors->cells[i];
            if(cur.blocked == false) { //if the neighbor is not blocked
                if(maze->distances[cur.pos.y][cur.pos.x] > newDist) { //if distance in neighbor cell greater than new distanc
                    maze->distances[cur.pos.y][cur.pos.x] = newDist; //replace distance
                    enqueue(&q, cur.pos); //add the cell to the end of queue, enqueue() increments tail
                }
            }
        }
    }
    
    // Prints entire distances array
    /*for (int i = 0; i < MAZE_SIZE; i++){
        for (int j =0; j < MAZE_SIZE; j++) {
            //std::cerr << "Maze Distance at " << i << "," << j << " = " << maze->distances[i][j] << std::endl;
            //log("getting here");
        }
    }*/
}