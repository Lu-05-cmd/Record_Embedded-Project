/**************************************************************************************************
 * @file    uart.h
 * @brief   Bare-metal USART driver for STM32F103xx microcontrollers
 *
 * @details
 * This file provides:
 *  - USART register definitions
 *  - USART configuration APIs
 *  - Blocking transmit/receive functions
 *  - Interrupt and DMA support
 *  - USART status and flag management
 *
 * @author  Tuan Doan
 * @date    2026-02-06
 * @version 0.1.0
 *
 * @target  STM32F103C8T6
 * @toolchain
 *  - arm-none-eabi-gcc
 *  - STM32CubeProgrammer
 *
 * @note
 *  - Developed for embedded systems learning and practice
 *  - Fully register-level programming (Bare-metal)
 *  - Based on STM32F10x Reference Manual (RM0008)
 *  - Supports polling, interrupt, and DMA communication modes
 *
 **************************************************************************************************/

 #ifndef STM32F103xx_USART_H
#define STM32F103xx_USART_H

/*INCLUDE============================================================================================================================================================*/
#include <stdint.h>

/*====================================================================================================================================================================
 |                       Configuration Universal Synchronous Asynchronous Receive Transmitter (USART) 
 =====================================================================================================================================================================*/
typedef struct{
    volatile uint32_t SR;       // Status Register                      - offset: 0x00
    volatile uint32_t DR;       // Data Register                        - offset: 0x04
    volatile uint32_t BRR;      // Baud Rate Register                   - offset: 0x08
    volatile uint32_t CR1;      // Control Register 1                   - offset: 0x0C
    volatile uint32_t CR2;      // Control Register 2                   - offset: 0x10
    volatile uint32_t CR3;      // Control Register 3                   - offset: 0x14
    volatile uint32_t GTPR;     // Gaurd Time and Prescaler Register    - offset: 0x18
}USART_TypeDef;


/*====================================================================================================================================================================
 |                                       USART BASE ADDRESS
 =====================================================================================================================================================================*/
#define USART1 ((USART_TypeDef *) 0x40013800UL)
#define USART3 ((USART_TypeDef *) 0x40004800UL)
#define USART2 ((USART_TypeDef *) 0x40004400UL)

/*====================================================================================================================================================================
|                                           DEFINE MASKS
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
#define USART_CR2_LINEN_POS     (14U)
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
#define USART_CR3_DMAT_MASK           (1U << USART_CR3_DMAT_POS)
#define USART_CR3_RTSE_MASK           (1U << USART_CR3_RTSE_POS)
#define USART_CR3_CTSE_MASK           (1U << USART_CR3_CTSE_POS)
#define USART_CR3_CTSIE_MASK          (1U << USART_CR3_CTSIE_POS)




/*====================================================================================================================================================================
|                                           DEFINE ENUMS
=====================================================================================================================================================================*/
/**
 * 
 * 
 * @brief   USART Status Enumeration
 * @note    This enumeration defines the possible status values for USART operations, including success, error, busy, and timeout conditions.
 *          - USART_OK: Indicates that the USART operation completed successfully without any errors. 
 *          - USART_ERROR: Indicates that an error occurred during the USART operation, such as a framing error, parity error, or overrun error.
 *          - USART_BUSY: Indicates that the USART peripheral is currently busy with an ongoing operation and cannot accept new commands or data until the current operation is complete.
 *          - USART_TIMEOUT: Indicates that a timeout occurred during a USART operation, which could happen if the expected data was not received within a specified time frame or if a transmission took too long to complete.
 */
typedef enum 
{
    USART_OK = 0,
    USART_ERROR,
    USART_BUSY,
    USART_TIMEOUT
}USART_Status_t;



