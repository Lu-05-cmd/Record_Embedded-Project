/**************************************************************************************************
 * @file    i2c.h
 * @brief   Bare-metal I2C driver for STM32F103xx microcontrollers
 *
 * @details
 * This file provides:
 *  - I2C register definitions
 *  - I2C peripheral configuration APIs
 *  - Master transmit/receive communication
 *  - Interrupt and DMA support
 *  - Memory read/write operations
 *  - Bus scanning and recovery functions
 *
 * @author  lu-05-cmd
 * @date    2025-2-28
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
#ifndef STM32F103xx_I2C_H
#define STM32F103xx_I2C_H

/*INCLUDE============================================================================================================================================================*/
#include <stdint.h>

/*====================================================================================================================================================================
 |                            Configuration Inter-Integrated Circuit (I2C) 
 =====================================================================================================================================================================*/
typedef struct
{
    volatile uint32_t CR1;          // Control Register 1           - offset: 0x00
    volatile uint32_t CR2;          // Control Register 2           - offset: 0x04
    volatile uint32_t OAR1;         // Own Address Register 1       - offset: 0x08
    volatile uint32_t OAR2;         // Own Address Register 2       - offset: 0x0C
    volatile uint32_t DR;           // Data Register                - offset: 0x10
    volatile uint32_t SR1;          // Status Register 1            - offset: 0x14
    volatile uint32_t SR2;          // Status Register 2            - offset: 0x18
    volatile uint32_t CCR;          // Clock Control Register       - offset: 0x1C
    volatile uint32_t TRISE;        // TRISE Register               - offset: 0x20
}I2C_TypeDef;

/*====================================================================================================================================================================
|                             I2C BASE ADDRESS
*====================================================================================================================================================================*/
#define I2C2     ((I2C_TypeDef *)0x40005800UL)
#define I2C1     ((I2C_TypeDef *)0x40005400UL)

/*====================================================================================================================================================================
|                             Register Bit Position Defines
=====================================================================================================================================================================*/

/* CR1 Register - Position Defines */
#define I2C_CR1_PE_POS                (0U)      
#define I2C_CR1_SMBUS_POS             (1U)       
#define I2C_CR1_SMBTYPE_POS           (3U)       
#define I2C_CR1_ENARP_POS             (4U)       
#define I2C_CR1_ENPEC_POS             (5U)       
#define I2C_CR1_ENGC_POS              (6U)
#define I2C_CR1_NOSTRETCH_POS         (7U)
#define I2C_CR1_START_POS             (8U)
#define I2C_CR1_STOP_POS              (9U)
#define I2C_CR1_ACK_POS               (10U)
#define I2C_CR1_POS_POS               (11U)
#define I2C_CR1_PEC_POS               (12U)
#define I2C_CR1_ALERT_POS             (13U)

/* CR2 Register - Position Defines */
#define I2C_CR2_FREQ_POS              (0U)
#define I2C_CR2_ITERREN_POS           (8U)
#define I2C_CR2_ITEVTEN_POS           (9U)
#define I2C_CR2_ITBUFEN_POS           (10U)

/* OAR1 Register - Position Defines */
#define I2C_OAR1_ADD0_POS             (0U)
#define I2C_OAR1_ADD_POS              (1U)
#define I2C_OAR1_ADD8_POS             (8U)
#define I2C_OAR1_ADD9_POS             (9U)
#define I2C_OAR1_BIT14_POS            (14U)
#define I2C_OAR1_ADDMODE_POS          (15U)

/* OAR2 Register - Position Defines */
#define I2C_OAR2_ENDUAL_POS           (0U)
#define I2C_OAR2_ADD2_POS             (1U)

/* DR Register - Position Defines */
#define I2C_DR_DR_POS                 (0U)

/* SR1 Register - Position Defines */
#define I2C_SR1_SB_POS                (0U) 
#define I2C_SR1_ADDR_POS              (1U) 
#define I2C_SR1_BTF_POS               (2U) 
#define I2C_SR1_ADD10_POS             (3U) 
#define I2C_SR1_STOPF_POS             (4U) 
#define I2C_SR1_RxNE_POS              (6U)  
#define I2C_SR1_TxE_POS               (7U)  
#define I2C_SR1_BERR_POS              (8U) 
#define I2C_SR1_ARLO_POS              (9U) 
#define I2C_SR1_AF_POS                (10U)
#define I2C_SR1_OVR_POS               (11U)
#define I2C_SR1_PECERR_POS            (12U)
#define I2C_SR1_TIMEOUT_POS           (14U) 

