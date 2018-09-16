#include "mainwindow.h"
#include "ui_mainwindow.h"

bool is_element_in_vector(vector<int> v,int element){
    vector<int>::iterator it;
    it=find(v.begin(),v.end(),element);
    if (it!=v.end()){
        return true;
    }
    else{
        return false;
    }
}
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("**象棋对战平台**制作by小翊仔"));
    this->setMinimumSize(QSize(650,547));
    hhh=0;
    load=1;
    yourcolor=0;
    yourturn=0;
    steptime=200;
    yourtimeleft=steptime;
    othertimeleft=steptime;
    listenSocket=new QTcpServer;
    player = new QMediaPlayer;
    id1=-1;
    id3=startTimer(1000);
    for (int i=0;i<2;i++)
    {
        for (int j=0;j<16;j++)
        {
            piece[i][j]=new Piece;
        }
    }
    piece[0][0]->setPiece(0,9,0,5);
    piece[0][1]->setPiece(1,9,0,4);
    piece[0][2]->setPiece(2,9,0,3);
    piece[0][3]->setPiece(3,9,0,2);
    piece[0][4]->setPiece(4,9,0,1);
    piece[0][5]->setPiece(5,9,0,2);
    piece[0][6]->setPiece(6,9,0,3);
    piece[0][7]->setPiece(7,9,0,4);
    piece[0][8]->setPiece(8,9,0,5);
    piece[0][9]->setPiece(1,7,0,6);
    piece[0][10]->setPiece(7,7,0,6);
    piece[0][11]->setPiece(0,6,0,7);
    piece[0][12]->setPiece(2,6,0,7);
    piece[0][13]->setPiece(4,6,0,7);
    piece[0][14]->setPiece(6,6,0,7);
    piece[0][15]->setPiece(8,6,0,7);

    piece[1][0]->setPiece(0,0,1,5);
    piece[1][1]->setPiece(1,0,1,4);
    piece[1][2]->setPiece(2,0,1,3);
    piece[1][3]->setPiece(3,0,1,2);
    piece[1][4]->setPiece(4,0,1,1);
    piece[1][5]->setPiece(5,0,1,2);
    piece[1][6]->setPiece(6,0,1,3);
    piece[1][7]->setPiece(7,0,1,4);
    piece[1][8]->setPiece(8,0,1,5);
    piece[1][9]->setPiece(1,2,1,6);
    piece[1][10]->setPiece(7,2,1,6);
    piece[1][11]->setPiece(0,3,1,7);
    piece[1][12]->setPiece(2,3,1,7);
    piece[1][13]->setPiece(4,3,1,7);
    piece[1][14]->setPiece(6,3,1,7);
    piece[1][15]->setPiece(8,3,1,7);

    fresh();
}
void MainWindow::timerEvent(QTimerEvent *event)
{
    if (event->timerId()==id1)
    {
        yourtimeleft--;
        ui->YourTimeLCD->display(yourtimeleft);
        if (yourtimeleft<=0)
        {
            QByteArray *array =new QByteArray;
            array->clear();
            array->append(tr("t"));
            readWriteSocket->write(array->data());
            QMessageBox::about(this,tr("Oops!"),tr("你超时了!"));
            id1=-1;id2=-1;
        }
    }
    if (event->timerId()==id2)
    {
        othertimeleft--;
        ui->OppositeTimeLCD->display(othertimeleft);
    }
    if (event->timerId()==id3)
    {
        ui->OppositeTimeLCD->display(othertimeleft);
        ui->YourTimeLCD->display(yourtimeleft);
        qDebug()<<listenSocket->isListening();
    }
}



MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setBrush(Qt::NoBrush);
    QPixmap pix1;
    pix1.load(":/new/images/image/background1.png");
    painter.drawPixmap(0, 0, 500, 547, pix1);
    QPixmap pix;
    pix.load(":/new/images/image/background.png");
    painter.drawPixmap(50, 50, 400, 450, pix);
    for (int i=0;i<2;i++)
    {
        for (int j=0;j<16;j++)
        {
            if (piece[i][j]->color!=-1)
            {
                pix.load(piece[i][j]->path);
                painter.drawPixmap(25+50*piece[i][j]->x, 25+50*piece[i][j]->y, 50, 50, pix);
                if (piece[i][j]->clicked==1)
                {
                    QPen *pen=new QPen;
                    pen->setStyle(Qt::SolidLine);
                    pen->setWidth(3);
                    pen->setColor(Qt::yellow);
                    painter.setPen(*pen);
                    painter.drawRect(25+50*piece[i][j]->x, 25+50*piece[i][j]->y, 50, 50);
                    vector<int> a=movable(piece[i][j]);
                    for (int k=0;k<a.size();k++)
                    {
                        qDebug()<<k;
                        int _x=a[k]/10;
                        int _y=a[k]%10;
                        pen->setWidth(3);
                        pen->setColor(Qt::darkRed);
                        pen->setStyle(Qt::DashLine);
                        painter.setPen(*pen);
                        painter.drawRect(28+50*_x, 28+50*_y, 44, 44);
                    }
                }
            }
        }
    }


    //board[0][0]->setGeometry(250,250,50,50);
    //board[0][0]->setStyleSheet(board[0][0]->styleSheet().append(QString("border:5px solid rgb(53,206,141);")));

}

