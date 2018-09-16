#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMouseEvent>
#include <QPaintEvent>
#include <QMainWindow>
#include <QWidget>
#include <QtNetwork>
#include <QDialog>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QDebug>
#include <QSignalMapper>
#include <vector>
#include <algorithm>
#include <QStringList>
#include <QEventLoop>
#include <QTimer>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <stdlib.h>
#include <QPalette>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QMediaPlayer>
#include <QTimerEvent>
#include "cell.h"
#include "piece.h"
using namespace std;

struct Move
{
    vector<int> eatChess;
    vector<int> path;
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    int load;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    int yourcolor,yourturn,yourtimeleft,othertimeleft;
    int hhh;
    int id1,id2,id3,id4,steptime;
    void timerEvent(QTimerEvent*);
    void initialize();
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);
    void sentMessage();
    void move(int,int,int,int);
    vector<int> getJumpMove(int,int);
    vector<int> getDirectMove(int,int);
    void dfs(int,int,bool);
    void checkFail();
    void fail();
    void moveEnemy_Jump(int,int,vector<int>,vector<int>);
    void moveEnemy_Direct(int,int,int,int);
    void moveSelf_Jump(int,int,vector<int>,vector<int>);
    void moveSelf_Direct(int,int,int,int);
    void enemyGiveUp();
    void enemySueForDraw();
    //void showMovable();
    void draw();
    vector<int> movable(Piece*);
    int exist[9][10];
    void fresh();
public slots:
    void connectHost();
    void initServer();
    void recvMessage();
    void connectHost_2();
    void initServer_2();
    void acceptConnection();
    void connected();
    void connectFail();
    //void chessClicked(int);
    //void boardClicked(int);
    //void succeed();
    //void giveUp();
    //void sueForDraw();
    //void customBoard();
    //void openSound();
    //void closeSound();

private slots:
    void on_sendButton_clicked();

    void on_actionConnect_C_triggered();

    void on_pushButton_clicked();

    void on_actionSave_S_triggered();

    void on_actionLoad_L_triggered();

    void on_pushButton_2_clicked();

    void on_actionDisconnect_D_triggered();

    void on_pushButton_3_clicked();

private:

    Ui::MainWindow *ui;
    QDialog *connectingDialog;
    QDialog *waitingDialog;
    QDialog *initializeDialog;
    QDialog *sueDialog;
    QTcpServer  *listenSocket;
    QTcpSocket *readWriteSocket;
    QTcpSocket *chatSocket;
    QLineEdit *line_1;
    QLineEdit *line_2;
    Cell *chess[20][20];
    Cell *board[20][20];
    Piece* piece[2][20];
    QSignalMapper *mapper;
    int selfColor;
    Move moveState[10][10];
    bool myTurn;
    bool isFirstStage;
    int nowChess;
    int counter;
    bool isCounting;
    bool finished;
    vector<int> direct;
    vector<int> jump;
    //QPushButton *buttonGiveUp;
    QLineEdit *edit;
    //QPushButton *buttonSueForDraw;
    QDialog *newWaitingDialog;
    QMediaPlayer *player;
    //vector<int> movable;
    bool onlyDirect;
    QString address;
};


#endif // MAINWINDOW_H
