// #include "render.h"

// extern struct enemyBullet bullets[500];
// extern uint16_t playerPosition[2];

// void render() {
//     uint16_t frame[240][320]; //长240高320px
//     while (1) {
//         for (uint16_t i; i < 240; i++)
//             for (uint16_t j; j < 320; j++)
//                 frame[i][j] = 0xFFFF; //rgb565格式，背景为白色
//         for (uint16_t i; i < 500; i++) {
//             if (bullets[i].type != 0) {
//                 switch (bullets[i].type) {
//                 case 1:
//                     // 画一个实心圆在frame中，要求颜色为蓝色，大小为长30高30px，若是在边上，则显示1/2个圆或1/4个圆，坐标为bullets[i].x和bullets[i].y
//                     break;
//                 case 2:
//                     // 画一个实心箭头在frame中，要求颜色为蓝色，大小为长30高30px，若是在边上，则显示1/2个圆或1/4个部分，自带旋转效果，旋转方向根据bullets[i].velocity_x和bullets[i].velocity_y进行处理，箭头向下
//                     break;
//                 default:
//                     break;
//                 }
//             }
//         }

//         //画出player，在frame中根据playerPosition[0]为x,playerPosition[1]为y。player是一个向上的箭头，在边缘时则显示1/2个圆或1/4个部分。
//     }
    
// }


#include "render.h"
#include "stdlib.h"
#include "frames.h"
#include "display.h"
#include "stdio.h"
#include "main.h"

extern struct enemyBullet bullets[500];
extern uint16_t playerPosition[2];

extern bool renderFlag;
extern UART_HandleTypeDef huart2;

void drawCircle(uint16_t frame[240][320], uint16_t x, uint16_t y, uint16_t radius, uint16_t color) {
    for (int i = -radius; i <= radius; i++) {
        for (int j = -radius; j <= radius; j++) {
            if (i * i + j * j <= radius * radius) {
                int drawX = x + j;
                int drawY = y + i;
                if (drawX >= 0 && drawX < 320 && drawY >= 0 && drawY < 240) {
                    frame[drawY][drawX] = color;
                }
            }
        }
    }
}

void render(void *argument) {
    uint16_t frame[240][320];
    uint16_t flatFrame[240 * 320];

    while (renderFlag) {
        // 初始化背景为白色
        for (uint16_t i = 0; i < 240; i++) {
            for (uint16_t j = 0; j < 320; j++) {
                frame[i][j] = 0xFFFF; // 白色背景
            }
        }

        // 绘制子弹（蓝色圆）
        for (uint16_t i = 0; i < 500; i++) {
            if (bullets[i].type == 1) {
                drawCircle(frame, bullets[i].x, bullets[i].y, 15, 0x001F); // 半径15像素，蓝色
            }
        }

        // 绘制玩家（红色圆）
        drawCircle(frame, playerPosition[0], playerPosition[1], 15, 0xF800); // 半径15像素，红色

        // 展平 frame 并发送到 LCD 显示
        for (uint16_t i = 0; i < 240; i++) {
            for (uint16_t j = 0; j < 320; j++) {
                flatFrame[i * 320 + j] = frame[i][j];
            }
        }
        
        // 发送图像数据到LCD
        LCDDisplayImage(flatFrame, 240, 320);
    }
}