void MainWindow::fresh()
{
    for (int i=0;i<9;i++)
    {
        for (int j=0;j<10;j++)
        {
            exist[i][j]=-1;
        }
    }
    for (int i=0;i<2;i++)
    {
        for (int j=0;j<16;j++)
        {
            if (piece[i][j]->color!=-1)
            {
                exist[piece[i][j]->x][piece[i][j]->y]=piece[i][j]->color;
            }
        }
    }
}
vector<int> MainWindow::movable(Piece *p)
{
    vector<int> a;
    a.clear();
    fresh();
    if (p->color!=yourcolor||yourturn==0)
        return a;
    int x=p->x;
    int y=p->y;
    if (p->type==1)
    {
        if (x<5&&exist[x+1][y]!=p->color) a.push_back((x+1)*10+y);
        if (x>3&&exist[x-1][y]!=p->color) a.push_back((x-1)*10+y);
        if (y<9&&exist[x][y+1]!=p->color) a.push_back((x)*10+y+1);
        if (y>7&&exist[x][y-1]!=p->color) a.push_back((x)*10+y-1);
        int g=1;
        if (piece[p->color][4]->x==piece[!p->color][4]->x)
        {
            for (int i=piece[!p->color][4]->y+1;i<piece[p->color][4]->y;i++)
            {
                if (exist[piece[p->color][4]->x][i]!=-1)
                {
                    g=0;break;
                }
            }
            if (g)
            {
                a.push_back(piece[!p->color][4]->x*10+piece[!p->color][4]->y);
            }
        }

    }
    if (p->type==2)
    {
        if (x+1<=5&&y+1<=9&&exist[x+1][y+1]!=p->color) a.push_back((x+1)*10+y+1);
        if (x-1>=3&&y+1<=9&&exist[x-1][y+1]!=p->color) a.push_back((x-1)*10+y+1);
        if (x+1<=5&&y-1>=7&&exist[x+1][y-1]!=p->color) a.push_back((x+1)*10+y-1);
        if (x-1>=3&&y-1>=7&&exist[x-1][y-1]!=p->color) a.push_back((x-1)*10+y-1);
    }
    if (p->type==3)
    {
        if (x+2<=8&&y+2<=9&&exist[x+2][y+2]!=p->color&&exist[x+1][y+1]==-1) a.push_back((x+2)*10+y+2);
        if (x-2>=0&&y+2<=9&&exist[x-2][y+2]!=p->color&&exist[x-1][y+1]==-1) a.push_back((x-2)*10+y+2);
        if (x+2<=8&&y-2>=5&&exist[x+2][y-2]!=p->color&&exist[x+1][y-1]==-1) a.push_back((x+2)*10+y-2);
        if (x-2>=0&&y-2>=5&&exist[x-2][y-2]!=p->color&&exist[x-1][y-1]==-1) a.push_back((x-2)*10+y-2);
    }
    if (p->type==4)
    {
        if (x+2<=8&&y+1<=9&&exist[x+2][y+1]!=p->color&&exist[x+1][y]==-1) a.push_back((x+2)*10+y+1);
        if (x+2<=8&&y-1>=0&&exist[x+2][y-1]!=p->color&&exist[x+1][y]==-1) a.push_back((x+2)*10+y-1);

        if (x+1<=8&&y+2<=9&&exist[x+1][y+2]!=p->color&&exist[x][y+1]==-1) a.push_back((x+1)*10+y+2);
        if (x-1>=0&&y+2<=9&&exist[x-1][y+2]!=p->color&&exist[x][y+1]==-1) a.push_back((x-1)*10+y+2);

        if (x-2>=0&&y+1<=9&&exist[x-2][y+1]!=p->color&&exist[x-1][y]==-1) a.push_back((x-2)*10+y+1);
        if (x-2>=0&&y-1>=0&&exist[x-2][y-1]!=p->color&&exist[x-1][y]==-1) a.push_back((x-2)*10+y-1);

        if (x+1<=8&&y-2<=9&&exist[x+1][y-2]!=p->color&&exist[x][y-1]==-1) a.push_back((x+1)*10+y-2);
        if (x-1<=8&&y-2<=9&&exist[x-1][y-2]!=p->color&&exist[x][y-1]==-1) a.push_back((x-1)*10+y-2);
    }
    if (p->type==5)
    {
        int i=1;
        while(x+i<=8)
        {
            if (exist[x+i][y]==p->color)
            {
                break;
            }
            else if (exist[x+i][y]==!p->color)
            {
                a.push_back((x+i)*10+y);
                break;
            }
            a.push_back((x+i)*10+y);
            i++;
        }
        i=1;
        while(y-i>=0)
        {
            if (exist[x][y-i]==p->color)
            {
                break;
            }
            else if (exist[x][y-i]==!p->color)
            {
                a.push_back((x)*10+y-i);
                break;
            }
            a.push_back((x)*10+y-i);
            i++;
        }
        i=1;
        while(x-i>=0)
        {
            if (exist[x-i][y]==p->color)
            {
                break;
            }
            else if (exist[x-i][y]==!p->color)
            {
                a.push_back((x-i)*10+y);
                break;
            }
            a.push_back((x-i)*10+y);
            i++;
        }
        i=1;
        while(y+i<=9)
        {
            if (exist[x][y+i]==p->color)
            {
                break;
            }
            else if (exist[x][y+i]==!p->color)
            {
                a.push_back((x)*10+y+i);
                break;
            }
            a.push_back((x)*10+y+i);
            i++;
        }
    }
    if (p->type==6)
    {
        int i=1;
        while(x+i<=8)
        {
            if (exist[x+i][y]!=-1)
            {
                i++;
                while(x+i<=8)
                {
                    if (exist[x+i][y]==!p->color)
                    {
                        a.push_back((x+i)*10+y);
                        break;
                    }
                    else if (exist[x+i][y]==p->color)
                    {
                        break;
                    }
                    i++;
                }
                break;
            }
            a.push_back((x+i)*10+y);
            i++;
        }
        i=1;
        while(y-i>=0)
        {
            if (exist[x][y-i]!=-1)
            {
                i++;
                while(y-i>=0)
                {
                    if (exist[x][y-i]==!p->color)
                    {
                        a.push_back((x)*10+y-i);
                        break;
                    }
                    else if (exist[x][y-i]==p->color)
                    {
                        break;
                    }
                    i++;
                }
                break;
            }
            a.push_back((x)*10+y-i);
            i++;
        }
        i=1;
        while(x-i>=0)
        {
            if (exist[x-i][y]!=-1)
            {
                i++;
                while(x-i>=0)
                {
                    if (exist[x-i][y]==!p->color)
                    {
                        a.push_back((x-i)*10+y);
                        break;
                    }
                    else if (exist[x-i][y]==p->color)
                    {
                        break;
                    }
                    i++;
                }
                break;
            }
            a.push_back((x-i)*10+y);
            i++;
        }
        i=1;
        while(y+i<=9)
        {
            if (exist[x][y+i]!=-1)
            {
                i++;
                while(y+i<=9)
                {
                    if (exist[x][y+i]==!p->color)
                    {
                        a.push_back((x)*10+y+i);
                        break;
                    }
                    else if (exist[x][y+i]==p->color)
                    {
                        break;
                    }
                    i++;
                }
                break;
            }
            a.push_back((x)*10+y+i);
            i++;
        }
    }
    if (p->type==7)
    {
        if (y>=5)
        {
            if (exist[x][y-1]!=p->color) a.push_back((x)*10+y-1);
        }
        if (y<=4)
        {
            if (x+1<=8)
            if (exist[x+1][y]!=p->color) a.push_back((x+1)*10+y);
            if (x-1>=0)
            if (exist[x-1][y]!=p->color) a.push_back((x-1)*10+y);
            if (y-1>=0)
            if (exist[x][y-1]!=p->color) a.push_back((x)*10+y-1);
        }
    }
    return a;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    int x=event->x();
    int y=event->y();
    int tmpx=0,tmpy=0,g1=0,g2=0,win=0;
    int i1=0,j1=0,i2=0,j2=0;
    for (int i=0;i<9;i++)
    {
        for (int j=0;j<10;j++)
        {
            if (25+i*50<x&&x<25+50+i*50&&25+j*50<y&&y<25+50+j*50)
            {
                tmpx=i,tmpy=j;
                break;
            }
        }
    }
    for (int i=0;i<2;i++)
    {
        for (int j=0;j<16;j++)
        {
            if (piece[i][j]->color!=-1&&piece[i][j]->clicked==1)
            {
                g1=1;
                i1=i;
                j1=j;
            }
        }
    }

    for (int i=0;i<2;i++)
    {
        for (int j=0;j<16;j++)
        {
            if (piece[i][j]->color!=-1&&piece[i][j]->x==tmpx&&piece[i][j]->y==tmpy)
            {
                g2=1;i2=i;j2=j;
            }
        }
    }

    if (g1)
    {
        if(!is_element_in_vector(movable(piece[i1][j1]),tmpx*10+tmpy))
        {
            piece[i1][j1]->clicked=0;
            if (g2)
            {
                piece[i2][j2]->clicked=1;
            }
            repaint();
            return;
        }
    }//如果没有点中

    if (g1&&g2)//eat
    {
        piece[i2][j2]->color=-1;
        piece[i1][j1]->setXY(tmpx,tmpy);
        piece[i1][j1]->clicked=0;
        if (i2==!i1&&j2==4) win=1;
        repaint();

    }
    if (g1&&!g2)//走，但不吃
    {
        piece[i1][j1]->setXY(tmpx,tmpy);
        piece[i1][j1]->clicked=0;

        repaint();

    }
    if (g1&&hhh)
    {
        QString writeString;
        writeString.clear();
        writeString.append('m');

        writeString.append(",");
        writeString.append(QString::number(i1));
        writeString.append(",");
        writeString.append(QString::number(j1));
        writeString.append(",");
        writeString.append(QString::number(tmpx));
        writeString.append(",");
        writeString.append(QString::number(tmpy));
        if (win==1)
        {
            writeString.append(",");
            writeString.append("l");
            player->setMedia(QUrl("qrc:/new/sound/sound/win.mp3"));
            player->play();

        }
        else if (is_element_in_vector(movable(piece[i1][j1]),piece[!i1][4]->x*10+piece[!i1][4]->y))
        {
            writeString.append(",");
            writeString.append("j");
            player->setMedia(QUrl("qrc:/new/sound/sound/start.mp3"));
            player->play();
        }
        else if (g2)
        {
            writeString.append(",");
            writeString.append("e");
            player->setMedia(QUrl("qrc:/new/sound/sound/jumpMove.mp3"));
            player->play();
        }
        else
        {
            writeString.append(",");
            writeString.append("m");
            player->setMedia(QUrl("qrc:/new/sound/sound/directMove.mp3"));
            player->play();
        }
        QByteArray *array =new QByteArray;
        array->clear();
        array->append(writeString);
        readWriteSocket->write(array->data());
        yourturn=0;
        id1=-1;
        id2=startTimer(1000);
        othertimeleft=steptime;
        yourtimeleft=steptime;
        if (win)
        {
            id1=-1;
            id2=-1;
            QMessageBox::about(this,tr("Congratulations!"),tr("你赢了!"));
        }
    }
    if (!g1&&g2)
    {
        piece[i2][j2]->clicked=1;
        repaint();
    }
    fresh();

}

