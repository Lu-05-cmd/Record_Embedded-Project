/*********************************************************************************************************************************************************************
 * @file    usart.c
 * @brief   USART driver implementation for STM32F103xx
 *
 * @details
 * This file provides comprehensive USART peripheral configuration and operation functions,
 * including initialization, baud rate/parity/stop-bit configuration, blocking transmit/receive
 * operations, interrupt control, DMA support, and status flag management for STM32F103xx
 * microcontrollers.
 *
 * @author  lu-05-cmd
 * @date    2026-02-06
 * @version 0.1.0
 *
 * @note
 * The implementation uses direct register access and assumes USART peripheral base structures
 * are defined in the included header. Supports polling, interrupt, and DMA communication modes.
 *
 *********************************************************************************************************************************************************************/

/*INCLUDE============================================================================================================================================================*/
#include <stdint.h>
#include "../Inc/rcc.h"
#include "../Inc/usart.h"
#include "../Inc/dma.h"
#include "../stm32f103xx.h"




/*********************************************************************************************************************************************************************
 * @brief   Clear Transmit Complete Flag from USART Status Register
 * @param   USARTx: Pointer to USART instance (e.g., USART1, USART2, USART3)
 * @note    This function clears the TC (Transmit Complete) flag by writing to the SR register.
 *          It is typically used after a transmission is complete to reset the flag state.
 */
static inline void USART_ClearTC(USART_TypeDef *USARTx)
{
    USARTx->SR &= ~USART_SR_TC_MASK;
}


/*********************************************************************************************************************************************************************
 * @brief   Clear CTS (Clear To Send) Flag from USART Status Register
 * @param   USARTx: Pointer to USART instance (e.g., USART1, USART2, USART3)
 * @note    This function clears the CTS flag which is used in hardware flow control mode.
 *          CTS indicates the Clear To Send status for transmit operations.
 */
static inline void USART_ClearCTS(USART_TypeDef *USARTx)
{
    USARTx->SR &= ~USART_SR_CTS_MASK;
}


/*********************************************************************************************************************************************************************
 * @brief   Clear LBD (LIN Break Detection) Flag from USART Status Register
 * @param   USARTx: Pointer to USART instance (e.g., USART1, USART2, USART3)
 * @note    This function clears the LBD flag which is set when a LIN break is detected.
 *          The function only clears the flag if LBD interrupt is enabled in CR2.
 */
static inline void USART_ClearLBD(USART_TypeDef *USARTx)
{
    if(USARTx->CR2 & USART_CR2_LBDIE_MASK)
    {
        USARTx->SR &= ~USART_SR_LBD_MASK;
    }
}


/*********************************************************************************************************************************************************************
 * @brief   Clear Receive Status Flags by reading USART Status and Data Registers
 * @param   USARTx: Pointer to USART instance (e.g., USART1, USART2, USART3)
 * @note    This function clears receive-related status flags by performing a read-clear sequence.
 *          Reading the SR register followed by DR register clears IDLE, RXNE, and other flags.
 */
static inline void USART_ClearRxStatus(USART_TypeDef *USARTx)
{
    /* Clear USART Status All*/
    volatile uint32_t tmp;
    tmp = USARTx->SR;
    tmp = USARTx->DR;
    (void)tmp;
}


/*********************************************************************************************************************************************************************
 * @brief   Clear Receive Error Flags (PE, FE, NE, ORE) from USART Status Register
 * @param   USARTx: Pointer to USART instance (e.g., USART1, USART2, USART3)
 * @note    This function clears error flags by performing a read-clear sequence on SR and DR registers.
 *          It handles parity errors, framing errors, noise errors, and overrun errors.
 */
static inline void USART_ClearRxErrorFlags(USART_TypeDef *USARTx)
{
    volatile uint32_t tmp;
    tmp = USARTx->SR;
    tmp = USARTx->DR;
    (void)tmp;    
}




