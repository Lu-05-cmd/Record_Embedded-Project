/**********************************************************************************************************************************************************************
 * @file    : afio.h
 * @brief   : AFIO (Alternate Function I/O) driver interface for STM32F103xx devices.
 *              - This header file provides AFIO register map definition, AFIO base address
 *              - Enumeration for EXTI line, GPIO port mapping, and debug configuration
 * @author  : lu-05-cmd
 * @date    : Juanary, 31 2025
 * @version : 0.1
 * 
 * @note : 
 *      - Designed according to STM32F10x Reference Manual (RM0008).
 *      - AFIO peripheral is varialble only on STM32F103xx series.
 *      - The AFIO clock must be enable in RCC before using this driver.
 **********************************************************************************************************************************************************************/


#ifndef STM32F103xx_AFIO_H
#define STM32F103xx_AFIO_H


/*INCLUDE============================================================================================================================================================*/
#include <stdint.h>




/*====================================================================================================================================================================
 |                                 Configuration Alternate Function Input/Output (AFIO)
 =====================================================================================================================================================================*/
typedef struct{
    volatile uint32_t EVCR;                 // Event Control Register                               - offset: 0x00
    volatile uint32_t MAPR;                 // AF Remap and Debug I/O Configuration Register        - offset: 0x04
    volatile uint32_t EXTICR[4];            // External Interrupt Configuration Register x          - offset: 0x08 - 0x14
    volatile uint32_t reserved;             // Reserved                                             - offset: 0x018
    volatile uint32_t MAPR2;                // AF Remap and Debug I/O Configuration Register 2      - offset: 0x1C
}AFIO_TypeDef;

/*====================================================================================================================================================================
 |                                       AFIO BASE ADDRESS
 =====================================================================================================================================================================*/
#define AFIO ((volatile AFIO_TypeDef *) 0x40010000UL)

/*====================================================================================================================================================================
 |                                          AFIO Mode Enum & Inline Functions
 =====================================================================================================================================================================*/
/*AF Remap and debug I/O Configuration Register*/
#define AFIO_MAPR_SPI1_REMAP_POS        (0U)
#define AFIO_MAPR_I2C1_REMAP_POS        (1U)
#define AFIO_MAPR_USART1_REMAP_POS      (2U)
#define AFIO_MAPR_USART2_REMAP_POS      (3U)
#define AFIO_MAPR_USART3_REMAP_POS      (4U)
#define AFIO_MAPR_TIM1_REMAP_POS        (6U)
#define AFIO_MAPR_TIM2_REMAP_POS        (8U)
#define AFIO_MAPR_TIM3_REMAP_POS        (10U)
#define AFIO_MAPR_TIM4_REMAP_POS        (12U)

#define AFIO_MAPR_SWJ_CFG_POS   (24U)


/*MAPR_Mask*/
#define AFIO_MAPR_SPI1_REMAP_MASK       (1U << AFIO_MAPR_SPI1_REMAP_POS)
#define AFIO_MAPR_I2C1_REMAP_MASK       (1U << AFIO_MAPR_I2C1_REMAP_POS)
#define AFIO_MAPR_USART1_REMAP_MASK     (1U << AFIO_MAPR_USART1_REMAP_POS)
#define AFIO_MAPR_USART2_REMAP_MASK     (1U << AFIO_MAPR_USART2_REMAP_POS)

#define AFIO_MAPR_TIM4_REMAP_MASK       (1U << AFIO_MAPR_TIM4_REMAP_POS)
/* -------------------- AFIO mode enumeration -------------------- */
typedef enum
{
    AFIO_DEBUG_FULL_SWJ = 0x0,
    AFIO_DEBUG_NO_JTAG  = 0x2,
    AFIO_DEBUG_DISABLE  = 0x4
}AFIO_Debug_Mode;

typedef enum
{
    EXTI_LINE_0 = 0,
    EXTI_LINE_1,
    EXTI_LINE_2,
    EXTI_LINE_3,
    EXTI_LINE_4,
    EXTI_LINE_5,
    EXTI_LINE_6,
    EXTI_LINE_7,
    EXTI_LINE_8,
    EXTI_LINE_9,
    EXTI_LINE_10,
    EXTI_LINE_11,
    EXTI_LINE_12,
    EXTI_LINE_13,
    EXTI_LINE_14,
    EXTI_LINE_15,
}AFIO_EXTI_LINE;

typedef enum
{
    AFIO_NO_REMAP = 0,
    AFIO_REMAP
}Function_2Remap_t;

typedef enum
{
    AFIO_USART3_NO_REMAP = 0,
    AFIO_USART3_PARTIAL_REMAP,
    AFIO_USART3_FULL_REMAP
}Function_RemapUSART3_t;

typedef enum 
{
    AFIO_TIM1_NO_REMAP = 0,
    AFIO_TIM1_PARTIAL_REMAP,
    AFIO_TIM1_FULL_REMAP
}Function_RemapTIM1_t;

typedef enum
{
    AFIO_TIM2_NO_REMAP = 0,
    AFIO_TIM2_PARTIAL1_REMAP,
    AFIO_TIM2_PARTIAL2_REMAP,
    AFIO_TIM2_FULL_REMAP
}Function_RemapTIM2_t;

typedef enum
{
    AFIO_TIM3_NO_REMAP = 0,
    AFIO_TIM3_PARTIAL_REMAP,
    AFIO_TIM3_FULL_REMAP
}Function_RemapTIM3_t;

/*====================================================================================================================================================================
 |                            AFIO Inline Functions
 =====================================================================================================================================================================*/
void AFIO_Init(void);
void AFIO_DebugPortConfig(AFIO_Debug_Mode mode);
void AFIO_EXTIConfig(AFIO_EXTI_LINE line, uint32_t port_mask);

void AFIO_SPI1Remap(Function_2Remap_t status);
void AFIO_I2C1Remap(Function_2Remap_t status);

void AFIO_USART1Remap(Function_2Remap_t status);
void AFIO_USART3Remap(Function_RemapUSART3_t status);

void AFIO_TIM1Remap(Function_RemapTIM1_t status);
void AFIO_TIM2Remap(Function_RemapTIM2_t status);
void AFIO_TIM3Remap(Function_RemapTIM3_t status);
void AFIO_TIM4Remap(Function_2Remap_t status);
#endif  /*  STM32F103xx_AFIO_H    */