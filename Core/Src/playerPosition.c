#include "playerPosition.h"
#include "FreeRTOS.h"
#include "semphr.h"

extern uint16_t playerPosition[2];
extern uint16_t adcBuff[2];
extern UART_HandleTypeDef huart2;
extern SemaphoreHandle_t renderFlag;

void updatePlayerPosition(void *argument) {
    while(1) {
        if (xSemaphoreTake(renderFlag, (TickType_t)10) == pdTRUE) {
            HAL_UART_Transmit(&huart2, (uint8_t*)"P\r\n", 14, HAL_MAX_DELAY);
            int8_t velocityX, velocityY;

            velocityX = (adcBuff[0]-(4095/2))/500;
            velocityY = (adcBuff[1]-(4095/2))/500;
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
            
            xSemaphoreGive(renderFlag);
            osDelay(1);
        }
    }
}