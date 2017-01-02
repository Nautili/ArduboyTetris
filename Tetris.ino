#include "Arduboy.h"
#include "Piece.h"

Arduboy arduboy;

const int BOARD_HEIGHT = 22;
const int BOARD_WIDTH = 10;
unsigned char board[BOARD_HEIGHT][BOARD_WIDTH];

bool gameRunning = false;

Piece bag[7];

void manageGame() {
  // handle delay between moves
  handleInput();

  // handle gravity delay
  movePiece();
  
}

void drawBoard() {
  
}

void setup() {
  for(int i = 0; i < BOARD_HEIGHT; ++i) {
    for(int j = 0; j < BOARD_WIDTH; ++j) {
      board[i][j] = 0;
    }
  }
}

void loop() {
  if(!arduboy.nextFrame()) {
    return;
  }
  
  //TODO: Handle start
  while(!gameRunning) {
    //wait until start button pressed
  }
  
  manageGame();
  drawBoard();
}
