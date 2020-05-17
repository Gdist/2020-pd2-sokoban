#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Sokoban");

    num_step=0;
    mode = 0;

    connect(ui->StartButton, SIGNAL(clicked()), this, SLOT(on_actionStart_triggered()));
    connect(ui->ExitButton, SIGNAL(clicked()), this, SLOT(on_actionExit_triggered()));
    connect(ui->ResetButton, SIGNAL(clicked()), this, SLOT(on_actionReset_triggered()));
    connect(&sokoban, &player::send_AddStep, this, &MainWindow::AddStep);
    connect(&sokoban, &player::send_AddTStep, this, &MainWindow::AddTStep);
    connect(&sokoban, &player::send_AddScore, this, &MainWindow::AddScore);

}

MainWindow::~MainWindow(){
    delete ui;
}
void MainWindow::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_Up: case Qt::Key_W: sokoban.MoveStep(1); break;
        case Qt::Key_Down: case Qt::Key_S: sokoban.MoveStep(2); break;
        case Qt::Key_Left: case Qt::Key_A: sokoban.MoveStep(3); break;
        case Qt::Key_Right: case Qt::Key_D: sokoban.MoveStep(4); break;
        default: break;
    }
    update();

    if (sokoban.check_win) {
        QMessageBox::information(this, "", "You Win! Click \"OK\" to challenge next level.");
        SwitchLevel(++cur_level);
    }
}
void MainWindow::AddStep(int num){
    num_step += num;
    ui->labelStep->setNum(num_step);
}
void MainWindow::AddTStep(int num){
    num_tstep += num;
    ui->labelTStep->setNum(num_tstep);
}
void MainWindow::AddScore(int num){
    score += num;
    ui->labelScore->setNum(score);
}
bool MainWindow::ReadMapFile(MapVec &mapData) {
    QFile file(map_filepath);
    if(!file.open(QIODevice::ReadOnly)) return false;

    QTextStream *out = new QTextStream(&file);
    QStringList temp_data = out->readAll().split("\r\n");
    QStringList dataList = temp_data.at(0).split(",");

    map_height = dataList.at(0).toInt();
    map_width = dataList.at(1).toInt();
    for (int i = 1; i < temp_data.count(); i++){
        vector<int> line_map;
        QStringList dataList = temp_data.at(i).split(",");
        for (int j = 0; j < map_width; j++) {
            line_map.push_back(dataList.at(j).toInt());
        }
        mapData.push_back(line_map);
    }
    file.close();
    return true;
}
void MainWindow::LoadGame() {
    MapVec mapData;
    if (ReadMapFile(mapData)) {
        sokoban.LoadMap(mapData, map_height, map_width);
        mode = 1;
        update();
    } else {
        QMessageBox::information(this, "", "Load Game Failed!");
    }
}
void MainWindow::SwitchLevel(int level){
    cur_level = level;
    num_step = 0;
    switch (level) {
        case 1: map_filepath = ":/maps/level1.txt"; break;
        case 2: map_filepath = ":/maps/level2.txt"; break;
        case 3: map_filepath = ":/maps/level3.txt"; break;
        case 4: map_filepath = ":/maps/level4.txt"; break;
        //case 5: map_filepath = ":/maps/level5.txt"; break;
        default: map_filepath = ":/maps/level1.txt"; break;
    }
    LoadGame();
}
void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    //background
    painter.setBrush(QBrush(bg_color));
    painter.drawRect(QRect(origin.rx(), origin.ry(),map_width*block_width, map_height*block_width));

    if (mode==1||mode==2){
        //-1:wall, 0:space, 1:box, 2:user, 3:target, 4:target&user, 5:target&box
        for (int row = 0; row < map_height; row++) {
            for (int col = 0; col < map_width; col++) {
                QRect rect(origin.rx() + col*block_width, origin.ry() + row*block_width, block_width, block_width);
                switch (sokoban.gameMap[row][col]) {
                    case -1: painter.drawImage(rect, img_wall);break;
                    case 0: break;
                    case 1: painter.drawImage(rect, img_box); break;
                    case 2: painter.drawImage(rect, img_user); break;
                    case 3: painter.drawImage(rect, img_target); break;
                    case 4: painter.drawImage(rect, img_user); break;
                    case 5: painter.drawImage(rect, img_tbox); break;
                    case 6: painter.drawImage(rect, img_coin); break;
                }
            }
        }
    }
}
void MainWindow::on_actionStart_triggered() {
    SwitchLevel(1);
}
void MainWindow::on_actionExit_triggered() {
    /*Save Record*/
    this->close();
}
void MainWindow::on_actionReset_triggered() {
    SwitchLevel(cur_level);
}
void MainWindow::on_actionSelect_triggered() {
    map_filepath = QFileDialog::getOpenFileName(this,"Choose your level file.",".","*.txt");
    LoadGame();
}
void MainWindow::on_actionLV1_triggered() {
    SwitchLevel(1);
}
void MainWindow::on_actionLV2_triggered() {
    SwitchLevel(2);
}
void MainWindow::on_actionLV3_triggered() {
    SwitchLevel(3);
}
void MainWindow::on_actionLV4_triggered() {
    SwitchLevel(4);
}
