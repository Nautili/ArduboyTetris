
#ifndef PIECE_H
#define PIECE_H

class Piece {
  public:
    int xPos;
    int yPos;
    int width;
    unsigned char shape[4][4];

    Piece();
    Piece(int, int, int);
    void rotateCW();
    void rotateCCW();
    
  private:
    static const unsigned char possibleShapes[][4][4];
    static const unsigned char possibleWidths[];
};

#endif
