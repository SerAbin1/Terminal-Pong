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
void initializePlanks(std::vector<Coordinate>&, std::vector<Coordinate>&, int, int, int);
void plankWallCollision(std::vector<Coordinate>, int , int&);
void ballWallCollision(Coordinate, Coordinate&);
void ballPlankCollision(Coordinate, std::vector<Coordinate>, Coordinate&);

int main () {
    initscr();//Initialize ncurses for terminal-based graphical interface.
    noecho();//Disable input echo to hide characters typed by the user.
    cbreak(); //Disable line buffering to proccess input immediately withou waiting for enter.
    keypad(stdscr, TRUE); //Enable arrow key and special key handling.
    curs_set(0); //To hide the cursor.
    timeout(42); // Non-blocking input (100ms delay)

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

    int top_plank_direction = 1; //Initial direction is right
    int bottom_plank_direction = 1; //Initial direction is right
    ball_direction.vertical = 1; //down
    ball_direction.horizontal = 0; //no movement

    //set initial ball position at the center
    ball.vertical = center_rows;
    ball.horizontal = center_columns;

    initializePlanks(bottom_plank, top_plank, center_columns, bottom, top); //Initialize the planks 

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
        
        plankWallCollision(bottom_plank, left, bottom_plank_direction); //Check bottom_plank collision with wall
        plankWallCollision(top_plank, left, top_plank_direction); //Check top_plank collision with wall
        
        ballWallCollision(ball, ball_direction); //Check for ball collision with wallll
        
        //Check for ball collision with planks and change direction accordingly
        ballPlankCollision(ball, bottom_plank, ball_direction); 
        ballPlankCollision(ball, top_plank, ball_direction);
        
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

void initializePlanks(std::vector<Coordinate>& bottom_plank, std::vector<Coordinate>& top_plank, int center_columns, int bottom, int top) {
   for (int i = 0; i < 3; i++) {
        //bottom_plank starts at bottom center
        bottom_plank[i].horizontal = center_columns + i;
        bottom_plank[i].vertical = bottom;
        //top_plank starts at top center
        top_plank[i].horizontal = center_columns + i;
        top_plank[i].vertical = top;
    } 
}

void plankWallCollision(std::vector<Coordinate> plank, int left, int& plank_direction) {
    int max_columns = COLS; //get max_columns

    if (plank[FIRST_P].horizontal == left) {
            plank_direction = 1; //right
    } 
    else if (plank[LAST_P].horizontal == max_columns - 1) {
        plank_direction = -1; //left
    }
}

void ballWallCollision(Coordinate ball, Coordinate& ball_direction) {
    int left = 0;
    int max_columns = COLS;

    if (ball.horizontal == left + 1) {
            ball_direction.horizontal = 1; //right
    }
    else if (ball.horizontal == max_columns -1) {
        ball_direction.horizontal = -1; //left
    }
}

void ballPlankCollision(Coordinate ball, std::vector<Coordinate> plank, Coordinate& ball_direction) {
        if (ball.horizontal == plank[FIRST_P].horizontal && ball.vertical == plank[FIRST_P].vertical) {
            //left diagonal if collided with left side of the bottom plank
            ball_direction.vertical = -1; //up
            ball_direction.horizontal = -2; //left
        }
        else if (ball.horizontal == plank[CENTER_P].horizontal && ball.vertical == plank[CENTER_P].vertical) {
            //straight up if collided with centre of the plank
            ball_direction.vertical = -1; //up
            ball_direction.horizontal = 0; //nill
        }
        else if (ball.horizontal == plank[LAST_P].horizontal && ball.vertical == plank[LAST_P].vertical) {
            //right diagonal if collided with centre of the plank
            ball_direction.vertical = -1; //up
            ball_direction.horizontal = 2; //right
        }
}

void topPlankDirection(int& top_plank_direction, Coordinate ball, std::vector<Coordinate> top_plank) {
    //if ball is to the right of plank, move plank right,
    //otherwise move it left
    if (ball.horizontal > top_plank[FIRST_P].horizontal) {
            top_plank_direction = 1;
    } else if (ball.horizontal < top_plank[FIRST_P].horizontal) {
            top_plank_direction = -1;
    }
    else {
        top_plank_direction = 0;
    }
}
