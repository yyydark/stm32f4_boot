#ifndef __DHT11_H
#define __DHT11_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


#if defined (STM32F40_41xxx)

#include "stm32f4xx.h"

typedef    GPIO_TypeDef*  DHT11_GPIO_Port_t;

#else

    #error dht11.h: No processor defined!

#endif

typedef struct dht11_t
{
    DHT11_GPIO_Port_t     port;
    uint32_t              pin;
    double                temperature;
    double                humidity;
    bool                  init;
}DHT11_t;


int   DHT11_Init(DHT11_t* dht11, DHT11_GPIO_Port_t port, uint32_t pin);
int   DHT11_ReadData(DHT11_t* dht11);


#endif















