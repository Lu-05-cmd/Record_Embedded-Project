/**********************************************************************************************************************************************************************
 * @file    : uart.h
 * @brief   :     
 *      - Generic USART driver interface for STM32F103xx drivices.
 *      - This file provides register definition, base address and basic function
 * @author  : lu-05-cmd
 * @date    : January, 6 2026
 * @version : 0.1
 * @note : 
 *      - This is file Design based on STM32F10xx reference manual (RM008).
 *      - GPIO register layout follows the STM32F103 memory map and register description.
 *      - Defines the position of each bit in the register and supporting enum structures.
 **********************************************************************************************************************************************************************/

#ifndef STM32F103xx_USART_H
#define STM32F103xx_USART_H

/*INCLUDE============================================================================================================================================================*/
#include <stdint.h>

/*====================================================================================================================================================================
 |                       Configuration Universal Synchronous Asynchronous Receive Transmitter (USART) 
 =====================================================================================================================================================================*/
typedef struct{
    volatile uint32_t SR;       // Status Register                      - offer: 0x00
    volatile uint32_t DR;       // Data Register                        - offer: 0x04
    volatile uint32_t BRR;      // Baud Rate Register                   - offer: 0x08
    volatile uint32_t CR1;      // Control Register 1                   - offer: 0x0C
    volatile uint32_t CR2;      // Control Register 2                   - offer: 0x10
    volatile uint32_t CR3;      // Control Register 3                   - offer: 0x14
    volatile uint32_t GTPR;     // Gaurd Time ans Prescaler Register    - offer: 0x18
}USART_TypeDef;


/*====================================================================================================================================================================
 |                                       USART BASE ADDRESS
 =====================================================================================================================================================================*/
#define USART1 ((USART_TypeDef *) 0x40013800UL)
#define USART3 ((USART_TypeDef *) 0x40004800UL)
#define USART2 ((USART_TypeDef *) 0x40004400UL)

/*====================================================================================================================================================================
 |                                           Mode Define
 =====================================================================================================================================================================*/
/*Status Regiater*/
#define USART_SR_PE_POS     (0U)
#define USART_SR_FE_POS     (1U)
#define USART_SR_NE_POS     (2U)
#define USART_SR_ORE_POS    (3U)
#define USART_SR_IDLE_POS   (4U)
#define USART_SR_RXNE_POS   (5U)
#define USART_SR_TC_POS     (6U)
#define USART_SR_TXE_POS    (7U)
#define USART_SR_LBD_POS    (8U)
#define USART_SR_CTS_POS    (9U)
 /*Baud Rate Register*/
#define USART_BRR_DIV_FRACTION_POS  (0U)
#define USART_BRR_DIV_MANTISSA_POS  (4U)
 /*Control Register 1*/
#define USART_CR1_RE_POS        (2U)
#define USART_CR1_TE_POS        (3U)
#define USART_CR1_IDLEIE_POS    (4U)
#define USART_CR1_RXNEIE_POS    (5U)
#define USART_CR1_TCIE_POS      (6U)
#define USART_CR1_TXEIE_POS     (7U)
#define USART_CR1_PEIE_POS      (8U)
#define USART_CR1_PS_POS        (9U)
#define USART_CR1_PCE_POS       (10U)
#define USART_CR1_M_POS         (12U)
#define USART_CR1_UE_POS        (13U)
/*Control Register 2*/
#define USART_CR2_LBDIE_POS     (6U)
#define USART_CR2_STOP_POS      (12U)
#define USART_CR2_LINEN_POS      (12U)
/*Control Register 3*/
#define USART_CR3_HDSEL_POS     (3U)
#define USART_CR3_NACK_POS      (4U)
#define USART_CR3_SCEN_POS      (5U)
#define USART_CR3_DMAR_POS      (6U)
#define USART_CR3_DMAT_POS      (7U)
#define USART_CR3_RTSE_POS      (8U)
#define USART_CR3_CTSE_POS      (9U)
#define USART_CR3_CTSIE_POS     (10U)
/*Gaurd Time and Prescaler Register*/


