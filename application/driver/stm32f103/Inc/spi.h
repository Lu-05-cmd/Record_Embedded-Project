/**************************************************************************************************
 * @file    spi.h
 * @brief   Bare-metal SPI driver for STM32F103xx microcontrollers
 *
 * @details
 * This file provides:
 *  - SPI register definitions
 *  - SPI configuration APIs
 *  - Blocking transmit/receive functions
 *  - Interrupt and DMA support
 *  - SPI status and flag management
 *
 * @author  lu-05-cmd
 * @date    2026-02-23
 * @version 0.1.0
 *
 * @target  STM32F103xx Series
 *
 * @note
 *  - Developed for embedded systems learning and practice
 *  - Fully register-level programming (Bare-metal)
 *  - Based on STM32F10x Reference Manual (RM0008)
 *  - Supports polling, interrupt, and DMA communication modes
 *
 **************************************************************************************************/



#ifndef STM32F103XX_SPI_H
#define STM32F103XX_SPI_H

/*INCLUDE============================================================================================================================================================*/
#include <stdint.h>

/*====================================================================================================================================================================
 |                            Configuration Serial Peripheral Interface (SPI) 
 =====================================================================================================================================================================*/
typedef struct {
    volatile uint32_t CR1;        // Control Register                  - offset: 0x00
    volatile uint32_t CR2;        // Control Register                  - offset: 0x04
    volatile uint32_t SR;         // Status Register                   - offset: 0x08
    volatile uint32_t DR;         // Data Register                     - offset: 0x0C
    volatile uint32_t CRCPR;      // CRC Polynomial Register           - offset: 0x10
    volatile uint32_t RXCRCR;     // RX CRC Register                   - offset: 0x14
    volatile uint32_t TXCRCR;     // TX CRC Register                   - offset: 0x18
    volatile uint32_t I2SCFGR;    // I2S Configuration Register        - offset: 0x1C
    volatile uint32_t I2SPR;      // I2S Prescaler Register            - offset: 0x20
} SPI_TypeDef;

/*====================================================================================================================================================================
|                             SPI BASE ADDRESS
=====================================================================================================================================================================*/
#define SPI1 ((SPI_TypeDef *)0x40013000UL)
#define SPI2 ((SPI_TypeDef *)0x40003800UL)

/*====================================================================================================================================================================
 |                                           Mode Define
 =====================================================================================================================================================================*/
/* SPI Control Register 1*/
#define SPI_CR1_CPHA_POS        (0U)
#define SPI_CR1_CPOL_POS        (1U)
#define SPI_CR1_MSTR_POS        (2U)
#define SPI_CR1_BR_POS          (3U)
#define SPI_CR1_SPE_POS         (6U)
#define SPI_CR1_LSBFIRST_POS    (7U)
#define SPI_CR1_SSI_POS         (8U)
#define SPI_CR1_SSM_POS         (9U)
#define SPI_CR1_RXONLY_POS      (10U)
#define SPI_CR1_DFF_POS         (11U)
#define SPI_CR1_CRCNEXT_POS     (12U)
#define SPI_CR1_CRCEN_POS       (13U)
#define SPI_CR1_BIDIOE_POS      (14U)
#define SPI_CR1_BIDIMODE_POS    (15U)
/*SPI Control Register 2*/
#define SPI_CR2_RXDMAEN_POS     (0U)
#define SPI_CR2_TXDMAEN_POS     (1U)
#define SPI_CR2_SSOE_POS        (2U)
#define SPI_CR2_ERRIE_POS       (5U)
#define SPI_CR2_RXNEIE_POS      (6U)
#define SPI_CR2_TXEIE_POS       (7U)
/*SPI Status Register*/
#define SPI_SR_RXNE_POS         (0U)
#define SPI_SR_TXE_POS          (1U)
#define SPI_SR_CHSIDE_POS       (2U)
#define SPI_SR_UDR_POS          (3U)
#define SPI_SR_CRCERR_POS       (4U)
#define SPI_SR_MODF_POS         (5U)
#define SPI_SR_OVR_POS          (6U)
#define SPI_SR_BSY_POS          (7U)


