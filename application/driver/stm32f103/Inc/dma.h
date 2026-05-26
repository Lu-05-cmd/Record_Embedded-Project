/**************************************************************************************************
 * @file    dma.h
 * @brief   Bare-metal DMA driver for STM32F103xx microcontrollers
 *
 * @details
 * This file provides:
 *  - DMA register definitions
 *  - DMA channel configuration APIs
 *  - Memory-to-peripheral and peripheral-to-memory transfer support
 *  - DMA interrupt and flag management
 *  - DMA transfer control functions
 *
 * @author  lu-05-cmd
 * @date    2026-03-20
 * @version 0.1.0
 *
 * @target STM32F103xx Series
 * 
 * @note
 *  - Developed for embedded systems learning and practice
 *  - Fully register-level programming (Bare-metal)
 *  - Based on STM32F10x Reference Manual (RM0008)
 *  - Supports normal, circular, and memory-to-memory transfer modes
 *
 **************************************************************************************************/
#ifndef STM32F103xx_DMA_H
#define STM32F103xx_DMA_H


/*INCLUDE============================================================================================================================================================*/
#include <stdio.h>



/*====================================================================================================================================================================
 |                             Configuration Direct Memory Access (DMA)
 =====================================================================================================================================================================*/
typedef struct {
    volatile uint32_t ISR;   // DMA Interrupt Status Register         - offset: 0x00
    volatile uint32_t IFCR;  // DMA Interrupt Flag Clear Register     - offset: 0x04
}DMA_TypeDef;

typedef struct {
    volatile uint32_t CCR;    // Channel Configuration Register        - offset: 0x08
    volatile uint32_t CNDTR;  // Channel Number of Data Register       - offset: 0x0C
    volatile uint32_t CPAR;   // Channel Peripheral Address Register   - offset: 0x10
    volatile uint32_t CMAR;   // Channel Memory Address Register       - offset: 0x14
}DMA_Channel_TypeDef;

/*====================================================================================================================================================================
|                             DMA BASE ADDRESS
=====================================================================================================================================================================*/
#define DMA        ((DMA_TypeDef *)0x40020000UL)
#define DMA_CH1    ((DMA_Channel_TypeDef *)0x40020008UL)
#define DMA_CH2    ((DMA_Channel_TypeDef *)0x4002001CUL)
#define DMA_CH3    ((DMA_Channel_TypeDef *)0x40020030UL)
#define DMA_CH4    ((DMA_Channel_TypeDef *)0x40020044UL)
#define DMA_CH5    ((DMA_Channel_TypeDef *)0x40020058UL)
#define DMA_CH6    ((DMA_Channel_TypeDef *)0x40020060UL)
#define DMA_CH7    ((DMA_Channel_TypeDef *)0x40020080UL)

/*====================================================================================================================================================================
 |                                           Mode Define
 =====================================================================================================================================================================*/
/*DMA Interrupt Status Register*/
#define DMA_ISR_FLAG_OFFSET     (4U)
#define DMA_ISR_GIF_OFFSET      (0U)
#define DMA_ISR_TCIF_OFFSET     (1U)
#define DMA_ISR_HTIF_OFFSET     (2U)
#define DMA_ISR_TEIF_OFFSET     (3U)
/*DMA Interrupt Flag Clear Register*/
#define DMA_IFCR_FLAG_OFFSET    (4U)
#define DMA_IFCR_CGIF_OFFSET    (0U)
#define DMA_IFCR_CTCIF_OFFSET   (1U)
#define DMA_IFCR_CHTIF_OFFSET   (2U)
#define DMA_IFCR_CTEIF_OFFSET   (3U)
/*DMA Channel X Configuration Register*/
#define DMA_CCRx_EN_POS         (0U)
#define DMA_CCRx_TCIE_POS       (1U)
#define DMA_CCRx_HTIE_POS       (2U)
#define DMA_CCRx_TEIE_POS       (3U)
#define DMA_CCRx_DIR_POS        (4U)
#define DMA_CCRx_CIRC_POS       (5U)
#define DMA_CCRx_PINC_POS       (6U)
#define DMA_CCRx_MINC_POS       (7U)
#define DMA_CCRx_PSIZE_POS      (8U)
#define DMA_CCRx_MSIZE_POS      (10U)
#define DMA_CCRx_PL_POS         (12U)
#define DMA_CCRx_MEM2MEM_POS    (14U)
/*DMA Channel x Number of Data Register*/
#define DMA_CNDTRx_NDT_POS      (0U)
/*DMA Channle x Peripheral Address Register*/
#define DMA_CPARx_PA_POS        (0U)
/*DMA Channel x Memory Address Register*/
#define DMA_CMARx_MA_POS        (0u)