/*********************************************************************************************************************************************************************
 * @brief   Enable the USART peripheral
 * @param   USARTx: Pointer to USART instance (e.g., USART1, USART2, USART3)
 * @note    This function enables the USART by setting the UE (USART Enable) bit in CR1 register.
 *          USART must be enabled before any communication can take place.
 */
void USART_Enable(USART_TypeDef *USARTx)
{
    // Enable USART
    USARTx->CR1 |= USART_CR1_UE_MASK;
}

/**
 * @brief   Disable the USART peripheral after TX complete.
 * @param   USARTx  Pointer to USART instance.
 * @param   timeout Timeout counter value for TX complete wait.
 * @retval  USART_OK      USART disabled successfully.
 * @retval  USART_TIMEOUT Timeout while waiting for TX complete.
 */
USART_Status_t USART_Disable(USART_TypeDef *USARTx, uint32_t timeout)
{
    while(!(USARTx->SR & USART_SR_TC_MASK))
    {
        if(timeout-- == 0) return USART_TIMEOUT;
    }
    
    USARTx->CR1 &= ~USART_CR1_UE_MASK;
    return USART_OK;
}


/*********************************************************************************************************************************************************************
 * @brief   Configure USART Baud Rate
 * @param   USARTx: Pointer to USART instance (e.g., USART1, USART2, USART3)
 * @param   fck: APB clock frequency in Hz (e.g., 36000000 for 36MHz)
 * @param   baud: Desired baud rate in bits per second (e.g., 115200, 9600)
 * @note    This function calculates the baud rate divider and writes it to the BRR register.
 *          The calculation includes rounding: div = (fck + baud/2) / baud for improved accuracy.
 */
void USART_SetBaudRate(USART_TypeDef *USARTx, uint16_t fck, uint32_t baud)
{
    uint32_t div = (fck + (baud / 2U)) / baud;
    USARTx->BRR = div;
}


/*********************************************************************************************************************************************************************
 * @brief   Configure USART Stop Bits
 * @param   USARTx: Pointer to USART instance (e.g., USART1, USART2, USART3)
 * @param   stopbits: Stop bits configuration (USART_StopBits_t enum - 0.5, 1, 1.5, or 2 bits)
 * @note    This function updates the STOP[1:0] bits in CR2 register to set the number of stop bits.
 *          Stop bits indicate the end of a data frame in serial communication.
 */
void USART_SetStopBits(USART_TypeDef *USARTx, USART_StopBits_t stopbits)
{
    /* Clear old field-bit*/
    USARTx->CR2 &= ~USART_CR2_STOP_MASK;
    /* Set new stop-bit*/
   USARTx->CR2 |= (stopbits << USART_CR2_STOP_POS);
}

/*********************************************************************************************************************************************************************
 * @brief   Configure USART Parity Mode
 * @param   USARTx: Pointer to USART instance (e.g., USART1, USART2, USART3)
 * @param   parity: Parity mode (USART_Parity_t enum - EVEN, ODD, or NONE)
 * @note    This function configures parity error detection by updating PS and PCE bits in CR1 register.
 *          Parity is used as a simple error detection method in serial communication.
 */
void USART_SetParity(USART_TypeDef *USARTx, USART_Parity_t parity)
{
    /* Clear old bit*/
    USARTx->CR1 &= ~(USART_CR1_PS_MASK | USART_CR1_PCE_MASK);
    /* Set Parity*/
    if(parity == USART_EVEN_PARITY) USARTx->CR1 |= USART_CR1_PCE_MASK;
    else if(parity == USART_ODD_PARITY) 
    {
        USARTx->CR1 |= (parity << USART_CR1_PS_POS);
        USARTx->CR1 |= USART_CR1_PCE_MASK;
    }
}

/*********************************************************************************************************************************************************************
 * @brief   Configure USART Communication Mode
 * @param   USARTx: Pointer to USART instance (e.g., USART1, USART2, USART3)
 * @param   mode: Communication mode (USART_Mode_t enum - RX only, TX only, or RX+TX)
 * @note    This function enables/disables the receiver and transmitter by setting RE and TE bits in CR1.
 *          The mode determines whether USART operates in Rx, Tx, or full-duplex mode.
 */