/*CR1_MASK*/
#define SPI_CR1_CPHA_MASK       (1U << SPI_CR1_CPHA_POS)
#define SPI_CR1_CPOL_MASK       (1U << SPI_CR1_CPOL_POS)
#define SPI_CR1_MSTR_MASK       (1U << SPI_CR1_MSTR_POS)
#define SPI_CR1_BR_MASK         (0x7U << SPI_CR1_BR_POS) 
#define SPI_CR1_SPE_MASK        (1U << SPI_CR1_SPE_POS)
#define SPI_CR1_LSBFIRST_MASK   (1U << SPI_CR1_LSBFIRST_POS)
#define SPI_CR1_SSI_MASK        (1U << SPI_CR1_SSI_POS)
#define SPI_CR1_SSM_MASK        (1U << SPI_CR1_SSM_POS)
#define SPI_CR1_RXONLY_MASK     (1U << SPI_CR1_RXONLY_POS)
#define SPI_CR1_DFF_MASK        (1U << SPI_CR1_DFF_POS)
#define SPI_CR1_CRCNEXT_MASK    (1U << SPI_CR1_CRCNEXT_POS)
#define SPI_CR1_CRCEN_MASK      (1U << SPI_CR1_CRCEN_POS)
#define SPI_CR1_BIDIOE_MASK     (1U << SPI_CR1_BIDIOE_POS)
#define SPI_CR1_BIDIMODE_MASK   (1U << SPI_CR1_BIDIMODE_POS)
/*CR2_MASK*/
#define SPI_CR2_RXDMAEN_MASK    (1U << SPI_CR2_RXDMAEN_POS)
#define SPI_CR2_TXDMAEN_MASK    (1U << SPI_CR2_TXDMAEN_POS)
#define SPI_CR2_SSOE_MASK       (1U << SPI_CR2_SSOE_POS)
#define SPI_CR2_ERRIE_MASK      (1U << SPI_CR2_ERRIE_POS)
#define SPI_CR2_RXNEIE_MASK     (1U << SPI_CR2_RXNEIE_POS)
#define SPI_CR2_TXEIE_MASK      (1U << SPI_CR2_TXEIE_POS)
/*SR_MASK*/
#define SPI_SR_RXNE_MASK        (1U << SPI_SR_RXNE_POS)
#define SPI_SR_TXE_MASK         (1U << SPI_SR_TXE_POS)
#define SPI_SR_CHSIDE_MASK      (1U << SPI_SR_CHSIDE_POS)
#define SPI_SR_UDR_MASK         (1U << SPI_SR_UDR_POS)
#define SPI_SR_CRCERR_MASK      (1U << SPI_SR_CRCERR_POS)
#define SPI_SR_MODF_MASK        (1U << SPI_SR_MODF_POS)
#define SPI_SR_OVR_MASK         (1U << SPI_SR_OVR_POS)
#define SPI_SR_BSY_MASK         (1U << SPI_SR_BSY_POS)

/*====================================================================================================================================================================
 |                                           DEFINE ENUMERATIONS
 =====================================================================================================================================================================*/

/**
 * 
 * 
 * @brief SPI Status Enumeration
 * @note  This enumeration defines the possible status codes that can be returned by SPI functions. It includes:
 *          - `SPI_OK`: Operation completed successfully.
 *          - `SPI_ERROR`: An error occurred during the SPI operation.
 *          - `SPI_BUSY`: The SPI peripheral is currently busy and cannot perform the requested operation.
 *          - `SPI_TIMEOUT`: The SPI operation timed out while waiting for a response or completion.
 *          - `SPI_INVALID_PARAM`: An invalid parameter was passed to the SPI function.
 *          - `SPI_NOT_INITIALIZED`: The SPI peripheral has not been initialized before use.
 *          - `SPI_OVERRUN`: An overrun error occurred during SPI communication.
 *          - `SPI_MODE_FAULT`: A mode fault error occurred, typically when the SPI is configured as a slave and the master fails to provide the correct clock signal.
 */
typedef enum
{
    SPI_OK = 0U,
    SPI_ERROR,
    SPI_BUSY,
    SPI_TIMEOUT,
    SPI_INVALID_PARAM,
    SPI_NOT_INITIALIZED,
    SPI_OVERRUN,
    SPI_MODE_FAULT
} SPI_Status_t;