/*ISR_MASK*/
#define DMA_ISR_GIF(chl)    \
    (1U << ((DMA_ISR_FLAG_OFFSET * chl) + DMA_ISR_GIF_OFFSET))
#define DMA_ISR_TCIF(chl)   \
    (1U << ((DMA_ISR_FLAG_OFFSET * chl) + DMA_ISR_TCIF_OFFSET))
#define DMA_ISR_HTIF(chl)   \
    (1U << ((DMA_ISR_FLAG_OFFSET * chl) + DMA_ISR_HTIF_OFFSET))
#define DMA_ISR_TEIF(chl)   \
    (1U << ((DMA_ISR_FLAG_OFFSET * chl) + DMA_ISR_TEIF_OFFSET))
/*IFCR_MASK*/
#define DMA_IFCR_CGIF(chl)  \
    (1U << ((DMA_IFCR_FLAG_OFFSET * chl) + DMA_IFCR_CGIF_OFFSET))
#define DMA_IFCR_CTCIF(chl)  \
    (1U << ((DMA_IFCR_FLAG_OFFSET * chl) + DMA_IFCR_CTCIF_OFFSET))
#define DMA_IFCR_CHTIF(chl)  \
    (1U << ((DMA_IFCR_FLAG_OFFSET * chl) + DMA_IFCR_CHTIF_OFFSET))
#define DMA_IFCR_CTEIF(chl)  \
    (1U << ((DMA_IFCR_FLAG_OFFSET * chl) + DMA_IFCR_CTEIF_OFFSET))
/*CCRx_MASK*/
#define DMA_CCRx_EN_MASK        (1U << DMA_CCRx_EN_POS)
#define DMA_CCRx_TCIE_MASK      (1U << DMA_CCRx_TCIE_POS)
#define DMA_CCRx_HTIE_MASK      (1U << DMA_CCRx_HTIE_POS)
#define DMA_CCRx_TEIE_MASK      (1U << DMA_CCRx_TEIE_POS)
#define DMA_CCRx_DIR_MASK       (1U << DMA_CCRx_DIR_POS)
#define DMA_CCRx_CIRC_MASK      (1U << DMA_CCRx_CIRC_POS)
#define DMA_CCRx_PINC_MASK      (1U << DMA_CCRx_PINC_POS)
#define DMA_CCRx_MINC_MASK      (1U << DMA_CCRx_MINC_POS)
#define DMA_CCRx_PSIZE_MASK     (0x3U << DMA_CCRx_PSIZE_POS)
#define DMA_CCRx_MSIZE_MASK     (0x3U << DMA_CCRx_MSIZE_POS)
#define DMA_CCRx_PL_MASK        (0x3U << DMA_CCRx_PL_POS)
#define DMA_CCRx_MEM2MEM_MASK   (1U << DMA_CCRx_MEM2MEM_POS)


/*====================================================================================================================================================================
|                                           DEFINE ENUMERATIONS
=====================================================================================================================================================================*/
/**
 * 
 * 
 * @brief  DMA  Status Enumeration
 * @note   This enumeration defines possible status values for DMA operations, including success, error, timeout, and invalid configuration. It can be used to indicate the outcome of DMA initialization, transfer, and interrupt handling functions, allowing for standardized error handling and status reporting in applications that utilize DMA for data transfers.   
 *      - DMA_STATUS_OK: Indicates that the DMA operation completed successfully without any errors.    
 *      - DMA_STATUS_ERROR: Indicates that an error occurred during the DMA operation, which could be due to various reasons such as invalid parameters, hardware faults, or transfer errors.    
 *      - DMA_STATUS_TIMEOUT: Indicates that the DMA operation timed out, which can occur if the transfer takes longer than expected or if there is a communication issue between the DMA controller and the peripheral or memory.    
 *      - DMA_STATUS_INVALID_CONFIG: Indicates that the DMA configuration provided was invalid, such as incorrect channel settings, unsupported data sizes, or incompatible transfer directions. This status can be used to catch configuration errors before starting a DMA transfer.  
 */