void USART_SetMode(USART_TypeDef *USARTx, USART_Mode_t mode)
{
    USARTx->CR1 &= ~(USART_CR1_RE_MASK | USART_CR1_TE_MASK);
    USARTx->CR1 |= mode;
}

/*********************************************************************************************************************************************************************
 * @brief   Configure USART Half-Duplex Mode
 * @param   USARTx: Pointer to USART instance (e.g., USART1, USART2, USART3)
 * @param   duplex: Half-duplex mode configuration (USART_Duplex_t enum)
 * @note    This function enables half-duplex mode by setting HDSEL bit in CR3 register.
 *          In half-duplex mode, the TX and RX pins are internally connected to the same line.
 */
void USART_SetHalfDuplex(USART_TypeDef *USARTx, USART_Duplex_t duplex)
{
    USARTx->CR3 &= ~USART_CR3_HDSEL_MASK;
    USARTx->CR3 |= duplex;
}

/*********************************************************************************************************************************************************************
 * @brief   Receive data from USART in blocking/polling mode
 * @param   USARTx: Pointer to USART instance (e.g., USART1, USART2, USART3)
 * @param   buffer_rx: Pointer to buffer where received data will be stored
 * @param   rx_len: Number of bytes to receive
 * @retval  USART_OK: Reception completed successfully
 * @note    This function blocks until all requested bytes are received. Erroneous bytes
 *          (with PE, FE, NE, ORE flags set) are stored as 0x00 in the buffer.
 */
USART_Status_t USART_Receive(USART_TypeDef *USARTx, uint8_t *buffer_rx, uint32_t rx_len)
{
    /* Enable USARTx if it's disable*/
    if(!(USARTx->CR1 & USART_CR1_UE_MASK))  USART_Enable(USARTx);
    
    volatile uint32_t state, data;

    for(uint32_t inc = 0; inc < rx_len; inc++)
    {
        /* while to when reg DR not empty */
        while(!(USARTx->SR & USART_SR_RXNE_MASK));
        /* Read Status*/
        state = USARTx->SR;
        /* Assign data*/
        data = USARTx->DR;
        
        /* if error then the buffer at index as empty*/
        if(state & (USART_SR_PE_MASK | 
                    USART_SR_FE_MASK |
                    USART_SR_NE_MASK |
                    USART_SR_ORE_MASK)) 
        {
            /* Error data, receive buffer is 0*/
            *buffer_rx++ = 0;
            continue;
        }
        else{
            /* Write data to buffer receive*/
            *buffer_rx++ = (uint8_t)data;
        }
    }
    return USART_OK;
}

/*********************************************************************************************************************************************************************
 * @brief   Transmit data over USART in blocking/polling mode
 * @param   USARTx: Pointer to USART instance (e.g., USART1, USART2, USART3)
 * @param   buffer_tx: Pointer to buffer containing data to transmit
 * @param   tx_len: Number of bytes to transmit
 * @retval  USART_OK: Transmission completed successfully
 * @note    This function blocks until all data is transmitted and the transmit complete flag is set.
 *          Auto-enables USART if it is disabled before transmission.
 */
USART_Status_t USART_Transmit(USART_TypeDef *USARTx, const uint8_t *buffer_tx, uint32_t tx_len)
{
    if(!(USARTx->CR1 & USART_CR1_UE_MASK))  USART_Enable(USARTx);

    for(uint32_t inc = 0; inc < tx_len; inc++)
    { 
        while(!(USARTx->SR & USART_SR_TXE_MASK));
        USARTx->DR = *buffer_tx;
        buffer_tx++;
    }
    while(!(USARTx->SR & USART_SR_TC_MASK));
    return USART_OK;
}

/*********************************************************************************************************************************************************************
 * @brief   Enable USART Receive Data Register Not Empty (RXNE) Interrupt
 * @param   USARTx: Pointer to USART instance (e.g., USART1, USART2, USART3)
 * @note    This function enables the RXNEIE interrupt by setting the corresponding bit in CR1 register.
 *          When enabled, USART generates an interrupt when data is available in the receive register.
 */
