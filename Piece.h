
#ifndef PIECE_H
#define PIECE_H

class Piece {
  public:
    int xPos;
    int yPos;
    int width;
    unsigned char shape[4][4];
    
    Piece(int, int);
    void tryRotateCW();
    void tryRotateCCW();
    
  private:
    static const unsigned char possibleShapes[][4][4];
    static const unsigned char possibleWidths[];
};

#endif
