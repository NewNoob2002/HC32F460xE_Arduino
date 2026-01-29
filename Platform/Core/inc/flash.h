#ifndef FLASH_H
#define FLASH_H
#include "hc32_ll.h"
#include "core_log.h"

#ifdef __cplusplus
extern "C"
{
#endif

int32_t FlashWriteData(uint32_t Address, uint8_t *flashdata, uint32_t len);
int32_t FlashEraseSector(uint32_t u32Addr);
void FlashRead(uint32_t Address, unsigned char *pdata,uint32_t len);

uint16_t FLASH_PageNumber(uint32_t u32Size);
#ifdef __cplusplus
}
#endif

#endif