void MainWindow::sentMessage()
{
    QByteArray *array =new QByteArray;
    array->clear();
    array->append(ui->inputEdit->toPlainText());
    this->readWriteSocket->write(array->data());
}

void MainWindow::move(int i1, int j1, int x, int y)
{
    int i2=0,j2=0,g2=0;
    for (int i=0;i<2;i++)
    {
        for (int j=0;j<16;j++)
        {
            if (piece[i][j]->color!=-1&&piece[i][j]->x==x&&piece[i][j]->y==y)
            {
                g2=1;i2=i;j2=j;
            }
        }
    }
    if (g2)//eat
    {
        piece[i2][j2]->color=-1;
        piece[i1][j1]->setXY(x,y);
        piece[i1][j1]->clicked=0;
        repaint();
    }
    if (!g2)
    {
        piece[i1][j1]->setXY(x,y);
        piece[i1][j1]->clicked=0;
        repaint();
    }
    fresh();
}


void MainWindow::initialize()
{
    initializeDialog = new QDialog(this);
    QLabel *label_0 = new QLabel;
    QPushButton *button_1 = new QPushButton;
    QPushButton *button_2 = new QPushButton;
    QVBoxLayout *layout = new QVBoxLayout;
    QHBoxLayout *layout_1 = new QHBoxLayout;
    label_0->setText(tr("Choose the way to start"));
    button_1->setText(tr("Start a room"));
    button_2->setText(tr("Enter a room"));
    layout_1->addWidget(button_1);
    layout_1->addWidget(button_2);
    layout->addWidget(label_0);
    layout->addLayout(layout_1);
    initializeDialog->setLayout(layout);
    connect(button_1,SIGNAL(clicked(bool)),this,SLOT(initServer()));
    connect(button_2,SIGNAL(clicked(bool)),this,SLOT(connectHost()));
    initializeDialog->show();
    initializeDialog->exec();
}