/* SR2 Register - Position Defines */
#define I2C_SR2_MSL_POS               (0U)
#define I2C_SR2_BUSY_POS              (1U)
#define I2C_SR2_TRA_POS               (2U)
#define I2C_SR2_GENCALL_POS           (4U)
#define I2C_SR2_SMBDEFAULT_POS        (5U)
#define I2C_SR2_SMBHOST_POS           (6U)
#define I2C_SR2_DUALF_POS             (7U)

/* CCR Register - Position Defines */
#define I2C_CCR_CCR_POS              (0U) 
#define I2C_CCR_DUTY_POS             (14U)
#define I2C_CCR_FS_POS               (15U)

/* TRISE Register - Position Defines */
#define I2C_TRISE_TRISE_POS          (0U)

/*====================================================================================================================================================================
|                             Register Bit Mask Defines
=====================================================================================================================================================================*/

/* CR1 Register - Mask Defines */
#define I2C_CR1_PE_MASK               (1U << I2C_CR1_PE_POS)
#define I2C_CR1_SMBUS_MASK            (1U << I2C_CR1_SMBUS_POS)
#define I2C_CR1_SMBTYPE_MASK          (1U << I2C_CR1_SMBTYPE_POS)
#define I2C_CR1_ENARP_MASK            (1U << I2C_CR1_ENARP_POS)
#define I2C_CR1_ENPEC_MASK            (1U << I2C_CR1_ENPEC_POS)
#define I2C_CR1_ENGC_MASK             (1U << I2C_CR1_ENGC_POS)
#define I2C_CR1_NOSTRETCH_MASK        (1U << I2C_CR1_NOSTRETCH_POS)
#define I2C_CR1_START_MASK            (1U << I2C_CR1_START_POS)
#define I2C_CR1_STOP_MASK             (1U << I2C_CR1_STOP_POS)
#define I2C_CR1_ACK_MASK              (1U << I2C_CR1_ACK_POS)
#define I2C_CR1_POS_MASK              (1U << I2C_CR1_POS_POS)
#define I2C_CR1_PEC_MASK              (1U << I2C_CR1_PEC_POS)
#define I2C_CR1_ALERT_MASK            (1U << I2C_CR1_ALERT_POS)

/* CR2 Register - Mask Defines */
#define I2C_CR2_FREQ_MASK             (0x3FU << I2C_CR2_FREQ_POS)
#define I2C_CR2_ITERREN_MASK          (1U << I2C_CR2_ITERREN_POS)
#define I2C_CR2_ITEVTEN_MASK          (1U << I2C_CR2_ITEVTEN_POS)
#define I2C_CR2_ITBUFEN_MASK          (1U << I2C_CR2_ITBUFEN_POS)

/* OAR1 Register - Mask Defines */
#define I2C_OAR1_ADD0_MASK            (1U << I2C_OAR1_ADD0_POS)
#define I2C_OAR1_ADD_MASK             (1U << I2C_OAR1_ADD_POS)
#define I2C_OAR1_ADD8_MASK            (1U << I2C_OAR1_ADD8_POS)
#define I2C_OAR1_ADD9_MASK            (1U << I2C_OAR1_ADD9_POS)
#define I2C_OAR1_BIT14_MASK           (1U << I2C_OAR1_BIT14_POS)
#define I2C_OAR1_ADDMODE_MASK         (1U << I2C_OAR1_ADDMODE_POS)

/* OAR2 Register - Mask Defines */
#define I2C_OAR2_ENDUAL_MASK          (1U << I2C_OAR2_ENDUAL_POS)
#define I2C_OAR2_ADD2_MASK            (1U << I2C_OAR2_ADD2_POS)

/* SR1 Register - Mask Defines */
#define I2C_DR_DR_MASK                (0xFFU << I2C_DR_DR_POS)

