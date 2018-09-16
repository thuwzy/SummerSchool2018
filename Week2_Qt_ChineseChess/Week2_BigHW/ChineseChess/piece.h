#ifndef PIECE_H
#define PIECE_H
#include <QString>

class Piece
{
public:
    Piece();
    int x,y;
    int color,type;
    int clicked;
    void setXY(int,int);
    void setPiece(int,int,int,int);
    QString path;
};

#endif // PIECE_H
