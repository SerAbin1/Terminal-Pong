//Implement a multiplayer pong game with 2 players
//3 objects. Two planks which can only move horizontally
//One ball which moves constantly. 
//If you fail to hit the ball and reverse its direction. You lose.
//Ball speed increases after each round (round = one hit by both players)
//Ball movement
//Plank movement
//Collision detection
//Render
/*
2. 
*/
void game();

#include <ncurses.h>
struct Coordinate {
    int horizontal, vertical;
};

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
    int max_rows, max_columns;
    getmaxyx(stdscr, max_rows, max_columns);
    int left = 0;
    int bottom = max_rows - 1;
    int center_rows = max_rows / 2;
    int top = 0;
    int right = max_columns - 1;
    int center_columns = max_columns / 2;
    //Both planks start at the center
    bottom_plank.horizontal = center_columns;
    bottom_plank.vertical = bottom;
     

    while (true) {
        int ch = getch(); //get direction from user

        switch (ch) {
            case KEY_LEFT: bottom_plank_direction = -1; break;
            case KEY_RIGHT: bottom_plank_direction = 1; break;
        }

        //Move ball according to the direction
        bottom_plank.horizontal += bottom_plank_direction;

        //Check for collison with the Wall
        if (bottom_plank.horizontal == 0) {
            bottom_plank_direction = 1; //change direction to right
        } else if (bottom_plank.horizontal == max_columns - 1) {
            bottom_plank_direction = -1; //change direction to left
        }
        
        clear();
        //Render the ball
        mvprintw(bottom_plank.vertical, bottom_plank.horizontal, "---");
    }
}