/* SR1 Register - Mask Defines */
#define I2C_SR1_SB_MASK               (1U << I2C_SR1_SB_POS)
#define I2C_SR1_ADDR_MASK             (1U << I2C_SR1_ADDR_POS)
#define I2C_SR1_BTF_MASK              (1U << I2C_SR1_BTF_POS)
#define I2C_SR1_ADD10_MASK            (1U << I2C_SR1_ADD10_POS)
#define I2C_SR1_STOPF_MASK            (1U << I2C_SR1_STOPF_POS)
#define I2C_SR1_RxNE_MASK             (1U << I2C_SR1_RxNE_POS)
#define I2C_SR1_TxE_MASK              (1U << I2C_SR1_TxE_POS)
#define I2C_SR1_BERR_MASK             (1U << I2C_SR1_BERR_POS)
#define I2C_SR1_ARLO_MASK             (1U << I2C_SR1_ARLO_POS)
#define I2C_SR1_AF_MASK               (1U << I2C_SR1_AF_POS)
#define I2C_SR1_OVR_MASK              (1U << I2C_SR1_OVR_POS)
#define I2C_SR1_PECERR_MASK           (1U << I2C_SR1_PECERR_POS)
#define I2C_SR1_TIMEOUT_MASK          (1U << I2C_SR1_TIMEOUT_POS)

/* SR2 Register - Mask Defines */
#define I2C_SR2_MSL_MASK              (1U << I2C_SR2_MSL_POS)
#define I2C_SR2_BUSY_MASK             (1U << I2C_SR2_BUSY_POS)
#define I2C_SR2_TRA_MASK              (1U << I2C_SR2_TRA_POS)
#define I2C_SR2_GENCALL_MASK          (1U << I2C_SR2_GENCALL_POS)
#define I2C_SR2_SMBDEFAULT_MASK       (1U << I2C_SR2_SMBDEFAULT_POS)
#define I2C_SR2_SMBHOST_MASK          (1U << I2C_SR2_SMBHOST_POS)
#define I2C_SR2_DUALF_MASK            (1U << I2C_SR2_DUALF_POS)

/* CCR Register - Mask Defines */
#define I2C_CCR_CCR_MASK             (0xFFFU << I2C_CCR_CCR_POS)
#define I2C_CCR_DUTY_MASK            (1U << I2C_CCR_DUTY_POS)
#define I2C_CCR_FS_MASK              (1U << I2C_CCR_FS_POS)

/* TRISE Register - Mask Defines*/
#define I2C_TRISE_TRISE_MASK         (0x3FU << I2C_TRISE_TRISE_POS)



/*====================================================================================================================================================================
|                                       DEFINE ENUMERATIONS
=====================================================================================================================================================================*/
/**
 * 
 * 
 * @brief I2C Status Enumeration
 * @note  This enumeration defines the possible status codes that can be returned by I2C functions. It includes:
 *          - I2C_STATUS_OK: Indicates that the I2C operation completed successfully without any errors.
 *          - I2C_STATUS_ERROR: Indicates that an error occurred during the I2C operation, such as a bus error, arbitration loss, ACK failure, overrun, or timeout.
 *          - I2C_STATUS_BUSY: Indicates that the I2C peripheral is currently busy with an ongoing operation and cannot accept new commands until the current operation is complete.
 *          - I2C_STATUS_TIMEOUT: Indicates that the I2C operation timed out, which can occur if the expected events (e.g., start condition, address acknowledgment) do not happen within a specified time frame.
 *          - I2C_STATUS_NAK: Indicates that a NACK (Not Acknowledge) was received during an I2C communication, which can occur if the slave device does not acknowledge its address or data byte.
 */
typedef enum
{
    I2C_STATUS_OK = 0U,

    I2C_STATUS_BUSY,
    I2C_STATUS_TIMEOUT,

    I2C_STATUS_NACK,
    I2C_STATUS_BUS_ERROR,
    I2C_STATUS_ARBITRATION_LOST,
    I2C_STATUS_ACK_FAIL,
    I2C_STATUS_OVERRUN,

    I2C_STATUS_INVALID_PARAM,
    I2C_STATUS_ERROR
} I2C_Status_t;



/**
 * 
 * 
 * @brief I2C Peripheral State Enumeration
 * @note  This enumeration defines the enable or disable state of the I2C peripheral.
 */
