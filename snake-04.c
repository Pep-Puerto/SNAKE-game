#include <ncurses.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>

#define DELAY         100000
#define MAX_SNAKE        300
#define MAX_FOOD          50
#define NO_FOOD           -1

#define UP          -1
#define DOWN         1
#define RIGHT        1
#define LEFT        -1

#define ARROW_UP      65
#define ARROW_DOWN    66
#define ARROW_RIGHT   67
#define ARROW_LEFT    68

#define SIM_SNAKE         ACS_DIAMOND
#define SIM_SNAKE_HEAD    ACS_DEGREE
#define SIM_FOOD          ACS_CKBOARD


//Window
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

//Coordenades del food
int x_food[MAX_FOOD], y_food[MAX_FOOD];
int num_food;

//Funcions prototips
void init_rand();
void init_ncurses();
void init_snake();
void init_world();
void end_ncurses();
void print_box();
void print_food();
void print_snake();
void detect_food();
void genera_food();
int test_norabide( int key );
void delay();
int kbhit(void);

int main(){
int i, key;

int kk=0;

   init_rand();
   init_ncurses();
   init_world();
   init_snake();
   init_food();
   print_box(finestra);

   key = random_norabide();
   while(1){
        delay();
        print_food();
        if (kbhit()) key = getchar();
        x_cap = x[0];
        y_cap = y[0];
        switch( key ) { 
            case ARROW_UP:
                y_cap+=UP;
                break;
            case ARROW_DOWN:
                y_cap+=DOWN;
                break;
            case ARROW_LEFT:
                x_cap+=LEFT;
                break;
            case ARROW_RIGHT:
                x_cap+=RIGHT;
                break;
            default:        
                refresh();
                break;
                }
        //Si xoca contra laterals no passa
        if ( x_cap > X_MUGA_max )    x_cap = X_MUGA_max;
        if ( x_cap < X_MUGA_min )    x_cap = X_MUGA_min;   
        //Si xoca contra dalt o baix no passa
        if ( y_cap > Y_MUGA_max )    y_cap = Y_MUGA_max;    
        if ( y_cap < Y_MUGA_min )    y_cap = Y_MUGA_min;   
        if (( x_cap > X_MUGA_min ) && ( x_cap < X_MUGA_max ) &&
           ( y_cap > Y_MUGA_min ) && ( y_cap < Y_MUGA_max ))
        {
         //Si troba menjar creix
         detect_food();
         //Generem food al atzar
         genera_food();
         //Dibuixem la serp   
         print_snake();
        }
        print_box();
   }
   end_ncurses();
   return 0;
}

void detect_food(){
int i;
    for ( i = 0; i < num_food; i++ ){
        if ( ( x_food[i] == x_cap ) && ( y_food[i] == y_cap ) ) {
            mvwprintw(finestra,2,2,"Menjat ( %d %d )",x_food[i], y_food[i]  );
            x_food[i] = NO_FOOD;
            y_food[i] = NO_FOOD;
            len_snake++;   
            mvwprintw(finestra,1,2,"Snake %d ",len_snake );
        }       
    }
}

void genera_food(){
int i;
    for ( i = 0; i < num_food; i++ ){
        if  ( x_food[i] == NO_FOOD ) {
            x_food[i] = rand()%X_MUGA_max;
            y_food[i] = rand()%Y_MUGA_max;
            mvwprintw(finestra,3,2,"Generat ( %d %d )",x_food[i], y_food[i]  );
        }       
    }
}

void print_snake(){
int i;
    //Dibuixa nou cap  
    mvwaddch(finestra, y_cap, x_cap, SIM_SNAKE_HEAD );
    //Esborra cua
    mvwaddch(finestra, y[len_snake - 1], x[len_snake - 1], ' ' );
    //Avança posicions i repinta
    for ( i = len_snake-1 ; i > 0; i-- ){
         x[i] = x[i-1];
         y[i] = y[i-1];
         mvwaddch(finestra, y[i], x[i], SIM_SNAKE );
    }
    //Actualitza cap
    x[0] = x_cap;
    y[0] = y_cap;
    wrefresh(finestra);
}

void print_food(){
int i;
    for (i = 0; i < num_food; i++)
            if ( x_food[i] != NO_FOOD )
                 mvwaddch(finestra, y_food[i], x_food[i], SIM_FOOD );
}

void print_box(){
    box(finestra, min_x, min_y);
    wrefresh(finestra);
}

void init_world(){
    min_x = 0;
    min_y = 0;
    X_MUGA_min = min_x + 1;
    X_MUGA_max = max_x - 1;
    Y_MUGA_min = min_y;
    Y_MUGA_max = max_y - 1;
}

int init_food(){
    int i;
    num_food = 10;
    for (i = 0; i < num_food; i++ ){
    x_food[i] = rand()%X_MUGA_max;
    y_food[i] = rand()%Y_MUGA_max;
    }
}

void init_snake(){
    int i;
    len_snake = 4;
    x[0] = rand()%X_MUGA_max;
    y[0] = rand()%Y_MUGA_max;
    for ( i = 1; i < len_snake; i++ )    {x[i] = x[i-1] + 1; y[i] = y[i-1];}
}

void init_ncurses(){
   initscr();
   getmaxyx(stdscr, max_y, max_x);   clear();
   noecho();
   cbreak();
   curs_set(FALSE); 
   nodelay(finestra, TRUE);
   finestra = newwin(max_y, max_x, min_y, min_x);
   keypad(finestra, TRUE);
   refresh();
}

int random_norabide(){
    return (ARROW_UP + rand()%4);
}

void init_rand(){
    srand(time(NULL));
}

void end_ncurses(){
   clrtoeol();
   refresh();
   endwin();
}

void delay(){
    usleep(DELAY);
}

int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;
 
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
 
  ch = getchar();
 
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);
 
  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }
 
  return 0;
}
