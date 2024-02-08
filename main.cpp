#include <iostream>
#include <string>
#include <stdbool.h>
#include "API.h"
#include "maze.h"
#include "queue.h"
#include "nav.h"
#include "makepath.h"
#include "commands.h"

Maze maze;

void log(const std::string& text) {
    std::cerr << text << std::endl;
}

void run(bool to_start) {
    // Determine current state of maze
    scanWalls(&maze);
    floodFill(&maze, to_start);
    updateSimulator(maze);   

    // Turn and movement
    bool turned = turnToMinDist(&maze, true);
    API::moveForward();
    if(turned) {
        log("turned");
    } else {
        maze.commands += 'F';
    }
    log("moved forward");

    // Update mouse position
    updateMousePos(&maze.mouse_pos, maze.mouse_dir);
    maze.explored[maze.mouse_pos.y][maze.mouse_pos.x] = true;
    std::cerr << "Mouse Pos: (" << maze.mouse_pos.x << ", " << maze.mouse_pos.y << ")" << std::endl;

    // Command handling
    // std::cerr << "Commands:" << maze.commands << std::endl;
    // char* flruString = flruStringToCommands(maze.commands);
    // makeDiagonalPath(flruString);
    // listCommands();
    
    log(" "); // newline
}

void search_run() {
    // initialize
    maze.mouse_pos = (Coord){0, 0};
    maze.mouse_dir = NORTH;
    maze.goalPos = (Coord*)(malloc(4 * sizeof(Coord)));

    // initialize for the run to the center
    initCenterGoalPos(&maze);
    bool to_start = false;
    for(int i = 0; i < MAZE_SIZE; i++) {
        for(int j = 0; j < MAZE_SIZE; j++) {
            maze.explored[i][j] = false;
        }
    }

    while (!onGoal(maze)) { // on the way from the start to the goalPos
        run(to_start);
    }

    // reset for the run back to the start
    maze.commands = "";
    initStartGoalPos(&maze);
    to_start = true;

    while (!isCoordSame(maze.mouse_pos, (Coord){0,0})) { // on the way back from the goalPos to the start
        run(to_start);
    }
}

void speed_run() {
    // initialize
    maze.mouse_pos.x = 0;
    maze.mouse_pos.y = 0;
    // maze.mouse_dir = NORTH;
    maze.commands = "";

    initCenterGoalPos(&maze);
    floodFill(&maze, false);

    updateSimulator(maze);
    
    while (!onGoal(maze)) {
        bool turned = turnToMinDist(&maze, false);
        API::moveForward();
        if(turned) {
            log("turned");
        } else {
            maze.commands += 'F';
        }

        // Update mouse position
        updateMousePos(&maze.mouse_pos, maze.mouse_dir);

        // Command handling
        std::cerr << "Commands:" << maze.commands << std::endl;
        char* flruString = flruStringToCommands(maze.commands);
        makeDiagonalPath(flruString);
        listCommands();

        log(" "); // newline
    }
}

int main(int argc, char* argv[]) {
    search_run();
    API::turnLeft();
    maze.mouse_dir = (Direction)((maze.mouse_dir + 3) % 4);
    // maze->commands += 'L';
    API::turnLeft();
    maze.mouse_dir = (Direction)((maze.mouse_dir + 3) % 4);
    log("finished");
    speed_run();
}