/**
 * @brief SPI functional state
 * @note  Defines the functional state values used to enable or disable SPI features.
 *          - `DISABLE`: Disable the SPI feature.
 *          - `ENABLE`: Enable the SPI feature.
 */
typedef enum
{
    DISABLE = 0U,
    ENABLE = 1U
}SPI_State_t;

/**
 * @brief SPI Clock Phase Configuration
 * @note  Configures the clock transition used for data capture. This field corresponds to the CPHA bit in the SPI_CR1 register.
 */
typedef enum
{
    SPI_CPHA_FIRST_CLOCK_TRANSITION  = 0U,
    SPI_CPHA_SECOND_CLOCK_TRANSITION = SPI_CR1_CPHA_MASK
} SPI_ClockPhase_t;


/**
 * @brief SPI Clock Polarity Configuration
 * @note  Configures the idle state of the serial clock. This field corresponds to the CPOL bit in the SPI_CR1 register.
 */
typedef enum
{
    SPI_CPOL_CK_TO_0 = 0U,
    SPI_CPOL_CK_TO_1 = SPI_CR1_CPOL_MASK
} SPI_ClockPolarity_t;


/**
 * @brief SPI Master/Slave Configuration
 * @note  Selects SPI operating mode. This field corresponds to the MSTR bit in the SPI_CR1 register.
 *
 *         Available modes:
 *          - Slave mode
 *          - Master mode
 */
typedef enum
{
    SPI_SLAVE_CONFIG  = 0U,
    SPI_MASTER_CONFIG = SPI_CR1_MSTR_MASK
} SPI_SlaveMaster_Select_t;


/**
 * @brief SPI Baud Rate Prescaler Configuration
 * @note  Configures the SPI serial clock baud rate.
 *         This field corresponds to the BR[2:0] bits in the SPI_CR1 register.
 
 *         SPI clock is derived from APB clock according to:
 *              SPI_CLK = PCLK / Prescaler
 */
typedef enum
{
    SPI_PCLK_BY_2   = 0x0U,
    SPI_PCLK_BY_4   = 0x1U,
    SPI_PCLK_BY_8   = 0x2U,
    SPI_PCLK_BY_16  = 0x3U,
    SPI_PCLK_BY_32  = 0x4U,
    SPI_PCLK_BY_64  = 0x5U,
    SPI_PCLK_BY_128 = 0x6U,
    SPI_PCLK_BY_256 = 0x7U
} SPI_BaudRate_t;


/**
 * @brief SPI Frame Format Configuration
 * @note  Configures the bit transmission order. This field corresponds to the LSBFIRST bit in the SPI_CR1 register.
 *          - MSB transmitted first
 *          - LSB transmitted first
 */
typedef enum
{
    SPI_MSB_FIRST = 0U,
    SPI_LSB_FIRST = SPI_CR1_LSBFIRST_MASK
} SPI_FrameFormat_t;


/**
 * @brief SPI Internal Slave Select Configuration
 * @note  Controls the internal NSS signal level when software slave management is enabled.
 *         This field corresponds to the SSI bit in the SPI_CR1 register.
 */
typedef enum
{
    SPI_SSI_LOW  = 0U,
    SPI_SSI_HIGH = SPI_CR1_SSI_MASK
} SPI_SSI_t;


/**
 * @brief SPI NSS Output Configuration
 * @note  Enables or disables automatic NSS output in master mode.
 *         This field corresponds to the SSOE bit in the SPI_CR2 register.
 */
typedef enum
{
    SPI_SSOE_DISABLE = 0U,
    SPI_SSOE_ENABLE  = SPI_CR2_SSOE_MASK
} SPI_NSS_Output_t;


/**
 * @brief SPI NSS Management Mode
 * @note  Selects NSS signal management method. This field corresponds to the SSM bit in the SPI_CR1 register.
 *          - Hardware management: NSS pin is managed by hardware and must be connected to the slave select line. In master mode, NSS pin is driven low when the SPI is enabled and high when disabled.
 *          - Software management: NSS pin is not used for hardware slave selection. The internal SSI bit is used to control the NSS signal level when software slave management is enabled.
 */
    typedef enum
    {
        SPI_NSS_HARDWARE_MANAGEMENT = 0U,
        SPI_NSS_SOFTWARE_MANAGEMENT = SPI_CR1_SSM_MASK
    } SPI_NSSMode_t;


