#include "render.h"
#include "stdlib.h"
#include "playerPosition.h"
#include "display.h"
#include "stdio.h"
#include "main.h"
#include "FreeRTOS.h"
#include "semphr.h"

extern struct enemyBullet bullets[10];
extern uint16_t playerPosition[2];
extern SemaphoreHandle_t renderFlag;
extern UART_HandleTypeDef huart2;

// 辅助函数：限制值范围
static inline int16_t constrain(int16_t value, int16_t min, int16_t max) {
    if (value <= min) return min;
    if (value >= max) return max;
    return value;
}

// 用于记录上一帧玩家和子弹状态
typedef struct {
    uint16_t x;
    uint16_t y;
    uint8_t  type;
} LastObjectState;

static LastObjectState lastPlayer = {120, 280, 1};
static LastObjectState lastBullets[10];

// 初始化渲染（如第一次全屏清空）
void initRender() {
    LCDClear(0xFFFF);
    // 初始化上一次的子弹状态为无效
    for (int i = 0; i < 10; i++) {
        lastBullets[i].type = 0;
    }
}

void render(void *argument) {
    uint16_t color;
    uint16_t bgColor = 0xFFFF; // 背景色

    while(1) {
        if (xSemaphoreTake(renderFlag, (TickType_t)10) == pdTRUE) {
            // 1. 擦除上一帧玩家位置（用背景色填充小矩形）
            {
                uint16_t x1 = constrain((int16_t)lastPlayer.x - 5, 0, 240);
                uint16_t y1 = constrain((int16_t)lastPlayer.y - 5, 0, 320);
                uint16_t x2 = constrain((int16_t)lastPlayer.x + 5, 0, 240);
                uint16_t y2 = constrain((int16_t)lastPlayer.y + 5, 0, 320);
                LCDClearWindow(x1, y1, x2, y2, bgColor);
            }

            // 擦除上一帧所有子弹位置
            for (uint16_t i = 0; i < 10; i++) {
                if (lastBullets[i].type != 0) {
                    uint16_t x1 = constrain((int16_t)lastBullets[i].x - 5, 0, 240);
                    uint16_t y1 = constrain((int16_t)lastBullets[i].y - 5, 0, 320);
                    uint16_t x2 = constrain((int16_t)lastBullets[i].x + 5, 0, 240);
                    uint16_t y2 = constrain((int16_t)lastBullets[i].y + 5, 0, 320);
                    LCDClearWindow(x1, y1, x2, y2, bgColor);
                }
            }

            // 2. 绘制本帧玩家位置
            {
                uint16_t playerColor = 0x0000; // 黑色
                uint16_t playerX1 = constrain((int16_t)playerPosition[0] - 5, 0, 240);
                uint16_t playerY1 = constrain((int16_t)playerPosition[1] - 5, 0, 320);
                uint16_t playerX2 = constrain((int16_t)playerPosition[0] + 5, 0, 240);
                uint16_t playerY2 = constrain((int16_t)playerPosition[1] + 5, 0, 320);
                LCDClearWindow(playerX1, playerY1, playerX2, playerY2, playerColor);
            }

            // 绘制本帧子弹位置
            for (uint16_t i = 0; i < 10; i++) {
                if (bullets[i].type == 1) {
                    color = 0xFF00; // 绿色
                } else if (bullets[i].type == 2) {
                    color = 0x00FF; // 蓝色
                } else {
                    // 不绘制非激活子弹
                    continue;
                }

                uint16_t x1 = constrain((int16_t)bullets[i].x - 5, 0, 240);
                uint16_t y1 = constrain((int16_t)bullets[i].y - 5, 0, 320);
                uint16_t x2 = constrain((int16_t)bullets[i].x + 5, 0, 240);
                uint16_t y2 = constrain((int16_t)bullets[i].y + 5, 0, 320);
                LCDClearWindow(x1, y1, x2, y2, color);
            }

            // 3. 更新上一帧的对象状态信息
            lastPlayer.x = playerPosition[0];
            lastPlayer.y = playerPosition[1];
            lastPlayer.type = 1; // 玩家无实际type, 可固定为1

            for (uint16_t i = 0; i < 10; i++) {
                lastBullets[i].x = bullets[i].x;
                lastBullets[i].y = bullets[i].y;
                lastBullets[i].type = bullets[i].type;
            }

            xSemaphoreGive(renderFlag);        
            osDelay(50);
        }
    }
}
