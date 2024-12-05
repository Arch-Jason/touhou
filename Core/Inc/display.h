#ifndef DISPLAY_H
#define DISPLAY_H

#include "main.h"
#include "stdint.h"
#include "stdbool.h"

extern SPI_HandleTypeDef hspi1;
extern volatile bool spiTransferComplete;
void LCDReset(void);
void LCDWriteCommand(uint16_t command);
void LCDWriteData(uint16_t data);
void LCDInit(void);
void LCDSetWindow(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend);
void LCDSetCursor(uint16_t X, uint16_t Y);
void LCDClear(uint16_t Color);
void LCDClearWindow(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend, uint16_t color);
void LCDDrawPaint(uint16_t x, uint16_t y, uint16_t Color);
void LCDDisplayImage(const uint16_t *image, uint16_t width, uint16_t height);


#endif /* DISPLAY_H */