/**************************************************************************************************
 * @file    exti.h
 * @brief   Bare-metal EXTI driver for STM32F103xx microcontrollers
 *
 * @details
 * This file provides:
 *  - EXTI register definitions
 *  - External interrupt/event configuration APIs
 *  - Trigger and mode configuration
 *  - Callback registration support
 *  - EXTI interrupt handler interfaces
 *
 * @author  lu-05-cmd
 * @date    2026-03-11
 * @version 0.1.0
 *
 * @target  STM32F103xx Series
 * 
 * @note
 *  - Developed for embedded systems learning and practice
 *  - Fully register-level programming (Bare-metal)
 *  - Based on STM32F10x Reference Manual (RM0008)
 *  - Supports interrupt and event modes
 *
 **************************************************************************************************/

#ifndef STM32F103xx_EXTI_H
#define STM32F103xx_EXTI_H


/*INCLUDE============================================================================================================================================================*/
#include <stdint.h>

/*====================================================================================================================================================================
 |                                 Configuration External Interrupt/Event Controller (EXTI)
 =====================================================================================================================================================================*/
typedef struct
{
    volatile uint32_t IMR;      // Interrupt Mask Register                  - offset: 0x00
    volatile uint32_t EMR;      // Event Mask Register                      - offset: 0x04
    volatile uint32_t RTSR;     // Rising Trigger Selection Register        - offset: 0x08
    volatile uint32_t FTSR;     // Falling Trigger Selection Register       - offset: 0x0C
    volatile uint32_t SWIER;    // Software Interrupt Event Register        - offset: 0x10
    volatile uint32_t PR;       // Pending Register                         - offset: 0x14
} EXTI_TypeDef_t;

/*====================================================================================================================================================================
 |                                       EXTI BASE ADDRESS
 =====================================================================================================================================================================*/
#define EXTI ((EXTI_TypeDef_t *)0x40010800UL)

/*====================================================================================================================================================================
 |                                 EXTI Line Position Definitions
 =====================================================================================================================================================================*/
#define EXTI_LINE_0_POS             (0U)
#define EXTI_LINE_1_POS             (1U)
#define EXTI_LINE_2_POS             (2U)
#define EXTI_LINE_3_POS             (3U)
#define EXTI_LINE_4_POS             (4U)
#define EXTI_LINE_5_POS             (5U)
#define EXTI_LINE_6_POS             (6U)
#define EXTI_LINE_7_POS             (7U)
#define EXTI_LINE_8_POS             (8U)
#define EXTI_LINE_9_POS             (9U)
#define EXTI_LINE_10_POS            (10U)
#define EXTI_LINE_11_POS            (11U)
#define EXTI_LINE_12_POS            (12U)
#define EXTI_LINE_13_POS            (13U)
#define EXTI_LINE_14_POS            (14U)
#define EXTI_LINE_15_POS            (15U)
#define EXTI_LINE_16_POS            (16U)
#define EXTI_LINE_17_POS            (17U)
#define EXTI_LINE_18_POS            (18U)
#define EXTI_LINE_19_POS            (19U)

/*====================================================================================================================================================================
 |                                 EXTI Line Mask Definitions
 =====================================================================================================================================================================*/
#define EXTI_CALLBACK_TABLE_SIZE   (16U)
#define EXTI_LINE_ALL    (0x000FFFFFU)



/*====================================================================================================================================================================
|                                           DEFINE ENUMERATIONS
=====================================================================================================================================================================*/
typedef uint32_t EXTI_Line_t;


/**
 * @brief EXTI Status Enumeration
 * @note  This enumeration defines the possible status values for EXTI operations, including success, error, invalid trigger configuration, and invalid mode configuration. It can be used to indicate the outcome of EXTI initialization, de-initialization, and other related functions, allowing for standardized error handling and status reporting in applications that utilize external interrupts and events.
 *      - EXTI_STATUS_OK: Operation completed successfully.
 *      - EXTI_STATUS_ERROR: An error occurred during the EXTI operation.
 *      - EXTI_STATUS_INVALID_TRIGGER: The specified trigger configuration is invalid (e.g., unsupported trigger type).
 *      - EXTI_STATUS_INVALID_MODE: The specified mode configuration is invalid (e.g., unsupported mode type).
 */
