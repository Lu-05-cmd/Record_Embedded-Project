/**************************************************************************************************
 * @file    spi.c
 * @brief   SPI driver implementation for STM32F103xx
 *
 * @details
 * This file implements polling, interrupt, and DMA-related SPI operations for
 * STM32F103xx devices using direct register access.
 *
 * @author  lu-05-cmd
 * @date    2026-05-02
 * @version 0.1.0
 *
 * @note
 *   - Bare-metal register-level implementation.
 *   - Supports transmit/receive, NSS configuration, CRC handling, and flag management.
 **************************************************************************************************/

/*INCLUDE============================================================================================================================================================*/
#include <stdint.h>
#include <stdio.h>
#include "../Inc/spi.h"
#include "../Inc/rcc.h"
#include "../Inc/dma.h"


/**
 * @brief   Configure SPI clock phase and polarity
 * @param   SPIx: Pointer to SPI peripheral
 * @param   cfg: Pointer to SPI clock configuration structure
 * @note    This helper writes the CPHA/CPOL bits in CR1.
 */
static inline void SPI_SetClock(SPI_TypeDef *SPIx, SPI_Clock_Config_t *cfg)
{
    SPIx->CR1 &= ~SPI_CR1_CPOL_MASK;
    SPIx->CR1 &= ~SPI_CR1_CPHA_MASK;
    /* Set clock*/
    if(cfg->clk_phase == SPI_CPHA_FIRST_CLOCK_TRANSITION)   SPIx->CR1 |= SPI_CPHA_FIRST_CLOCK_TRANSITION;
    else if(cfg->clk_phase == SPI_CPHA_SECOND_CLOCK_TRANSITION) SPIx->CR1 |= SPI_CPHA_SECOND_CLOCK_TRANSITION;
    else if(cfg->clk_polarity == SPI_CPOL_CK_TO_0)  SPIx->CR1 |= SPI_CPOL_CK_TO_0;
    else if(cfg->clk_polarity == SPI_CPOL_CK_TO_1)  SPIx->CR1 |= SPI_CPOL_CK_TO_1;
}

/**
 * @brief   Configure SPI baud rate prescaler
 * @param   SPIx: Pointer to SPI peripheral
 * @param   br: Baud rate selection value
 * @note    The SPI peripheral is temporarily disabled while the prescaler is changed.
 */
static inline void SPI_SetBaudrate(SPI_TypeDef *SPIx, SPI_BaudRate_t br)
{
    while((SPIx->SR & SPI_SR_BSY_MASK));
    SPIx->CR1 &= ~SPI_CR1_SPE_MASK;
    SPIx->CR1 &= ~SPI_CR1_BR_MASK;
    SPIx->CR1 |= (br << SPI_CR1_BR_POS);
    SPIx->CR1 |= SPI_CR1_SPE_MASK;
}

/**
 * @brief   Configure SPI frame transmission order
 * @param   SPIx: Pointer to SPI peripheral
 * @param   format: Frame format selection (MSB or LSB first)
 */
static inline void SPI_SetFrameFormat(SPI_TypeDef *SPIx, SPI_FrameFormat_t format)
{
    if(format == SPI_MSB_FIRST)
    {
        SPIx->CR1 &= ~SPI_CR1_LSBFIRST_MASK;
    }
    else
    {
        SPIx->CR1 |= format;
    }
}

/**
 * @brief   Configure SPI communication direction mode
 * @param   SPIx: Pointer to SPI peripheral
 * @param   dir: Direction mode selection
 */
static inline void SPI_SetDirection(SPI_TypeDef *SPIx, SPI_DirectionMode_t dir)
{
    SPIx->CR1 &= ~(SPI_CR1_BIDIMODE_MASK |
                   SPI_CR1_BIDIOE_MASK |
                   SPI_CR1_RXONLY_MASK);

    SPIx->CR1 |= dir;
}

/**
 * @brief   Enable or disable SPI CRC hardware calculation
 * @param   SPIx: Pointer to SPI peripheral
 * @param   crc: CRC enable/disable setting
 */
static inline void SPI_SetCRC(SPI_TypeDef *SPIx, SPI_CRC_Caculator_t crc)
{
    SPIx->CR1 &= ~SPI_CR1_CRCEN_MASK;
    SPIx->CR1 |= crc;
}

/**
 * @brief   Configure NSS management mode
 * @param   SPIx: Pointer to SPI peripheral
 * @param   nss: NSS management mode selection
 */
