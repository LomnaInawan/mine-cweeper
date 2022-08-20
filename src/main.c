#include <ncurses.h>

WINDOW *game_win;
int screenX, screenY, cursorX, cursorY;
char input = 'a';
char boardA[81];
char boardB[81];

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
  game_win = newwin(12,22,0,0); //Create the game window
  box(game_win, 0, 0); //For the border for game window
  wprintw(game_win, "Hello"); //Print hello in window
  wrefresh(game_win);

  getyx(game_win, cursorY, cursorX);

  //main game loop
  while(input != 'c'){
    inputFunction();
    writeBoard();
    wmove(game_win, cursorY, cursorX);
    getyx(game_win, cursorY, cursorX);
    wrefresh(game_win);
  }

  endwin();
  return 0;
}