void MainWindow::initServer()
{
    initializeDialog->close();
    waitingDialog = new QDialog(this);
    QLabel *label_1 = new QLabel;
    QLabel *label_2 = new QLabel;
    QPushButton *button = new QPushButton;
    edit = new QLineEdit;
    QVBoxLayout *layout = new QVBoxLayout;
    QHBoxLayout *layout_1 = new QHBoxLayout;
    QString ipAddress;
        QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
        for (int i = 0; i < ipAddressesList.size(); ++i) {
            qDebug()<<ipAddressesList.at(i).toString();
             if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
                 ipAddressesList.at(i).toIPv4Address()) {
                 ipAddress = ipAddressesList.at(i).toString();
                 break;
           }
        }
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    address = ipAddress;
    QString sentence_1;
    sentence_1.clear();
    sentence_1.append(tr("Your IP :"));
    sentence_1.append(address);
    label_1->setText(sentence_1);
    label_2->setText(tr("Port:"));
    button->setText(tr("Ok"));
    layout_1->addWidget(label_2);
    layout_1->addWidget(edit);
    layout->addWidget(label_1);
    layout->addLayout(layout_1);
    layout->addWidget(button);
    waitingDialog->setLayout(layout);
    connect(button,SIGNAL(clicked(bool)),this,SLOT(initServer_2()));
    //this->listenSocket =new QTcpServer;
    waitingDialog->show();
    waitingDialog->exec();
}

void MainWindow::initServer_2()
{
    waitingDialog->close();
    newWaitingDialog = new QDialog(this);
    QLabel *label_1 = new QLabel;
    QLabel *label_2 = new QLabel;
    QLabel *label_3 = new QLabel;
    QPushButton *button = new QPushButton;
    QString sentence_1;
    QString sentence_2;
    button->setText(tr("Cancel"));
    sentence_1.append("Your IP :");
    sentence_1.append(address);
    label_2->setText(sentence_1);
    sentence_2.append("Port:");
    sentence_2.append(edit->text());
    label_3->setText(sentence_2);
    label_1->setText(tr("Waiting for connection..."));
    QVBoxLayout *layout_1 = new QVBoxLayout;
    layout_1->addWidget(label_2);
    layout_1->addWidget(label_3);
    layout_1->addWidget(label_1);
    layout_1->addWidget(button);
    newWaitingDialog->setLayout(layout_1);
    QString port = edit->text();
    int portInt = port.toInt();
    this->listenSocket->listen(QHostAddress::Any,portInt);
    QObject::connect(this->listenSocket,SIGNAL(newConnection()),this,SLOT(acceptConnection()));

    QObject::connect(button,SIGNAL(clicked(bool)),newWaitingDialog,SLOT(close()));
    QObject::connect(button,SIGNAL(clicked(bool)),this->listenSocket,SLOT(close()));
    newWaitingDialog->show();
    newWaitingDialog->exec();
}



void MainWindow::acceptConnection()
{
    hhh=1;
    newWaitingDialog->close();
    ui->outputEdit->append(tr("Connected!"));
    this->readWriteSocket = this->listenSocket->nextPendingConnection();
    QObject::connect(this->readWriteSocket,SIGNAL(readyRead()),this,SLOT(recvMessage()));
    //id1=startTimer(1000);
    //this->show();
}

