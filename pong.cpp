/*
2. Add a top Plank
3. Add a function that returns plank directiom based on the ball position to simulate AI for the top plank
*/
#include <ncurses.h>
#include <cstdlib>

struct Coordinate {
    int horizontal, vertical;
};

void game();

int main () {
    initscr();//Initialize ncurses for terminal-based graphical interface.
    noecho();//Disable input echo to hide characters typed by the user.
    cbreak(); //Disable line buffering to proccess input immediately withou waiting for enter.
    keypad(stdscr, TRUE); //Enable arrow key and special key handling.
    curs_set(0); //To hide the cursor.
    timeout(100); // Non-blocking input (100ms delay)

    game();
    endwin();
    return 0;
}

void game(){
    Coordinate ball, bottom_plank, top_plank;
    int bottom_plank_direction = 1; //Initial direction is right
    int top_plank_direction = 1; //Initial direction is right
    int max_rows, max_columns;
    getmaxyx(stdscr, max_rows, max_columns);
    int left = 0;
    int right = max_columns - 1;
    int top = 0;
    int bottom = max_rows - 1;
    int center_rows = max_rows / 2;
    int center_columns = max_columns / 2;
    //bottom_plank starts at bottom center
    bottom_plank.horizontal = center_columns;
    bottom_plank.vertical = bottom;
    //top_plank starts at top center
    top_plank.horizontal = center_columns;
    top_plank.vertical = top;

    while (true) {
        int ch = getch(); //get direction from user

        switch (ch) {
            case KEY_LEFT: bottom_plank_direction = -1; break;
            case KEY_RIGHT: bottom_plank_direction = 1; break;
        }

        //Move plank according to the direction
        bottom_plank.horizontal += bottom_plank_direction;
        top_plank.horizontal += top_plank_direction;

        //Check for collison with the Wall
        if (bottom_plank.horizontal == 0) {
            bottom_plank_direction = 1; //change direction to right
        } else if (bottom_plank.horizontal == max_columns - 3) {
            bottom_plank_direction = -1; //change direction to left
        }
        //check for the top plank
        if (top_plank.horizontal == 0) {
            top_plank_direction = 1; //change direction to right
        } else if (top_plank.horizontal == max_columns - 3) {
            top_plank_direction = -1; //change direction to left
        } 
        
        clear();
        //Render the planks
        mvprintw(bottom_plank.vertical, bottom_plank.horizontal, "---");
        mvprintw(top_plank.vertical, top_plank.horizontal, "---");
    }
}
