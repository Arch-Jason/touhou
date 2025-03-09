#ifndef PLAYER_POSITION_H
#define PLAYER_POSITION_H

#include "main.h"
#include "stdint.h"
#include "stdbool.h"
#include "FreeRTOS.h"
#include "semphr.h"

extern uint16_t playerPosition[2];
extern uint16_t adcBuff[2];
extern SemaphoreHandle_t renderFlag;
extern uint16_t js_x0, js_y0;

void updatePlayerPosition(void *argument);

#endif