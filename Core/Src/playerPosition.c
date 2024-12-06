#include "playerPosition.h"
#include "FreeRTOS.h"
#include "semphr.h"

extern uint16_t playerPosition[2];
extern uint16_t adcBuff[2];
extern UART_HandleTypeDef huart2;
extern SemaphoreHandle_t renderFlag;
extern uint16_t js_x0, js_y0;

void updatePlayerPosition(void *argument) {
    while(1) {
        if (xSemaphoreTake(renderFlag, (TickType_t)10) == pdTRUE) {
            // HAL_UART_Transmit(&huart2, (uint8_t*)"P\r\n", 14, HAL_MAX_DELAY);
            int8_t velocityX, velocityY;

            velocityX = (adcBuff[0]-(js_x0))/500;
            velocityY = (adcBuff[1]-(js_y0))/500;
            int16_t tmp_x, tmp_y;
            tmp_x = playerPosition[0] += velocityX;
            tmp_y = playerPosition[1] += velocityY;

            if (tmp_x <= 0)
                tmp_x = 0;
            if (tmp_x >= 240)
                tmp_x = 240;
            if (tmp_y <= 0)
                tmp_y = 0;
            if (tmp_y >= 320)
                tmp_y = 320;

            playerPosition[0] = tmp_x;
            playerPosition[1] = tmp_y;
            
            xSemaphoreGive(renderFlag);
            osDelay(1);
        }
    }
}