#include "stdint.h"
#include "main.h"
#include "stdbool.h"
#include "display.h"

#define LCD_WIDTH 240
#define LCD_HEIGHT 320

#define SPI_MAX_TRANSFER_SIZE 128

extern SPI_HandleTypeDef hspi1;

void LCDReset() {
    HAL_Delay(200);
    HAL_Delay(200);
    HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, 0);
    HAL_Delay(200);
    HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, 1);
    HAL_Delay(200);
}

void LCDWriteCommand(uint16_t command) {
    while (HAL_SPI_GetState(&hspi1) == HAL_SPI_STATE_BUSY_TX) {}
    
    HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, 0);
    HAL_SPI_Transmit_DMA(&hspi1, (uint8_t*)&command, 1);
}

void LCDWriteData(uint16_t data) {
    while (HAL_SPI_GetState(&hspi1) == HAL_SPI_STATE_BUSY_TX) {}
    
    HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, 1);
    HAL_SPI_Transmit_DMA(&hspi1, (uint8_t*)&data, 1);
}

void LCDInit() {
    HAL_GPIO_WritePin(BL_GPIO_Port, BL_Pin, 1);
    LCDReset();

    LCDWriteCommand(0x36);
    LCDWriteData(0x00);

    LCDWriteCommand(0x3A); 
    LCDWriteData(0x05);

    LCDWriteCommand(0x21);

    LCDWriteCommand(0x2A);
    LCDWriteData(0x00);
    LCDWriteData(0x00);
    LCDWriteData(0x01);
    LCDWriteData(0x3F);

    LCDWriteCommand(0x2B);
    LCDWriteData(0x00);
    LCDWriteData(0x00);
    LCDWriteData(0x00);
    LCDWriteData(0xEF);

    LCDWriteCommand(0xB2);
    LCDWriteData(0x0C);
    LCDWriteData(0x0C);
    LCDWriteData(0x00);
    LCDWriteData(0x33);
    LCDWriteData(0x33);

    LCDWriteCommand(0xB7);
    LCDWriteData(0x35); 

    LCDWriteCommand(0xBB);
    LCDWriteData(0x1F);

    LCDWriteCommand(0xC0);
    LCDWriteData(0x2C);

    LCDWriteCommand(0xC2);
    LCDWriteData(0x01);

    LCDWriteCommand(0xC3);
    LCDWriteData(0x12);   

    LCDWriteCommand(0xC4);
    LCDWriteData(0x20);

    LCDWriteCommand(0xC6);
    LCDWriteData(0x0F); 

    LCDWriteCommand(0xD0);
    LCDWriteData(0xA4);
    LCDWriteData(0xA1);

    LCDWriteCommand(0xE0);
    LCDWriteData(0xD0);
    LCDWriteData(0x08);
    LCDWriteData(0x11);
    LCDWriteData(0x08);
    LCDWriteData(0x0C);
    LCDWriteData(0x15);
    LCDWriteData(0x39);
    LCDWriteData(0x33);
    LCDWriteData(0x50);
    LCDWriteData(0x36);
    LCDWriteData(0x13);
    LCDWriteData(0x14);
    LCDWriteData(0x29);
    LCDWriteData(0x2D);

    LCDWriteCommand(0xE1);
    LCDWriteData(0xD0);
    LCDWriteData(0x08);
    LCDWriteData(0x10);
    LCDWriteData(0x08);
    LCDWriteData(0x06);
    LCDWriteData(0x06);
    LCDWriteData(0x39);
    LCDWriteData(0x44);
    LCDWriteData(0x51);
    LCDWriteData(0x0B);
    LCDWriteData(0x16);
    LCDWriteData(0x14);
    LCDWriteData(0x2F);
    LCDWriteData(0x31);
    LCDWriteCommand(0x21);

    LCDWriteCommand(0x11);
    HAL_Delay(120);

    LCDWriteCommand(0x29);
    HAL_Delay(10);
}

void LCDSetWindow(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend) {
    LCDWriteCommand(0x2A);
    LCDWriteData(Xstart >> 8);
    LCDWriteData(Xstart & 0xFF);
    LCDWriteData((Xend - 1) >> 8);
    LCDWriteData((Xend - 1) & 0xFF);

    LCDWriteCommand(0x2B);
    LCDWriteData(Ystart >> 8);
    LCDWriteData(Ystart & 0xFF);
    LCDWriteData((Yend - 1) >> 8);
    LCDWriteData((Yend - 1) & 0xFF);

    LCDWriteCommand(0x2C);
}

void LCDSetCursor(uint16_t X, uint16_t Y) {
    LCDWriteCommand(0x2A);
    LCDWriteData(X >> 8);
    LCDWriteData(X);
    LCDWriteData(X >> 8);
    LCDWriteData(X);

    LCDWriteCommand(0x2B);
    LCDWriteData(Y >> 8);
    LCDWriteData(Y);
    LCDWriteData(Y >> 8);
    LCDWriteData(Y);

    LCDWriteCommand(0x2C);
}

