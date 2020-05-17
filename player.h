#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <vector>
#include <cmath>

using std::vector;
#define MapVec vector<vector<int> >

struct PosData {
    int row;
    int col;
    PosData() : row(0), col(0) {}
    PosData(int r, int c) : row(r), col(c) {}
};
struct NextData {
    PosData pos; //Current position
    int direction; //Next Direction

    NextData() {}
    NextData(PosData pos, int dir) : pos(pos), direction(dir) {}
    NextData(int row, int col, int dir) {
        pos = PosData(row, col);
        direction = dir;
    }
};

class player : public QObject
{
    Q_OBJECT

public:
    player();

    bool check_win = false;
    //-1:wall, 0:space, 1:box, 2:user, 3:target, 4:target&user, 5:target&box, 6:coin
    MapVec gameMap;
    //Map
    void LoadMap(MapVec mapData, int height, int width);
    //Push
    void MoveStep(int direction);
    bool updateGameMap(MapVec &mapData, NextData &msg);
    bool MoveUser(MapVec &mapData, NextData &msg);
    bool MoveBox(MapVec &mapData, NextData msg);
    bool CheckEnd(MapVec mapData);
private:
    int map_width;
    int map_height;
    PosData pos_user;
    int num_box; //Check for end game
public slots:
signals:
    void send_AddStep(int num);
    void send_AddTStep(int num);
    void send_AddScore(int num);

};

#endif // PLAYER_H
