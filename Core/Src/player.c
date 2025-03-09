#include <stdint.h>
#include "main.h"
#include "player.h"

void Tone(uint32_t Frequency, uint32_t Duration)
{
    TIM3->ARR = (1000000UL / Frequency) - 1; // Set The PWM Frequency
    TIM3->CCR1 = (TIM3->ARR >> 1); // Set Duty Cycle 50%
    osDelay(Duration); // Wait For The Tone Duration
}

void noTone()
{
    TIM3->CCR1 = 0; // Set Duty Cycle 0%
}
