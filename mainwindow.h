#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include "player.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    player sokoban;

    int mode; //0:wating, 1:playing, 2:win

    //Record
    int num_step=0; //current level steps
    int num_tstep=0; //total steps
    int score=0;
    int record;

    //Image
    QImage img_cover = QImage(":/imgs/cover.jpeg");
    QImage img_wall = QImage(":/imgs/wall.png");
    QImage img_box = QImage(":/imgs/box.png");
    QImage img_target = QImage(":/imgs/target.png");
    QImage img_tbox = QImage(":/imgs/tbox.png");
    QImage img_user = QImage(":/imgs/user.png");
    QImage img_coin = QImage(":/imgs/coin.png");
    QImage img_portal = QImage(":/imgs/portal.png");
    QImage img_bomb = QImage(":/imgs/bomb.png");
    QColor bg_color = QColor("#ffffff");

    //Map
    int map_width;
    int map_height;
    int block_width = 50;
    QPoint origin=QPoint(0, 20); //Position

    //Read&Load
    QString map_filepath;
    bool ReadMapFile(MapVec &mapData);
    int cur_level;
    void SwitchLevel(int level);
    void LoadGame();

    void ReadRecord();
    void SaveRecord();
    //void SaveStatus(); //Save Step&Score
    //void SaveGameMap(); //Save Map

    //Event
    void keyPressEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *event);

public slots:
    void AddStep(int num);
    void AddTStep(int num);
    void AddScore(int num);

    void on_actionStart_triggered();
    void on_actionReset_triggered();
    void on_actionExit_triggered();
    //void on_actionLoad_triggered();
    //void on_actionSave_triggered();
    void on_actionSelect_triggered();
    void on_actionLV1_triggered();
    void on_actionLV2_triggered();
    void on_actionLV3_triggered();
    void on_actionLV4_triggered();
    void on_actionLV5_triggered();
    void on_actionDefault_triggered();
    void on_actionBlackAndWhite_triggered();

};

#endif // MAINWINDOW_H
