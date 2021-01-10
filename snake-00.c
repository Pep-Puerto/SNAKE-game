#include <stdio.h>
#include <ncurses.h>


#define UP          -1
#define DOWN         1
#define RIGHT        1
#define LEFT        -1

//Finestra
WINDOW *finestra;

//Coordenades del box
int start_x = 0;
int start_y = 0;
int max_x;
int max_y;

//Coordenades del bitxo
int x_ant, y_ant;
int x, y;

void print_box(WINDOW *finestra);
void print_cuc(WINDOW *finestra);

int main(){
int c;

   initscr();
   clear();
   noecho();
   cbreak();
   curs_set(FALSE);  

   x = start_x;
   y = start_y;  
   getmaxyx(stdscr, max_y, max_x);
   finestra = newwin(max_y, max_x, start_y, start_x);
   keypad(finestra, TRUE);
   refresh();
   print_box(finestra);
  
   while(1)    {  
           x_ant = x;
           y_ant = y;
        c = wgetch(finestra);
        switch(c) {  
            case KEY_UP:
                y+=UP;
                break;
            case KEY_DOWN:
                y+=DOWN;
                break;
            case KEY_LEFT:
                x+=LEFT;
                break;
            case KEY_RIGHT:
                x+=RIGHT;
                break;
            default:
                refresh();
                break;
   }
   //Si xoca contra laterals rebota
   if ( x > max_x )   x = max_x;
   if ( x < 0 )       x = 0;    
   //Si xoca contra dalt o baix rebota
   if ( y > max_y )   y = max_y;     
   if ( y < 0 )       y = 0;    
   print_box(finestra);
   print_cuc(finestra);
   }  
   clrtoeol();
   refresh();
   endwin();
   return 0;
}

void print_cuc(WINDOW *finestra){
mvwprintw(finestra,10,10,"( %d, %d ) ( %d, %d )", y_ant, x_ant, y, x );   
     mvwaddch(finestra, y, x, ACS_CKBOARD );
     mvwaddch(finestra, y_ant, x_ant, ' ' );
    wrefresh(finestra);
}

void print_box(WINDOW *finestra){
    box(finestra, start_x, start_y);
    wrefresh(finestra);
}