/**
 * @brief SPI Data Frame Format Configuration
 * @note  Configures SPI data frame size. This field corresponds to the DFF bit in the SPI_CR1 register.
 *          - 8-bit data frame
 *          - 16-bit data frame
 */
typedef enum
{
    SPI_DFF_8_BIT  = 0U,
    SPI_DFF_16_BIT = SPI_CR1_DFF_MASK
} SPI_DataFrame_t;


/**
 * @brief SPI CRC Calculation Configuration
 * @note  Enables or disables hardware CRC calculation.
 *         This field corresponds to the CRCEN bit in the SPI_CR1 register.
 */
typedef enum
{
    SPI_CRC_DISABLE = 0U,
    SPI_CRC_ENABLE  = SPI_CR1_CRCEN_MASK
} SPI_CRC_Caculator_t;


/**
 * @brief SPI RX Buffer DMA Configuration
 * @note  Enables or disables DMA requests for RX buffer.
 *         This field corresponds to the RXDMAEN bit in the SPI_CR2 register.
 */
typedef enum
{
    SPI_RX_BUFFER_DMA_DISABLE = 0U,
    SPI_RX_BUFFER_DMA_ENABLE  = SPI_CR2_RXDMAEN_MASK
} SPI_RxBufferDMA_t;


/**
 * @brief SPI TX Buffer DMA Configuration
 * @note  Enables or disables DMA requests for TX buffer.
 *         This field corresponds to the TXDMAEN bit in the SPI_CR2 register.
 */
typedef enum
{
    SPI_TX_BUFFER_DMA_DISABLE = 0U,
    SPI_TX_BUFFER_DMA_ENABLE  = SPI_CR2_TXDMAEN_MASK
} SPI_TxBufferDMA_t;


/**
 * @brief SPI Error Interrupt Configuration
 * @note  Enables or disables SPI error interrupt generation.
 *         This field corresponds to the ERRIE bit in the SPI_CR2 register.
 */
typedef enum
{
    SPI_ERROR_INTERRUPT_DISABLE = 0U,
    SPI_ERROR_INTERRUPT_ENABLE  = SPI_CR2_ERRIE_MASK
} SPI_ErrorInterrupt_t;


/**
 * @brief SPI RXNE Interrupt Configuration
 * @note  Enables or disables RX buffer not empty interrupt generation.
 *         This field corresponds to the RXNEIE bit in the SPI_CR2 register.
 */
typedef enum
{
    SPI_RXNE_INTERRUPT_DISABLE = 0U,
    SPI_RXNE_INTERRUPT_ENABLE  = SPI_CR2_RXNEIE_MASK
} SPI_RXNE_Interrupt_t;


/**
 * @brief SPI TXE Interrupt Configuration
 * @note  Enables or disables TX buffer empty interrupt generation.
 *         This field corresponds to the TXEIE bit in the SPI_CR2 register.
 */
typedef enum
{
    SPI_TXE_INTERRUPT_DISABLE = 0U,
    SPI_TXE_INTERRUPT_ENABLE  = SPI_CR2_TXEIE_MASK
} SPI_TXE_Interrupt_t;


/**
 * @brief SPI Communication Direction Configuration
 * @note  Configures SPI communication mode and data direction. This field corresponds to the BIDIMODE, BIDIOE, and RXONLY bits in the SPI_CR1 register.
 *          - 2-line full duplex: separate lines for transmit and receive, both enabled.    
 *          - 2-line receive only: separate lines for transmit and receive, but transmit line is disabled (RXONLY = 1).
 *          - 1-line transmit only: single bidirectional line used for transmission, configured as output (BIDIMODE = 1, BIDIOE = 1).
 *          - 1-line receive only: single bidirectional line used for reception, configured as input (BIDIMODE = 1, BIDIOE = 0).
 */
