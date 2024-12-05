#ifndef ENEMYBULLETS_H
#define ENEMYBULLETS_H

#include "stdint.h"
#include "stdbool.h"

extern uint16_t playerPosition[2];
extern bool renderFlag;

struct enemyBullet {
    uint16_t x;
    uint16_t y;
    int16_t velocity_x;
    int16_t velocity_y;
    uint8_t type; // 0-deactivated, 1-随机弹幕, 2-自狙击
};

void initBullets();
void generateBullets();
void updateBullets();
void enemyBullets(void *argument);

#endif