/*SR_MASK*/
#define USART_SR_PE_MASK              (1U << USART_SR_PE_POS)
#define USART_SR_FE_MASK              (1U << USART_SR_FE_POS)
#define USART_SR_NE_MASK              (1U << USART_SR_NE_POS)
#define USART_SR_ORE_MASK             (1U << USART_SR_ORE_POS)
#define USART_SR_IDLE_MASK            (1U << USART_SR_IDLE_POS) 
#define USART_SR_RXNE_MASK            (1U << USART_SR_RXNE_POS)
#define USART_SR_TC_MASK              (1U << USART_SR_TC_POS)
#define USART_SR_TXE_MASK             (1U << USART_SR_TXE_POS)
#define USART_SR_LBD_MASK             (1U << USART_SR_LBD_POS)
#define USART_SR_CTS_MASK             (1U << USART_SR_CTS_POS)
/*CR1_MASK*/
#define USART_CR1_RE_MASK             (1U << USART_CR1_RE_POS)
#define USART_CR1_TE_MASK             (1U << USART_CR1_TE_POS)
#define USART_CR1_IDLEIE_MASK         (1U << USART_CR1_IDLEIE_POS)
#define USART_CR1_RXNEIE_MASK         (1U << USART_CR1_RXNEIE_POS)
#define USART_CR1_TCIE_MASK           (1U << USART_CR1_TCIE_POS)
#define USART_CR1_TXEIE_MASK          (1U << USART_CR1_TXEIE_POS)
#define USART_CR1_PEIE_MASK           (1U << USART_CR1_PEIE_POS)
#define USART_CR1_PS_MASK             (1U << USART_CR1_PS_POS)
#define USART_CR1_PCE_MASK            (1U << USART_CR1_PCE_POS)
#define USART_CR1_M_MASK              (1U << USART_CR1_M_POS)
#define USART_CR1_UE_MASK             (1U << USART_CR1_UE_POS)
/*CR2_MASK*/
#define USART_CR2_LBDIE_MASK          (1U << USART_CR2_LBDIE_POS)
#define USART_CR2_STOP_MASK           (0x3U << USART_CR2_STOP_POS)
#define USART_CR2_LINEN_MASK          (1U << USART_CR2_LINEN_POS)
/*CR3_MASK*/
#define USART_CR3_HDSEL_MASK          (1U << USART_CR3_HDSEL_POS)
#define USART_CR3_NACK_MASK           (1U << USART_CR3_NACK_POS)
#define USART_CR3_SCEN_MASK           (1U << USART_CR3_SCEN_POS)
#define USART_CR3_DMAR_MASK           (1U << USART_CR3_DMAR_POS)
#define USART_CR3_DMAT_MASK           (1U << USART_CR3_DMAR_POS)
#define USART_CR3_RTSE_MASK           (1U << USART_CR3_RTSE_POS)
#define USART_CR3_CTSE_MASK           (1U << USART_CR3_CTSE_POS)
#define USART_CR3_CTSIE_MASK          (1U << USART_CR3_CTSIE_POS)




/*------------------------ USART Mode Enum-----------------------*/
typedef enum
{
    USART_EVEN_PARITY = 0,
    USART_ODD_PARITY
}USART_Parity_t;

typedef enum
{
    USART_PE_FLAG   = USART_SR_PE_MASK,
    USART_FE_FLAG   = USART_SR_FE_MASK,
    USART_NE_FLAG   = USART_SR_NE_MASK,
    USART_ORE_FLAG  = USART_SR_ORE_MASK,
    USART_IDLE_FLAG = USART_SR_IDLE_MASK,
    USART_RXNE_FLAG = USART_SR_RXNE_MASK,
    USART_TC_FLAG   = USART_SR_TC_MASK,
    USART_TXE_FLAG  = USART_SR_TXE_MASK,
    USART_LBD_FLAG  = USART_SR_LBD_MASK,
    USART_CTS_FLAG  = USART_SR_CTS_MASK
}USART_Flag_t;