void MainWindow::connectHost()
{

    initializeDialog->close();
    connectingDialog = new QDialog(this);
    QLabel *label_1 = new QLabel;
    QLabel *label_2 = new QLabel;
    line_1 = new QLineEdit;
    line_2 = new QLineEdit;
    QPushButton *button = new QPushButton;
    QHBoxLayout *layout_1 = new QHBoxLayout;
    QHBoxLayout *layout_2 = new QHBoxLayout;
    QVBoxLayout *layout = new QVBoxLayout;
    label_1->setText(tr("The other IP:"));
    label_2->setText(tr("Port:"));
    button->setText(tr("Connect"));
    layout_1->addWidget(label_1);
    layout_1->addWidget(line_1);
    layout_2->addWidget(label_2);
    layout_2->addWidget(line_2);
    layout->addLayout(layout_1);
    layout->addLayout(layout_2);
    layout->addWidget(button);
    connectingDialog->setLayout(layout);
    this->readWriteSocket = new QTcpSocket;
    this->chatSocket = new QTcpSocket;
    connect(button,SIGNAL(clicked(bool)),this,SLOT(connectHost_2()));
    connectingDialog->show();
    connectingDialog->exec();
}

void MainWindow::connectHost_2()
{

    QString address = line_1->text();
    QString portString = line_2->text();
    int port = portString.toInt();
    this->readWriteSocket->connectToHost(QHostAddress(address),port);
    connect(readWriteSocket,SIGNAL(connected()),this,SLOT(connected()));
    if(!readWriteSocket->waitForConnected())
        connectFail();

}

void MainWindow::connected()
{
    hhh=1;
    ui->outputEdit->append(tr("Connected!"));
    connectingDialog->close();
    QObject::connect(this->readWriteSocket,SIGNAL(readyRead()),this,SLOT(recvMessage()));
    //id2=startTimer(1000);
    player->setMedia(QUrl("qrc:/new/prefix1/sounds/jumpMove.mp3"));
    player->play();
    //this->show();
}

void MainWindow::connectFail()
{
    QMessageBox::about(NULL,tr("Error"),tr("IP or Port Invalid!"));
}

void MainWindow::recvMessage()
{
    QString info;
    info += this->readWriteSocket->readAll();
    for (int i=0;i<info.size();i++)
    {
        //qDebug()<<info[i];
    }
    if (info[0]=='d')
    {
        QString info1;
        info1.clear();
        for (int i=1;i<info.size();i++)
        {
            info1[i-1]=info[i];
        }
        ui->outputEdit->append(info1);
    }

    if (info[0]=="m")
    {
        QStringList list_0 = info.split(",");
        //qDebug()<<list_0;
        QString string_1 = list_0[1];
        QString string_2 = list_0[2];
        QString string_3 = list_0[3];
        QString string_4 = list_0[4];
        QString string_5 = list_0[5];
        qDebug()<<string_1;
        int i=string_1.toInt();

        int j=string_2.toInt();
        int x=8-string_3.toInt();
        int y=9-string_4.toInt();
        qDebug()<<i<<" "<<j<<" "<<x<<" "<<y;
        move(i,j,x,y);
        yourturn=1;
        if (string_5[0]=='m')
        {
            player->setMedia(QUrl("qrc:/new/sound/sound/directMove.mp3"));
            player->play();
        }
        if (string_5[0]=='e')
        {
            player->setMedia(QUrl("qrc:/new/sound/sound/jumpMove.mp3"));
            player->play();
        }
        if (string_5[0]=='j')
        {
            player->setMedia(QUrl("qrc:/new/sound/sound/start.mp3"));
            player->play();
        }
        if (string_5[0]=='l')
        {
            player->setMedia(QUrl("qrc:/new/sound/sound/start.mp3"));
            player->play();
            id1=-1;
            QMessageBox::about(this,tr("Oops!"),tr("你输了hhhh"));
        }
        yourtimeleft=steptime;
        id1=startTimer(1000);if (string_5[0]=='l') id1=-1;
        othertimeleft=steptime;
        id2=-1;
    }
    if(info[0]=='w')
    {
        id1=-1;
        id2=-1;
        QMessageBox::about(this,tr("Congratulations!"),tr("对方认输!"));
    }
    if(info[0]=='t')
    {
        id1=-1;
        id2=-1;
        QMessageBox::about(this,tr("Congratulations!"),tr("对方超时,你获得胜利!"));
    }
    if (info[0]=='s')
    {
        yourcolor=1;
        yourturn=0;
        piece[0][0]->setPiece(8-0,9-9,0,5);
        piece[0][1]->setPiece(8-1,9-9,0,4);
        piece[0][2]->setPiece(8-2,9-9,0,3);
        piece[0][3]->setPiece(8-3,9-9,0,2);
        piece[0][4]->setPiece(8-4,9-9,0,1);
        piece[0][5]->setPiece(8-5,9-9,0,2);
        piece[0][6]->setPiece(8-6,9-9,0,3);
        piece[0][7]->setPiece(8-7,9-9,0,4);
        piece[0][8]->setPiece(8-8,9-9,0,5);
        piece[0][9]->setPiece(8-1,9-7,0,6);
        piece[0][10]->setPiece(8-7,9-7,0,6);
        piece[0][11]->setPiece(8-0,9-6,0,7);
        piece[0][12]->setPiece(8-2,9-6,0,7);
        piece[0][13]->setPiece(8-4,9-6,0,7);
        piece[0][14]->setPiece(8-6,9-6,0,7);
        piece[0][15]->setPiece(8-8,9-6,0,7);

        piece[1][0]->setPiece(8-0,9-0,1,5);
        piece[1][1]->setPiece(8-1,9-0,1,4);
        piece[1][2]->setPiece(8-2,9-0,1,3);
        piece[1][3]->setPiece(8-3,9-0,1,2);
        piece[1][4]->setPiece(8-4,9-0,1,1);
        piece[1][5]->setPiece(8-5,9-0,1,2);
        piece[1][6]->setPiece(8-6,9-0,1,3);
        piece[1][7]->setPiece(8-7,9-0,1,4);
        piece[1][8]->setPiece(8-8,9-0,1,5);
        piece[1][9]->setPiece(8-1,9-2,1,6);
        piece[1][10]->setPiece(8-7,9-2,1,6);
        piece[1][11]->setPiece(8-0,9-3,1,7);
        piece[1][12]->setPiece(8-2,9-3,1,7);
        piece[1][13]->setPiece(8-4,9-3,1,7);
        piece[1][14]->setPiece(8-6,9-3,1,7);
        piece[1][15]->setPiece(8-8,9-3,1,7);
        id1=-1;
        id2=startTimer(1000);
        yourtimeleft=steptime;
        othertimeleft=steptime;
        repaint();
    }
    if (info[0]=='c')
    {
        //yourturn=0;
        for (int i=0;i<2;i++)
        {
            for (int j=0;j<16;j++)
            {
                piece[i][j]->color=-1;
            }
        }
        QStringList list = info.split(",");

        QString string1 = list[1];
        int cfirst=string1.toInt();

        //qDebug()<<list;
        for (int k=1;k*6<=list.size();k++)
        {
            QString string_1 = list[6*k-5];
            QString string_2 = list[6*k-4];
            QString string_3 = list[6*k-3];
            QString string_4 = list[6*k-2];
            QString string_5 = list[6*k-1];
            QString string_6 = list[6*k];
            int color=string_1.toInt();
            int num=string_2.toInt();
            int x=8-string_3.toInt();
            int y=9-string_4.toInt();
            int i=string_5.toInt();
            int j=string_6.toInt();
            piece[color][num]->setPiece(x,y,i,j);
        }
        if (piece[0][4]->y<4)
        {
            yourcolor=1;
        }
        else
        {
            yourcolor=0;
        }
        if (yourcolor==cfirst) yourturn=1;
        else yourturn=0;
        repaint();
    }
    if (info[0]=='b')
    {
        load=0;
        ui->actionSave_S->trigger();
    }
    if (info[0]=='a')
    {
        ui->outputEdit->append(tr("对方向你发送了一句【我喜欢你！！】"));
    }
}


