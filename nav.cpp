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

// Returns true if there is a direction to turn to, false if there is not.
bool turnToMinDist(Maze* maze, bool search, bool moving) {
    Coord cur_pos = maze->mouse_pos;
    Coord next_pos = maze->mouse_pos;
    int minDist = maze->distances[next_pos.y][next_pos.x];
    int maxDist = maze->distances[next_pos.y][next_pos.x];

    //MOVEMENT
    //get neighbor cells
    //check if blocked and then check distance values 
    CellList* surround = getNeighborCells(maze, cur_pos);
    bool cur_exp = true;
    bool max_bro = false;
    for(int i = 0 ; i < surround->size; i++) {
        Cell cur = surround->cells[i];
        int curDist = maze->distances[cur.pos.y][cur.pos.x];
        bool curExp = maze->explored[cur.pos.y][cur.pos.x];
        std::cerr << maze->mouse_pos.x << ", " << maze->mouse_pos.y << std::endl;
        if (maze->mouse_pos.x == 3 && maze->mouse_pos.y == 8) {
            //std::cerr << cur.pos.x << ", " << cur.pos.y << ": " << curDist << std::endl;
            //std::cerr << next_pos.x << ", " << next_pos.y << ": bro  " << curDist << std::endl;
        }
        // std::cerr << cur.pos.x << ", " << cur.pos.y << ": " << maze->distances[cur.pos.y][cur.pos.x] << std::endl;
        // std::cerr << "cur exp: " << cur_exp << " maze explored: " << maze->explored[cur.pos.y][cur.pos.x] << std::endl;
        if(cur.blocked == false) {
            if(search) {
                if(!curExp) {
                    if(curDist >= maxDist) {
                        next_pos = cur.pos;
                        cur_exp = curExp;
                        maxDist = maze->distances[next_pos.y][next_pos.x];
                        max_bro = true;
                    } else if (max_bro == false) {
                        next_pos = cur.pos;
                        cur_exp = curExp;
                        minDist = maze->distances[next_pos.y][next_pos.x];
                    } 
                } else {
                    if(curDist < minDist && cur_exp) {
                        next_pos = cur.pos;
                        cur_exp = curExp;
                        minDist = maze->distances[next_pos.y][next_pos.x];
                    }
                }
                // if(maze->distances[cur.pos.y][cur.pos.x] < minDist) {
                //     next_pos = cur.pos;
                //     cur_exp = curExp;
                //     minDist = maze->distances[next_pos.y][next_pos.x];
                // } else if(maze->distances[cur.pos.y][cur.pos.x] == minDist) {
                //     if (!curExp && cur_exp) {
                //         next_pos = cur.pos;
                //         cur_exp = curExp;
                //     }
                // }
            } else {
                if(curDist < minDist) {
                    next_pos = cur.pos;
                    cur_exp = curExp;
                    minDist = maze->distances[next_pos.y][next_pos.x];
                }
            }
        }
        std::cerr << "next pos:" << next_pos.x << ", " << next_pos.y << std::endl;
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

    // std::cerr << "got here " << next_direction << std::endl;

    if(next_direction  == 'r') {
        if(moving) {
            API::turnRight();
        }
        maze->mouse_dir = (Direction)((maze->mouse_dir + 1) % 4);
        maze->commands += 'R';
    } else if(next_direction == 'l') {
        if(moving) {
            API::turnLeft();
        }
        maze->mouse_dir = (Direction)((maze->mouse_dir + 3) % 4);
        maze->commands += 'L';
    } else if (next_direction == 'u') {
        if(moving) {
            API::turnLeft();
        }
        maze->mouse_dir = (Direction)((maze->mouse_dir + 3) % 4);
        // maze->commands += 'L';
        if(moving) {
            API::turnLeft();
        }
        maze->mouse_dir = (Direction)((maze->mouse_dir + 3) % 4);
        if (maze->commands.size() != 0){
            maze->commands += 'U';
        }
    } else {
        return false;
    }
    return true;
}