/**
 * 
 * 
 * @brief  USART Flag/  Enumeration
 * @note  This enumeration defines the various flags that can be checked in the USART status register (SR) to determine the current state of the USART peripheral.  
 *      - USART_PE_FLAG     : Indicates a parity error has occurred during reception.
 *      - USART_FE_FLAG     : Indicates a framing error has occurred during reception.
 *      - USART_NE_FLAG     : Indicates a noise error has been detected during reception.
 *      - USART_ORE_FLAG    : Indicates an overrun error has occurred, meaning that data was received but not read before the next data arrived.
 *      - USART_IDLE_FLAG   : Indicates that an idle line has been detected on the USART communication line, which can be used to detect the end of a transmission or a break condition.
 *      - USART_RXNE_FLAG   : Indicates that the receive data register is not empty and contains valid data that can be read.
 *      - USART_TC_FLAG     : Indicates that the transmission of a frame is complete, including the last data bit and the stop bit.
 *      - USART_TXE_FLAG    : Indicates that the transmit data register is empty and can accept new data to be transmitted.
 *      - USART_LBD_FLAG    : Indicates that a LIN break detection event has occurred, which can be used in LIN mode to detect the break signal sent by the master.
 *      - USART_CTS_FLAG    : Indicates the status of the Clear To Send (CTS) flag, which can be used in hardware flow control mode to determine if the USART is clear to send data.  
 */
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


/**
 * 
 * 
 * @brief USART Clear Flag Enumeration
 * @note This enumeration defines the flags that can be cleared in the USART status register (SR) by writing to the corresponding bits in the USART control registers.
 *      - USART_FLAG_RX_STATUS: This flag can be cleared by reading the USART_SR register followed by reading the USART_DR register.
 *      - USART_FLAG_RX_ERROR: This flag can be cleared by reading the USART_SR register followed by reading the USART_DR register.
 *      - USART_FLAG_TC: This flag can be cleared by writing a 0 to the TC (Transmission Complete) bit in the USART_SR register.
 *      - USART_FLAG_CTS: This flag can be cleared by writing a 0 to the CTS (Clear To Send) bit in the USART_SR register.
 */
typedef enum     
{
    USART_FLAG_RX_STATUS = 0,
    USART_FLAG_RX_ERROR,
    USART_FLAG_TC,
    USART_FLAG_CTS,
    USART_FLAG_LBD
}USART_ClearFlag_t;


/**
 * 
 * 
 * @brief USART Parity Configuration
 * @note This enumeration defines the parity configuration options for the USART peripheral. Parity is a method of error detection that can be used in serial communication to ensure data integrity. 
 *      - USART_EVEN_PARITY: Configures the USART to use even parity, where the parity bit is set to 1 if the number of 1s in the data frame is odd, and set to 0 if the number of 1s is even.   
 *      - USART_ODD_PARITY: Configures the USART to use odd parity, where the parity bit is set to 1 if the number of 1s in the data frame is even, and set to 0 if the number of 1s is odd.  
 */
typedef enum  
{
    USART_EVEN_PARITY = 0,
    USART_ODD_PARITY
}USART_Parity_t;


/**
 * 
 * 
 * @brief USART Word Length Enumeration
 * @note This enumeration defines the word length configuration options for the USART peripheral. The word length determines the number of data bits that are transmitted or received in each USART frame. 
 *      - USART_M_8DATABITS: Configures the USART to use 8 data bits in each frame, which is the standard configuration for most serial communication applications. 
 *      - USART_M_9DATABITS: Configures the USART to use 9 data bits in each frame, which can be useful in certain applications that require an additional data bit for specific purposes, such as addressing in multi-drop communication. 
 */
typedef enum    
{
    USART_M_8DATABITS = 0U,
    USART_M_9DATABITS
}USART_WordLength_t;


