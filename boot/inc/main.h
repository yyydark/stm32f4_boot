#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "stm32f4xx.h"
#include "i2c.h"
#include "lcd.h"

void board_lowlevel_init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
}


#endif

