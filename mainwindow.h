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
    int num_step;
    int cur_level;

    //Image
    QImage img_wall = QImage(":/imgs/wall.png");
    QImage img_box = QImage(":/imgs/box.png");
    QImage img_target = QImage(":/imgs/target.png");
    QImage img_tbox = QImage(":/imgs/tbox.png");
    QImage img_user = QImage(":/imgs/user.png");

    //Map
    int map_width;
    int map_height;
    int block_width = 50;
    QPoint origin=QPoint(0, 20); //Position
    QString map_filepath;

    //Load
    bool readMapFromFile(MapVec &mapData);
    void LoadGame();
    void SwitchLevel(int level);

    //Event
    void keyPressEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *event);

public slots:
    void AddStep(int num);
    void on_actionStart_triggered();
    //void on_actionReset_triggered();
    void on_actionExit_triggered();
    //void on_actionLoad_triggered();
    //void on_actionSave_triggered();
    void on_actionSelect_triggered();
    void on_actionLV1_triggered();
    void on_actionLV2_triggered();
    void on_actionLV3_triggered();
    void on_actionLV4_triggered();
    //void on_actionLV5_triggered();
};

#endif // MAINWINDOW_H
