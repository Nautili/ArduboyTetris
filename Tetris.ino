#include "Arduboy.h"
#include "Piece.h"

Arduboy arduboy;

const int BOARD_HEIGHT = 22;
const int BOARD_WIDTH = 10;
unsigned char board[BOARD_HEIGHT][BOARD_WIDTH];

const int BOARD_X = 20;
const int BOARD_Y = 2;
const int CELL_SIZE = 3;

const int INIT_DROP_DELAY = 60; // delay in frames
const int MOVE_DELAY = 10;
int dropDelay;
int dropTimer;
int moveTimer;

bool gameRunning = false;
Piece bag[7];
int bagIndex = 7;
Piece curPiece;
bool pieceActive = false;

bool inBounds(Piece& piece) {
  for(int row = 0; row < piece.width; ++row) {
    for(int col = 0; col < piece.width; ++col) {
      if(piece.shape[row][col] == 1 && 
         ((piece.col + col < 0) || (piece.col + col >= BOARD_WIDTH))) 
      {
        return false;    
      }
    }
  }
  return true;
}

bool isValid(Piece& piece) {
  if(!inBounds(piece)) {
    return false;
  }
  
  for(int i = 0; i < piece.width; ++i) {
    for(int j = 0; j < piece.width; ++j) {
      if((piece.shape[i][j] == 1 && board[piece.row + i][piece.col + j]) == 1) {
        return false;
      }
    }
  }

  return true;
}

void clearLines() {
  //TODO: Implement this  
}

void dropPiece(Piece& piece) {
  bool isDropped = false;
  for(int row = 0; row < piece.width; ++row) {
    for(int col = 0; col < piece.width; ++col) {
      if((piece.shape[row][col] == 1) && 
        ((board[piece.row + row + 1][piece.col + col] == 1) || (piece.row + row + 1 >= BOARD_HEIGHT)))
      {
        isDropped = true;
      }
    }
  }

  if(isDropped) {
    for(int i = 0; i < piece.width; ++i) {
      for(int j = 0; j < piece.width; ++j) {
        if(piece.shape[i][j] == 1) {
          board[piece.row + i][piece.col + j] = 1;
        }
      }
    }
    pieceActive = false;
    clearLines();
  }
  else {
    piece.row++;
  }
}

//TODO: Polish inputs
void handleInput() {
  if(moveTimer <= 0) {
    moveTimer = MOVE_DELAY;
    if(arduboy.pressed(LEFT_BUTTON)) {
      curPiece.col--;
      if(!isValid(curPiece)) {
        curPiece.col++;
      }
    }
    if(arduboy.pressed(RIGHT_BUTTON)) {
      curPiece.col++;
      if(!isValid(curPiece)) {
        curPiece.col--;
      }
    }
    if(arduboy.pressed(DOWN_BUTTON)) {
      dropPiece(curPiece);
      dropTimer = dropDelay;
    }
    if(arduboy.pressed(UP_BUTTON)) {
      while(pieceActive) {
        dropPiece(curPiece);
      }
    }
    if(arduboy.pressed(A_BUTTON)) {
      curPiece.rotateCW();
      if(!isValid(curPiece)) {
        curPiece.rotateCCW();
      }
    }
    if(arduboy.pressed(B_BUTTON)) {
      curPiece.rotateCCW();
      if(!isValid(curPiece)) {
        curPiece.rotateCW();
      }
    }
  }
}

void getNewBag(Piece bag[]) {
  for(int i = 0; i < 7; ++i) {
    bag[i] = Piece(BOARD_WIDTH/2 - 1, 0, i);
  }

  for(int i = 0; i < 7; ++i) {
    int randIndex = random(7);
    Piece temp = bag[randIndex];
    bag[randIndex] = bag[i];
    bag[i] = temp;
  }
}

void manageGame() {
  dropTimer--;
  moveTimer--;

  if(bagIndex > 6) {
    getNewBag(bag);
    bagIndex = 0;
  }

  if(!pieceActive) {
    curPiece = bag[bagIndex++];
    pieceActive = true;
  }
  
  handleInput();
  if(dropTimer <= 0) {
    dropPiece(curPiece);
    dropTimer = dropDelay;
  }
}

void drawBoard() {
  arduboy.drawRect(BOARD_X - 2, BOARD_Y - 2, BOARD_WIDTH * CELL_SIZE + 4, (BOARD_HEIGHT - 1) * CELL_SIZE + 1, 1);
  
  for(int i = 2; i < BOARD_HEIGHT; ++i) {
    for(int j = 0; j < BOARD_WIDTH; ++j) {
      if(board[i][j] == 1) {
        arduboy.drawRect(BOARD_X + j * CELL_SIZE, BOARD_Y + (i - 2) * CELL_SIZE, CELL_SIZE, CELL_SIZE, 1);
      }
    }
  }
}

void drawPieces(Piece& piece) {
  for(int i = 0; i < piece.width; ++i) {
    for(int j = 0; j < piece.width; ++j) {
      if(piece.shape[i][j] == 1) {
        arduboy.drawRect(BOARD_X + (piece.col + j) * CELL_SIZE, BOARD_Y + (piece.row + (i-2)) * CELL_SIZE, CELL_SIZE, CELL_SIZE, 1);
      }
    }
  }
}

void setup() {
  arduboy.begin();
  arduboy.setFrameRate(60);
  arduboy.display();
  
  dropDelay = INIT_DROP_DELAY;
  dropTimer = dropDelay;
  moveTimer = MOVE_DELAY;
  
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
  /*while(!gameRunning) {
    if(arduboy.pressed(B_BUTTON)) {
      gameRunning = true;
    }
  }*/
  
  manageGame();
  arduboy.clear();
  drawBoard();
  drawPieces(curPiece);
  arduboy.display();
}
