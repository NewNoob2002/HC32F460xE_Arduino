#include "flash.h"

int32_t FlashWriteData(uint32_t u32Addr, uint8_t *pu8Buff, uint32_t u32Len)
{
    int32_t result = LL_OK;
	  if ((pu8Buff == NULL) || (u32Len == 0U) || ((u32Addr + u32Len) > (EFM_BASE + EFM_END_ADDR))) {
        return LL_ERR_INVD_PARAM;
    }
		if (0UL != (u32Addr % 4U)) {
        return LL_ERR_ADDR_ALIGN;
    }

    while (SET != EFM_GetStatus(EFM_FLAG_RDY))
    {
        LOG_INFO("Wait Flash Ready");
    }
    /* EFM_FWMC write enable */
    EFM_FWMC_Cmd(ENABLE);
    /* Hold bus while erase & program */
    EFM_SetBusStatus(EFM_BUS_HOLD);

    result = EFM_Program(u32Addr, pu8Buff, u32Len);

    /* Release bus after program */
    EFM_SetBusStatus(EFM_BUS_RELEASE);

    return result;
}
int32_t FlashEraseSector(uint32_t u32Addr)
{
    int32_t result = LL_OK;
    /* Wait flash ready. */
    while (SET != EFM_GetStatus(EFM_FLAG_RDY))
    {
        LOG_INFO("Wait Flash Ready");
    }
    /* EFM_FWMC write enable */
    EFM_FWMC_Cmd(ENABLE);
    /* Hold bus while erase & program */
    EFM_SetBusStatus(EFM_BUS_HOLD);

    result = EFM_SectorErase(u32Addr);

    /* Release bus after erase */
    EFM_SetBusStatus(EFM_BUS_RELEASE);

    return result;
}
void FlashRead(uint32_t Address, unsigned char *pdata, uint32_t len)
{
    int i;
    unsigned char *pageaddr;
    pageaddr = (unsigned char *)(Address);
    for (i = 0; i < len; i++)
    {
        pdata[i] = pageaddr[i];
    }
}

uint16_t FLASH_PageNumber(uint32_t u32Addr)
{
    uint16_t u32PageNum = u32Addr / EFM_SECTOR_SIZE;

    if ((u32Addr % EFM_SECTOR_SIZE) != 0)
    {
        u32PageNum += 1u;
    }

    return u32PageNum;
}