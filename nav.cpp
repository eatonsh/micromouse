#include "maze.h"
#include "API.h"
#include "queue.h"
#include "nav.h"

char northTurn(int dx, int dy, Maze* maze) {
    if(dx == 1) {
        return 'r';
    } else if (dx == -1) {
        return 'l';
    } else if (dy == -1) {
        return 'u';
    }
    return ' ';
}

char eastTurn(int dx, int dy, Maze* maze) {
    if(dy == 1) {
        return 'l';
    } else if (dy == -1) {
        return 'r';
    } else if (dx == -1) {
        return 'u';
    }
    return ' ';
}

char southTurn(int dx, int dy, Maze* maze) {
    if(dx == -1) {
        return 'r';
    } else if (dx == 1) {
        return 'l';
    } else if (dy == 1) {
        return 'u';
    }
    return ' ';
}

char westTurn(int dx, int dy, Maze* maze) {
    if(dy == -1) {
        return 'l';
    } else if (dy == 1) {
        return 'r';
    } else if (dx == 1) {
        return 'u';
    }
    return ' ';
}

void turnToMinDist(Maze* maze) {
    Coord cur_pos = maze->mouse_pos;
    Coord next_pos = maze->mouse_pos;
    int minDist = maze->distances[cur_pos.y][cur_pos.x];

    //MOVEMENT
    //get neighbor cells
    //check if blocked and then check distance values 
    CellList* surround = getNeighborCells(maze, cur_pos);
    for(int i = 0 ; i < surround->size; i++) {
        Cell cur = surround->cells[i];
        if(cur.blocked == false) {
            if(maze->distances[cur.pos.y][cur.pos.x] < minDist) {
                minDist = maze->distances[cur.pos.y][cur.pos.x];
                next_pos = cur.pos;
            }
        }
    }
    
    int dx = next_pos.x - maze->mouse_pos.x;
    int dy = next_pos.y - maze->mouse_pos.y;
    char next_direction;

    if(maze->mouse_dir == NORTH) {
        next_direction = northTurn(dx, dy, maze);
    } else if(maze->mouse_dir == EAST) {
        next_direction = eastTurn(dx, dy, maze);
    } else if(maze->mouse_dir == SOUTH) {
        next_direction = southTurn(dx, dy, maze);
    } else if(maze->mouse_dir == WEST){
        next_direction = westTurn(dx, dy, maze);
    }

    std::cerr << "got here " << next_direction << std::endl;

    if(next_direction  == 'r') {
        API::turnRight();
        maze->mouse_dir = (Direction)((maze->mouse_dir + 1) % 4);
        maze->commands += 'R';
    } else if(next_direction == 'l') {
        API::turnLeft();
        maze->mouse_dir = (Direction)((maze->mouse_dir + 3) % 4);
        maze->commands += 'L';
    } else if (next_direction == 'u') {
        API::turnLeft();
        maze->mouse_dir = (Direction)((maze->mouse_dir + 3) % 4);
        API::turnLeft();
        maze->mouse_dir = (Direction)((maze->mouse_dir + 3) % 4);
        maze->commands += 'L';
        maze->commands += 'L';
    }
}