void LCDClear(uint16_t Color) {
    uint8_t colorBuffer[SPI_MAX_TRANSFER_SIZE];

    for (int i = 0; i < SPI_MAX_TRANSFER_SIZE; i += 2) {
        colorBuffer[i] = (Color >> 8) & 0xFF;
        colorBuffer[i + 1] = Color & 0xFF;
    }

    LCDSetWindow(0, 0, LCD_WIDTH, LCD_HEIGHT);
    HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, 1);

    for (uint32_t i = 0; i < (LCD_WIDTH * LCD_HEIGHT * 2 / SPI_MAX_TRANSFER_SIZE); i++) {
        while (HAL_SPI_GetState(&hspi1) == HAL_SPI_STATE_BUSY_TX) {}
        HAL_SPI_Transmit_DMA(&hspi1, colorBuffer, SPI_MAX_TRANSFER_SIZE);
    }
}

void LCDClearWindow(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend, uint16_t color) {          
    uint16_t i, j; 
    LCDSetWindow(Xstart, Ystart, Xend-1, Yend-1);
    uint8_t highByte = (color >> 8) & 0xFF;
    uint8_t lowByte = color & 0xFF;

    uint32_t size = (Xend-Xstart) * (Yend-Ystart);
    uint8_t buffer[size * 2];
    
    for (uint16_t i = 0; i < size; i++) {
        buffer[2 * i] = lowByte;
        buffer[2 * i + 1] = highByte;
    }

    HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, 1);
    while (HAL_SPI_GetState(&hspi1) == HAL_SPI_STATE_BUSY_TX) {}
    HAL_SPI_Transmit_DMA(&hspi1, buffer, size * 2);
}

void LCDDrawPaint(uint16_t x, uint16_t y, uint16_t Color) {
    LCDSetCursor(x, y);
    LCDWriteData(Color);       
}

void LCDDisplayImage(const uint16_t *image, uint16_t width, uint16_t height) {
    uint8_t buffer[SPI_MAX_TRANSFER_SIZE];

    // Setting the window once is sufficient before starting the transfer
    LCDSetWindow(0, 0, width, height);
    HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, 1);

    uint32_t totalPixels = width * height;
    uint32_t totalBytes = totalPixels * 2; // Each pixel is represented by 2 bytes

    for (uint32_t i = 0; i < totalPixels;) {
        uint32_t remainingBytes = (totalBytes - i * 2);
        uint32_t dataSize = remainingBytes > SPI_MAX_TRANSFER_SIZE ? SPI_MAX_TRANSFER_SIZE : remainingBytes;
        
        uint32_t numPixels = dataSize / 2; // Each pixel is 2 bytes
        for (uint32_t j = 0; j < numPixels; j++) {
            buffer[2 * j] = (image[i + j] >> 8) & 0xFF;
            buffer[2 * j + 1] = image[i + j] & 0xFF;
        }

        i += numPixels; // Move the index forward by the number of pixels processed
        while (HAL_SPI_GetState(&hspi1) == HAL_SPI_STATE_BUSY_TX) {}
        HAL_SPI_Transmit_DMA(&hspi1, buffer, dataSize);
    }
}


void LCDDisplayRegion(const uint16_t *image, uint16_t startX, uint16_t startY, uint16_t Xend, uint16_t Yend) {
    uint8_t buffer[SPI_MAX_TRANSFER_SIZE];

    // Setting the window once is sufficient before starting the transfer
    LCDSetWindow(startX, startY, Xend, Yend);
    uint32_t width = Xend - startX;
    uint32_t height = Yend - startY;
    HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, 1);

    uint32_t totalPixels = width * height;
    uint32_t totalBytes = totalPixels * 2; // Each pixel is represented by 2 bytes

    for (uint32_t i = 0; i < totalPixels;) {
        uint32_t remainingBytes = (totalBytes - i * 2);
        uint32_t dataSize = remainingBytes > SPI_MAX_TRANSFER_SIZE ? SPI_MAX_TRANSFER_SIZE : remainingBytes;
        
        uint32_t numPixels = dataSize / 2; // Each pixel is 2 bytes
        for (uint32_t j = 0; j < numPixels; j++) {
            buffer[2 * j] = (image[i + j] >> 8) & 0xFF;
            buffer[2 * j + 1] = image[i + j] & 0xFF;
        }

        i += numPixels; // Move the index forward by the number of pixels processed
        while (HAL_SPI_GetState(&hspi1) == HAL_SPI_STATE_BUSY_TX) {}
        HAL_SPI_Transmit_DMA(&hspi1, buffer, dataSize);
    }
}