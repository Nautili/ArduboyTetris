#include "Piece.h"

Shape::Shape() {
  
}

Piece::Piece() {
  //memcpy(&shape, &shapeTypes[random(7)], sizeof(unsigned char[4][4]));
}

void transpose(unsigned char** a, int width) {
  for(int i = 0; i < width; ++i) {
    for(int j = i + 1; j < width; ++j) {
      unsigned char temp = a[i][j];
      a[i][j] = a[j][i];
      a[j][i] = temp;
    }
  }
}

void flipColumns(unsigned char** a, int width) {
  for(int i = 0; i < width; ++i) {
    for(int j = 0; j < width / 2; ++j) {
      unsigned char temp = a[i][j];
      a[i][j] = a[i][width - j];
      a[i][width - j] = temp;
    }
  }
}

void flipRows(unsigned char** a, int width) {
  for(int j = 0; j < width; ++j) {
    for(int i = 0; i < width / 2; ++i) {
      unsigned char temp = a[i][j];
      a[i][j] = a[width - i][j];
      a[width - i][j] = temp;
    }
  }
}

void Piece::tryRotateCW() {
  transpose(shape.shapeArray, shape.width);
  flipColumns(shape.shapeArray, shape.width);
}

void Piece::tryRotateCCW() {
  transpose(shape.shapeArray, shape.width);
  flipRows(shape.shapeArray, shape.width);
}


