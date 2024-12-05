#include "playerPosition.h"

extern uint16_t playerPosition[2];
extern uint16_t adcBuff[2];
extern UART_HandleTypeDef huart2;
extern bool renderFlag;

void updatePlayerPosition(void *argument) {
    while(1) {
        renderFlag = false;
        int8_t velocityX, velocityY;

        velocityX = (adcBuff[0]-(4095/2))/2000;
        velocityY = (adcBuff[1]-(4095/2))/2000;
        playerPosition[0] += velocityX;
        playerPosition[1] += velocityY;

        if (playerPosition[0] < 0)
            playerPosition[0] = 0;
        if (playerPosition[0] > 240)
            playerPosition[0] = 240;
        if (playerPosition[1] < 0)
            playerPosition[1] = 0;
        if (playerPosition[1] > 320)
            playerPosition[1] = 320;
        
        renderFlag = true;
        osDelay(20);
    }
}