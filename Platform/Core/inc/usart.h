#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#ifdef __cplusplus
extern "C"
{
#endif
void USART_Init(void);

size_t USART_PutC(char ch);
size_t usart1_write(const uint8_t *buffer, size_t size);
#ifdef __cplusplus
}
#endif