typedef enum    // DMA Status
{
    DMA_STATUS_OK = 0x00U,
    DMA_STATUS_ERROR = 0x01U,
    DMA_STATUS_TIMEOUT = 0x02U,
    DMA_STATUS_INVALID_CONFIG = 0x03U
}DMA_Status_t;


/**
 * 
 * 
 * @brief DMA Channel Enumeration
 * @note This enumeration defines the available DMA channels for the STM32F103xx series. Each channel corresponds to a specific set of peripherals that can be configured for DMA transfers. The enumeration values can be used to select the desired DMA channel when initializing and configuring DMA transfers in the application.
 *      - DMA_CHANNEL1: Corresponds to DMA Channel 1, 
 *                      which can be used for peripherals such as ADC1, USART1_TX, etc.
 *      - DMA_CHANNEL2: Corresponds to DMA Channel 2, 
 *                      which can be used for peripherals such as ADC2, USART1_RX, etc.
 *      - DMA_CHANNEL3: Corresponds to DMA Channel 3, 
 *                      which can be used for peripherals such as SPI1_TX, etc.
 *      - DMA_CHANNEL4: Corresponds to DMA Channel 4, 
 *                      which can be used for peripherals such as SPI1_RX, etc.    
 *      - DMA_CHANNEL5: Corresponds to DMA Channel 5, 
 *                      which can be used for peripherals such as USART2_TX, etc.  
 *      - DMA_CHANNEL6: Corresponds to DMA Channel 6, 
 *                      which can be used for peripherals such as USART2_RX, etc.   
 *      - DMA_CHANNEL7: Corresponds to DMA Channel 7, 
 *                      which can be used for peripherals such as I2C1_TX, etc.
 */
typedef enum
{
    DMA_CHANNEL1 = 0,
    DMA_CHANNEL2,
    DMA_CHANNEL3,
    DMA_CHANNEL4,
    DMA_CHANNEL5,
    DMA_CHANNEL6,
    DMA_CHANNEL7
}DMA_Channel_Name;


/**
 * 
 * 
 * @brief DMA Interrupt Enable Enumeration
 * @note This enumeration defines the available DMA interrupt sources for STM32F103xx DMA channels. The enumeration values can be used to enable interrupt generation during DMA transfer operations, allowing the application to handle transfer completion, half-transfer events, and transfer errors through interrupt service routines.
 *      - DMA_TC_ENABLE: Enables DMA Transfer Complete interrupt generation.
 *      - DMA_HT_ENABLE: Enables DMA Half Transfer interrupt generation.
 *      - DMA_TE_ENABLE: Enables DMA Transfer Error interrupt generation.
 */
typedef enum
{
    DMA_TC_ENABLE = DMA_CCRx_TCIE_MASK,
    DMA_HT_ENABLE = DMA_CCRx_HTIE_MASK,
    DMA_TE_ENABLE = DMA_CCRx_TEIE_MASK 
}DMA_Interrupt_Name;


/**
 * 
 * 
 * @brief DMA Transfer Direction Enumeration
 * @note This enumeration defines the available DMA transfer directions for STM32F103xx DMA channels. The enumeration values can be used to configure the direction of data transfer between peripheral and memory during DMA operations.
 *      - DMA_DIR_PERIPHERAL_TO_MEMORY: Configures DMA transfer direction from peripheral to memory.
 *      - DMA_DIR_MEMORY_TO_PERIPHERAL: Configures DMA transfer direction from memory to peripheral.
 */
