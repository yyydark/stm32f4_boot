#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx.h"

void usart_init();
void usart_send(uint8_t data);
uint8_t usart_recv();