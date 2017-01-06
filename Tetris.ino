#include "Arduboy.h"
#include "Piece.h"

Arduboy arduboy;

const int BOARD_HEIGHT = 22;
const int BOARD_WIDTH = 10;
unsigned char board[BOARD_HEIGHT][BOARD_WIDTH];

const int BOARD_X = 20;
const int BOARD_Y = 2;
const int CELL_SIZE = 3;

const int INIT_DROP_DELAY = 1000; // delay in ms
const int MOVE_DELAY = 100;
int dropDelay;
int dropTimer;
int moveTimer;

bool gameRunning = false;
Piece *bag;
Piece *curPiece;

bool isValid(Piece piece) {
  if(piece.xPos < 0 || piece.xPos + piece.width > BOARD_WIDTH) {
    return false;
  }

  for(int i = 0; i < piece.width; ++i) {
    for(int j = 0; j < piece.width; ++j) {
      if((piece.shape[i][j] & board[piece.xPos + i][piece.yPos + j]) == 1) {
        return false;
      }
    }
  }

  return true;
}

void handleInput() {
  if(moveTimer <= 0) {
    if(arduboy.pressed(LEFT_BUTTON)) {
      
    }
    if(arduboy.pressed(RIGHT_BUTTON)) {
      
    }
    if(arduboy.pressed(DOWN_BUTTON)) {
      
    }
    if(arduboy.pressed(UP_BUTTON)) {
      
    }
    if(arduboy.pressed(A_BUTTON)) {
      curPiece->tryRotateCCW();
    }
    if(arduboy.pressed(B_BUTTON)) {
      curPiece->tryRotateCW();
    }
  }
}

void dropPiece() {
  if(dropTimer <= 0) {
    
  }
}

void manageGame() {
  // handle delay between moves
  handleInput();

  // handle gravity delay
  dropPiece();
  
}

void drawBoard() {
  for(int i = 0; i < BOARD_WIDTH; ++i) {
    for(int j = 0; j < BOARD_HEIGHT; ++j) {
      arduboy.drawRect(BOARD_X + i * CELL_SIZE, BOARD_Y + j * CELL_SIZE, CELL_SIZE, CELL_SIZE, 1);
    }
  }
}

void drawPieces(Piece piece) {
  for(int i = 0; i < piece.width; ++i) {
    for(int j = 0; j < piece.width; ++j) {
      if(piece.shape[i][j] == 1) {
        arduboy.drawRect(BOARD_X + (piece.xPos + i) * CELL_SIZE, BOARD_Y + (piece.yPos + j) * CELL_SIZE, CELL_SIZE, CELL_SIZE, 1);
      }
    }
  }
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
  //drawPieces();
}