typedef enum    // Transfer Direction (bit 4)
{
    DMA_DIR_PERIPHERAL_TO_MEMORY  = (0U << DMA_CCRx_DIR_MASK),
    DMA_DIR_MEMORY_TO_PERIPHERAL  = (1U << DMA_CCRx_DIR_POS)
}DMA_Direction_Name;


/**
 * 
 * 
 * @brief DMA Peripheral Increment Enumeration
 * @note This enumeration defines the peripheral address increment modes for STM32F103xx DMA channels. The enumeration values can be used to configure whether the peripheral address is incremented automatically after each DMA data transfer.
 *      - DMA_PINC_ENABLE: Enables peripheral address increment mode.
 *      - DMA_PINC_DISABLE: Disables peripheral address increment mode.
 */
typedef enum    // Peripheral Increment (bit 6)
{
    DMA_PINC_ENABLE     = (1U << DMA_CCRx_PINC_POS),
    DMA_PINC_DISABLE    = (0U << DMA_CCRx_PINC_POS)
}DMA_PeriIncrement_Name;


/**
 * 
 * 
 * @brief DMA Memory Increment Enumeration
 * @note This enumeration defines the memory address increment modes for STM32F103xx DMA channels. The enumeration values can be used to configure whether the memory address is incremented automatically after each DMA data transfer.
 *      - DMA_MINC_ENABLE: Enables memory address increment mode.
 *      - DMA_MINC_DISABLE: Disables memory address increment mode.
 */
typedef enum    // Memory Increment (bit 7)
{
    DMA_MINC_ENABLE     = (1U << DMA_CCRx_MINC_POS),
    DMA_MINC_DISABLE    = (0U << DMA_CCRx_MINC_POS),
}DMA_MemIncrement_Name;


/**
 * 
 * 
 * @brief DMA Data Size Enumeration
 * @note This enumeration defines the available peripheral and memory data sizes for STM32F103xx DMA channels. The enumeration values can be used to configure the data width for DMA transfers between memory and peripherals.
 *      - DMA_MSIZE_8BIT: Configures memory data size as 8-bit.
 *      - DMA_MSIZE_16BIT: Configures memory data size as 16-bit.
 *      - DMA_MSIZE_32BIT: Configures memory data size as 32-bit.
 *      - DMA_PSIZE_8BIT: Configures peripheral data size as 8-bit.
 *      - DMA_PSIZE_16BIT: Configures peripheral data size as 16-bit.
 *      - DMA_PSIZE_32BIT: Configures peripheral data size as 32-bit.
 */
typedef enum    // Data Size (bits 8-11)
{
    DMA_MSIZE_8BIT  = (0x0U << DMA_CCRx_MSIZE_POS),
    DMA_MSIZE_16BIT = (0x1U << DMA_CCRx_MSIZE_POS),
    DMA_MSIZE_32BIT = (0x2U << DMA_CCRx_MSIZE_POS),
    DMA_PSIZE_8BIT  = (0x0U << DMA_CCRx_PSIZE_POS),
    DMA_PSIZE_16BIT = (0x1U << DMA_CCRx_PSIZE_POS),
    DMA_PSIZE_32BIT = (0x2U << DMA_CCRx_PSIZE_POS)
}DMA_DataSize_Name;


/**
 * 
 * 
 * @brief DMA Priority Level Enumeration
 * @note This enumeration defines the available priority levels for STM32F103xx DMA channels. The enumeration values can be used to configure the arbitration priority of DMA channels when multiple DMA requests occur simultaneously.
 *      - DMA_PL_LOW: Configures low priority level.
 *      - DMA_PL_MEDIUM: Configures medium priority level.
 *      - DMA_PL_HIGH: Configures high priority level.
 *      - DMA_PL_VERY_HIGH: Configures very high priority level.
 */
typedef enum    // DMA Priority Level (bits 12-13)
{
    DMA_PL_LOW        = (0x0U << DMA_CCRx_PL_POS),
    DMA_PL_MEDIUM     = (0x1U << DMA_CCRx_PL_POS),
    DMA_PL_HIGH       = (0x2U << DMA_CCRx_PL_POS),
    DMA_PL_VERY_HIGH  = (0x3U << DMA_CCRx_PL_POS)
}DMA_Priority_Name;


