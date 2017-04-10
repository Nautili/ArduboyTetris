#include "Arduboy.h"
#include "Piece.h"
#include "Background.h"

Arduboy arduboy;

const int BOARD_HEIGHT = 22;
const int BOARD_WIDTH = 10;
unsigned char board[BOARD_HEIGHT][BOARD_WIDTH]; //TODO: Convert to less memory intensive data structure

const int BOARD_X = 25;
const int BOARD_Y = 2;
const int CELL_SIZE = 3;

const int INIT_DROP_DELAY = 60; // delay in frames
const int MOVE_DELAY = 8;
const int FASTEST_LEVEL = 10;
int dropDelay;
int dropTimer;
int moveTimer;

const int MAX_GAME_OVER_DELAY = 30;
int gameOverDelay;

bool gameRunning = false;
bool gameOver = false;
int bagIndex = 0;
Piece bag[7];
Piece curPiece;
Piece nextPiece;
bool pieceActive = false;

int level;
int clearedLines;
unsigned long score;

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

void addScore(int numLines) {
  int points;
  switch(numLines) {
    case 1:
      points = 40 * level;
      break;
    case 2:
      points = 100 * level;
      break;
    case 3:
      points = 300 * level;
      break;
    case 4:
      points = 1200 * level;
      break;
    default:
      points = 0;
  }

  score += points;
}

void clearLine(int clearRow) {
  for(int row = clearRow; row > 0; --row) {
    for(int col = 0; col < BOARD_WIDTH; ++col) {
      board[row][col] = board[row-1][col];
    }
  }
}

void clearLines() {
  int currentClears = 0;
  
  for(int row = 2; row < BOARD_HEIGHT; ++row) {
    bool lineComplete = true;
    for(int col = 0; col < BOARD_WIDTH; ++col) {
      if(board[row][col] == 0) {
        lineComplete = false;
      }
    }

    if(lineComplete) {
      clearLine(row);
      currentClears++;
    }
  }

  addScore(currentClears);
  clearedLines += currentClears;
  level = (clearedLines / 10) + 1;

  float speedup = (FASTEST_LEVEL - level + 1) / (float)FASTEST_LEVEL;
  dropDelay = max(INIT_DROP_DELAY * speedup, 0);
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
    bool inBounds = false;
    for(int i = 0; i < piece.width; ++i) {
      for(int j = 0; j < piece.width; ++j) {
        if(piece.shape[i][j] == 1) {
          board[piece.row + i][piece.col + j] = 1;

          if(piece.row + i > 1) {
            inBounds = true;
          }
        }
      }
    }
    pieceActive = false;
    
    if(!inBounds) {
      gameRunning = false;
      gameOver = true;
    }
    else {
      clearLines();
    }
  }
  else {
    piece.row++;
  }
}

void handleInput() {
  if(moveTimer <= 0) {
    if(arduboy.buttonsState() > 0) {
      moveTimer = MOVE_DELAY;
    }
    
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
    else if(arduboy.pressed(UP_BUTTON)) {
      while(pieceActive) {
        dropPiece(curPiece);
      }
      dropTimer = dropDelay;
    }
    
    if(arduboy.pressed(A_BUTTON)) {
      curPiece.rotateCCW();
      if(!isValid(curPiece)) {
        curPiece.rotateCW();
      }
    }
    if(arduboy.pressed(B_BUTTON)) {
      curPiece.rotateCW();
      if(!isValid(curPiece)) {
        curPiece.rotateCCW();
      }
    }
  }
}

