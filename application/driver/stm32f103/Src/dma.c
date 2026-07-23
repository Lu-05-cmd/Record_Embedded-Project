/*********************************************************************************************************************************************************************
 * @file dma.c
 * @brief DMA driver implementation for STM32F103.
 *
 * @details
 * This file provides DMA initialization, configuration, start/stop/reload control,
 * interrupt flag handling, and status query functions for STM32F103 DMA channels.
 *
 * @author lu-05-cmd
 * @date 2026-05-12
 * @version 0.1.0
 *
 * @note
 * The implementation uses direct register access and assumes the DMA peripheral
 * base structures are defined in the included header.
 ********************************************************************************************************************************************************************/


/*INCLUDE============================================================================================================================================================*/
#include <stdint.h>
#include "../Inc/dma.h"


/*====================================================================================================================================================================
|                       Static Inline Function Definition
=====================================================================================================================================================================*/

/*********************************************************************************************************************************************************************
 * @brief Clear all interrupt status flags for a DMA channel
 * 
 * @param DMAx: Pointer to the DMA controller
 * @param channel: DMA channel whose global, half-transfer, transfer-complete, and error flags are cleared
 * 
 * @note 
 * This helper writes the corresponding bits to the IFCR register. It is used internally during DMA deinitialization.
 */
static inline void DMA_ClearFlag(DMA_TypeDef *DMAx, DMA_Channel_Name channel)
{
    DMAx->IFCR = DMA_IFCR_CGIF(channel)
               | DMA_IFCR_CHTIF(channel) 
               | DMA_IFCR_CTCIF(channel) 
               | DMA_IFCR_CTEIF(channel); 
}

/*********************************************************************************************************************************************************************
 * @brief Build an interrupt-flag clear mask for a DMA channel
 * 
 * @param channel: DMA channel associated with the flag
 * @param flag: Interrupt flag to clear
 * 
 * @return Bit mask suitable for writing to the DMA IFCR register
 */
static inline uint32_t DMA_MakeClearFlagMask(DMA_Channel_Name channel, DMA_IT_FLAG flag)
{
    return (flag << (channel * DMA_IFCR_FLAG_OFFSET));
}



/*====================================================================================================================================================================
|                        DMA Init/DeInit Function Definition
=====================================================================================================================================================================*/
/*********************************************************************************************************************************************************************
 * @brief Initialize one or more DMA channels from a configuration structure
 * 
 * @param DMAx: Pointer to the DMA controller
 * @param config: Pointer to the DMA channel configuration list
 * 
 * @return DMA_STATUS_OK when all channels are configured; DMA_STATUS_INVALID_CONFIG for an invalid configuration
 * 
 * @note 
 * Each channel is disabled before its addresses, transfer length, data size, direction, increment modes, operating mode, and priority are configured.
 */
DMA_Status_t DMA_Init(DMA_TypeDef *DMAx, DMA_Config_t *config)
{
    if(config == NULL || config->channel == NULL || config->num_channels == 0)
    {
        return DMA_STATUS_INVALID_CONFIG;
    }

    for(uint8_t i = 0; i < config->num_channels; i++)
    {
        DMA_ChannelConfig_t *channel_cfg = &config->channel[i];
        DMA_Channel_TypeDef *DMAy_Channelx;

        switch(channel_cfg->channel)
        {
            case DMA_CHANNEL1: DMAy_Channelx = DMA_CH1; break;
            case DMA_CHANNEL2: DMAy_Channelx = DMA_CH2; break;
            case DMA_CHANNEL3: DMAy_Channelx = DMA_CH3; break;
            case DMA_CHANNEL4: DMAy_Channelx = DMA_CH4; break;
            case DMA_CHANNEL5: DMAy_Channelx = DMA_CH5; break;
            case DMA_CHANNEL6: DMAy_Channelx = DMA_CH6; break;
            case DMA_CHANNEL7: DMAy_Channelx = DMA_CH7; break;
            default: return DMA_STATUS_INVALID_CONFIG;
        }

        /* Disable Channel*/
        DMA_Disable(DMAy_Channelx);
        /* Configure Channel with settings from channel_cfg*/
        DMA_ConfigPeripheralAddress(DMAy_Channelx, channel_cfg->peripheral_address);
        DMA_ConfigMemoryAddress(DMAy_Channelx, channel_cfg->memory_address);
        DMA_ConfigTransferLength(DMAy_Channelx, channel_cfg->transfer_length);
        DMA_ConfigDataSize(DMAy_Channelx, channel_cfg->data_size);
        DMA_ConfigDirection(DMAy_Channelx, channel_cfg->direction);
        DMA_ConfigMemoryIncrement(DMAy_Channelx, channel_cfg->mem_increment);
        DMA_ConfigPeripheralIncrement(DMAy_Channelx, channel_cfg->peri_increment);
        DMA_ConfigMode(DMAy_Channelx, channel_cfg->mode);
        DMA_ConfigPriority(DMAy_Channelx, channel_cfg->priority);
    }

    return DMA_STATUS_OK;
}