typedef enum
{
    /*================ 2-Line mode (BIDIMODE = 0) ================*/

    SPI_DIR_2LINE_FULL_DUPLEX = 0U,

    SPI_DIR_2LINE_RX_ONLY = SPI_CR1_RXONLY_MASK,

    /*================ 1-Line mode (BIDIMODE = 1) ================*/

    SPI_DIR_1LINE_TX_ONLY =
        SPI_CR1_BIDIMODE_MASK | SPI_CR1_BIDIOE_MASK,

    SPI_DIR_1LINE_RX_ONLY =
        SPI_CR1_BIDIMODE_MASK

} SPI_DirectionMode_t;


/**
 * @brief SPI Status Flag Definitions
 * @note  Defines SPI status flags available in the SPI_SR register.
 *          - BSY     : Busy flag
 *          - OVR     : Overrun flag
 *          - MODF    : Mode fault flag
 *          - CRCERR  : CRC error flag
 *          - UDR     : Underrun flag
 *          - CHSIDE  : Channel side flag
 *          - TXE     : Transmit buffer empty flag
 *          - RXNE    : Receive buffer not empty flag
 */
typedef enum
{
    SPI_BSY_FLAG    = SPI_SR_BSY_MASK,
    SPI_OVR_FLAG    = SPI_SR_OVR_MASK,
    SPI_MODF_FLAG   = SPI_SR_MODF_MASK,
    SPI_CRCERR_FLAG = SPI_SR_CRCERR_MASK,
    SPI_UDR_FLAG    = SPI_SR_UDR_MASK,
    SPI_CHSIDE_FLAG = SPI_SR_CHSIDE_MASK,
    SPI_TXE_FLAG    = SPI_SR_TXE_MASK,
    SPI_RXNE_FLAG   = SPI_SR_RXNE_MASK
} SPI_Flag_t;
/*====================================================================================================================================================================
|                                           DEFINE STRUCTURES
=====================================================================================================================================================================*/
/**
 * 
 * 
 * @brief   Define a structure for SPI Clock config
 * @note    This structure is used to configuration clock phase and clock polariry for SPI peripheral.
 *          - The `clk_phase` : field specifies the clock phase for SPI communication, which determines when data is captured and propagated in relation to the clock signal (e.g., first clock transition or second clock transition).
 *          - The `clk_polarity` : field specifies the clock polarity for SPI communication, which determines the idle state of the clock signal (e.g., CK to 0 when idle or CK to 1 when idle).
 */
typedef struct
{
    SPI_ClockPhase_t clk_phase;
    SPI_ClockPolarity_t clk_polarity;
}SPI_Clock_Config_t;

/**
 * 
 * 
 * @brief   Define a structure for SPI interrupt config
 * @note    This structure is used to configuration enable interrupt for spi.
 *        - The `rxne_interrupt`  : field specifies whether the RXNE (Receive Buffer Not Empty) interrupt is enabled or disabled for the SPI peripheral. 
 *                                  When enabled, an interrupt request will be generated whenever the RXNE flag is set, indicating that there is data in the receive buffer that can be read.
 *        - The `txe_interrupt`   : field specifies whether the TXE (Transmit Buffer Empty) interrupt is enabled or disabled for the SPI peripheral. 
 *                                  When enabled, an interrupt request will be generated whenever the TXE flag is set, indicating that the transmit buffer is empty and new data can be written to it for transmission.
 *        - The `error_interrupt` : field specifies whether the error interrupt is enabled or disabled for the SPI peripheral. 
 *                                  When enabled, an interrupt request will be generated whenever an error condition occurs (e.g., CRC error, overrun error, mode fault, etc.) that requires attention from the software.
 */
typedef struct 
{
    SPI_RXNE_Interrupt_t rxne_interrupt;
    SPI_TXE_Interrupt_t  txe_interrupt;
    SPI_ErrorInterrupt_t error_interrupt;
}SPI_InterruptConfig_t;

/**
 * 
 * 
 * @brief   Define a structure for SPI configuration
 * @note    This structure is used to configure the SPI peripheral, including settings for the control registers CR1 and CR2.
 *          - The `SPI_CR1_Config_t` : structure contains fields for device mode (master or slave), baud rate, frame format, software slave management configuration, data frame size, and direction mode.
 *          - The `SPI_CR2_Config_t` : structure contains fields for enabling or disabling DMA for transmission and reception, as well as enabling or disabling error, RXNE, and TXE interrupts.
 *          - The `SPI_Config_t`     : structure combines both CR1 and CR2 configurations into a single structure for easier initialization of the SPI peripheral.
 */
