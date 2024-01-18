#include <iostream>
#include <string>
#include <stdbool.h>
#include "API.h"
#include "maze.h"
#include "queue.h"
#include "nav.h"

Maze maze; //INIT MAZE

void log(const std::string& text) {
    std::cerr << text << std::endl;
}

int main(int argc, char* argv[]) {
    maze.mouse_pos = (Coord){0, 0};
    maze.mouse_dir = NORTH;

    //goal position shit
    /* center four our goal positions 
    goalPos -> type is Coord*
    goalPos[i] -> type is Coord */
    {
    maze.goalPos = (Coord*)(malloc(4 * sizeof(Coord)));
    initCenterGoalPos(&maze);
    }
    
    while (true) { //while mouse not at goalPos
        scanWalls(&maze);
        floodFill(&maze);
        updateSimulator(maze);   
        turnToMinDist(&maze);

        API::moveForward();
        maze.commands += 'F';
        log("moved forward");

        updateMousePos(&maze.mouse_pos, maze.mouse_dir);

        if (onGoal(maze)) { //If mouse reaches goal position at center, set new goal position to (0, 0)
            initStartGoalPos(&maze);
            log("Goal Position Reached!");
        }
        
        if(isCoordSame(maze.mouse_pos, (Coord){0, 0})) { //If mouse reaches goal position at start, set new goal position to center four boxes
            initCenterGoalPos(&maze);
            log("Goal Position Reached!");
        }

        std::cerr << "Mouse Pos: (" << maze.mouse_pos.x << ", " << maze.mouse_pos.y << ")" << std::endl;

        std::cerr << "Commands:" << maze.commands << std::endl;
        log(" ");
    }
    
}