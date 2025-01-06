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
    int x, y;
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
    Coordinate ball, plank1, plank2;
    int plank1_direction = 1; //Initial direction is right
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    int left = 0;
    int right = max_y;
    int center_x = max_y / 2;
    int top = 0;
    int down = max_x;
    int center_y = max_x / 2;
    //Both planks start at the center
    plank1.y = center_y;
    plank1.x = center_x;
     

    while (true) {
        int ch = getch(); //get direction from user

        switch (ch) {
            case KEY_LEFT: plank1_direction = -1; break;
            case KEY_RIGHT: plank1_direction = 1; break;
        }

        //Move ball according to the direction
        plank1.y += plank1_direction;

        //Check for collison with the Wall
        if (plank1.y == 0) {
            plank1_direction = 1; //change direction to right
        } else if (plank1.y == max_x - 1) {
            plank1_direction = -1; //change direction to left
        }
        
        clear();
        //Render the ball
        mvprintw(plank1.x, plank1.y, "---");
    }
}

