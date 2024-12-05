#ifndef RENDER_H
#define RENDER_H

#include "display.h"
#include "enemyBullets.h"

extern struct enemyBullet bullets[500];
extern uint16_t playerPosition[2];
extern bool renderFlag;
void render(void *argument);

#endif