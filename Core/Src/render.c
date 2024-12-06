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
static inline uint16_t constrain(int16_t value, int16_t min, int16_t max) {
    if (value <= min) return min;
    if (value >= max) return max;
    return value;
}

uint16_t color;
void render(void *argument) {
    while(1) {
        if (xSemaphoreTake(renderFlag, (TickType_t)10) == pdTRUE) {
            // HAL_UART_Transmit(&huart2, (uint8_t *)"render start!\r\n", 16, HAL_MAX_DELAY);
            LCDClear(0xFFFF);

            for (uint16_t i = 0; i < 50; i++) {
                // char a;
                // sprintf(a, "%d\n", bullets[i].type);
                // HAL_UART_Transmit(&huart2, (uint8_t *)a, 2, HAL_MAX_DELAY);
                if (bullets[i].type == 1) {
                    color = 0xFF00;
                }
                else if (bullets[i].type == 2) {
                    color = 0x00FF;
                }
                uint16_t x1 = constrain((int16_t)bullets[i].x - 5, 0, 240);
                uint16_t y1 = constrain((int16_t)bullets[i].y - 5, 0, 320);
                uint16_t x2 = constrain((int16_t)bullets[i].x + 5, 0, 240);
                uint16_t y2 = constrain((int16_t)bullets[i].y + 5, 0, 320);
                LCDClearWindow(x1, y1, x2, y2, color);
            }

            uint16_t playerX1 = constrain((int16_t)playerPosition[0] - 5, 0, 240);
            uint16_t playerY1 = constrain((int16_t)playerPosition[1] - 5, 0, 320);
            uint16_t playerX2 = constrain((int16_t)playerPosition[0] + 5, 0, 240);
            uint16_t playerY2 = constrain((int16_t)playerPosition[1] + 5, 0, 320);
            LCDClearWindow(playerX1, playerY1, playerX2, playerY2, 0x0000);

            xSemaphoreGive(renderFlag);        
            osDelay(5);
        }
    }
}
