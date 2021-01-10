#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>

#define MAX_SNAKE        50

#define UP          -1
#define DOWN         1
#define RIGHT        1
#define LEFT        -1

//Finestra
WINDOW *finestra;

//Coordenades del box
int min_x, min_y;
int max_x, max_y;
int X_MUGA_min, X_MUGA_max;
int Y_MUGA_min, Y_MUGA_max;

//Coordenades del snake
int x_cap, y_cap;
int x[MAX_SNAKE], y[MAX_SNAKE];
int len_snake;


//Funcions
void init_rand();
void init_ncurses();
void init_snake();
void init_mon();

void end_ncurses();

void print_box(WINDOW *finestra);
void print_snake(WINDOW *finestra);

int main(){
int i, key;

    init_rand();
    init_ncurses();
    init_mon();
    init_snake();

    print_box(finestra);
   while(1){
        key = wgetch(finestra);
        x_cap = x[0];
        y_cap = y[0];
        switch( key ) {  
            case KEY_UP:
                y_cap+=UP;
                break;
            case KEY_DOWN:
                y_cap+=DOWN;
                break;
            case KEY_LEFT:
                x_cap+=LEFT;
                break;
            case KEY_RIGHT:
                x_cap+=RIGHT;
                break;
            default:
                refresh();
                break;
           }
           //Si xoca contra laterals rebota
           if ( x_cap > X_MUGA_max )    x_cap = X_MUGA_max;
           if ( x_cap < X_MUGA_min )    x_cap = X_MUGA_min;    
           //Si xoca contra dalt o baix rebota
           if ( y_cap > Y_MUGA_max )    y_cap = Y_MUGA_max;     
           if ( y_cap < Y_MUGA_min )    y_cap = Y_MUGA_min;    
           print_box(finestra);
           if (    ( x_cap > X_MUGA_min ) && ( x_cap < X_MUGA_max ) &&
                   ( y_cap > Y_MUGA_min ) && ( y_cap < Y_MUGA_max )
               )    print_snake(finestra);
   }
    end_ncurses();
   return 0;
}

void print_snake(WINDOW *finestra){
int i;
mvwprintw(finestra,20,10,"( %d, %d ) ( %d, %d ) ( %d, %d ) ( %d, %d ) ( %d, %d )", x[0], y[0], x[1], y[1], x[2], y[2], x[3], y[3], x[4], y[4] );

    //Dibuixa nou cap   
    mvwaddch(finestra, y_cap, x_cap, ACS_CKBOARD );
    //Esborra cua
    mvwaddch(finestra, y[len_snake - 1], x[len_snake - 1], ' ' );

    //Avança posicions i repinta
    for ( i = len_snake-1 ; i > 0; i-- ){
         x[i] = x[i-1];
         y[i] = y[i-1];
         mvwaddch(finestra, y[i], x[i], ACS_CKBOARD );
    }
    //Actualitza cap
    x[0] = x_cap;
    y[0] = y_cap;

mvwprintw(finestra,22,10,"( %d, %d ) ( %d, %d ) ( %d, %d ) ( %d, %d ) ( %d, %d )", x[0], y[0], x[1], y[1], x[2], y[2], x[3], y[3], x[4], y[4] );
    wrefresh(finestra);
}

void print_box(WINDOW *finestra){
    box(finestra, min_x, min_y);
    wrefresh(finestra);
}

void init_mon(){
    getmaxyx(stdscr, max_y, max_x);
    min_x = 0;
    min_y = 0;
    X_MUGA_min = min_x + 1;
    X_MUGA_max = max_x - 1;
    Y_MUGA_min = min_y;
    Y_MUGA_max = max_y - 1;
    finestra = newwin(max_y, max_x, min_y, min_x);
    keypad(finestra, TRUE);
}

void init_snake(){
    int i;
    len_snake = MAX_SNAKE;
    x[0] = rand()%X_MUGA_max;
    y[0] = rand()%Y_MUGA_max;
    for ( i = 1; i < len_snake; i++ )    {x[i] = x[i-1] + 1; y[i] = y[i-1];}
}

void init_ncurses(){
   initscr();
   clear();
   noecho();
   cbreak();
   curs_set(FALSE);  
   refresh();
}

void init_rand(){
    srand(time(NULL));
}

void end_ncurses(){
   clrtoeol();
   refresh();
   endwin();
}

Compile
gcc -lncurses snake.c -o snake
