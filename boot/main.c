#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "stm32f4xx.h"
#include "led.h"
#include "delay.h"

static LED_t led1;
static LED_t led2;
int main(void)
{
    LED_Init(&led1, GPIOE, GPIO_Pin_5);
    LED_Init(&led2, GPIOE, GPIO_Pin_6);
    Delay_Init();
    while(1)
    {
        LED_On(&led1);
        LED_On(&led2);
        Delay_Ms(500);
        LED_Off(&led1);
        LED_Off(&led2);
        Delay_Ms(500);
    }
}
