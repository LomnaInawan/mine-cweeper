#include <ncurses.h>

WINDOW *game_win;
WINDOW *game_frame;
int screenX, screenY, cursorX = 0, cursorY = 0;
char input = 'a';
char boardA[81]; //The board with mines and numbers
char boardB[81]; //The board with tiles
char board_str[9][17] ={
                      {"# # # # # # # # #"},
                      {"# # # # # # # # #"},
                      {"# # # # # # # # #"},
                      {"# # # # # # # # #"},
                      {"# # # # # # # # #"},
                      {"# # # # # # # # #"},
                      {"# # # # # # # # #"},
                      {"# # # # # # # # #"},
                      {"# # # # # # # # #"}
                      };

int boardIndex(int x, int y){
  return (x * 9 + y);
}

void inputFunction(){
  input = getch();
  //Input switch case
  switch(input){
  case 'd':
    cursorX += 2;
    break;
  case 'a':
    cursorX -= 2;
    break;
  case 'w':
    cursorY--;
    break;
  case 's':
    cursorY++;
    break;
  case 'q':
    input = 'c';
    break;
  }
}

void writeBoard(){
  for(int i = 0; i < 9; i++)
    mvwprintw(game_win, i, 0, board_str[i]);
}

int main(void){
  //Set initial board
  for(int i = 0; i < 81; i++){
    boardA[i] = '0';
    boardB[i] = 'h';
  }
  //Initialize function
  initscr();
  raw(); //Take input without needing to press return
  noecho(); //Don't show user input on screen
  refresh();
  getmaxyx(stdscr, screenY, screenX); //Get terminal dimensions
  game_win = newwin(9,17,1,2); //Create the game window
  game_frame = newwin(11,21,0,0); //Create the game frame
  box(game_frame, 0, 0); //For the border for game window
  wprintw(game_frame, "Mines"); //Print mines in frame
  wrefresh(game_frame);

  //main game loop
  while(input != 'c'){
    writeBoard();
    wmove(game_win, cursorY, cursorX);
    getyx(game_win, cursorY, cursorX);
    wrefresh(game_win);
    inputFunction();
  }

  endwin();
  return 0;
}
