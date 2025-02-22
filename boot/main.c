#include "main.h"

int main(void)
{
    board_lowlevel_init();

    LCD_Init();
    LCD_DisplayOn();
    LCD_Clear(BLACK);
    LCD_ShowString(0, 0, 16, 16, 16, (uint8_t*)"Hello, World!");

    i2c_init();
    i2c_start();
    while(1)    
    {
        ;
    }
    return 0;
}