/**
 * 
 * 
 * @brief DMA Mode Enumeration
 * @note This enumeration defines the available operating modes for STM32F103xx DMA channels. The enumeration values can be used to configure memory-to-memory transfer mode and circular transfer mode during DMA operations.
 *      - DMA_MEM2MEM_ENABLE: Enables memory-to-memory transfer mode.
 *      - DMA_MEM2MEM_DISABLE: Disables memory-to-memory transfer mode.
 *      - DMA_CIRCULAR_ENABLE: Enables circular transfer mode.
 *      - DMA_CIRCULAR_DISABLE: Disables circular transfer mode.
 */
typedef enum
{
    DMA_MEM2MEM_ENABLE  = (1U << DMA_CCRx_MEM2MEM_POS),
    DMA_MEM2MEM_DISABLE = (0U << DMA_CCRx_MEM2MEM_POS),
    DMA_CIRCULAR_ENABLE = (1U << DMA_CCRx_CIRC_POS),
    DMA_CIRCULAR_DISABLE= (0U << DMA_CCRx_CIRC_POS)
}DMA_Mode_Name;


/**
 * 
 * 
 * @brief DMA Interrupt Flag Enumeration
 * @note This enumeration defines the available DMA interrupt flag clear bits for STM32F103xx DMA channels. The enumeration values can be used to clear global, transfer complete, half-transfer, and transfer error interrupt flags in the DMA interrupt flag clear register.
 *      - DMA_IT_CG: Clears DMA global interrupt flag.
 *      - DMA_IT_CTC: Clears DMA transfer complete interrupt flag.
 *      - DMA_IT_CHT: Clears DMA half-transfer interrupt flag.
 *      - DMA_IT_CTE: Clears DMA transfer error interrupt flag.
 */
typedef enum
{
    DMA_IT_CG   = (1U << DMA_IFCR_CGIF_OFFSET),
    DMA_IT_CTC  = (1U << DMA_IFCR_CTCIF_OFFSET),
    DMA_IT_CHT  = (1U << DMA_IFCR_CHTIF_OFFSET),
    DMA_IT_CTE  = (1U << DMA_IFCR_CTEIF_OFFSET)
}DMA_IT_FLAG;



/*====================================================================================================================================================================
|                                           DEFINE STRUCTURES
=====================================================================================================================================================================*/
/**
 * @brief DMA Channel Configuration Structure
 * @note This structure is used to configure a specific DMA channel with all necessary parameters for a DMA transfer. It includes settings for the channel number, transfer direction, data size, increment modes, priority level, and addresses for peripheral and memory.
 *      - The channel field specifies which DMA channel to configure (e.g., DMA_CHANNEL1, DMA_CHANNEL2, etc.).
 *      - The direction field specifies the direction of data transfer (peripheral to memory or memory to peripheral).
 *      - The data_size field specifies the size of data units for both peripheral and memory (e.g., 8-bit, 16-bit, or 32-bit).
 *      - The mem_increment and peri_increment fields specify whether the memory and peripheral addresses should be incremented after each data transfer, which is useful for transferring blocks of data.
 *      - - The mode field specifies the DMA operation mode, such as normal mode or circular mode.
 *      - The priority field specifies the priority level of the DMA channel, which determines how the  DMA controller arbitrates between multiple channels when they are active simultaneously.
 *      - The peripheral_address and memory_address fields specify the starting addresses for the peripheral and memory involved in the DMA transfer,
 * and the transfer_length field specifies the total number of data units to be transferred. This comprehensive configuration structure allows for flexible and efficient setup of DMA transfers in various application scenarios.
 *      - This structure can be used in the DMA_Init function to initialize a DMA channel with the specified configuration settings, ensuring that all necessary parameters are set correctly before starting the DMA transfer.
 */
typedef struct
{
    DMA_Channel_Name channel;
    DMA_Direction_Name direction;
    DMA_DataSize_Name data_size;
    DMA_MemIncrement_Name mem_increment;
    DMA_PeriIncrement_Name peri_increment;
    DMA_Mode_Name mode;
    DMA_Priority_Name priority;
    uint32_t peripheral_address;
    uint32_t memory_address;
    uint32_t transfer_length;
}DMA_ChannelConfig_t;