/**
 * 
 * 
 * @brief USART Stop Bits Enumeration
 * @note This enumeration defines the stop bit configuration options for the USART peripheral. Stop bits are used to indicate the end of a data frame in serial communication.
 *      - STOP_BIT_1: Configures the USART to use 1 stop bit, which is the most common configuration for standard serial communication.
 *      - STOP_BIT_0_5: Configures the USART to use 0.5 stop bits, which is a less common configuration that can be used in specific applications that require shorter stop periods. 
 *      - STOP_BIT_2: Configures the USART to use 2 stop bits, which can provide greater tolerance for timing variations in the communication but reduces the overall data throughput.
 *      - STOP_BIT_1_5: Configures the USART to use 1.5 stop bits, which is a less common configuration that can be used in specific applications that require a stop period longer than 1 stop bit but shorter than 2 stop bits.
 */
typedef enum  
{
    STOP_BIT_1   = 0x0U,
    STOP_BIT_0_5 = 0x1U,
    STOP_BIT_2   = 0x2U,
    STOP_BIT_1_5 = 0x3U,
}USART_StopBits_t;


/**
 * 
 * 
 * @brief USART LIN Mode Enumeration
 * @note This enumeration defines the LIN mode configuration for the USART peripheral. 
 *       LIN (Local Interconnect Network) is a serial communication protocol used in automotive applications for communication between components. 
 *       The LIN mode configuration determines whether the USART peripheral operates in LIN mode or standard USART mode. When LIN mode is enabled, the USART peripheral will generate and detect LIN break signals, which are used to synchronize communication in a LIN network. 
 *       This configuration is essential for applications that require compatibility with LIN communication standards.
 *          - USART_LIN_MODE_DISABLE : Configures the USART to operate in standard USART mode, where it does not generate or detect LIN break signals.
 *          - USART_LIN_MODE_ENABLE  : Configures the USART to operate in LIN mode, where it will generate and detect LIN break signals. 
 */
typedef enum 
{
    USART_LIN_MODE_DISABLE = 0,
    USART_LIN_MODE_ENABLE
}USART_LINMode_t;


/** 
 * 
 * 
 * @brief USART Duplex Mode Enumeration
 * @note This enumeration defines the duplex mode configuration for the USART peripheral.
 *      - USART_DUPLEX_FULL: Configures the USART to operate in full-duplex mode, allowing for simultaneous transmission and reception of data. 
 *      - USART_HALF_DUPLEX: Configures the USART to operate in half-duplex mode, where it can either transmit or receive data at a given time but not both simultaneously.
 */
typedef enum 
{
    USART_DUPLEX_FULL = 0U,
    USART_HALF_DUPLEX = USART_CR3_HDSEL_MASK
}USART_Duplex_t;



/// @brief USART Mode configuration--------------------------------------------------------------------------------
/**
 * 
 * 
 * @brief USART Mode Enumeration
 * @note This enumeration defines the mode configuration for the USART peripheral, specifying whether the USART is configured for reception, transmission, or both.
 *     - USART_MODE_RX: Configures the USART to operate in receive-only mode, where it can only receive data and the transmission functionality is disabled.    
 *    - USART_MODE_TX : Configures the USART to operate in transmit-only mode, where it can only transmit data and the reception functionality is disabled.
 */
typedef enum  
{
    USART_MODE_RX = USART_CR1_RE_MASK,
    USART_MODE_TX = USART_CR1_TE_MASK,
    USART_MODE_TX_RX = USART_CR1_RE_MASK | USART_CR1_TE_MASK
}USART_Mode_t;


/*====================================================================================================================================================================
|                                           DEFINE STRUCTURES
=====================================================================================================================================================================*/
/**
 * 
 * 
 * @brief   USART CR1 configuration structure definition
 * @note This structure is used to configure the USART CR1 register, including word length, stop bits, parity, and mode settings.
 *      - The `word_length`       : field specifies the number of data bits in each USART frame (e.g., 8 bits or 9 bits).
 *      - The `parity`            : field specifies the parity bit configuration for the USART communication (e.g., even parity or odd parity).
 */
typedef struct
{
    USART_WordLength_t word_length;
    USART_Parity_t parity;
}USART_CR1_Config_t;

