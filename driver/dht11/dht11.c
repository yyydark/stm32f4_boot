#include "dht11.h"
#include "delay.h"



#define    DHT11_DelayMs(t)                      Delay_Ms(t)
#define    DHT11_DelayUs(t)                      Delay_Us(t)


#define    DHT11_PIN_HIGH                        1
#define    DHT11_PIN_LOW                         0


#if  defined  (STM32F40_41xxx)

#include "stm32f4xx.h"

#define    __DHT11_CONFIG_IO_INPUT(DHT11)           {  GPIO_InitTypeDef GPIO_InitStructure; \
                                                       GPIO_InitStructure.GPIO_Mode     = GPIO_Mode_IN; \
                                                       GPIO_InitStructure.GPIO_PuPd     = GPIO_PuPd_UP; \
                                                       GPIO_InitStructure.GPIO_Speed    = GPIO_Speed_50MHz; \
                                                       GPIO_InitStructure.GPIO_Pin      = DHT11->pin; \
                                                       GPIO_Init(DHT11->port, &GPIO_InitStructure); \
                                                     }

#define    __DHT11_CONFIG_IO_OUTPUT(DHT11)          {  GPIO_InitTypeDef GPIO_InitStructure; \
                                                       GPIO_InitStructure.GPIO_Mode     = GPIO_Mode_OUT; \
                                                       GPIO_InitStructure.GPIO_OType    = GPIO_OType_PP; \
                                                       GPIO_InitStructure.GPIO_PuPd     = GPIO_PuPd_UP; \
                                                       GPIO_InitStructure.GPIO_Speed    = GPIO_Speed_50MHz; \
                                                       GPIO_InitStructure.GPIO_Pin      = DHT11->pin; \
                                                       GPIO_Init(DHT11->port, &GPIO_InitStructure); \
                                                     }

#define    __DHT11_IO_SET(DHT11, value)             {  if (value == DHT11_PIN_HIGH) \
                                                            GPIO_SetBits(DHT11->port, DHT11->pin); \
                                                        else \
                                                            GPIO_ResetBits(DHT11->port, DHT11->pin); \
                                                     }

#define    DHT11_IO_H(DHT11)                        {__DHT11_IO_SET(DHT11, DHT11_PIN_HIGH)}
#define    DHT11_IO_L(DHT11)                        {__DHT11_IO_SET(DHT11, DHT11_PIN_LOW)}
#define    DHT11_IO_IN(DHT11)                       GPIO_ReadInputDataBit(DHT11->port, DHT11->pin)

#endif


static int DHT11_Rst(DHT11_t *dht11)
{
    __DHT11_CONFIG_IO_OUTPUT(dht11);
    DHT11_IO_L(dht11);
    DHT11_DelayMs(20);
    DHT11_IO_H(dht11);
    DHT11_DelayUs(30);
    __DHT11_CONFIG_IO_INPUT(dht11);

    return 0;
}

static int DHT11_Check(DHT11_t *dht11)
{
    int retry = 0;
    while (DHT11_IO_IN(dht11) && (retry < 100)) {
        retry++;
        DHT11_DelayUs(1);
    };

    if (retry >= 100) {
        return -2;
    } else {
        retry = 0;
    }

    while (!DHT11_IO_IN(dht11) && (retry < 100)) {
        retry++;
        DHT11_DelayUs(1);
    };
    if (retry >= 100) {
        return -3;
    }

    return 0;
}


static uint8_t DHT11_ReadBit(DHT11_t *dht11)
{
    int retry = 0;
    while (DHT11_IO_IN(dht11) && (retry < 100)) {
        retry++;
        DHT11_DelayUs(1);
    }

    retry = 0;
    while (!DHT11_IO_IN(dht11) && (retry < 100)) {
        retry++;
        DHT11_DelayUs(1);
    }

    DHT11_DelayUs(40);
    if (DHT11_IO_IN(dht11)) {
        return 1;
    } else {
        return 0;
    }
}




static uint8_t DHT11_ReadByte(DHT11_t *dht11)
{
    uint8_t i, dat;
    dat = 0;
    for (i = 0; i < 8; i++) {
        dat <<= 1;
        dat |= DHT11_ReadBit(dht11);
    }

    return dat;
}


int DHT11_ReadData(DHT11_t *dht11)
{
    if (!dht11 || !dht11->init)
        return -1;

    uint8_t buf[5];
    uint8_t i;
    DHT11_Rst(dht11);

    if (DHT11_Check(dht11) == 0) {
        for (i = 0; i < 5; i++) {
            buf[i] = DHT11_ReadByte(dht11);
        }
        if ((buf[0] + buf[1] + buf[2] + buf[3]) == buf[4]) {
            dht11->humidity = buf[0];
            dht11->temperature = buf[2];
        }
    } else {
        return -2;
    }

    return 0;
}


int DHT11_Init(DHT11_t *dht11, DHT11_GPIO_Port_t port, uint32_t pin)
{
    if(!dht11)
        return -1;

#if defined (STM32F40_41xxx)

    assert_param(IS_GPIO_ALL_PERIPH(port));

    if (port  == GPIOA)        { RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); }
    else if (port  == GPIOB)   { RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); }
    else if (port  == GPIOC)   { RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); }
    else if (port  == GPIOD)   { RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); }
    else if (port  == GPIOE)   { RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); }
    else if (port  == GPIOF)   { RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE); }
    else if (port  == GPIOG)   { RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE); }
    else return -1;

    __DHT11_CONFIG_IO_OUTPUT(dht11);

#endif

    dht11->port = port;
    dht11->pin = pin;
    dht11->temperature = 0.0;
    dht11->humidity = 0.0;
    dht11->init = true;

    DHT11_IO_H(dht11);
    DHT11_Rst(dht11);

    return DHT11_Check(dht11);
}

