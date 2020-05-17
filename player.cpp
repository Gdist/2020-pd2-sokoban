#include "player.h"
#include "mainwindow.h"
player::player(){}

void player::LoadMap(MapVec mapData, int height, int width) {
    map_height = height;
    map_width = width;
    check_win = false;
    num_box = 0;

    gameMap = std::move(mapData);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (gameMap[i][j] == 2) {
                pos_user = PosData(i, j);
            }
            if (gameMap[i][j] == 1) {
                num_box++; //check for end game
            }
        }
    }
}
// direction: 1:up 2:down 3:left 4:right
void player::MoveStep(int direction) {
    if (check_win) return;
    NextData NextData(pos_user, direction);
    if(updateGameMap(gameMap, NextData)){
        emit send_AddStep(1);
        emit send_AddTStep(1);
        emit send_AddScore(-1);
    }
    pos_user = NextData.pos; //Update user position
    check_win = CheckEnd(gameMap);
}

bool player::updateGameMap(MapVec &mapData, NextData &next_data) {
    int cur_row = next_data.pos.row, cur_col = next_data.pos.col;
    int pre_row, pre_col,pre_element;
    //Clean & Save previous user position. (Not upate new->Do in MoveMan()&MoveBox())
    for (int row = 0; row < map_height; row++) {
        for (int col = 0; col < map_width; col++) {
            if (mapData[row][col] == 2) { //find user
                mapData[row][col] = 0; //clean
                pre_row = row; pre_col = col; pre_element=2; //save
            } else if (mapData[row][col] == 4) { //find user&target
                mapData[row][col] = 3; //reset to target
                pre_row = row; pre_col = col; pre_element=4;
            }
        }
    }
    // Give new position.
    int check_next = mapData[cur_row][cur_col];
    if (check_next == -1 || check_next == 1 || check_next == 5) {
        // Can't move & Return previous position
        if (mapData[pre_row][pre_col] == 3) mapData[pre_row][pre_col] = 4;
        else mapData[pre_row][pre_col] = 2;
        return false;
    }
    else if (check_next == 0) mapData[cur_row][cur_col] = 2;
    else if (check_next == 3) mapData[cur_row][cur_col] = 4;
    /*else if (check_next == 6){
        mapData[cur_row][cur_col] = 2;
    }*/

    //Get effect of move & Update if can move
    if (MoveUser(mapData, next_data)) return true;
    else {
        // Can't move & Return previous position
        if (mapData[cur_row][cur_col] == 3) mapData[cur_row][cur_col] = 4;
        else mapData[cur_row][cur_col] = 2;
        mapData[pre_row][pre_col]=pre_element;
        return false;
    }
}

bool player::MoveUser(MapVec &mapData, NextData &next_data) {
    int next_row = next_data.pos.row, next_col = next_data.pos.col;
    int cur_row = next_row, cur_col = next_col, cur_element;

    switch (next_data.direction) {
        case 1: next_row--; break;
        case 2: next_row++; break;
        case 3: next_col--; break;
        case 4: next_col++; break;
    }

    if (mapData[cur_row][cur_col] == 4) cur_element=3;
    else cur_element=0;
    switch (mapData[next_row][next_col]) {
        case 0: //space or coin
            mapData[cur_row][cur_col] = cur_element;
            mapData[next_row][next_col] = 2;
            next_data.pos = PosData(next_row, next_col); //Save as current position
            return true;
        case 1: //box
            if (MoveBox(mapData, NextData(PosData(next_row, next_col), next_data.direction))) {
                mapData[cur_row][cur_col] = cur_element;
                mapData[next_row][next_col] = 2;
                next_data.pos = PosData(next_row, next_col);
                return true;
            }
            else return false;
        case 3: //target
            mapData[cur_row][cur_col] = cur_element;
            mapData[next_row][next_col] = 4; //target&user
            next_data.pos = PosData(next_row, next_col);
            return true;
        case 5: //target & box (tbox)
            if (MoveBox(mapData, NextData(PosData(next_row, next_col), next_data.direction))) {
                mapData[cur_row][cur_col] = cur_element;
                mapData[next_row][next_col] = 4;
                next_data.pos = PosData(next_row, next_col);
                return true;
            }
            else return false;
        case 6: //coin
            mapData[cur_row][cur_col] = cur_element;
            mapData[next_row][next_col] = 2;
            next_data.pos = PosData(next_row, next_col); //Save as current position
            emit send_AddScore(10);
            return true;
        case 7: //portal
            emit send_AddScore(10);
            go_next_level = true;
            return true;
        case -1: default: //wall or not define
            return false;
        }
}

bool player::MoveBox(MapVec &mapData, NextData next_data) {
    int next_row = next_data.pos.row, next_col = next_data.pos.col;
    switch (next_data.direction) {
        case 1: next_row--; break;
        case 2: next_row++; break;
        case 3: next_col--; break;
        case 4: next_col++; break;
    }
    //Move if next element is space or target
    switch (mapData[next_row][next_col]) {
        case 0: mapData[next_row][next_col] = 1; return true;
        case 3: mapData[next_row][next_col] = 5; emit send_AddScore(5); return true;
        default: return false;
    }
}

bool player::CheckEnd(MapVec mapData) {
    int tbox_num = 0;
    for (int row = 0; row < map_height; row++) {
        for (int col = 0; col < map_width; col++) {
            if (mapData[row][col] == 1) return false; //Exist box not on target
            else if (mapData[row][col] == 5) tbox_num++;
        }
    }
    return tbox_num == num_box;
}
