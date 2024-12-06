#include "render.h"
#include "stdlib.h"
#include "frames.h"
#include "display.h"
#include "stdio.h"
#include "main.h"
#include "FreeRTOS.h"
#include "semphr.h"

extern struct enemyBullet bullets[50];
extern uint16_t playerPosition[2];
extern SemaphoreHandle_t renderFlag;

extern UART_HandleTypeDef huart2;

// Helper function to constrain values within a range
static inline uint16_t constrain(uint16_t value, uint16_t min, uint16_t max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

void render(void *argument) {
    while(1) {
        if (xSemaphoreTake(renderFlag, (TickType_t)10) == pdTRUE) {
            HAL_UART_Transmit(&huart2, (uint8_t *)"render start!\r\n", 16, HAL_MAX_DELAY);
            LCDClear(0xFFFF);

            for (uint16_t i = 0; i < 50; i++) {
                if (bullets[i].type == 1) {
                    uint16_t x1 = constrain(bullets[i].x - 5, 0, 240);
                    uint16_t y1 = constrain(bullets[i].y - 5, 0, 320);
                    uint16_t x2 = constrain(bullets[i].x + 5, 0, 240);
                    uint16_t y2 = constrain(bullets[i].y + 5, 0, 320);
                    LCDClearWindow(x1, y1, x2, y2, 0xFF00);
                }
                if (bullets[i].type == 2) {
                    uint16_t x1 = constrain(bullets[i].x - 5, 0, 240);
                    uint16_t y1 = constrain(bullets[i].y - 5, 0, 320);
                    uint16_t x2 = constrain(bullets[i].x + 5, 0, 240);
                    uint16_t y2 = constrain(bullets[i].y + 5, 0, 320);
                    LCDClearWindow(x1, y1, x2, y2, 0x00FF);
                }
            }

            uint16_t playerX1 = constrain(playerPosition[0] - 5, 0, 240);
            uint16_t playerY1 = constrain(playerPosition[1] - 5, 0, 320);
            uint16_t playerX2 = constrain(playerPosition[0] + 5, 0, 240);
            uint16_t playerY2 = constrain(playerPosition[1] + 5, 0, 320);
            LCDClearWindow(playerX1, playerY1, playerX2, playerY2, 0x0000);

            xSemaphoreGive(renderFlag);        
            osDelay(5);
        }
    }
}