void MainWindow::on_sendButton_clicked()
{
    if (hhh==0)
    {
        ui->outputEdit->append(tr("Not Connected!"));
        ui->inputEdit->clear();
        return;
    }
    QByteArray *array =new QByteArray;
    array->clear();
    array->append(tr("d"));
    array->append(tr("对方: "));
    array->append(ui->inputEdit->toPlainText());
    this->readWriteSocket->write(array->data());
    ui->outputEdit->append(tr("我: ")+ui->inputEdit->toPlainText());
    ui->inputEdit->clear();
}

void MainWindow::on_actionConnect_C_triggered()
{
    initialize();
}

void MainWindow::on_pushButton_clicked()
{
    if (hhh==0) return;
    id1=-1;
    id2=-1;
    QByteArray *array =new QByteArray;
    array->clear();
    array->append(tr("w"));
    this->readWriteSocket->write(array->data());
    QMessageBox::about(this,tr("Oops!"),tr("您已认输!"));
}

void MainWindow::on_actionSave_S_triggered()
{


    QString fileName = QFileDialog::getSaveFileName (this,
                tr("保存残局"),"",tr("日志文件 (*.txt)"));
    if(!fileName.isEmpty ())
    {
        QFile file(fileName);
        if(!file.open (QIODevice::WriteOnly))
        {
            QMessageBox msgBox;
            msgBox.setText("保存文件失败！");
            msgBox.exec();
        }
        else
        {
            vector<int> x[8],y[8];
            QString qs;
            qs.clear();
            for (int i=0;i<=7;i++)
            {
                x[i].clear();y[i].clear();
            }

            if (yourturn)
            {
                if (yourcolor==0)
                {
                    qs.append(tr("red"));qs.append('\n');
                }
                else
                {
                    qs.append(tr("black"));qs.append('\n');
                }
                for (int i=0;i<2;i++)
                {
                    for (int j=0;j<16;j++)
                    {
                        if (piece[i][j]->color==yourcolor)
                        {
                            x[piece[i][j]->type].push_back(piece[i][j]->x);
                            y[piece[i][j]->type].push_back(piece[i][j]->y);
                        }
                    }
                }
                for (int i=1;i<=7;i++)
                {
                    qs.append(QString::number(x[i].size()));
                    for (int j=0;j<x[i].size();j++)
                    {
                        qs.append(" <");qs.append(QString::number(x[i][j]));
                        qs.append(",");qs.append(QString::number(9-y[i][j]));
                        qs.append(">");
                    }
                    qs.append('\n');
                }

                for (int i=0;i<=7;i++)
                {
                    x[i].clear();y[i].clear();
                }

                if (!yourcolor==0)
                {
                    qs.append(tr("red"));qs.append('\n');
                }
                else
                {
                    qs.append(tr("black"));qs.append('\n');
                }
                for (int i=0;i<2;i++)
                {
                    for (int j=0;j<16;j++)
                    {
                        if (piece[i][j]->color==!yourcolor)
                        {
                            x[piece[i][j]->type].push_back(piece[i][j]->x);
                            y[piece[i][j]->type].push_back(piece[i][j]->y);
                        }
                    }
                }
                for (int i=1;i<=7;i++)
                {
                    qs.append(QString::number(x[i].size()));
                    for (int j=0;j<x[i].size();j++)
                    {
                        qs.append(" <");qs.append(QString::number(x[i][j]));
                        qs.append(",");qs.append(QString::number(9-y[i][j]));
                        qs.append(">");
                    }
                    qs.append('\n');
                }
            }
            if (!yourturn)
            {
                if (!yourcolor==0)
                {
                    qs.append(tr("red"));qs.append('\n');
                }
                else
                {
                    qs.append(tr("black"));qs.append('\n');
                }
                for (int i=0;i<2;i++)
                {
                    for (int j=0;j<16;j++)
                    {
                        if (piece[i][j]->color==!yourcolor)
                        {
                            x[piece[i][j]->type].push_back(piece[i][j]->x);
                            y[piece[i][j]->type].push_back(piece[i][j]->y);
                        }
                    }
                }
                for (int i=1;i<=7;i++)
                {
                    qs.append(QString::number(x[i].size()));
                    for (int j=0;j<x[i].size();j++)
                    {
                        qs.append(" <");qs.append(QString::number(x[i][j]));
                        qs.append(",");qs.append(QString::number(9-y[i][j]));
                        qs.append(">");
                    }
                    qs.append('\n');
                }

                for (int i=0;i<=7;i++)
                {
                    x[i].clear();y[i].clear();
                }

                if (yourcolor==0)
                {
                    qs.append(tr("red"));qs.append('\n');
                }
                else
                {
                    qs.append(tr("black"));qs.append('\n');
                }
                for (int i=0;i<2;i++)
                {
                    for (int j=0;j<16;j++)
                    {
                        if (piece[i][j]->color==yourcolor)
                        {
                            x[piece[i][j]->type].push_back(piece[i][j]->x);
                            y[piece[i][j]->type].push_back(piece[i][j]->y);
                        }
                    }
                }
                for (int i=1;i<=7;i++)
                {
                    qs.append(QString::number(x[i].size()));
                    for (int j=0;j<x[i].size();j++)
                    {
                        qs.append(" <");qs.append(QString::number(x[i][j]));
                        qs.append(",");qs.append(QString::number(9-y[i][j]));
                        qs.append(">");
                    }
                    qs.append('\n');
                }
            }


            //qs.append(tr("hhh"));
            QTextStream stream(&file);
            stream << qs ;
            stream.flush ();
            file.close ();
        }
    }

    if (load)
    {
        QByteArray *array =new QByteArray;
        array->clear();
        array->append(tr("b"));
        this->readWriteSocket->write(array->data());
    }
    load=1;


}