typedef enum
{
    DISABLE = 0U,
    ENABLE,
}I2C_State_t;

/**
 * 
 * 
 * @brief I2C SMBus Mode Enumeration
 * @note  This enumeration defines the operating mode of the I2C peripheral. It includes:
 *          - I2C_MODE: Selects standard I2C mode operation.
 *          - SMBUS_MODE: Selects SMBus mode operation.
 */
typedef enum
{
    I2C_MODE = 0U,
    SMBUS_MODE
}I2C_SMBusMode_t;

/**
 * 
 * 
 * @brief I2C SMBus Type Enumeration
 * @note  This enumeration defines the SMBus device type configuration. It includes:
 *          - I2C_SMBUS_DEVICE: Configures the peripheral as an SMBus device.
 *          - I2C_SMBUS_HOST: Configures the peripheral as an SMBus host.
 */
typedef enum
{
    I2C_SMBUS_DEVICE = 0U,
    I2C_SMBUS_HOST
} I2C_SMBusType_t;



/**
 * 
 * 
 * @brief I2C Duty Cycle Enumeration
 * @note  This enumeration defines the duty cycle configuration for fast mode operation. It includes:
 *          - I2C_DUTY_CYCLE_2: Selects duty cycle ratio 2.
 *          - I2C_DUTY_CYCLE_16_9: Selects duty cycle ratio 16/9.
 */
typedef enum
{
    I2C_DUTY_CYCLE_2 = 0U,
    I2C_DUTY_CYCLE_16_9 = I2C_CCR_DUTY_MASK
} I2C_DutyCycle_t;

/**
 * 
 * 
 * @brief I2C Data Direction Enumeration
 * @note  This enumeration defines the transfer direction for I2C communication. It includes:
 *          - I2C_DIRECTION_WRITE: Selects master transmit mode.
 *          - I2C_DIRECTION_READ: Selects master receive mode.
 */
typedef enum
{
    I2C_DIRECTION_WRITE = 0U,
    I2C_DIRECTION_READ
} I2C_Direction_t;


/**
 * 
 * 
 * @brief I2C ACK Position Enumeration
 * @note  This enumeration defines the ACK bit position behavior during data reception. It includes:
 *          - I2C_ACK_CURRENT_BYTE: ACK bit controls the current byte.
 *          - I2C_ACK_NEXT_BYTE: ACK bit controls the next byte.
 */
typedef enum
{
    I2C_ACK_CURRENT_BYTE = 0U,
    I2C_ACK_NEXT_BYTE = I2C_CR1_POS_MASK
} I2C_AckPosition_t;


/**
 * 
 * 
 * @brief I2C Error Flag Enumeration
 * @note  This enumeration defines the possible I2C error conditions detected by the peripheral. It includes:
 *          - I2C_ERROR_BUS: Indicates a bus error condition.
 *          - I2C_ERROR_ARBITRATION_LOSS: Indicates arbitration loss detection.
 *          - I2C_ERROR_ACK_FAIL: Indicates acknowledge failure detection.
 *          - I2C_ERROR_OVERRUN: Indicates overrun or underrun detection.
 *          - I2C_ERROR_TIMEOUT: Indicates timeout or Tlow error detection.
 *          - I2C_ERROR_PEC: Indicates PEC error detection.
 */
typedef enum
{
    I2C_ERROR_BUS = I2C_SR1_BERR_MASK,
    I2C_ERROR_ARBITRATION_LOSS =I2C_SR1_ARLO_MASK,
    I2C_ERROR_ACK_FAIL = I2C_SR1_AF_MASK,
    I2C_ERROR_OVERRUN = I2C_SR1_OVR_MASK,
    I2C_ERROR_TIMEOUT = I2C_SR1_TIMEOUT_MASK,
    I2C_ERROR_PEC = I2C_SR1_PECERR_MASK
} I2C_ErrorFlag_t;

/*====================================================================================================================================================================
|                             Typedef Structures
=====================================================================================================================================================================*/
/**
 * 
 * 
 * @brief I2C Transfer Data Structure
 * @note This structure can be used for both master and slave transfers, with the direction field indicating the transfer type. 
 *      - For master transfers, the slave address and data buffers will be used.
 *      - For slave transfers, the data buffer will be used to receive or transmit data based on the direction.
 *      - The index field can be used to track the progress of the transfer, especially for multi-byte transfers.
 */