static inline void SPI_SetNSSMode(SPI_TypeDef *SPIx, SPI_NSSMode_t nss)
{
    if(nss == SPI_NSS_SOFTWARE_MANAGEMENT)
    {
        SPIx->CR1 |= SPI_CR1_SSM_MASK;
    }
    else
    {
        SPIx->CR1 &= ~SPI_CR1_SSM_MASK;
    }
}

/**
 * @brief   Configure NSS output pin state
 * @param   SPIx: Pointer to SPI peripheral
 * @param   state: Output state (enable/disable)
 */
static inline void SPI_SetNSSOut(SPI_TypeDef *SPIx, SPI_State_t state)
{
    SPIx->CR2 &= ~SPI_CR2_SSOE_MASK;
    SPIx->CR2 |= (state << SPI_CR2_SSOE_POS);
}



/*====================================================================================================================================================================
                    SPI Initialization, De-initialization and Configuration Functions
=====================================================================================================================================================================*/
SPI_Status_t SPI_Init(SPI_TypeDef *SPIx, SPI_Config_t *config)
{
    if((SPIx == NULL) || (config == NULL))
    {
        return SPI_INVALID_PARAM;
    }

    if(SPIx == SPI1)
    {
        RCC_EnableSPIClock(RCC_SPI1);
    }
    else if(SPIx == SPI2)
    {
        RCC_EnableSPIClock(RCC_SPI2);
    }
    else
    {
        return SPI_INVALID_PARAM;
    }

    /* Disable SPI before changing configuration */
    SPI_Disable(SPIx, 1000);

    /* Reset CR1/CR2 to a known state */
    SPIx->CR1 = 0U;
    SPIx->CR2 = 0U;

    /* Configure clock phase/polarity */
    SPIx->CR1 &= ~(SPI_CR1_CPHA_MASK | SPI_CR1_CPOL_MASK);
    SPIx->CR1 |= config->cr1_config.clock_config.clk_phase;
    SPIx->CR1 |= config->cr1_config.clock_config.clk_polarity;

    /* Configure baud rate, frame format, direction, CRC */
    SPI_SetBaudrate(SPIx, config->cr1_config.baudrate);
    SPI_SetFrameFormat(SPIx, config->cr1_config.format);
    SPI_SetDirection(SPIx, config->cr1_config.direction);
    SPI_SetCRC(SPIx, config->cr1_config.crc_calculation);
    SPI_SetNSSMode(SPIx, config->cr1_config.nss_mode);

    /* Configure master/slave mode and frame size */
    SPIx->CR1 &= ~SPI_CR1_MSTR_MASK;
    SPIx->CR1 |= config->cr1_config.device_mode;

    SPIx->CR1 &= ~SPI_CR1_DFF_MASK;
    SPIx->CR1 |= config->cr1_config.datasize;

    /* Configure NSS internal state when software mode is selected */
    if(config->cr1_config.nss_mode == SPI_NSS_SOFTWARE_MANAGEMENT)
    {
        SPIx->CR1 &= ~SPI_CR1_SSI_MASK;
        SPIx->CR1 |= config->cr1_config.ssi;
    }
    else
    {
        SPIx->CR1 &= ~SPI_CR1_SSI_MASK;
    }

    /* Configure CR2: DMA, NSS output and interrupts */
    SPIx->CR2 &= ~(SPI_CR2_RXDMAEN_MASK | SPI_CR2_TXDMAEN_MASK |
                   SPI_CR2_SSOE_MASK | SPI_CR2_ERRIE_MASK |
                   SPI_CR2_RXNEIE_MASK | SPI_CR2_TXEIE_MASK);

    SPIx->CR2 |= config->cr2_config.tx_dma_enable;
    SPIx->CR2 |= config->cr2_config.rx_dma_enable;
    SPIx->CR2 |= config->cr2_config.nss_output;
    SPIx->CR2 |= config->cr2_config.itr_config.error_interrupt;
    SPIx->CR2 |= config->cr2_config.itr_config.rxne_interrupt;
    SPIx->CR2 |= config->cr2_config.itr_config.txe_interrupt;

    /* Leave the peripheral enabled for immediate use */
    SPI_Enable(SPIx);

    return SPI_OK;
}

/**
 * @brief   De-initialize the specified SPI peripheral
 * @param   SPIx: Pointer to SPI peripheral
 * @retval  SPI_OK on success
 * @note    This function resets the SPI peripheral using the RCC reset helper.
 */
SPI_Status_t SPI_DeInit(SPI_TypeDef *SPIx)
{
    if(SPIx == SPI1)
    {
        RCC_ResetSPI(RCC_SPI1);
    }
    else if(SPIx == SPI2)
    {
        RCC_ResetSPI(RCC_SPI2);
    }
    return SPI_OK;
}

