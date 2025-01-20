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
bool GAME_OVER = false;

void topPlankDirection(int&, Coordinate, std::vector<Coordinate>);
void game();
void initializePlanks(std::vector<Coordinate>&, std::vector<Coordinate>&, int, int, int);
void movePlank(std::vector<Coordinate>&, int , int&);
void moveBall(Coordinate&, Coordinate&);
void ballPlankCollision(Coordinate, std::vector<Coordinate>, Coordinate&); 
void gameRound(Coordinate, bool&, int, int);
void gameMenu(int, int);

int main () {
    initscr();//Initialize ncurses for terminal-based graphical interface.
    noecho();//Disable input echo to hide characters typed by the user.
    cbreak(); //Disable line buffering to proccess input immediately withou waiting for enter.
    keypad(stdscr, TRUE); //Enable arrow key and special key handling.
    curs_set(0); //To hide the cursor.
    timeout(100); // Non-blocking input (100ms delay)

    while (!GAME_OVER) {
        game();
    }
    endwin();
    return 0;
}

void game(){
    //Initialize game variables
    bool game_over = false;
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

    int top_plank_direction = 0; //No initial movement
    int bottom_plank_direction = 0; //No initial movement
    ball_direction.vertical = 1; //down
    ball_direction.horizontal = 0; //no movement

    //set initial ball position at the center
    ball.vertical = center_rows;
    ball.horizontal = center_columns;

    initializePlanks(bottom_plank, top_plank, center_columns, bottom, top); //Initialize the planks 

    while (!game_over) {
        int ch = getch(); //get direction from user

        switch (ch) {
            case KEY_LEFT: bottom_plank_direction = -1; break;
            case KEY_RIGHT: bottom_plank_direction = 1; break;
        }
        
        //Get top_plank_direction
        topPlankDirection(top_plank_direction, ball, top_plank);
        
        //Check for collision and move plank
        movePlank(bottom_plank, left, bottom_plank_direction);
        movePlank(top_plank, left, top_plank_direction);
        
        //Check for ball collision with planks and change direction accordingly
        ballPlankCollision(ball, bottom_plank, ball_direction); 
        ballPlankCollision(ball, top_plank, ball_direction);
        
        //Check for collsion and move ball
        moveBall(ball, ball_direction);

        
        clear();
        //Render the ball
        mvprintw(ball.vertical, ball.horizontal, "O");
        //Render the planks
        for (int i = 0; i < 3; i++) {
            mvprintw(bottom_plank[i].vertical, bottom_plank[i].horizontal, "-");
            mvprintw(top_plank[i].vertical, top_plank[i].horizontal, "-");
        }
       //reset game if scored && track scored
        gameRound(ball, game_over, bottom, top);
 
    }
}

void initializePlanks(std::vector<Coordinate>& bottom_plank, std::vector<Coordinate>& top_plank, int center_columns, int bottom, int top) {
   for (int i = 0; i < 3; i++) {
        //bottom_plank starts at bottom center
        bottom_plank[i].horizontal = center_columns - 1 + i;
        bottom_plank[i].vertical = bottom;
        //top_plank starts at top center
        top_plank[i].horizontal = center_columns - 1 + i;
        top_plank[i].vertical = top;
    } 
}

void movePlank(std::vector<Coordinate>& plank, int left, int& plank_direction) {
    int max_columns = COLS - 1; //get max_columns

    if (plank[FIRST_P].horizontal == left) {
            plank_direction = 1; //right
    } 
    else if (plank[LAST_P].horizontal == max_columns) {
        plank_direction = -1; //left
    }
    for (int i = 0; i < 3; i++) {
        plank[i].horizontal += plank_direction;
    }
}

void moveBall(Coordinate& ball, Coordinate& ball_direction) {
    int left = 0;
    int max_columns = COLS - 1;

    if (ball.horizontal <= left || ball.horizontal >= max_columns) {
            ball_direction.horizontal = -ball_direction.horizontal; //Reverse direction
    }
    
    ball.vertical += ball_direction.vertical;
    ball.horizontal += ball_direction.horizontal;
}

void ballPlankCollision(Coordinate ball, std::vector<Coordinate> plank, Coordinate& ball_direction) {
    //Check if ball and plank on same vertical level
    if (ball.vertical == plank[FIRST_P].vertical) {
       if (ball.horizontal == plank[FIRST_P].horizontal) {
            ball_direction.vertical = -ball_direction.vertical; //Reverse direction
            ball_direction.horizontal = -2; //Bounce left
       }
       else if (ball.horizontal == plank[CENTER_P].horizontal) {
           ball_direction.vertical = -ball_direction.vertical; //Reverse direction
            ball_direction.horizontal = 0; //nill
       }
       else if (ball.horizontal == plank[LAST_P].horizontal) {
            ball_direction.vertical = -ball_direction.vertical; //Reverse direction
            ball_direction.horizontal = 2; //Bounce right
       } 
    }      
}

void topPlankDirection(int& top_plank_direction, Coordinate ball, std::vector<Coordinate> top_plank) {
    //if ball is to the right of plank, move plank right,
    //otherwise move it left
    if (ball.horizontal > top_plank[CENTER_P].horizontal) {
            top_plank_direction = 1;
    } else if (ball.horizontal < top_plank[CENTER_P].horizontal) {
            top_plank_direction = -2;
    }
    else {
        top_plank_direction = 0;
    }
}

void gameRound(Coordinate ball, bool& game_over, int bottom, int top) {
    static int top_plank_score;
    static int bottom_plank_score;

   if (top_plank_score >= 5 || bottom_plank_score >= 5) {
        game_over = true;
        gameMenu(top_plank_score, bottom_plank_score);
        top_plank_score = 0; bottom_plank_score = 0;
    } 
    
   else if (ball.vertical > bottom) {
        top_plank_score += 1;
        game_over = true;
   }
   else if (ball.vertical < top) {
        bottom_plank_score += 1;
        game_over = true;
   }
}

void gameMenu(int top_score, int bottom_score) {
    int center_rows = LINES / 2;
    int winner_pos_v = center_rows - (center_rows / 2);
    int center_columns = COLS / 2;
    int winner_pos_h = center_columns - 5;
    clear();
    if (bottom_score >= 5) {
       mvprintw(winner_pos_v, winner_pos_h, "You Win!"); 
    }
    else {
       mvprintw(winner_pos_v, winner_pos_h, "You Lose!"); 
    }
    mvprintw(winner_pos_v + 3, winner_pos_h + 2, "Your score = %d", bottom_score);
    mvprintw(winner_pos_v + 4, winner_pos_h + 2, "AI score = %d", top_score);
    
    int ch;
    int highlight = 1;
    int choice = 0;

    while (true) {
        ch = getch();
        switch (ch) {
            case KEY_UP:
                highlight = (highlight == 1) ? 2 : 1;
                break;
            case KEY_DOWN:
                highlight = (highlight == 2) ? 1 : 2;
                break;
            case 10:
                choice = highlight;
                break;
        }

        if (highlight == 1) {
            mvprintw(center_rows, center_columns, "> Play");
            mvprintw(center_rows + 1, center_columns, "  Quit");
        }
        else if (highlight == 2) {
            mvprintw(center_rows, center_columns, "  Play");
            mvprintw(center_rows + 1, center_columns, "> Quit");
        }
        if (choice != 0) break;
    }
    if (choice == 1) GAME_OVER = false;
    else if (choice == 2) GAME_OVER = true; 
}
