#include "collision.h"
#include "frames.h"

extern struct enemyBullet bullets[10];
extern uint16_t playerPosition[2];

float euclidDis(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    return sqrtf(pow(x1-x2, 2) + pow(y1-y2, 2));
}

void collisionDetect(void *argument) {
    while (1) {
        for (uint8_t i = 0; i < 10; i++) {
            if (euclidDis(bullets[i].x, bullets[i].y, playerPosition[0], playerPosition[1]) < 10) {
                Tone(300, 100);
                noTone();
                NVIC_SystemReset();
            }
        }
        osDelay(2);
    }
}