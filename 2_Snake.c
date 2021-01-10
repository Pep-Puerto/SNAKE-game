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

#define MARGE            1
#define FINESTRA_DADES_H    5

#define ARROW_UP      65
#define ARROW_DOWN    66
#define ARROW_RIGHT   67
#define ARROW_LEFT    68

#define SIM_SNAKE         ACS_DIAMOND
#define SIM_SNAKE_HEAD    ACS_DEGREE
#define SIM_FOOD          ACS_CKBOARD

//Windows
WINDOW *finestra_joc;
WINDOW *finestra_dades;
WINDOW *finestra_final;

//Màximes coordenades detectades de pantalla
int max_x, max_y;

//Coordenades dels boxes
int min_x_joc, min_y_joc;
int max_x_joc, max_y_joc;
int min_x_dades, min_y_dades;
int max_x_dades, max_y_dades;


//Fronteres de la snake
int X_MUGA_min, X_MUGA_max;
int Y_MUGA_min, Y_MUGA_max;

//Coordenades del snake
int x_capA, y_capA;
int xA[MAX_SNAKE], yA[MAX_SNAKE];
int len_snakeA;
int x_capB, y_capB;
int xB[MAX_SNAKE], yB[MAX_SNAKE];
int len_snakeB;




//Coordenades del food
int x_food[MAX_FOOD], y_food[MAX_FOOD];
int num_food;

//Pastissets menjats
int food;

//Funcions prototips
void init_rand();
void init_ncurses();
void init_snakeA();
void init_snakeB();
void init_world();
void end_ncurses();
void print_boxes();
void print_food();
void print_snakeA();
void print_snakeB();
void print_final();
void detect_food();
void genera_food();
int test_norabide();
int autoxoc( int x_capA, int y_capA);
void delay();
int kbhit(void);

//Programa principal
int main(){
int i, keyA, keyB;

   init_rand();
   init_ncurses();
   init_world();
   init_snakeA();
   init_snakeB();
   init_food();
   print_boxes(finestra_joc);
   keyA = random_norabide();
   keyB = random_norabide();
 
   while(1){
        delay();
        print_food();
        //if (kbhit()) key = getchar();
          keyA = random_norabide();
          keyB = random_norabide();
        x_capA = xA[0];
        y_capA = yA[0];
        x_capB = xB[0];
        y_capB = yB[0];
        switch( keyA ) {
            case ARROW_UP:
                y_capA+=UP;
                break;
            case ARROW_DOWN:
                y_capA+=DOWN;
                break;
            case ARROW_LEFT:
                x_capA+=LEFT;
                break;
            case ARROW_RIGHT:
                x_capA+=RIGHT;
                break;
            default:      
                refresh();
                break;
                }
        switch( keyB ) {
            case ARROW_UP:
                y_capB+=UP;
                break;
            case ARROW_DOWN:
                y_capB+=DOWN;
                break;
            case ARROW_LEFT:
                x_capB+=LEFT;
                break;
            case ARROW_RIGHT:
                x_capB+=RIGHT;
                break;
            default:      
                refresh();
                break;
                }
        if ( x_capA > X_MUGA_max ) x_capA = X_MUGA_min;
        if ( x_capA < X_MUGA_min    ) x_capA = X_MUGA_max;
        if ( y_capA > Y_MUGA_max ) y_capA = Y_MUGA_min;
        if ( y_capA < Y_MUGA_min    ) y_capA = Y_MUGA_max;

        if ( x_capB > X_MUGA_max ) x_capB = X_MUGA_min;
        if ( x_capB < X_MUGA_min    ) x_capB = X_MUGA_max;
        if ( y_capB > Y_MUGA_max ) y_capB = Y_MUGA_min;
        if ( y_capB < Y_MUGA_min    ) y_capB = Y_MUGA_max;

        detect_food();
        genera_food();
        print_snakeA();
        print_snakeB();
        print_boxes();
   }
   end_ncurses();
   return 0;
}

void detect_food(){
int i;
    wattron(finestra_dades, COLOR_PAIR(1));
    for ( i = 0; i < num_food; i++ ){
        if ( ( x_food[i] == x_capA ) && ( y_food[i] == y_capA ) ) {
                 //Snake ha dinat
                 food++;         
            //mvwprintw(finestra_dades, 2, (int)( max_x_dades / 4 ) ," FOOD %d ", food  );
            x_food[i] = NO_FOOD;
            y_food[i] = NO_FOOD;
            len_snakeA++; 
            mvwprintw(finestra_dades, 2, (int)(3 * ( max_x_dades / 4 ))," SNAKE %d ",len_snakeA );
        }     
    }
    wattroff(finestra_dades, COLOR_PAIR(1));
        wattron(finestra_dades, COLOR_PAIR(2));
    for ( i = 0; i < num_food; i++ ){
        if ( ( x_food[i] == x_capB ) && ( y_food[i] == y_capB ) ) {
                 //Snake ha dinat
                 food++;         
            //mvwprintw(finestra_dades, 3, (int)( max_x_dades / 4 ) ," FOOD %d ", food  );
            x_food[i] = NO_FOOD;
            y_food[i] = NO_FOOD;
            len_snakeB++; 
            mvwprintw(finestra_dades, 3, (int)(3 * ( max_x_dades / 4 ))," SNAKE %d ",len_snakeB );
        }     
    }
    wattroff(finestra_dades, COLOR_PAIR(2));
}

void genera_food(){
int i;
    for ( i = 0; i < num_food; i++ ){
        if  ( x_food[i] == NO_FOOD ) {
            x_food[i] = rand()%X_MUGA_max;
            y_food[i] = rand()%Y_MUGA_max;
        }     
    }
}