/*********************************************************************************************************************************************************************
 * @brief Reset a DMA channel and clear its interrupt status flags
 * 
 * @param DMAx: Pointer to the DMA controller
 * @param DMAy_Channelx: Pointer to the DMA channel to reset
 * @param channel: Identifier of the DMA channel
 * 
 * @note 
 * The channel is disabled before its control, transfer count, peripheral address, and memory address registers are cleared.
 */
void DMA_DeInit(DMA_TypeDef *DMAx, DMA_Channel_TypeDef *DMAy_Channelx, DMA_Channel_Name channel)
{
    /* Disable Channel*/
    DMAy_Channelx->CCR &= ~DMA_CCRx_EN_MASK;
    /* Clear Interrupt Flag*/
    DMA_ClearFlag(DMAx, channel);
    /* Clear Register*/
    DMAy_Channelx->CCR = 0;
    DMAy_Channelx->CNDTR = 0;
    DMAy_Channelx->CPAR = 0;
    DMAy_Channelx->CMAR = 0;
}

/*====================================================================================================================================================================
|                        DMA En/Disable Function Definition
=====================================================================================================================================================================*/
/*********************************************************************************************************************************************************************
 * @brief Enable a DMA channel
 * 
 * @param DMAy_Channelx: Pointer to the DMA channel to enable
 */
void DMA_Enable(DMA_Channel_TypeDef *DMAy_Channelx)
{  
    DMAy_Channelx->CCR |= DMA_CCRx_EN_MASK;
}

/*********************************************************************************************************************************************************************
 * @brief Disable a DMA channel
 * 
 * @param DMAyChannelx: Pointer to the DMA channel to disable
 */
void DMA_Disable(DMA_Channel_TypeDef *DMAyChannelx)
{  
    DMAyChannelx->CCR &= ~DMA_CCRx_EN_MASK;
}

/*====================================================================================================================================================================
|                        Configuration DMA Address Function Definition
=====================================================================================================================================================================*/
/*********************************************************************************************************************************************************************
 * @brief Configure the peripheral address for a DMA channel
 * 
 * @param DMAy_Channelx: Pointer to the DMA channel to configure
 * @param peri_addr: Peripheral data-register address
 * 
 * @note 
 * The channel is disabled before the CPAR register is updated.
 */
void DMA_ConfigPeripheralAddress(DMA_Channel_TypeDef *DMAy_Channelx, uint32_t peri_addr)
{
    /* Disable DMA*/
    DMAy_Channelx->CCR &= ~DMA_CCRx_EN_MASK;
    /* Clear old peripheral address*/
    DMAy_Channelx->CPAR &= ~0xFFFFFFFF;
    /* Set DMA peripheral address*/
    DMAy_Channelx->CPAR = peri_addr;
}

/*********************************************************************************************************************************************************************
 * @brief Configure the memory address for a DMA channel
 * 
 * @param DMAy_Channelx: Pointer to the DMA channel to configure
 * @param mem_addr: Memory buffer address
 * 
 * @note 
 * The channel is disabled before the CMAR register is updated.
 */
void DMA_ConfigMemoryAddress(DMA_Channel_TypeDef *DMAy_Channelx, uint32_t mem_addr)
{
    /* Disable DMA*/
    DMAy_Channelx->CCR &= ~DMA_CCRx_EN_MASK;
    /*Clear old Memory address*/
    DMAy_Channelx->CMAR &= ~0xFFFFFFFF;
    /* Set DMA Memmory Address*/
    DMAy_Channelx->CMAR = mem_addr;
}