typedef struct
{
    uint8_t *data;                 
    uint16_t length;               
    uint16_t index;                
    I2C_Direction_t direction;
} I2C_Transfer_t;

/**
 * 
 * 
 * @brief I2C Master Transaction Structure
 * @note This structure is specifically designed for master mode transactions, encapsulating all necessary information for a complete I2C communication session.
 *      - The slave_address field specifies the 7-bit address of the target slave device.   
 *      - The tx_data and rx_data pointers are used for transmit and receive buffers, respectively. Depending on the transaction type, one of these may be NULL.
 *      - The tx_length and rx_length fields specify the number of bytes to transmit or receive. For a transmit-only transaction, rx_length would be 0, and vice versa for a receive-only transaction.
 */
typedef struct
{
    uint8_t slave_address;
    uint8_t *tx_data;
    uint16_t tx_length;
    uint8_t *rx_data;
    uint16_t rx_length;
} I2C_Master_t;

/**
 * 
 * 
 * @brief I2C Status Information Structure
 * @note This structure can be used to provide detailed status information about the I2C peripheral, including the current state, any error conditions, and whether the bus is currently busy.
 *      - The status : field can indicate the overall status of the I2C operation (e.g., OK, ERROR, BUSY, TIMEOUT).
 *      - The error  : field can provide specific error flags that occurred during the I2C operation (e.g., bus error, arbitration loss, ACK failure).
 *      - The is_busy: field can be used to indicate whether the I2C peripheral is currently engaged in a transfer, which can be useful for non-blocking operations or when checking the status before initiating a new transfer.
 */
typedef struct
{
    I2C_Status_t status;           
    I2C_ErrorFlag_t error;
    uint8_t is_busy;               
} I2C_Status_Info_t;

/**
 * @brief Selects the address mode for the I2C OAR1 register.
 * @details Configures whether the peripheral uses a 7-bit or 10-bit slave address.
 * @note The 10-bit mode is represented by the OAR1 ADDMODE bit mask.
 */
typedef enum
{
    OAR1_ADDMODE_7BIT_SLAVE = 0U, 
    OAR1_ADDMODE_10BIT_SLAVE = I2C_OAR1_ADDMODE_MASK 
} OAR1_ModeAddr_t;

/**
 * @brief Selects the dual-address mode for the I2C OAR2 register.
 * @details Configures whether the secondary slave address is enabled together with OAR1.
 * @note The value is mapped to the OAR2 ENDUAL bit.
 */
typedef enum
{
    OAR2_ENDUAL_ONLY_OAR1_7BIT = 0U,    
    OAR2_ENDUAL_BOTH_7BIT = I2C_OAR2_ENDUAL_MASK
} OAR2_DualMode_t;

/**
 * @brief Configuration structure for the I2C OAR1 register.
 * @details Defines the slave address mode and the own address value used by the peripheral.
 */
typedef struct
{
    OAR1_ModeAddr_t addr_mode;  
    uint16_t own_address; 
} OAR1_Config_t;

/**
 * @brief Configuration structure for the I2C OAR2 register.
 * @details Defines the dual-address mode and the secondary own address value.
 */
typedef struct
{
    OAR2_DualMode_t dual_mode;          
    uint8_t own_address;                
} OAR2_Config_t;

/**
 * 
 * 
 * @brief I2C CR1 Configuration Structure
 * @note This structure is used to configure the I2C CR1 register, which includes settings for peripheral enable, SMBus mode, general call enable, no stretch mode, ACK enable, and ACK position.
 *      - The pe_enable          : field specifies whether the I2C peripheral is enabled or disabled.
 *      - The smbus_mode         : field specifies whether the I2C is operating in standard I2C mode or SMBus mode.
 *      - The smb_type           : Field specifies whether the device is operating as an SMBus host or device when in SMBus mode.
 *      - The general_call_enable: field specifies whether the I2C peripheral will respond to the general call address (0x00).
 *      - The no_stretch         : field specifies whether clock stretching is disabled (enabled = 0, disabled = 1).
 *      - The ack_enable         : field specifies whether ACK is enabled for received bytes. 
 *      - The ack_pos            : field specifies the ACK position (current byte or next byte) when ACK is enabled.  
 */