void getNewBag() {
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

void initGame() {
  level = 1;
  clearedLines = 0;
  score = 0;
  bagIndex = 0;
  getNewBag();
  nextPiece = bag[0];
  gameOverDelay = MAX_GAME_OVER_DELAY;
  
  dropDelay = INIT_DROP_DELAY;
  dropTimer = dropDelay;
  moveTimer = MOVE_DELAY;
  
  for(int i = 0; i < BOARD_HEIGHT; ++i) {
    for(int j = 0; j < BOARD_WIDTH; ++j) {
      board[i][j] = 0;
    }
  }
}

void manageGame() {
  //TODO: handle start screen
  if(gameOver) {
    gameOverDelay--;
    if(arduboy.buttonsState() > 0 && gameOverDelay <= 0) {
      gameOver = false;
      initGame();
    }
    return;
  }
  
  dropTimer--;
  moveTimer--;

  if(!pieceActive) {
    bagIndex = (bagIndex + 1) % 7;
    curPiece = nextPiece;
    nextPiece = bag[bagIndex];
    if(bagIndex == 6) {
      getNewBag();
    }
    pieceActive = true;
  }

  handleInput();
  if(dropTimer <= 0) {
    dropPiece(curPiece);
    dropTimer = dropDelay;
  }
}

void drawBackground() {
  arduboy.drawBitmap(0, 0, background, WIDTH, HEIGHT, WHITE);
}

void drawBoard() {
  arduboy.drawRect(BOARD_X - 2, BOARD_Y - 2, BOARD_WIDTH * CELL_SIZE + 4, (BOARD_HEIGHT - 1) * CELL_SIZE + 1, WHITE);
  
  for(int i = 2; i < BOARD_HEIGHT; ++i) {
    for(int j = 0; j < BOARD_WIDTH; ++j) {
      if(board[i][j] == 1) {
        arduboy.drawRect(BOARD_X + j * CELL_SIZE, BOARD_Y + (i - 2) * CELL_SIZE, CELL_SIZE, CELL_SIZE, WHITE);
      }
    }
  }
}

void drawNextPiece(Piece& piece, int x, int y) {
  for(int i = 0; i < piece.width; ++i) {
    for(int j = 0; j < piece.width; ++j) {
      if(piece.shape[i][j] == 1) {
        arduboy.drawRect(x + j * CELL_SIZE, y + i * CELL_SIZE, CELL_SIZE, CELL_SIZE, WHITE);
      }
    }
  }
}

void drawPiece(Piece& piece) {
  for(int i = 0; i < piece.width; ++i) {
    for(int j = 0; j < piece.width; ++j) {
      if(piece.shape[i][j] == 1) {
        arduboy.drawRect(BOARD_X + (piece.col + j) * CELL_SIZE, BOARD_Y + (piece.row + (i-2)) * CELL_SIZE, CELL_SIZE, CELL_SIZE, WHITE);
      }
    }
  }
}

void drawGameInfo() {
  arduboy.drawRect(60, 0, 47, 11, WHITE);
  arduboy.drawRect(60, 12, 47, 52, WHITE);
  arduboy.setCursor(66, 2);
  arduboy.print("Lvl:");
  arduboy.print(level);

  arduboy.setCursor(66, 14);
  arduboy.print("Lines:");
  arduboy.setCursor(66, 24);
  arduboy.print(clearedLines);
  arduboy.setCursor(66, 34);
  arduboy.print("Score:");
  arduboy.setCursor(66, 44);
  arduboy.print(score);
  arduboy.setCursor(66, 54);
  drawNextPiece(nextPiece, 80, 54);
}

void drawGameOver() {
  arduboy.fillRect(WIDTH / 2 - 28, HEIGHT / 2 - 14, 57, 10, BLACK);
  arduboy.setCursor(WIDTH / 2 - 27, HEIGHT / 2 - 13);
  arduboy.print("Game Over");
  arduboy.setCursor(WIDTH / 2 - 42, HEIGHT / 2 - 4);
  arduboy.print("Press any Button");
  arduboy.setCursor(WIDTH / 2 - 30, HEIGHT / 2 + 4);
  arduboy.print("To Restart");
}

void drawFrame() {
  arduboy.clear();
  drawBackground();
  drawBoard();
  drawPiece(curPiece);
  drawGameInfo();
  if(gameOver) {
    drawGameOver();
  }
  arduboy.display();
}

void setup() {
  arduboy.begin();
  arduboy.setFrameRate(60);
  arduboy.display();
  arduboy.initRandomSeed();
  
  initGame();
}

void loop() {
  if(!arduboy.nextFrame()) {
    return;
  }
  
  manageGame();
  drawFrame();
}