/*********************************************************************************************************************************************************************
 * @brief Configure the number of DMA data transfers
 * 
 * @param DMAy_Channelx: Pointer to the DMA channel to configure
 * @param length: Number of data items to transfer
 * 
 * @note 
 * The channel is disabled before the CNDTR register is updated.
 */
void DMA_ConfigTransferLength(DMA_Channel_TypeDef *DMAy_Channelx, uint32_t length)
{
    /* Disable DMA*/
    DMAy_Channelx->CCR &= ~DMA_CCRx_EN_MASK;
    /* Clear old length*/
    DMAy_Channelx->CNDTR &= ~0xFFFF;
    /* Set DMA Number of transmit*/
    DMAy_Channelx->CNDTR = length; 
}

/*********************************************************************************************************************************************************************
 * @brief Configure the memory and peripheral data widths for a DMA channel
 * 
 * @param DMAy_Channelx: Pointer to the DMA channel to configure
 * @param data_size: Combined memory and peripheral data-size setting
 * 
 * @note 
 * The channel is disabled and both MSIZE and PSIZE fields are cleared before the new setting is applied.
 */
void DMA_ConfigDataSize(DMA_Channel_TypeDef *DMAy_Channelx, DMA_DataSize_Name data_size)
{
    /* Disable DMA*/
    DMAy_Channelx->CCR &= ~DMA_CCRx_EN_MASK;
    /* Clear old data size*/
    DMAy_Channelx->CCR &= ~DMA_CCRx_MSIZE_MASK;
    DMAy_Channelx->CCR &= ~DMA_CCRx_PSIZE_MASK;
    /* Set DMA Peripheral/Memory size*/
    DMAy_Channelx->CCR |= data_size;
}


/*********************************************************************************************************************************************************************
 * @brief Configure the transfer direction for a DMA channel
 * 
 * @param DMAy_Channelx: Pointer to the DMA channel to configure
 * @param dir: Transfer direction, from peripheral to memory or from memory to peripheral
 * 
 * @note 
 * The channel is disabled before the DIR bit is updated.
 */
void DMA_ConfigDirection(DMA_Channel_TypeDef *DMAy_Channelx, DMA_Direction_Name dir)
{
    /* Disable DMA*/
    DMAy_Channelx->CCR &= ~DMA_CCRx_EN_MASK;
    /* Clear DIR*/
    DMAy_Channelx->CCR &= ~DMA_CCRx_DIR_MASK;
    /* Set DMA data tranfer direction*/
    DMAy_Channelx->CCR |= dir;
}

/*********************************************************************************************************************************************************************
 * @brief Configure memory address increment mode for a DMA channel
 * 
 * @param DMAy_Channelx: Pointer to the DMA channel to configure
 * @param inc: Memory increment enable or disable setting
 */
void DMA_ConfigMemoryIncrement(DMA_Channel_TypeDef *DMAy_Channelx, DMA_MemIncrement_Name inc)
{
    /* Disable DMA*/
    DMAy_Channelx->CCR &= ~DMA_CCRx_EN_MASK;
    /* Clear MINC bit*/
    DMAy_Channelx->CCR &= ~DMA_CCRx_MINC_MASK;
    /* Set DMA Memory increment mode*/
    DMAy_Channelx->CCR |= inc;
}

/*********************************************************************************************************************************************************************
 * @brief Configure peripheral address increment mode for a DMA channel
 * 
 * @param DMAy_Channelx: Pointer to the DMA channel to configure
 * @param inc: Peripheral increment enable or disable setting
 */
void DMA_ConfigPeripheralIncrement(DMA_Channel_TypeDef *DMAy_Channelx, DMA_PeriIncrement_Name inc)
{
    /* Disable DMA*/
    DMAy_Channelx->CCR &= ~DMA_CCRx_EN_MASK;
    /* Clear PINC bit*/
    DMAy_Channelx->CCR &= ~DMA_CCRx_PINC_MASK;
    /* Set DMA Peripheral increment mode*/
    DMAy_Channelx->CCR |= inc;
}