typedef struct
{
    I2C_State_t pe_enable; 
    I2C_SMBusMode_t smbus_mode; 
    I2C_SMBusType_t smb_type;
    I2C_State_t general_call_enable;
    I2C_State_t no_stretch;
    I2C_State_t ack_enable; 
    I2C_AckPosition_t ack_pos; 
} I2C_CR1_Config_t;

/**
 * @brief I2C CR2 Configuration Structure
 * @note This structure is used to configure the I2C CR2 register, which includes settings for peripheral clock frequency and interrupt enables.
 *      - The freq field specifies the APB1 clock frequency in MHz, which is used to generate the correct timing for I2C communication. Valid values typically range from 2 MHz to 36 MHz depending on the specific STM32F103 variant and its clock configuration.
 *      - The error_interrupt, event_interrupt, and buffer_interrupt fields specify whether the corresponding I2C interrupts are enabled or disabled. Enabling these interrupts allows the I2C peripheral to generate interrupt requests for error conditions,
 * events (like start/stop conditions), and buffer status changes (like TxE and RxNE), which can be handled in the I2C event and error interrupt service routines.
 */
typedef struct
{
    uint16_t freq;                 
    I2C_State_t error_interrupt;       
    I2C_State_t event_interrupt;       
    I2C_State_t buffer_interrupt;      
} I2C_CR2_Config_t;

/**
 * @brief Combined configuration for the I2C OAR1/OAR2 registers.
 * @details Groups the primary and secondary slave address configurations used by the peripheral.
 */
typedef struct
{
    OAR1_Config_t oar1_cfg;            
    OAR2_Config_t oar2_cfg;            
} I2C_OAR_Config_t;

/**
 * @brief I2C Configuration Structure
 * @note This structure is used to configure the I2C peripheral, including settings for SCL frequency, own address, duty cycle, and CR1/CR2 configurations.
 *      - The scl_frequency_khz field specifies the desired SCL clock frequency in kHz (e.g., 100 kHz for standard mode, 400 kHz for fast mode). This value is used to calculate the appropriate values for the CCR and TRISE registers based on the APB1 clock frequency.
 *      - The own_address field specifies the 7-bit address of the I2C device when operating in slave mode. This address is typically left-shifted by 1 when writing to the OAR1 register, as the least significant bit is used to indicate read/write operations.
 *      - The duty_cycle field specifies the duty cycle for fast mode (400 kHz), which can be either 2 (standard 1:1) or 16/9. This setting is only relevant when the SCL frequency is set to 400 kHz.
 *      - The cr1_cfg and cr2_cfg fields are nested structures that contain the specific configuration settings for the I2C CR1 and CR2 registers, respectively. This allows for a comprehensive configuration of the I2C peripheral using a single structure, making it easier to initialize the peripheral with all necessary settings in one step.
 *      - The OAR1/OAR2 address configuration for primary and secondary slave addresses.
 */
typedef struct
{
    uint16_t scl_frequency_khz;    
    I2C_DutyCycle_t duty_cycle;    

    I2C_CR1_Config_t cr1_cfg;
    I2C_CR2_Config_t cr2_cfg;

    I2C_OAR_Config_t oar_cfg;
} I2C_Config_t;


/*====================================================================================================================================================================
|                             Function Declarations
=====================================================================================================================================================================*/
/* Initialization and Configuration Functions */
I2C_Status_t I2C_Init(I2C_TypeDef *I2Cx, I2C_Config_t *i2c_cfg);
void I2C_DeInit(I2C_TypeDef *I2Cx);
I2C_Status_t I2C_SetOwnAddress(I2C_TypeDef *I2Cx, uint16_t addr);
void I2C_SetSpeed(I2C_TypeDef *I2Cx,
                  uint32_t pclk1_hz,
                  uint32_t scl_hz,
                  I2C_DutyCycle_t duty_cycle);

/* Address Functions */
I2C_Status_t I2C_SendAddress(I2C_TypeDef *I2Cx,
                     uint8_t address,
                     I2C_Direction_t dir);

void I2C_ClearADDRFlag(I2C_TypeDef *I2Cx);

/* Error Check Functions */
void I2C_ClearErrorFlag(I2C_TypeDef *I2Cx, I2C_ErrorFlag_t err_flag);