/**
 * 
 * 
 * @brief USART CR2 configuration structure definition
 * @note  This structure is used to configure the USART CR2 register, which includes settings for stop bits and LIN mode.
 *      - The `stop_bits`       : field specifies the number of stop bits used in the USART communication (e.g., 1 stop bit, 1.5 stop bits, or 2 stop bits).
 *      - The `lin_mode_enable` : field specifies whether LIN mode is enabled or disabled for the USART peripheral.   
 */
typedef struct
{
    USART_StopBits_t stop_bits;
    USART_LINMode_t lin_mode_enable;   
}USART_CR2_Config_t;


/**
 * 
 * 
 * @brief USART configuration structure definition
 * @note This structure is used to configure the USART peripheral, including baud rate and CR1/CR2 settings.
 *      - The `baud_rate`       : field specifies the desired baud rate for USART communication (e.g., 9600, 115200, etc.).
 *      - The `mode`            : field specifies the USART mode (e.g., RX, TX, or both).
 *      - The `cr1_config`      : field is a nested structure that contains the configuration settings for the USART CR1 register, including word length and parity settings.
 *      - The `cr2_config`      : field is a nested structure that contains the configuration settings for the USART CR2 register, including stop bits and LIN mode settings.
 */
typedef struct 
{
    uint32_t baud_rate;
    uint16_t fck;
    USART_Mode_t mode;

    USART_CR1_Config_t cr1_config;
    USART_CR2_Config_t cr2_config;
}USART_Config_t;
/*====================================================================================================================================================================
|                            USART Inline Functions
 =====================================================================================================================================================================*/
/*<---USART Init/Denit--->*/
USART_Status_t USART_Init(USART_TypeDef *USARTx,const USART_Config_t *config);
void USART_DeInit(USART_TypeDef *USARTx);

/*<--- Enable/Disable USART --->*/
void USART_Enable(USART_TypeDef *USARTx);
USART_Status_t USART_Disable(USART_TypeDef *USARTx, uint32_t timeout);

/*<--- Set USART Baud_rate/Stop_bit/Parity/Mode --->*/
void USART_SetBaudRate(USART_TypeDef *USARTx, uint16_t fck, uint32_t baud);
void USART_SetStopBits(USART_TypeDef *USARTx, USART_StopBits_t stopbits);
void USART_SetParity(USART_TypeDef *USARTx, USART_Parity_t parity);
void USART_SetMode(USART_TypeDef *USARTx, USART_Mode_t mode);

/*<--- Set USART Half-Duplex --->*/
void USART_SetHalfDuplex(USART_TypeDef *USARTx, USART_Duplex_t duplex);

/*<--- USART Receive/Transmit Blocking --->*/
USART_Status_t USART_Receive(USART_TypeDef *USARTx, uint8_t *buffer_rx, uint32_t rx_len);
USART_Status_t USART_Transmit(USART_TypeDef *USARTx, const uint8_t *buffer_tx, uint32_t tx_len);

/*<--- USART Receive/Transmit Interrupt --->*/
void USART_EnableRxInterrupt(USART_TypeDef *USARTx);
void USART_EnableTxInterrupt(USART_TypeDef *USARTx);

/*<--- Get/Clear USART Flag --->*/
uint8_t USART_GetFlag(USART_TypeDef *USARTx, USART_Flag_t flag);
void USART_ClearTCFlag(USART_TypeDef *USARTx);
void USART_ClearRXNE(USART_TypeDef *USARTx);
void USART_ClearErrorFlags(USART_TypeDef *USARTx);

/*<--- Enable/Disable USART DMA Rx/Tx --->*/
void USART_EnableDMA_Tx(USART_TypeDef *USARTx);
void USART_EnableDMA_Rx(USART_TypeDef *USARTx);
void USART_DisableDMA_Tx(USART_TypeDef *USARTx);
void USART_DisableDMA_Rx(USART_TypeDef *USARTx);

/*<--- Flush USART Rx --->*/
void USART_FlushRx(USART_TypeDef *USARTx);
#endif  /*  STM32F103xx_USART_H  */