/**
 * 
 * 
 * @brief DMA Configuration Structure
 * @note This structure is used to configure the overall DMA settings, which may include multiple channel configurations. 
 *       It can be extended in the future to include additional global DMA settings if needed.
 */
typedef struct
{
    DMA_ChannelConfig_t *channel;
    uint8_t num_channels;
}DMA_Config_t;

/*====================================================================================================================================================================
|                             Inline Functions
 =====================================================================================================================================================================*/
/*<---DMA Init/DeInit--->*/
DMA_Status_t DMA_Init(DMA_TypeDef *DMAx, DMA_Config_t *config);
void DMA_DeInit(DMA_TypeDef *DMAx, DMA_Channel_TypeDef *DMAy_Channelx, DMA_Channel_Name chl);

/*<---DMA Enable/Disable--->*/
void DMA_Enable(DMA_Channel_TypeDef *DMAy_Channelx);
void DMA_Disable(DMA_Channel_TypeDef *DMAy_Channelx);

/*<---DMA Configuration--->*/
void DMA_ConfigPeripheralAddress(DMA_Channel_TypeDef *DMAy_Channelx, uint32_t peri_addr);
void DMA_ConfigMemoryAddress(DMA_Channel_TypeDef *DMAy_Channelx, uint32_t mem_addr);
void DMA_ConfigDataSize(DMA_Channel_TypeDef *DMAy_Channelx, DMA_DataSize_Name data_size);
void DMA_ConfigDirection(DMA_Channel_TypeDef *DMAy_Channelx, DMA_Direction_Name dir);
void DMA_ConfigTransferLength(DMA_Channel_TypeDef *DMAy_Channelx, uint32_t length);
void DMA_ConfigMemoryIncrement(DMA_Channel_TypeDef *DMAy_Channelx, DMA_MemIncrement_Name inc);
void DMA_ConfigPeripheralIncrement(DMA_Channel_TypeDef *DMAy_Channelx, DMA_PeriIncrement_Name inc);
void DMA_ConfigMode(DMA_Channel_TypeDef *DMAy_Channely, DMA_Mode_Name mode);
void DMA_ConfigPriority(DMA_Channel_TypeDef *DMAy_Channelx, DMA_Priority_Name prior);

/*<---DMA Start/Stop/Rload--->*/
void DMA_Start(DMA_Channel_TypeDef *DMAy_Channelx, uint32_t nums_trans);
void DMA_Stop(DMA_Channel_TypeDef *DMAy_Channelx);
void DMA_Reload(DMA_Channel_TypeDef *DMAy_Channelx, uint32_t mem_addr, uint16_t len);

/*<---DMA Interrupts--->*/
void DMA_EnableInterrupt(DMA_Channel_TypeDef *DMAy_Channelx, DMA_Interrupt_Name it);
void DMA_DisableInterrupt(DMA_Channel_TypeDef *DMAy_Channelx, DMA_Interrupt_Name it);

/*<---DMA Get Flag--->*/
uint32_t DMA_GetFlagStatus(DMA_TypeDef *DMAx, uint32_t flag);
uint32_t DMA_GetInterruptStatusFlag(DMA_TypeDef *DMAx,DMA_Channel_Name channel, DMA_IT_FLAG flag);

/*<---DMA Clear Flag--->*/
void DMA_InterruptFlagClear(DMA_TypeDef *DMAx, DMA_Channel_TypeDef *DMAy_Channelx, uint32_t flag);
void DMA_ClearITPendingBit(DMA_TypeDef *DMAx, DMA_Channel_Name channel, DMA_IT_FLAG flag);

/*<---DMA Remaining Data--->*/
uint32_t DMA_GetRemainingData(DMA_Channel_TypeDef *DMAy_Channelx);

/*<---DMA Check Status--->*/
uint32_t DMA_IsEnable(DMA_Channel_TypeDef *DMAy_Channelx);

#endif  /*  STM32F103xx_DMA_H  */