void MainWindow::on_actionLoad_L_triggered()
{
    /*if (!hhh)
    {
        initialize();
        return;
    }*/
    QString fileName = QFileDialog::getOpenFileName(this);

         // 如果文件名不为空，则加载文件
    if (fileName.isEmpty())
    {
        return;
    }

    QFile file(fileName); // 新建QFile对象
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("多文档编辑器"),
                              tr("无法读取文件 %1:\n%2.")
                              .arg(fileName).arg(file.errorString()));
        return; // 只读方式打开文件，出错则提示，并返回false
    }
    for (int i=0;i<2;i++)
    {
        for (int j=0;j<16;j++)
        {
            piece[i][j]->color=-1;
        }
    }

    QTextStream in(&file);
    QString writeString;
    writeString.clear();
    writeString.append("c");
    QStringList userpwd;
    QString tmps;
    QString onecount=in.readLine();
    int cfirst=0;
    if (onecount[0]=='r')
    {
        cfirst=0;
    }
    else
    {
        cfirst=1;
    }
    {
        onecount = in.readLine();
        userpwd = onecount.split(' ');
        for (int j=1;j<userpwd.size();j++)
        {
            tmps=userpwd.at(j);
            int tmpa=tmps[1].toLatin1()-'0';
            int tmpb=tmps[3].toLatin1()-'0';tmpb=9-tmpb;
            piece[cfirst][4]->setPiece(tmpa,tmpb,cfirst,1);
            writeString.append(",");writeString.append(QString::number(cfirst));
            writeString.append(",");writeString.append(QString::number(4));
            writeString.append(",");writeString.append(QString::number(tmpa));
            writeString.append(",");writeString.append(QString::number(tmpb));
            writeString.append(",");writeString.append(QString::number(cfirst));
            writeString.append(",");writeString.append(QString::number(1));
        }
    }

    int num=-1;
    for (int i=2;i<=7;i++)
    {
        onecount = in.readLine();
        userpwd = onecount.split(' ');
        for (int j=1;j<userpwd.size();j++)
        {
            num++;if (num==4) continue;
            tmps=userpwd.at(j);
            int tmpa=tmps[1].toLatin1()-'0';
            int tmpb=tmps[3].toLatin1()-'0';tmpb=9-tmpb;
            piece[cfirst][num]->setPiece(tmpa,tmpb,cfirst,i);
            writeString.append(",");writeString.append(QString::number(cfirst));
            writeString.append(",");writeString.append(QString::number(num));
            writeString.append(",");writeString.append(QString::number(tmpa));
            writeString.append(",");writeString.append(QString::number(tmpb));
            writeString.append(",");writeString.append(QString::number(cfirst));
            writeString.append(",");writeString.append(QString::number(i));
        }
    }
    in.readLine();

    {
        onecount = in.readLine();
        userpwd = onecount.split(' ');
        for (int j=1;j<userpwd.size();j++)
        {
            tmps=userpwd.at(j);
            int tmpa=tmps[1].toLatin1()-'0';
            int tmpb=tmps[3].toLatin1()-'0';tmpb=9-tmpb;
            piece[!cfirst][4]->setPiece(tmpa,tmpb,!cfirst,1);
            writeString.append(",");writeString.append(QString::number(!cfirst));
            writeString.append(",");writeString.append(QString::number(4));
            writeString.append(",");writeString.append(QString::number(tmpa));
            writeString.append(",");writeString.append(QString::number(tmpb));
            writeString.append(",");writeString.append(QString::number(!cfirst));
            writeString.append(",");writeString.append(QString::number(1));
        }
    }
    num=-1;
    for (int i=2;i<=7;i++)
    {
        onecount = in.readLine();
        userpwd = onecount.split(' ');
        for (int j=1;j<userpwd.size();j++)
        {
            num++;if (num==4) continue;
            tmps=userpwd.at(j);
            int tmpa=tmps[1].toLatin1()-'0';
            int tmpb=tmps[3].toLatin1()-'0';tmpb=9-tmpb;
            piece[!cfirst][num]->setPiece(tmpa,tmpb,!cfirst,i);
            writeString.append(",");writeString.append(QString::number(!cfirst));
            writeString.append(",");writeString.append(QString::number(num));
            writeString.append(",");writeString.append(QString::number(tmpa));
            writeString.append(",");writeString.append(QString::number(tmpb));
            writeString.append(",");writeString.append(QString::number(!cfirst));
            writeString.append(",");writeString.append(QString::number(i));
        }
    }
    if (piece[0][4]->y<4)
    {
        yourcolor=1;

    }
    else
    {
        yourcolor=0;
    }
    if (cfirst==yourcolor)
    {
        yourturn=1;
    }
    else
    {
        yourturn=0;
    }
    QByteArray *array =new QByteArray;
    array->clear();
    array->append(writeString);
    this->readWriteSocket->write(array->data());
    file.close();

    repaint();
    return;
}