typedef struct
{

    SPI_Clock_Config_t clock_config;            /** @brief  Clock phase and clock polarity.
                                                    @note   This bit should not be changed while the SPI is enabled.
                                                    */

    SPI_SlaveMaster_Select_t device_mode;       /** @brief  Master or Slave configuration.
                                                    @note   This enum should not be changed while the SPI is enabled.
                                                    */
                                                   
    SPI_BaudRate_t baudrate;                    /** @brief  Baud rate control for SPI communication.  
                                                    @note   This enum should not be changed while the SPI is enabled.
                                                    */

    SPI_FrameFormat_t format;                   /** @brief Frame format (MSB first or LSB first).  
                                                    @note   This enum is not used in I2S mode.
                                                    */

    SPI_NSSMode_t nss_mode;                     /** @brief  Software slave management configuration.
                                                    @note   This enum should not be changed while the SPI is enabled.
                                                            When:  
                                                                1. Software NSS management (SSM = 1)
                                                                    - The external NSS pin remains free for other application uses.
                                                                2. Hardware NSS management (SSM = 0)
                                                                    - NSS output enable (SSM = 0, SSOE = 1):
                                                                        + This configuration is used only when the device operates in master mode.
                                                                        + The NSS sign is driven low when the master starts the communication and is kept low until the SPI is disabled.
                                                                    - NSS output disable (SSM = 0, SSOE = 0):
                                                                        + This configuration allows multimaster capability for devices operating in master mode
                                                                        + The NSS pin acts as a classical NSS input : the slave is selected when NSS is low and deselected when NSS high. 
                                                    */

    SPI_SSI_t ssi;                              /** @brief  Internal slave select
                                                    @note   This bit has an effect only when the SSM bit is set.
                                                            The value of this bit is forced onto the NSS pin and the IO value of the NSS pin is ignored.
                                                    */

    SPI_DataFrame_t datasize;                   /** @brief  Data frame size (8-bit or 16-bit).
                                                    @note   This enum should not be changed while the SPI is enabled.
                                                    */

    SPI_CRC_Caculator_t crc_calculation;        /** @brief  CRC calculation enable/disable.
                                                    @note   This enum is be write only when SPI is disable.
                                                            It's not used in I2S mode.
                                                    */

    SPI_DirectionMode_t direction;              /** @brief  Direction mode (full duplex, half duplex, or simplex).  
                                                    @note   This enum is not used in I2S mode.
                                                            FULL DUPLEX : simultaneous transmit and receive.
                                                            RX_ONLY: only receive (MOSI ignored). 
                                                            1LINE: half-duplex (shared data line).
                                                    */

}SPI_ConfigCR1_t;


/**
 * 
 * 
 * @brief Define a structure for SPI configuration
 * @note   This structure is used to configure the SPI peripheral, specifically for settings related to the control register CR2.
 *          - The `tx_dma_enable`: field specifies whether the DMA for transmission is enabled or disabled for the SPI peripheral.
 *          - The `rx_dma_enable`: field specifies whether the DMA for reception is enabled or disabled for the SPI peripheral.  
 *          - The `nss_output`   : field specifies whether the NSS output is enabled or disabled for the SPI peripheral. 
 *          - The `itr_config`   : field is a nested structure of type `SPI_InterruptConfig_t` that contains settings for enabling or disabling specific SPI interrupts, such as RXNE, TXE, and error interrupts. 
 *      
 */