/**
 * @brief   Enable the specified SPI peripheral
 * @param   SPIx: Pointer to SPI peripheral
 */
void SPI_Enable(SPI_TypeDef *SPIx)
{
    SPIx->CR1 |= SPI_CR1_SPE_MASK;
}

/**
 * @brief   Disable the specified SPI peripheral with timeout
 * @param   SPIx: Pointer to SPI peripheral
 * @param   timeout: Maximum busy-wait time
 * @retval  SPI_OK on success
 */
SPI_Status_t SPI_Disable(SPI_TypeDef *SPIx, uint32_t timeout)
{
    while((SPIx->SR & SPI_SR_BSY_MASK) && timeout--);
    if(SPIx->SR & SPI_SR_BSY_MASK)
    {
        return SPI_TIMEOUT;
    }
    SPIx->CR1 &= ~SPI_CR1_SPE_MASK;
    
    return SPI_OK;
}

/**
 * @brief   Transmit data using blocking polling mode
 * @param   SPIx: Pointer to SPI peripheral
 * @param   tx_buffer: Pointer to transmit buffer
 * @param   len: Number of data frames to transmit
 * @retval  SPI_OK on success
 */
SPI_Status_t SPI_Transmit_Polling(SPI_TypeDef *SPIx, const uint16_t *tx_buffer, uint32_t len)
{
    uint32_t tmp;
    /* Enable if SPI Disable*/
    if((SPIx->CR1 & SPI_CR1_SPE_MASK) == 0) SPI_Enable(SPIx);

    for(uint32_t inc = 0; inc < len; inc++)
    {
        while(!(SPIx->SR & SPI_SR_TXE_MASK));
        SPIx->DR = *tx_buffer;
        tx_buffer++;
        while(!(SPIx->SR & SPI_SR_RXNE_MASK));
        tmp = SPIx->DR;
        (void)tmp;
    }
    return SPI_OK;
}

/**
 * @brief   Receive data using blocking polling mode
 * @param   SPIx: Pointer to SPI peripheral
 * @param   rx_buffer: Pointer to receive buffer
 * @param   len: Number of data frames to receive
 * @retval  SPI_OK on success
 */
SPI_Status_t SPI_Receive_Polling(SPI_TypeDef *SPIx, uint16_t *rx_buffer, uint32_t len)
{
    uint32_t tmp = 0;

    /* Enable if SPI disable*/
    if((SPIx->CR1 & SPI_CR1_SPE_MASK) == 0) SPI_Enable(SPIx);
    
    for(uint32_t inc = 0; inc < len; inc++){
        
        /* Wait Tx buffer empty*/
        while((SPIx->SR & SPI_SR_TXE_MASK) == 0);
        /* Write DR, ignore transmit*/
        if((SPIx->CR1 & SPI_CR1_DFF_MASK) == 0){
            SPIx->DR = 0xFF;
        }else{
            SPIx->DR = 0xFFFF;
        }
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
        /* Process overrun*/
        if(SPIx->SR & SPI_SR_OVR_MASK)
        {
            tmp = SPIx->DR;
            tmp = SPIx->SR;
            (void)tmp;
        }
        /* Wait receive buffer not empty*/
        while(!(SPIx->SR & SPI_SR_RXNE_MASK));
        /* Write receive data to buffer*/
        *rx_buffer = SPIx->DR;
        rx_buffer++;
    }
    return SPI_OK;
}

/**
 * @brief   Transmit and receive data using blocking polling mode
 * @param   SPIx: Pointer to SPI peripheral
 * @param   tx_buffer: Pointer to transmit buffer
 * @param   rx_buffer: Pointer to receive buffer
 * @param   len: Number of data frames to process
 * @retval  SPI_OK on success
 */
SPI_Status_t SPI_TransmitReceive_Polling(SPI_TypeDef *SPIx, const uint16_t *tx_buffer, uint16_t *rx_buffer, uint32_t len)
{
    if((SPIx->CR1 & SPI_CR1_SPE_MASK) == 0) SPI_Enable(SPIx);
    for(uint32_t inc = 0; inc < len; inc++)
    {
        /* Transmit*/
        while((SPIx->SR & SPI_SR_TXE_MASK) == 0);
        SPIx->DR = *tx_buffer;
        tx_buffer++;
        /* Receive*/
        while((SPIx->SR & SPI_SR_RXNE_MASK) == 0);
        *rx_buffer = SPIx->DR;
        rx_buffer++;
        /* Process overrun*/
        if(SPIx->SR & SPI_SR_OVR_MASK)
        {
            uint32_t tmp = 0;
            tmp = SPIx->DR;
            tmp = SPIx->SR;
            (void)tmp;
        }
    }
    return SPI_OK;
}

