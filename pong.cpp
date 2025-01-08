/*1. A ball_direction struct to keep track of ball_direction
=======

1. A ball_direction struct to keep track of ball_direction
>>>>>>> ball
2. Initialize ball_direction to right
Lets keep things simple for now. If the ball hits the left plank it goes left diagonally.
if the ball its center, it goes straight.
if right, right diaginally
3. if ball.x equal to plank[0].x then ball_direction is left diagonal
*/

#include <ncurses.h>
#include <cstdlib>
#include <vector>

struct Coordinate {
    int horizontal, vertical;
};

//Name individual parts of the planks for readability and maintainence
int FIRST_P = 0;
int CENTER_P = 1;
int LAST_P = 2;

void topPlankDirection(int&, Coordinate, std::vector<Coordinate>);
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
    Coordinate ball_direction;
    std::vector<Coordinate> bottom_plank(3);
    std::vector<Coordinate> top_plank(3);
    int max_rows, max_columns;
    getmaxyx(stdscr, max_rows, max_columns);
    int left = 0;
    int right = max_columns - 1;
    int top = 0;
    int bottom = max_rows - 1;
    int center_rows = max_rows / 2;
    int center_columns = max_columns / 2;

    //set initial directions
    int top_plank_direction = 1; //Initial direction is right
    int bottom_plank_direction = 1; //Initial direction is right
    ball_direction.vertical = 1; //down
    ball_direction.horizontal = 0; //no movement

    //set initial ball position at the center
    ball.vertical = center_rows;
    ball.horizontal = center_columns;

    //Initialize the planks
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
        topPlankDirection(top_plank_direction, ball, top_plank);

        //Move ball according to the direction
        ball.vertical += ball_direction.vertical;
        ball.horizontal += ball_direction.horizontal;

        //Move plank according to the direction
        for (int i = 0; i < 3; i++) {
            bottom_plank[i].horizontal += bottom_plank_direction;
            top_plank[i].horizontal += top_plank_direction;
        }
        
        //Check for plank collision with wall
        if (bottom_plank[FIRST_P].horizontal == left) {
            bottom_plank_direction = 1; //change direction to right
        } else if (bottom_plank[LAST_P].horizontal == max_columns - 1) {                bottom_plank_direction = -1; //change direction to left
        }
        //check for the top plank
        if (top_plank[FIRST_P].horizontal == left) {
             top_plank_direction = 1; //change direction to right
        } else if (top_plank[LAST_P].horizontal == max_columns - 1) {
              top_plank_direction = -1; //change direction to left
        }

        //Check for ball collision with wall
        if (ball.horizontal == left + 1) {
            ball_direction.horizontal = 1; //Direction to right
        }
        else if (ball.horizontal == max_columns -1) {
            ball_direction.horizontal = -1; //Direction to left
        }
        //Check for bottom_plank collison with ball
        if (ball.horizontal == bottom_plank[FIRST_P].horizontal && ball.vertical == bottom_plank[FIRST_P].vertical) {
            //left diagonal if collided with left side of the bottom plank
            ball_direction.vertical = -1; //up
            ball_direction.horizontal = -2; //left
        }
        else if (ball.horizontal == bottom_plank[CENTER_P].horizontal && ball.vertical == bottom_plank[CENTER_P].vertical) {
            //straight up if collided with centre of the plank
            ball_direction.vertical = -1; //up
            ball_direction.horizontal = 0; //nill
        }
        else if (ball.horizontal == bottom_plank[LAST_P].horizontal && ball.vertical == bottom_plank[LAST_P].vertical) {
            //right diagonal if collided with centre of the plank
            ball_direction.vertical = -1; //up
            ball_direction.horizontal = 2; //right
        }
        //Check for top_plank collison with plank
        if (ball.horizontal == top_plank[FIRST_P].horizontal && ball.vertical == top_plank[FIRST_P].vertical) {
            //left diagonal if collided with left side of the bottom plank
            ball_direction.vertical = 1; //down
            ball_direction.horizontal = -2; //left
        }
        else if (ball.horizontal == top_plank[CENTER_P].horizontal && ball.vertical == top_plank[CENTER_P].vertical) {
            //straight down if collided with centre of the plank
            ball_direction.vertical = 1; //up
            ball_direction.horizontal = 0; //nill
        }
        else if (ball.horizontal == top_plank[LAST_P].horizontal && ball.vertical == top_plank[LAST_P].vertical) {
            //right diagonal if collided with centre of the plank
            ball_direction.vertical = 1; //up
            ball_direction.horizontal = 2; //right
        }

        clear();
        //Render the ball
        mvprintw(ball.vertical, ball.horizontal, "O");

        //Render the planks
        for (int i = 0; i < 3; i++) {
            mvprintw(bottom_plank[i].vertical, bottom_plank[i].horizontal, "-");
            mvprintw(top_plank[i].vertical, top_plank[i].horizontal, "-");
        }
    }
}

void topPlankDirection(int& top_plank_direction, Coordinate ball, std::vector<Coordinate> top_plank) {
    int random = rand() % 10;
    //if ball is to the right of plank, move plank right
    if (ball.horizontal > top_plank[FIRST_P].horizontal) {
        if (random < 9) {
            top_plank_direction = 1;
        }
        else {
            top_plank_direction = -1;
        }
    }
    //if ball is on left side, give weight to left movement
    else if (ball.horizontal < top_plank[LAST_P].horizontal) {
        if (random < 9) {
            top_plank_direction = -1;
        }
        else {
            top_plank_direction = 1;
        }
    }
    else {
        top_plank_direction = 0;
    }
}