/* ACK Configuration Functions */
void I2C_SetACKPosition(I2C_TypeDef *I2Cx,
                        I2C_AckPosition_t position);

/* Polling Wait Functions */
I2C_Status_t I2C_WaitForSB(I2C_TypeDef *I2Cx, uint32_t timeout);
I2C_Status_t I2C_WaitForADDR(I2C_TypeDef *I2Cx, uint32_t timeout);
I2C_Status_t I2C_WaitForTXE(I2C_TypeDef *I2Cx, uint32_t timeout);
I2C_Status_t I2C_WaitForRXNE(I2C_TypeDef *I2Cx, uint32_t timeout);
I2C_Status_t I2C_WaitForBTF(I2C_TypeDef *I2Cx, uint32_t timeout);

/* Master Communication Functions */
I2C_Status_t I2C_MasterTransmit(I2C_TypeDef *I2Cx,
                                uint8_t slave_addr,
                                uint8_t *pData,
                                uint16_t len);

I2C_Status_t I2C_MasterReceive(I2C_TypeDef *I2Cx,
                               uint8_t slave_addr,
                               uint8_t *pData,
                               uint16_t len);

I2C_Status_t I2C_MasterTransmitReceive(I2C_TypeDef *I2Cx,
                                       uint8_t slave_addr,
                                       uint8_t *txData,
                                       uint16_t txLen,
                                       uint8_t *rxData,
                                       uint16_t rxLen);

/* slave Communication Functions */
I2C_Status_t I2C_SlaveTransmit(I2C_TypeDef *I2Cx,
                               uint8_t *pData,
                               uint16_t len);

I2C_Status_t I2C_SlaveReceive(I2C_TypeDef *I2Cx,
                              uint8_t *pData,
                              uint16_t len);

/* Memory Read/Write Functions */
I2C_Status_t I2C_MemWrite(I2C_TypeDef *I2Cx,
                          uint8_t slave_addr,
                          uint8_t mem_addr,
                          uint8_t *pData,
                          uint16_t len);

I2C_Status_t I2C_MemRead(I2C_TypeDef *I2Cx,
                         uint8_t slave_addr,
                         uint8_t mem_addr,
                         uint8_t *pData,
                         uint16_t len);

/* Device Detection Functions */
I2C_Status_t I2C_IsDeviceReady(I2C_TypeDef *I2Cx,
                               uint8_t slave_addr,
                               uint32_t trials);

I2C_Status_t I2C_ScanBus(I2C_TypeDef *I2Cx,
                         uint8_t *found_list,
                         uint8_t *found_count);

/* Bus Recovery Function */
I2C_Status_t I2C_BusRecover(I2C_TypeDef *I2Cx);

/* Interrupt Handler Functions */
void I2C_EV_IRQHandler(I2C_TypeDef *I2Cx);
void I2C_ER_IRQHandler(I2C_TypeDef *I2Cx);

/* Interrupt-based Communication Functions */
I2C_Status_t I2C_MasterTransmit_IT(I2C_TypeDef *I2Cx,
                                   uint8_t slave_addr,
                                   uint8_t *pData,
                                   uint16_t len);

I2C_Status_t I2C_MasterReceive_IT(I2C_TypeDef *I2Cx,
                                  uint8_t slave_addr,
                                  uint8_t *pData,
                                  uint16_t len);

/* DMA-based Communication Functions */
I2C_Status_t I2C_MasterTransmit_DMA(I2C_TypeDef *I2Cx,
                                    uint8_t slave_addr,
                                    uint8_t *pData,
                                    uint16_t len);

I2C_Status_t I2C_MasterReceive_DMA(I2C_TypeDef *I2Cx,
                                   uint8_t slave_addr,
                                   uint8_t *pData,
                                   uint16_t len);   

/* Interrupt-based Communication Functions */
I2C_Status_t I2C_SlaveTransmit_IT(I2C_TypeDef *I2Cx,
                                  uint8_t *pData,
                                  uint16_t len);

I2C_Status_t I2C_SlaveReceive_IT(I2C_TypeDef *I2Cx,
                                  uint8_t *pData,
                                  uint16_t len);
#endif  /*    STM32F103xx_I2C_H     */