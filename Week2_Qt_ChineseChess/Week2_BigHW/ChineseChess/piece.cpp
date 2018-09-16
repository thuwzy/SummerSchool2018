#include "piece.h"

Piece::Piece()
{
    x=-1,y=-1,color=-1,type=-1;//color 0:red 1:black -1:unvisible
    clicked=0;
}

void Piece::setXY(int x1, int y1)
{
    x=x1,y=y1;
}

void Piece::setPiece(int x1, int y1, int color1, int type1)
{
    x=x1,y=y1,color=color1,type=type1;
    if (color==0)
    {
        if (type==1)path=":/new/images/image/R1.png";
        if (type==2)path=":/new/images/image/R2.png";
        if (type==3)path=":/new/images/image/R3.png";
        if (type==4)path=":/new/images/image/R4.png";
        if (type==5)path=":/new/images/image/R5.png";
        if (type==6)path=":/new/images/image/R6.png";
        if (type==7)path=":/new/images/image/R7.png";
    }
    if (color==1)
    {
        if (type==1)path=":/new/images/image/B1.png";
        if (type==2)path=":/new/images/image/B2.png";
        if (type==3)path=":/new/images/image/B3.png";
        if (type==4)path=":/new/images/image/B4.png";
        if (type==5)path=":/new/images/image/B5.png";
        if (type==6)path=":/new/images/image/B6.png";
        if (type==7)path=":/new/images/image/B7.png";
    }
}
