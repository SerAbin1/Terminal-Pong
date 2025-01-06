#include <ncurses.h>
#include <cstdlib>
#include <vector>

struct Coordinate {
    int horizontal, vertical;
};

void topPlankDirection(int&, Coordinate, int);
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
    //Initialize game variables
    Coordinate ball;
    std::vector<Coordinate> bottom_plank(3);
    std::vector<Coordinate> top_plank(3);
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
    for (int i = 0; i < 3; i++) {
        //bottom_plank starts at bottom center
        bottom_plank[i].horizontal = center_columns + i;
        bottom_plank[i].vertical = bottom;
        //top_plank starts at top center
        top_plank[i].horizontal = center_columns + i;
        top_plank[i].vertical = top;
    }

    while (true) {
        int ch = getch(); //get direction from user

        switch (ch) {
            case KEY_LEFT: bottom_plank_direction = -1; break;
            case KEY_RIGHT: bottom_plank_direction = 1; break;
        }
        
        //Get top_plank_direction
        topPlankDirection(top_plank_direction, ball, center_columns);

        //Move plank according to the direction
        for (int i = 0; i < 3; i++) {
            bottom_plank[i].horizontal += bottom_plank_direction;
            top_plank[i].horizontal += top_plank_direction;
        }
        
        //Check for collison with the Wall
        for (int i = 0; i < 3; i++) {
            if (bottom_plank[i].horizontal == 0) {
                bottom_plank_direction = 1; //change direction to right
            } else if (bottom_plank[i].horizontal == max_columns - 3) {
                bottom_plank_direction = -1; //change direction to left
            }
            //check for the top plank
            if (top_plank[i].horizontal == 0) {
                top_plank_direction = 1; //change direction to right
            } else if (top_plank[i].horizontal == max_columns - 3) {
                top_plank_direction = -1; //change direction to left
            }
        }
        
        clear();
        //Render the planks
        for (int i = 0; i < 3; i++) {
            mvprintw(bottom_plank[i].vertical, bottom_plank[i].horizontal, "-");
            mvprintw(top_plank[i].vertical, top_plank[i].horizontal, "-");
        }
    }
}

void topPlankDirection(int& top_plank_direction, Coordinate ball, int center) {
    int random = rand() % 100;
    //if ball is on right side, give weight to right movement
    if (ball.horizontal > center) {
        if (random < 70) {
            top_plank_direction = 1;
        }
        else {
            top_plank_direction = -1;
        }
    }
    //if ball is on left side, give weight to left movement
    else {
        if (random < 70) {
            top_plank_direction = -1;
        }
        else {
            top_plank_direction = 1;
        }
    }
}
