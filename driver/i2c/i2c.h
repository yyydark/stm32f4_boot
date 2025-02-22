#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx.h"


void i2c_init(void);
void i2c_start(void);
void i2c_stop(void);
void i2c_send(uint8_t addr, uint8_t reg, uint8_t* data, uint16_t length);
void i2c_recv(uint8_t addr, uint8_t reg, uint8_t* data, uint16_t length);