void USART_EnableRxInterrupt(USART_TypeDef *USARTx)
{
    /* Enable USART Receive Interrupt*/
    USARTx->CR1 |= USART_CR1_RXNEIE_MASK;
}

/*********************************************************************************************************************************************************************
 * @brief   Enable USART Transmit Data Register Empty (TXE) Interrupt
 * @param   USARTx: Pointer to USART instance (e.g., USART1, USART2, USART3)
 * @note    This function enables the TXEIE interrupt by setting the corresponding bit in CR1 register.
 *          When enabled, USART generates an interrupt when the transmit data register becomes empty.
 */
void USART_EnableTxInterrupt(USART_TypeDef *USARTx)
{
    /* Enable USARTx Transmit Interrupt */
    USARTx->CR1 |= USART_CR1_TXEIE_MASK;
}

/*********************************************************************************************************************************************************************
 * @brief   Read USART Status Flag
 * @param   USARTx: Pointer to USART instance (e.g., USART1, USART2, USART3)
 * @param   flag: Flag to check (USART_Flag_t enum - PE, FE, NE, ORE, IDLE, RXNE, TC, TXE, LBD, CTS)
 * @retval  1 if flag is set, 0 if flag is not set
 * @note    This function checks the status of a specific flag in the USART SR register without clearing it.
 */
uint8_t USART_GetFlag(USART_TypeDef *USARTx, USART_Flag_t flag)
{
    return (USARTx->SR & flag) ? 1U : 0U;
}

/*********************************************************************************************************************************************************************
 * @brief   Clear Transmit Complete (TC) Flag from USART Status Register
 * @param   USARTx: Pointer to USART instance (e.g., USART1, USART2, USART3)
 * @note    This function clears the TC flag by calling the USART_ClearTC internal function.
 *          TC flag indicates that the last data bit and stop bit have been transmitted.
 */
void USART_ClearTCFlag(USART_TypeDef *USARTx)
{
    USART_ClearTC(USARTx);
}

/*********************************************************************************************************************************************************************
 * @brief   Clear Receive Data Register Not Empty (RXNE) Flag
 * @param   USARTx: Pointer to USART instance (e.g., USART1, USART2, USART3)
 * @note    This function clears the RXNE flag by performing a read-clear sequence on SR and DR registers.
 *          RXNE indicates that unread data is available in the receive register.
 */
void USART_ClearRXNE(USART_TypeDef *USARTx)
{
    USART_ClearRxStatus(USARTx);
}

/*********************************************************************************************************************************************************************
 * @brief   Clear Receive Error Flags (PE, FE, NE, ORE)
 * @param   USARTx: Pointer to USART instance (e.g., USART1, USART2, USART3)
 * @note    This function clears all receive-related error flags by calling the internal error clearing function.
 *          Clears parity error, framing error, noise error, and overrun error flags.
 */
void USART_ClearErrorFlags(USART_TypeDef *USARTx)
{
    USART_ClearRxErrorFlags(USARTx);
}

/*********************************************************************************************************************************************************************
 * @brief   Enable USART DMA for Transmit (DMA Tx)
 * @param   USARTx: Pointer to USART instance (e.g., USART1, USART2, USART3)
 * @note    This function enables DMA transmission by setting the DMAT bit in CR3 register.
 *          When enabled, the DMA controller can manage transmit data transfers.
 */

void USART_EnableDMA_Tx(USART_TypeDef *USARTx)
{
    USARTx->CR3 |= USART_CR3_DMAT_MASK;
}

/*********************************************************************************************************************************************************************
 * @brief   Enable USART DMA for Receive (DMA Rx)
 * @param   USARTx: Pointer to USART instance (e.g., USART1, USART2, USART3)
 * @note    This function enables DMA reception by setting the DMAR bit in CR3 register.
 *          When enabled, the DMA controller can manage receive data transfers.
 */
void USART_EnableDMA_Rx(USART_TypeDef *USARTx)
{
    USARTx->CR3 |= USART_CR3_DMAR_MASK;
}