typedef enum
{
    EXTI_STATUS_OK                = 0x00U,
    EXTI_STATUS_ERROR             = 0x01U,

    EXTI_STATUS_INVALID_PARAM     = 0x02U,
    EXTI_STATUS_INVALID_LINE      = 0x03U,
    EXTI_STATUS_INVALID_PORT      = 0x04U,
    EXTI_STATUS_INVALID_TRIGGER   = 0x05U,
    EXTI_STATUS_INVALID_MODE      = 0x06U,

    EXTI_STATUS_TIMEOUT           = 0x07U,
    EXTI_STATUS_BUSY              = 0x08U,
} EXTI_Status_t;
/**
 * 
 * 
 * @brief EXTI Trigger Configuration Enumeration
 * @note  This enumeration defines the possible trigger configurations for EXTI lines on STM32F103 devices. It specifies whether an external interrupt or event is generated on a rising edge, falling edge, both edges, or disabled entirely. It can be used during EXTI initialization to configure the trigger sensitivity of a selected EXTI line.
 *      - EXTI_TRIGGER_NONE: Disable trigger detection for the EXTI line.
 *      - EXTI_TRIGGER_RISING: Generate interrupt/event on rising edge detection.
 *      - EXTI_TRIGGER_FALLING: Generate interrupt/event on falling edge detection.
 *      - EXTI_TRIGGER_BOTH: Generate interrupt/event on both rising and falling edge detection.
 */
typedef enum
{
    EXTI_TRIGGER_NONE       = 0x00U,
    EXTI_TRIGGER_RISING     = 0x01U,
    EXTI_TRIGGER_FALLING    = 0x02U,
    EXTI_TRIGGER_BOTH       = EXTI_TRIGGER_RISING | EXTI_TRIGGER_FALLING
}EXTI_Trigger_t;

/**
 * 
 * 
 * @brief EXTI Mode Configuration Enumeration
 * @note  This enumeration defines the possible operating modes for EXTI lines on STM32F103 devices. It specifies whether the EXTI line operates in interrupt mode or event mode. It can be used during EXTI initialization to configure how the processor responds to external signal events.
 *      - EXTI_MODE_INTERRUPT: Configure EXTI line in interrupt mode.
 *      - EXTI_MODE_EVENT: Configure EXTI line in event mode.
 */
typedef enum
{
    EXTI_MODE_INTERRUPT     = 0x00U,
    EXTI_MODE_EVENT         = 0x01U
}EXTI_Mode_t;

/**
 * 
 * 
 * @brief 
 * @note  
 */
typedef enum
{
    EXTI_GPIOA = 0U,
    EXTI_GPIOB,
    EXTI_GPIOC,
    EXTI_GPIOD,
    EXTI_GPIOE,
    EXTI_GPIOF,
    EXTI_GPIOG
}EXTI_Port_t;
/*====================================================================================================================================================================
|                             DEFINE STRUCTURES
=====================================================================================================================================================================*/
/**
 * 
 * 
 * @brief EXTI Configuration Structure
 * @note  This structure holds the configuration parameters for an EXTI line, including the line number, trigger configuration, and mode configuration. It can be used to initialize an EXTI line with specific settings.
 *      - line: Specifies the EXTI line to configure (e.g., EXTI_LINE_0, EXTI_LINE_1, etc.).
 *      - trigger: Specifies the trigger configuration for the EXTI line (e.g., EXTI_TRIGGER_RISING, EXTI_TRIGGER_FALLING, etc.).
 *      - mode: Specifies the operating mode for the EXTI line (e.g., EXTI_MODE_INTERRUPT or EXTI_MODE_EVENT).
 */
typedef struct
{
    EXTI_Line_t line;
    EXTI_Trigger_t trigger;
    EXTI_Mode_t mode;
    EXTI_Port_t port;
}EXTI_Config_t;

/*====================================================================================================================================================================
|                             Inline Functions
=====================================================================================================================================================================*/

EXTI_Status_t EXTI_Init(const EXTI_Config_t *config);
EXTI_Status_t EXTI_DeInit(void);

EXTI_Status_t EXTI_RegisterCallback(EXTI_Line_t line, void (*callback)(void));
EXTI_Status_t EXTI_IRQ_Handler(EXTI_Line_t line);
EXTI_Status_t EXTI_UnregisterCallback(EXTI_Line_t line);

void EXTI0_IRQHandler(void);    
void EXTI1_IRQHandler(void);
void EXTI2_IRQHandler(void);
void EXTI3_IRQHandler(void);
void EXTI4_IRQHandler(void);
void EXTI9_5_IRQHandler(void);
void EXTI15_10_IRQHandler(void);

#endif  /*   STM32F103xx_EXTI_H    */