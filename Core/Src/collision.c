#include "collision.h"

extern struct enemyBullet bullets[50];
extern uint16_t playerPosition[2];

float euclidDis(x1, y1, x2, y2) {
    return sqrtf(pow(x1-x2, 2) + pow(y1-y2, 2));
}

void collisionDetect(void *argument) {
    while (1) {
        for (uint8_t i = 0; i < 50; i++) {
            if (euclidDis(bullets[i].x, bullets[i].y, playerPosition[0], playerPosition[1]) < 10) {
                LCDClear(0xF000);
                osDelay(10);
                Tone(300, 50);
                noTone();
            }
        }
        osDelay(2);
    }
}