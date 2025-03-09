#ifndef RENDER_H
#define RENDER_H

#include "display.h"
#include "enemyBullets.h"
#include "FreeRTOS.h"
#include "semphr.h"

extern struct enemyBullet bullets[10];
extern uint16_t playerPosition[2];
extern SemaphoreHandle_t renderFlag;

/**
 * @brief 初始化渲染环境（例如清屏一次）
 */
void initRender();

/**
 * @brief 渲染任务函数
 */
void render(void *argument);

#endif
