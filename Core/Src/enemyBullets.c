#include "enemyBullets.h"
#include "stdlib.h"
#include "time.h"
#include "main.h"
#include "stdbool.h"

extern struct enemyBullet bullets[500];
extern uint16_t playerPosition[2];

extern UART_HandleTypeDef huart2;
extern bool renderFlag;

void initBullets() {
    srand(12345);
    for (uint16_t i = 0; i < 500; i++) {
        bullets[i].type = 0;
        bullets[i].x = rand() % 240;
        bullets[i].y = 0;
    }
}

void generateBullets() {
    for (uint16_t i = 0; i < 500; i++) {
        if (bullets[i].type == 0) {
            float randNum = rand() / RAND_MAX;
            if (randNum < 0.7) { //随机弹幕
                bullets[i].velocity_x = (rand() % 20) - 10;
                bullets[i].velocity_y = (rand() % 20);
                bullets[i].type = 1;
            } else { //自狙击
                int16_t delta_x = playerPosition[0] - bullets[i].x;
                int16_t delta_y = playerPosition[1] - bullets[i].y;
                float magnitude = sqrt(delta_x * delta_x + delta_y * delta_y);
                bullets[i].velocity_x = (delta_x / magnitude) * 10; // 固定速度为 10
                bullets[i].velocity_y = (delta_y / magnitude) * 10;
                bullets[i].type = 2;
            }
        }
    }
}

void updateBullets() {
    for (uint16_t i = 0; i < 500; i++) {
        if (bullets[i].type != 0) {
            bullets[i].x += bullets[i].velocity_x;
            bullets[i].y += bullets[i].velocity_y;
        }
        if (bullets[i].x < 0 || bullets[i].x >= 240 || bullets[i].y < 0 || bullets[i].y >= 320) {
            bullets[i].type = 0;
            bullets[i].x = rand() % 240;
            bullets[i].y = 0;
        }
    }
}

void enemyBullets(void *argument) {
    renderFlag = false;
    HAL_UART_Transmit(&huart2, (uint8_t*)"1 complete\r\n", 14, HAL_MAX_DELAY);
    initBullets();
    HAL_UART_Transmit(&huart2, (uint8_t*)"2 complete\r\n", 14, HAL_MAX_DELAY);
    while(1) {
        renderFlag = false;
        generateBullets();
        updateBullets();

        renderFlag = true;
        osDelay(20);
    }
}