/**
 * @brief   Enable SPI transmit DMA requests
 * @param   SPIx: Pointer to SPI peripheral
 */
void SPI_EnableDMA_Tx(SPI_TypeDef *SPIx)
{
    SPIx->CR1 |= SPI_CR1_SPE_MASK;
    /* Enable SPI Tx DMA, Disable SPI Rx DMA*/
    SPIx->CR2 |= SPI_CR2_TXDMAEN_MASK;
    SPIx->CR2 &= ~SPI_CR2_RXDMAEN_MASK;
}

/**
 * @brief   Enable SPI receive DMA requests
 * @param   SPIx: Pointer to SPI peripheral
 */
void SPI_EnableDMA_Rx(SPI_TypeDef *SPIx)
{
    /* Enable SPI Rx DMA */
    SPIx->CR2 |= SPI_CR2_RXDMAEN_MASK;
    /* Disable DMA Tx DMA*/
    SPIx->CR2 &= ~SPI_CR2_TXDMAEN_MASK;
}

/**
 * @brief   Check whether the SPI peripheral is busy
 * @param   SPIx: Pointer to SPI peripheral
 * @retval  1 if the SPI is busy, otherwise 0
 */
uint8_t SPI_IsBusy(SPI_TypeDef *SPIx)
{
    return (SPIx->SR & SPI_SR_BSY_MASK) ? 1U : 0U;
}

/**
 * @brief   Get the status of an SPI flag
 * @param   SPIx: Pointer to SPI peripheral
 * @param   flag: Flag to query
 * @retval  1 if the flag is set, otherwise 0
 */
uint8_t SPI_GetFlagStatus(SPI_TypeDef *SPIx, SPI_Flag_t flag)
{
    return (SPIx->SR & flag) ? 1U : 0U;
}

/**
 * @brief   Clear the SPI overrun flag
 * @param   SPIx: Pointer to SPI peripheral
 */
void SPI_ClearOVRFlag(SPI_TypeDef *SPIx)
{
    volatile uint32_t tmp;
    tmp = SPIx->DR;
    tmp = SPIx->SR;
    (void)tmp;
}

/**
 * @brief   Clear the SPI CRC error flag
 * @param   SPIx: Pointer to SPI peripheral
 */
void SPI_ClearCRCErrorFlag(SPI_TypeDef *SPIx)
{
    SPIx->SR &= ~SPI_SR_CRCERR_MASK;
}

/**
 * @brief   Clear the SPI underrun flag
 * @param   SPIx: Pointer to SPI peripheral
 */
void SPI_ClearUDRFlag(SPI_TypeDef *SPIx)
{
    volatile uint32_t tmp;
    tmp = SPIx->SR;
    (void)tmp;
}

/**
 * @brief   Flush the SPI receive FIFO/register
 * @param   SPIx: Pointer to SPI peripheral
 * @note    This placeholder is reserved for future receive-buffer flush handling.
 */
void SPI_FlushRx(SPI_TypeDef *SPIx)
{
    (void)SPIx->DR;
}


/**
 * @brief   Set the SPI data frame size
 * @param   SPIx: Pointer to SPI peripheral
 * @param   frame: Data frame width selection
 * @note    SPI should be disabled before changing the data frame format.
 */
void SPI_SetDataSize(SPI_TypeDef *SPIx, SPI_DataFrame_t frame)
{
    SPIx->CR1 &= ~SPI_CR1_SPE_MASK;
    SPIx->CR1 &= ~SPI_CR1_DFF_MASK;

    SPIx->CR1 |= frame;

    SPIx->CR1 |= SPI_CR1_SPE_MASK;
}

/**
 * @brief   Configure the SPI CRC polynomial register
 * @param   SPIx: Pointer to SPI peripheral
 * @param   poly: CRC polynomial value
 */
void SPI_SetCRCPolynomial(SPI_TypeDef *SPIx, uint16_t poly)
{
    uint8_t wasEnabled  = 0;
    if(SPIx->CR1 & SPI_CR1_SPE_MASK)
    {
        SPI_Disable(SPIx, 1000);
        wasEnabled  += 1;
    }
    /* CRC calculator enable and set CRC polynomial reg*/
    SPIx->CR1 |= SPI_CR1_CRCEN_MASK;
    SPIx->CRCPR = poly;
    if(wasEnabled  > 0)
    {
        SPI_Enable(SPIx);
    }
}


/* End of refactored SPI */