void print_snakeA(){
int i;
    wattron(finestra_joc, COLOR_PAIR(1));
    //Dibuixa nou cap
    mvwaddch(finestra_joc, y_capA, x_capA, SIM_SNAKE_HEAD );
    //Esborra cua
    mvwaddch(finestra_joc, yA[len_snakeA - 1], xA[len_snakeA - 1], ' ' );
    //Avança posicions i repinta
    for ( i = len_snakeA-1 ; i > 0; i-- ){
         xA[i] = xA[i-1];
         yA[i] = yA[i-1];
         mvwaddch(finestra_joc, yA[i], xA[i], SIM_SNAKE );
    }
    //Actualitza cap
    xA[0] = x_capA;
    yA[0] = y_capA;
    wattroff(finestra_joc, COLOR_PAIR(1));
    wrefresh(finestra_joc);
}

void print_snakeB(){
int i;
    wattron(finestra_joc, COLOR_PAIR(2));
    //Dibuixa nou cap
    mvwaddch(finestra_joc, y_capB, x_capB, SIM_SNAKE_HEAD );
    //Esborra cua
    mvwaddch(finestra_joc, yB[len_snakeB - 1], xB[len_snakeB - 1], ' ' );
    //Avança posicions i repinta
    for ( i = len_snakeB-1 ; i > 0; i-- ){
         xB[i] = xB[i-1];
         yB[i] = yB[i-1];
         mvwaddch(finestra_joc, yB[i], xB[i], SIM_SNAKE );
    }
    //Actualitza cap
    xB[0] = x_capB;
    yB[0] = y_capB;
    wattroff(finestra_joc, COLOR_PAIR(2));
    wrefresh(finestra_joc);
}


int autoxoc( int x_capA, int y_capA){
int i;
for ( i = 2; i < len_snakeA; i++ )
    if ( ( xA[i] == x_capA ) && ( yA[i] == y_capA ) ) return 1;
return 0;
}

void print_food(){
int i;
    wattron(finestra_joc, COLOR_PAIR(3));
    for (i = 0; i < num_food; i++)
            if ( x_food[i] != NO_FOOD )
                 mvwaddch(finestra_joc, y_food[i], x_food[i], SIM_FOOD );
    wattroff(finestra_joc, COLOR_PAIR(3));            
}

void print_final(){
    box(finestra_final, 0, 0);
    wattron(finestra_final, COLOR_PAIR(2));
    wattron(finestra_final, A_BLINK );
    mvwprintw(finestra_final, 2, 2 ," GAME OVER GUINEMAN ... "  );
    wrefresh(finestra_final);
    getchar();
    wattroff(finestra_final, A_BLINK );
    wattroff(finestra_final, COLOR_PAIR(2));
}

void print_boxes(){
    box(finestra_joc, 0, 0);
    wrefresh(finestra_joc);
    box(finestra_dades, 0, 0);
    wrefresh(finestra_dades);
}

void init_world(){
    food = 0;
    X_MUGA_min = min_x_joc + MARGE;
    X_MUGA_max = max_x_joc - MARGE;
    Y_MUGA_min = min_y_joc + MARGE;
    Y_MUGA_max = max_y_joc - MARGE;
}

int init_food(){
    int i;
    num_food = 50;
    for (i = 0; i < num_food; i++ ){
    x_food[i] = rand()%X_MUGA_max;
    y_food[i] = rand()%Y_MUGA_max;
    }
}

void init_snakeA(){
    int i;
    len_snakeA = 2;
    xA[0] = rand()%X_MUGA_max;
    yA[0] = rand()%Y_MUGA_max;
    for ( i = 1; i < len_snakeA; i++ )    {xA[i] = xA[i-1] + 1; yA[i] = yA[i-1];}
}

void init_snakeB(){
    int i;
    len_snakeB = 2;
    xB[0] = rand()%X_MUGA_max;
    yB[0] = rand()%Y_MUGA_max;
    for ( i = 1; i < len_snakeB; i++ )    {xB[i] = xB[i-1] + 1; yB[i] = yB[i-1];}
}


void init_ncurses(){
   initscr();
   getmaxyx(stdscr, max_y, max_x); 
   clear();
   noecho();
   cbreak();
   curs_set(FALSE);
  
   //Colors
   if(has_colors() == FALSE){    endwin(); printf("Compra't un altra pantalla, agarrat \n");    exit(1);    }
   start_color();              
   init_pair(1, COLOR_GREEN, COLOR_BLACK);
   init_pair(2, COLOR_RED, COLOR_BLACK);
   init_pair(3, COLOR_YELLOW, COLOR_BLACK);
   init_pair(4, COLOR_BLUE, COLOR_BLACK);
 
   min_x_joc = 0;
   max_x_joc = max_x;
   min_y_joc = 0;
   max_y_joc = max_y - FINESTRA_DADES_H;
   finestra_joc = newwin(max_y_joc, max_x_joc, min_y_joc, min_x_joc);
   nodelay(finestra_joc, TRUE);
   keypad(finestra_joc, TRUE);

   min_x_dades = 0;
   max_x_dades = max_x;    
   min_y_dades = max_y_joc;
   max_y_dades = FINESTRA_DADES_H;
   finestra_dades = newwin(max_y_dades, max_x_dades, min_y_dades, min_x_dades);
   nodelay(finestra_dades, TRUE);
   keypad(finestra_dades, TRUE);

   finestra_final = newwin(max_y / 4 , max_x / 4, max_y / 3, max_x / 3);
   nodelay(finestra_final, TRUE);
   keypad(finestra_final, TRUE);

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