#ifndef RENDER_H
#define RENDER_H

#include "display.h"
#include "enemyBullets.h"
#include "FreeRTOS.h"
#include "semphr.h"

extern struct enemyBullet bullets[50];
extern uint16_t playerPosition[2];
extern SemaphoreHandle_t renderFlag;
void render(void *argument);

#endif