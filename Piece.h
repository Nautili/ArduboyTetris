
#ifndef PIECE_H
#define PIECE_H

class Shape {
  public:
    Shape();
    int width;
    unsigned char** shapeArray;
};

class Piece {
  public:
    Piece();
    void tryRotateCW();
    void tryRotateCCW();
    
  private:
    Shape shape;
};

#endif