/*********************************************************************************************************************************************************************
 * @brief   Disable USART DMA for Transmit (DMA Tx)
 * @param   USARTx: Pointer to USART instance (e.g., USART1, USART2, USART3)
 * @note    This function disables DMA transmission by clearing the DMAT bit in CR3 register.
 */
void USART_DisableDMA_Tx(USART_TypeDef *USARTx)
{
    USARTx->CR3 &= ~USART_CR3_DMAT_MASK;
}

/*********************************************************************************************************************************************************************
 * @brief   Disable USART DMA for Receive (DMA Rx)
 * @param   USARTx: Pointer to USART instance (e.g., USART1, USART2, USART3)
 * @note    This function disables DMA reception by clearing the DMAR bit in CR3 register.
 */
void USART_DisableDMA_Rx(USART_TypeDef *USARTx)
{
    USARTx->CR3 &= ~USART_CR3_DMAR_MASK;
}

/*********************************************************************************************************************************************************************
 * @brief   Flush Receive FIFO by clearing all pending receive data
 * @param   USARTx: Pointer to USART instance (e.g., USART1, USART2, USART3)
 * @note    This function clears the receive data register and status flags by reading SR and DR registers.
 *          Any pending received data is discarded after this operation.
 */
void USART_FlushRx(USART_TypeDef *USARTx)
{
    volatile uint32_t tmp;
    tmp = USARTx->SR;
    tmp = USARTx->DR;
    (void)tmp;
}

/*********************************************************************************************************************************************************************
 * @brief   Initialize USART peripheral with configuration parameters
 * @param   USARTx: Pointer to USART instance (e.g., USART1, USART2, USART3)
 * @param   config: Pointer to USART_Config_t structure containing configuration parameters
 *          (baud rate, mode, parity, stop bits, word length, LIN mode)
 * @retval  USART_OK: USART initialized successfully
 * @note    This function configures baud rate, mode, parity, stop bits, word length, and LIN mode.
 *          USART is disabled during configuration and must be enabled separately with USART_Enable().
 */
USART_Status_t USART_Init(USART_TypeDef *USARTx,const USART_Config_t *config)
{
    // Before USART configuration is disable 
    USARTx->CR1 &= ~USART_CR1_UE_MASK;
    
    /* Set baud rate, mode and other parameters */
    USART_SetBaudRate(USARTx, config->fck, config->baud_rate);
    USART_SetMode(USARTx, config->mode);
    USART_SetParity(USARTx, config->cr1_config.parity);
    USART_SetStopBits(USARTx, config->cr2_config.stop_bits);
    
    /* Set LIN mode */
    if(config->cr2_config.lin_mode_enable == USART_LIN_MODE_ENABLE) USARTx->CR2 |= USART_CR2_LINEN_MASK;
    else if(config->cr2_config.lin_mode_enable == USART_LIN_MODE_DISABLE) USARTx->CR2 &= ~USART_CR2_LINEN_MASK;
    
    /* Set word length*/
    if(config->cr1_config.word_length == USART_M_9DATABITS) USARTx->CR1 |= USART_CR1_M_MASK;
    else if(config->cr1_config.word_length == USART_M_8DATABITS) USARTx->CR1 &= ~USART_CR1_M_MASK;
    
    return USART_OK;
}

/*********************************************************************************************************************************************************************
 * @brief   De-initialize USART peripheral and reset it
 * @param   USARTx: Pointer to USART instance (e.g., USART1, USART2, USART3)
 * @note    This function resets the USART peripheral using the RCC reset mechanism.
 *          After de-initialization, the USART must be initialized again before use.
 */

void USART_DeInit(USART_TypeDef *USARTx)
{
    if(USARTx == USART1)    RCC_ResetUSART(RCC_USART1);
    else if(USARTx == USART2)    RCC_ResetUSART(RCC_USART2);
    else if(USARTx == USART3)    RCC_ResetUSART(RCC_USART3);
}

/* End of refactored USART */