typedef struct
{
    SPI_TxBufferDMA_t tx_dma_enable;            /** @brief  Tx buffer DMA enable/disable.
                                                    */

    SPI_RxBufferDMA_t rx_dma_enable;            /** @brief  Rx buffer DMA enable/disable.
                                                    */

    SPI_NSS_Output_t nss_output;                /** @brief  SS output enable
                                                    @note   This bit is not used in I2S mode
                                                    */

    SPI_InterruptConfig_t itr_config;           /** @brief  Error interrupt enable/masked.
                                                            RX not empty and TX empty interrupt enable/maked.

                                                    @note   This bit controls the generation of an interrupt when an error condition occurs (CRCERR, OVR, MODF in SPI mode and UDR, OVR in I2S mode).
                                                            RXNE interrupt not mask, used to generation an interrupt request when the RXNE flag is set.
                                                            TXE interruptnot masked, used to gểnate an interrupt request when the TXE flag is set.
                                                    */
}SPI_ConfigCR2_t;

/**
 * 
 * 
 * @brief   Define a structure SPI configuration
 * @note    This structure is contain SPU_CR1_Config_t typedef structure and SPI_CR2_Config_t typedef structure.
 *      - The `SPI_CR1_Config_t` : structure contains fields for device mode (master or slave), baud rate, frame format, software slave management configuration, data frame size, and direction mode.  
 *      - The `SPI_CR2_Config_t` : structure contains fields for enabling or disabling DMA for transmission and reception, as well as enabling or disabling error, RXNE, and TXE interrupts. 
 */
typedef struct
{
    SPI_ConfigCR1_t cr1_config;

    SPI_ConfigCR2_t cr2_config;
}SPI_Config_t;



/*====================================================================================================================================================================
|                             Inline Functions
 =====================================================================================================================================================================*/
/*<---SPI Init/DeInit--->*/
SPI_Status_t SPI_Init(SPI_TypeDef *SPIx, SPI_Config_t *config);
SPI_Status_t SPI_DeInit(SPI_TypeDef *SPIx);

/*<---SPI Enable/Disable--->*/
void SPI_Enable(SPI_TypeDef *SPIx);
SPI_Status_t SPI_Disable(SPI_TypeDef *SPIx, uint32_t timeout);

/*<---SPI Transmit/Receive Polling--->*/
SPI_Status_t SPI_Transmit_Polling(SPI_TypeDef *SPIx, const uint16_t *tx_buffer, uint32_t len);
SPI_Status_t SPI_Receive_Polling(SPI_TypeDef *SPIx, uint16_t *rx_buffer, uint32_t len);
SPI_Status_t SPI_TransmitReceive_Polling(SPI_TypeDef *SPIx, const uint16_t *tx_buffer, uint16_t *rx_buffer, uint32_t len);

/*<---SPI Interrupt Configuration--->*/
void SPI_ConfigInterrupt(SPI_TypeDef *SPIx, SPI_InterruptConfig_t *itr_type);
SPI_Status_t SPI_Transmit_IT(SPI_TypeDef *SPIx, const uint16_t *tx_buffer, uint32_t len);
SPI_Status_t SPI_Receive_IT(SPI_TypeDef *SPIx, uint16_t *rx_buffer, uint32_t len);
SPI_Status_t SPI_TransmitReceive_IT(SPI_TypeDef *SPIx, const uint16_t *tx_buffer, uint16_t *rx_buffer, uint32_t len);

/*<---SPI DMA Configuration--->*/
void SPI_EnableDMA_Tx(SPI_TypeDef *SPIx);
void SPI_EnableDMA_Rx(SPI_TypeDef *SPIx);

/*<---SPI Check Status--->*/
uint8_t SPI_IsBusy(SPI_TypeDef *SPIx);

/*<---SPI Get/Clear Flag--->*/
uint8_t SPI_GetFlagStatus(SPI_TypeDef *SPIx, SPI_Flag_t flag);
void SPI_ClearOVRFlag(SPI_TypeDef *SPIx);
void SPI_ClearCRCErrorFlag(SPI_TypeDef *SPIx);
void SPI_ClearUDRFlag(SPI_TypeDef *SPIx);

/*<---SPI Utility Functions--->*/
void SPI_FlushRx(SPI_TypeDef *SPIx);

/*<---SPI Set DataSize--->*/
void SPI_SetDataSize(SPI_TypeDef *SPIx, SPI_DataFrame_t frame);

/*<---SPI Set CRC Polynomial--->*/
void SPI_SetCRCPolynomial(SPI_TypeDef *SPIx, uint16_t poly);




#endif  /*  STM32F103XX_SPI_H  */