/*********************************************************************************************************************************************************************
 * @brief Configure the operating mode for a DMA channel
 * 
 * @param DMAy_Channelx: Pointer to the DMA channel to configure
 * @param mode: Circular or memory-to-memory mode setting
 * 
 * @note 
 * The channel is disabled and the CIRC and MEM2MEM fields are cleared before the new mode is applied.
 */
void DMA_ConfigMode(DMA_Channel_TypeDef *DMAy_Channelx, DMA_Mode_Name mode)
{
    /* Disable DMA*/
    DMAy_Channelx->CCR &= ~DMA_CCRx_EN_MASK;
    /* Clear Mode*/
    DMAy_Channelx->CCR &= ~DMA_CCRx_MEM2MEM_MASK;
    DMAy_Channelx->CCR &= ~DMA_CCRx_CIRC_MASK;
    /* Set Mode*/
    DMAy_Channelx->CCR |= mode;
}

/*********************************************************************************************************************************************************************
 * @brief Configure the arbitration priority for a DMA channel
 * 
 * @param DMAy_Channelx: Pointer to the DMA channel to configure
 * @param prior: DMA channel priority setting
 */
void DMA_ConfigPriority(DMA_Channel_TypeDef *DMAy_Channelx, DMA_Priority_Name prior)
{
    /* Disable DMA*/
    DMAy_Channelx->CCR &= ~DMA_CCRx_EN_MASK;
    /* Clear old priority*/
    DMAy_Channelx->CCR &= ~DMA_CCRx_PL_MASK;
    /* Set periority for DMAy Channelx*/
    DMAy_Channelx->CCR |= prior;
}
/*====================================================================================================================================================================
|                        Start/Stop/Reload DMA Function Definition
=====================================================================================================================================================================*/
/*********************************************************************************************************************************************************************
 * @brief Start a DMA transfer with a specified transfer count
 * 
 * @param DMAy_Channelx: Pointer to the DMA channel to start
 * @param nums_trans: Number of data items to transfer
 * 
 * @note 
 * The channel is disabled, the transfer count is reloaded, and then the channel is enabled.
 */
void DMA_Start(DMA_Channel_TypeDef *DMAy_Channelx,  uint32_t nums_trans)
{
    /* Disable for last Config*/
    DMA_Disable(DMAy_Channelx);
    /* Set Number of transmit*/
    DMAy_Channelx->CNDTR = nums_trans;
    /* Enable -> Start*/
    DMA_Enable(DMAy_Channelx);
}

/*********************************************************************************************************************************************************************
 * @brief Stop a DMA transfer
 * 
 * @param DMAy_Channelx: Pointer to the DMA channel to stop
 * 
 * @note 
 * This function only disables the channel; it does not clear pending status flags.
 */
void DMA_Stop(DMA_Channel_TypeDef *DMAy_Channelx)
{
    /* Stop DMA, status flag is not clear*/
    DMA_Disable(DMAy_Channelx);
}

/*********************************************************************************************************************************************************************
 * @brief Reload the memory address and transfer length of a DMA channel
 * 
 * @param DMAy_Channelx: Pointer to the DMA channel to reload
 * @param mem_addr: New memory buffer address
 * @param len: New number of data items to transfer
 * 
 * @note 
 * The channel is disabled before the memory address and transfer count are updated. This function does not re-enable it.
 */
void DMA_Reload(DMA_Channel_TypeDef *DMAy_Channelx, uint32_t mem_addr, uint16_t len)
{
    DMAy_Channelx->CCR &= ~DMA_CCRx_EN_MASK;
    /* Before Relaod, clear old status*/
    DMAy_Channelx->CMAR &= ~0xFFFFFFFF;
    DMAy_Channelx->CNDTR &= ~0xFFFF;
    /* Relaod DMA memory address data*/
    DMAy_Channelx->CMAR = mem_addr;
    /* Relaod DMA number of data*/
    DMAy_Channelx->CNDTR = len;
}

/*====================================================================================================================================================================
|                        Interrupt/Flag DMA Function Definition
=====================================================================================================================================================================*/
/*********************************************************************************************************************************************************************
 * @brief Enable one or more DMA channel interrupt sources
 * 
 * @param DMAy_Channelx: Pointer to the DMA channel to configure
 * @param it: Transfer-complete, half-transfer, or transfer-error interrupt mask
 * 
 * @note 
 * The DMA channel is disabled before its interrupt-enable bits are modified.
 */
