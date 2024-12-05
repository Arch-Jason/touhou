#include "render.h"
#include "stdlib.h"
#include "display.h"
#include "stdio.h"
#include "main.h"

extern struct enemyBullet bullets[500];
extern uint16_t playerPosition[2];
extern bool renderFlag;
extern UART_HandleTypeDef huart2;

#define REGION_WIDTH 20
#define REGION_HEIGHT 20

// 临时缓冲区，用于小块数据传输
static uint16_t region[REGION_WIDTH * REGION_HEIGHT];

// 绘制实心圆到指定区域
void drawCircleToRegion(uint16_t startX, uint16_t startY, uint16_t x, uint16_t y, uint16_t radius, uint16_t color) {
    for (int i = -radius; i <= radius; i++) {
        for (int j = -radius; j <= radius; j++) {
            if (i * i + j * j <= radius * radius) {
                int drawX = x + j;
                int drawY = y + i;
                if (drawX >= startX && drawX < startX + REGION_WIDTH &&
                    drawY >= startY && drawY < startY + REGION_HEIGHT) {
                    int localX = drawX - startX;
                    int localY = drawY - startY;
                    region[localY * REGION_WIDTH + localX] = color;
                }
            }
        }
    }
}

// 渲染一块区域并显示
void renderRegion(uint16_t startX, uint16_t startY) {
    // 初始化区域缓冲区为白色
    for (uint16_t i = 0; i < REGION_HEIGHT; i++) {
        for (uint16_t j = 0; j < REGION_WIDTH; j++) {
            region[i * REGION_WIDTH + j] = 0xFFFF; // 白色背景
        }
    }

    // 绘制属于当前区域的子弹
    for (uint16_t i = 0; i < 500; i++) {
        if (bullets[i].type == 1) {
            drawCircleToRegion(startX, startY, bullets[i].x, bullets[i].y, 15, 0x001F); // 蓝色
        }
    }

    // 绘制玩家
    drawCircleToRegion(startX, startY, playerPosition[0], playerPosition[1], 15, 0xF800); // 红色

    // 使用 DMA 传输区域数据到 LCD
    LCDDisplayRegion(region, startX, startY, REGION_WIDTH, REGION_HEIGHT);
}

void render(void *argument) {
    // HAL_UART_Transmit(&huart2, (uint8_t*)"Render Thread Started\r\n", 22, HAL_MAX_DELAY);
    while (true) {
        for (uint16_t startY = 0; startY < 240; startY += REGION_HEIGHT) {
            for (uint16_t startX = 0; startX < 320; startX += REGION_WIDTH) {
                // HAL_UART_Transmit(&huart2, (uint8_t*)"Rendering Region\r\n", 18, HAL_MAX_DELAY);
                renderRegion(startX, startY);
            }
        }
        // HAL_UART_Transmit(&huart2, (uint8_t*)"Render Cycle Complete\r\n", 23, HAL_MAX_DELAY);
        osDelay(5);
    }
    // HAL_UART_Transmit(&huart2, (uint8_t*)"Render Thread Exited\r\n", 22, HAL_MAX_DELAY);
}
