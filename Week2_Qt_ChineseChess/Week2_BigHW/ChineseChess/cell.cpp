#include "cell.h"

Cell::Cell(QWidget *parent) :
    QPushButton(parent)
{

}

Cell::Cell()
{
    type = -1;
    color = -1;//1:Red 2:Black
    clicked = 0;
    x=-1;y=-1;
}

void Cell::setXY(int _x, int _y)
{
    x=_x;y=_y;
    setGeometry(25+50*x,25+50*y,50,50);
}
void Cell::set(int _color,int _type)
{
    color=_color;
    type=_type;
    fresh();
}

void Cell::fresh()
{

    QString ss;
    ss.clear();
    ss.append("border-style:flat;");
    if (color==1)
    {
        if (type==1)ss.append("background-image:url(:/new/images/image/R1.png);");
        if (type==2)ss.append("background-image:url(:/new/images/image/R2.png);");
        if (type==3)ss.append("background-image:url(:/new/images/image/R3.png);");
        if (type==4)ss.append("background-image:url(:/new/images/image/R4.png);");
        if (type==5)ss.append("background-image:url(:/new/images/image/R5.png);");
        if (type==6)ss.append("background-image:url(:/new/images/image/R6.png);");
        if (type==7)ss.append("background-image:url(:/new/images/image/R7.png);");
    }
    if (color==2)
    {
        if (type==1)ss.append("background-image:url(:/new/images/image/B1.png);");
        if (type==2)ss.append("background-image:url(:/new/images/image/B2.png);");
        if (type==3)ss.append("background-image:url(:/new/images/image/B3.png);");
        if (type==4)ss.append("background-image:url(:/new/images/image/B4.png);");
        if (type==5)ss.append("background-image:url(:/new/images/image/B5.png);");
        if (type==6)ss.append("background-image:url(:/new/images/image/B6.png);");
        if (type==7)ss.append("background-image:url(:/new/images/image/B7.png);");
    }


    this->setStyleSheet(ss);

}

void Cell::setType(int a)
{

    type = a;
    fresh();

}

