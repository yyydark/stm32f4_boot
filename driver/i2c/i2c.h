#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx.h"


void i2c_init(void);
void i2c_start(void);
void i2c_stop(void);
void i2c_send_byte(uint8_t byte);
void i2c_send_string(char *str);
void i2c_recv_byte(uint8_t *byte);
void i2c_recv_string(char *str);