void MainWindow::on_pushButton_2_clicked()
{
    if (!hhh)
    {
        initialize();
        return;
    }
    id1=startTimer(1000);
    id2=-1;
    yourtimeleft=steptime;
    othertimeleft=steptime;
    yourturn=1;
    yourcolor=0;

    piece[0][0]->setPiece(0,9,0,5);
    piece[0][1]->setPiece(1,9,0,4);
    piece[0][2]->setPiece(2,9,0,3);
    piece[0][3]->setPiece(3,9,0,2);
    piece[0][4]->setPiece(4,9,0,1);
    piece[0][5]->setPiece(5,9,0,2);
    piece[0][6]->setPiece(6,9,0,3);
    piece[0][7]->setPiece(7,9,0,4);
    piece[0][8]->setPiece(8,9,0,5);
    piece[0][9]->setPiece(1,7,0,6);
    piece[0][10]->setPiece(7,7,0,6);
    piece[0][11]->setPiece(0,6,0,7);
    piece[0][12]->setPiece(2,6,0,7);
    piece[0][13]->setPiece(4,6,0,7);
    piece[0][14]->setPiece(6,6,0,7);
    piece[0][15]->setPiece(8,6,0,7);

    piece[1][0]->setPiece(0,0,1,5);
    piece[1][1]->setPiece(1,0,1,4);
    piece[1][2]->setPiece(2,0,1,3);
    piece[1][3]->setPiece(3,0,1,2);
    piece[1][4]->setPiece(4,0,1,1);
    piece[1][5]->setPiece(5,0,1,2);
    piece[1][6]->setPiece(6,0,1,3);
    piece[1][7]->setPiece(7,0,1,4);
    piece[1][8]->setPiece(8,0,1,5);
    piece[1][9]->setPiece(1,2,1,6);
    piece[1][10]->setPiece(7,2,1,6);
    piece[1][11]->setPiece(0,3,1,7);
    piece[1][12]->setPiece(2,3,1,7);
    piece[1][13]->setPiece(4,3,1,7);
    piece[1][14]->setPiece(6,3,1,7);
    piece[1][15]->setPiece(8,3,1,7);

        QByteArray *array =new QByteArray;
        array->clear();
        array->append(tr("s"));
        this->readWriteSocket->write(array->data());
        repaint();

}

void MainWindow::on_actionDisconnect_D_triggered()
{
    listenSocket->close();
}

void MainWindow::on_pushButton_3_clicked()
{
    if (!hhh)
    {
        initialize();
        return;
    }
    QByteArray *array =new QByteArray;
    array->clear();
    array->append(tr("a"));
    this->readWriteSocket->write(array->data());
    repaint();
    ui->outputEdit->append(tr("你向对方发送了一句【我喜欢你！！】"));

}