typedef enum
{
    USART_FLAG_RX_STATUS = 0,
    USART_FLAG_RX_ERROR,
    USART_FLAG_TC,
    USART_FLAG_CTS,
    USART_FLAG_LBD
}USART_ClearFlag_t;

typedef enum
{
    STOP_BIT_1   = 0x0U,
    STOP_BIT_0_5 = 0x1U,
    STOP_BIT_2   = 0x2U,
    STOP_BIT_1_5 = 0x3U,
}USART_StopBits_t;

typedef enum
{
    USART_M_8DATABITS = ~USART_CR1_M_MASK,
    USART_M_9DATABITS = USART_CR1_M_MASK
}USART_WordLength_t;

typedef enum
{
    USART_RXNEIE = USART_CR1_RXNEIE_MASK,
    USART_TXEIE  = USART_CR1_TXEIE_MASK
}USART_Interrupt_t;

typedef enum 
{
    USART_MODE_RX = USART_CR1_RE_MASK,
    USART_MODE_TX = USART_CR1_TE_MASK,
    USART_MODE_TX_RX = USART_CR1_RE_MASK | USART_CR1_TE_MASK
}USART_Mode_t;

typedef enum
{
    USART_DUPLEX_FULL = 0U,
    USART_HALF_DUPLEX = USART_CR3_HDSEL_MASK
}USART_Duplex_t;
/*====================================================================================================================================================================
|                            USART Inline Functions
 =====================================================================================================================================================================*/
/*<---USART Init/Denit--->*/
void USART_Init(USART_TypeDef *USARTx, USART_WordLength_t word_length, USART_StopBits_t stop_bits);
void USART_Denit(USART_TypeDef *USARTx);

/*<--- Enable/Disable USART --->*/
void USART_Enable(USART_TypeDef *USARTx);
void USART_Disable(USART_TypeDef *USARTx);

/*<--- Set USART Baud_rate/Stop_bit/Parity/Mode --->*/
void USART_SetBaudRate(USART_TypeDef *USARTx, uint32_t fck, uint32_t baud);
void USART_SetStopBits(USART_TypeDef *USARTx, USART_StopBits_t stopbits);
void USART_SetParity(USART_TypeDef *USARTx, USART_Parity_t parity);
void USART_SetMode(USART_TypeDef *USARTx, USART_Mode_t mode);
void USART_SetHalfDuplex(USART_TypeDef *USARTx, USART_Duplex_t duplex);

/*<--- USART Receive/Transmit Blocking --->*/
void USART_Receive(USART_TypeDef *USARTx, uint8_t *buffer_rx, uint32_t rx_len);
void USART_Transmit(USART_TypeDef *USARTx, uint8_t *buffer_tx, uint32_t tx_len);

/*<--- USART Receive/Transmit Interrupt --->*/
void USART_EnableRxInterrupt(USART_TypeDef *USARTx);
void USART_EnableTxInterrupt(USART_TypeDef *USARTx);

/*<--- Get/Clear USART Flag --->*/
uint8_t USART_GetFlag(USART_TypeDef *USARTx, USART_Flag_t flag);
void USART_ClearFlag(USART_TypeDef *USARTx, USART_ClearFlag_t flag);


/*<--- Enable/Disable USART DMA Rx/Tx --->*/
void USART_EnableDMA_Tx(USART_TypeDef *USARTx);
void USART_EnableDMA_Rx(USART_TypeDef *USARTx);
void USART_DisableDMA_Tx(USART_TypeDef *USARTx);
void USART_DisableDMA_Rx(USART_TypeDef *USARTx);

/*<--- Flush USART Rx --->*/
void USART_FlushRx(USART_TypeDef *USARTx);
#endif  /*  STM32F103xx_USART_H  */