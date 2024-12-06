// enemyBullets.h
#ifndef ENEMYBULLETS_H
#define ENEMYBULLETS_H

#include <stdint.h>
#include <stdbool.h>

struct enemyBullet {
    uint16_t x;
    uint16_t y;
    int16_t velocity_x;
    int16_t velocity_y;
    uint8_t type; // 0: inactive, 1: random, 2: homing
    uint16_t previous_x; // 上一步的 X 坐标
    uint16_t previous_y; // 上一步的 Y 坐标
};

void initBullets();
void generateBullets();
void updateBullets();
void enemyBullets(void *argument);

#endif // ENEMYBULLETS_H
