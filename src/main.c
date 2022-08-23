#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#define boardIndex(x,y) (x*9+y)

WINDOW *game_win;
WINDOW *game_frame;
int screenX, screenY, cursorX = 0, cursorY = 0, moves = 0;
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

int surrounding[] = {0,1,
                     1,0,
                     1,1,
                     -1,-1,
                     -1,0,
                     0,-1,
                     -1,1,
                     1,-1};

char TileNumber(int x, int y){
  if(boardA[boardIndex(x,y)] == 'm')
    return 'm';
  int tempX, tempY;
  char _no_mines = 48; //zero
  for(int i = 0; i < 8; i++){
    tempX = x + surrounding[2*i];
    tempY = y + surrounding[2*i + 1];
    if(tempX > 8 || tempX < 0 ||tempY < 0 || tempY > 8)
      continue;
    if(boardA[boardIndex(tempX, tempY)] == 'm')
      _no_mines += 1;
  }
  return _no_mines;
}

void clearSurrounding(int x, int y){
  boardB[boardIndex(x,y)] = 'm'; //mark the tile as visited

  int tempX, tempY;
  for(int i = 0; i < 8; i++){
    tempX = x + surrounding[2*i];
    tempY = y + surrounding[2*i + 1];
    if(tempX > 8 || tempX < 0 ||tempY < 0 || tempY > 8)
      continue;
    if(boardA[boardIndex(tempX, tempY)] == '0' && boardB[boardIndex(tempX, tempY)] != 'm')
      clearSurrounding(tempX, tempY);
    else if(boardB[boardIndex(tempX, tempY)] == 'h')
      boardB[boardIndex(tempX, tempY)] = 's';
  }

  boardB[boardIndex(x,y)] = 's'; //return the tile to normal status
}

void generateBoard(){
  //Set initial board
  for(int i = 0; i < 81; i++){
    boardA[i] = '0'; //Numbers to show number of mines, m to show mines
    boardB[i] = 'h'; //h for hidden, s for shown
  }
  //Place mines
  int _randNum;
  for(int i = 0; i < 10; i++){
    _randNum = rand() % 81;
    while(boardA[_randNum] == 'm')
      _randNum = rand() % 81;
    boardA[_randNum] = 'm';
  }
  //Calculate numbers
  for(int y = 0; y < 9; y++){
    for(int x =0; x < 9; x++)
      boardA[boardIndex(x,y)] = TileNumber(x,y);
  }
}

void inputFunction(){
  input = getch();
  //Input switch case
  switch(input){
  case 'd':
    if(cursorX <= 14)
      cursorX += 2;
    break;
  case 'a':
    if(cursorX >= 2)
      cursorX -= 2;
    break;
  case 'w':
    if(cursorY >= 1)
      cursorY--;
    break;
  case 's':
    if(cursorY <= 7)
      cursorY++;
    break;
  case 'e':
    //Reveal tile and check if 0 or a mine
    if(boardA[boardIndex(cursorX / 2, cursorY)] == '0')
      clearSurrounding(cursorX/2 , cursorY);
    if(boardB[boardIndex(cursorX / 2, cursorY)] == 'h')
      boardB[boardIndex(cursorX / 2, cursorY)] = 's';
    break;
  case 'q':
    input = 'c';
    break;
  }
}

void writeBoard(){
  for(int y = 0; y < 9; y++){
    char *str = board_str[y];
    for(int x = 0; x < 9; x++){
      if(boardB[boardIndex(x,y)] == 's')
        board_str[y][2*x] = boardA[boardIndex(x,y)];
      else if(boardB[boardIndex(x,y)] == 'h')
        board_str[y][2*x] = '#';
    }
  }

  for(int i = 0; i < 9; i++)
    mvwprintw(game_win, i, 0, board_str[i]);
}

int main(void){
  srand(time(0));
  generateBoard();
  //Initialize function
  initscr();
  raw(); //Take input without needing to press return
  noecho(); //Don't show user input on screen
  mvprintw(11,0,"Press q or c to quit");
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