void DMA_EnableInterrupt(DMA_Channel_TypeDef *DMAy_Channelx, DMA_Interrupt_Name it)
{
    /* Disable DMA*/
    DMAy_Channelx->CCR &= ~DMA_CCRx_EN_MASK;
    /* Set interrupt for DMA*/
    DMAy_Channelx->CCR |= it;
}

/*********************************************************************************************************************************************************************
 * @brief Disable one or more DMA channel interrupt sources
 * 
 * @param DMAy_Channelx: Pointer to the DMA channel to configure
 * @param it: Transfer-complete, half-transfer, or transfer-error interrupt mask
 */
void DMA_DisableInterrupt(DMA_Channel_TypeDef *DMAy_Channelx, DMA_Interrupt_Name it)
{
    /* Before turn off interrupt, disable DMA*/
    DMAy_Channelx->CCR &= ~DMA_CCRx_EN_MASK;
    /* Set DMA interrupt*/
    DMAy_Channelx->CCR &= ~(it);
}

/*********************************************************************************************************************************************************************
 * @brief Get the state of a raw DMA interrupt status flag
 * 
 * @param DMAx: Pointer to the DMA controller
 * @param flag: Bit mask of the flag to read from the ISR register
 * 
 * @return 1U if the flag is set; otherwise 0U
 */
uint32_t DMA_GetFlagStatus(DMA_TypeDef *DMAx, uint32_t flag)
{
    return (DMAx->ISR & flag) ? 1U : 0U;
}

/*********************************************************************************************************************************************************************
 * @brief Clear one or more raw DMA interrupt status flags
 * 
 * @param DMAx: Pointer to the DMA controller
 * @param DMAy_Channelx: Pointer to the related DMA channel; retained for API compatibility
 * @param flag: Bit mask to write to the IFCR register
 */
void DMA_InterruptFlagClear(DMA_TypeDef *DMAx, DMA_Channel_TypeDef *DMAy_Channelx, uint32_t flag)
{
    /* Clear Interrupt Flag*/
    DMAx->IFCR = flag;
}

/*********************************************************************************************************************************************************************
 * @brief Get the state of a DMA channel interrupt flag
 * 
 * @param DMAx: Pointer to the DMA controller
 * @param channel: DMA channel associated with the flag
 * @param flag: Channel-relative interrupt flag to read
 * 
 * @return 1U if the selected flag is set; otherwise 0U
 */
uint32_t DMA_GetInterruptStatusFlag(DMA_TypeDef *DMAx, DMA_Channel_Name channel, DMA_IT_FLAG flag)
{
    return (DMAx->ISR & (flag << (channel * DMA_ISR_FLAG_OFFSET))) ? 1U : 0U;
}

/*********************************************************************************************************************************************************************
 * @brief Clear a pending interrupt flag for a DMA channel
 * 
 * @param DMAx: Pointer to the DMA controller
 * @param channel: DMA channel associated with the flag
 * @param flag: Channel-relative interrupt flag to clear
 */
void DMA_ClearITPendingBit(DMA_TypeDef *DMAx, DMA_Channel_Name channel, DMA_IT_FLAG flag)
{
    DMAx->IFCR = DMA_MakeClearFlagMask(channel, flag);
}

/*====================================================================================================================================================================
|                        Interrupt/Flag DMA Function Definition
=====================================================================================================================================================================*/
/*********************************************************************************************************************************************************************
 * @brief Get the remaining number of DMA data transfers
 * 
 * @param DMAy_Channelx: Pointer to the DMA channel to query
 * 
 * @return Current value of the CNDTR register
 */
uint32_t DMA_GetRemainingData(DMA_Channel_TypeDef *DMAy_Channelx)
{
    return (uint32_t)(DMAy_Channelx->CNDTR);
}

/*********************************************************************************************************************************************************************
 * @brief Check whether a DMA channel is enabled
 * 
 * @param DMAy_Channelx: Pointer to the DMA channel to query
 * 
 * @return 1U if the channel enable bit is set; otherwise 0U
 */
uint32_t DMA_IsEnable(DMA_Channel_TypeDef *DMAy_Channelx)
{
    return (DMAy_Channelx->CCR & DMA_CCRx_EN_MASK) ? 1U : 0U; 
}

/* End of refactored DMA */
