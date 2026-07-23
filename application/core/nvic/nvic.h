/**************************************************************************************************
 * @file    nvic.h
 * @brief   Bare-metal NVIC driver for STM32F103xx microcontrollers
 *
 * @details
 * This file provides:
 *  - NVIC register definitions
 *  - IRQ enable/disable APIs
 *  - Interrupt priority configuration
 *  - Pending interrupt management
 *  - Cortex-M3 interrupt vector definitions
 *
 * @author  lu-05-cmd
 * @date    2025-11-29
 * @version 0.1.0
 *
 * @note
 *  - Developed for embedded systems learning and practice
 *  - Fully register-level programming (Bare-metal)
 *  - Based on ARM Cortex-M3 architecture
 *  - Applicable to STM32F103xx series devices
 *  - NVIC priority configuration depends on priority grouping
 *
 **************************************************************************************************/

#ifndef STM32F103xx_NVIC_H
#define STM32F103xx_NVIC_H

//Include ==============================================================================================================================================================                                                                                             
#include <stdint.h>
#include "../scb/scb.h"

/*====================================================================================================================================================================
 |                   Nested Vectored Interrupt Controller Register Map 
 =====================================================================================================================================================================*/
typedef struct
{
    volatile uint32_t ICTR;
    volatile uint32_t _reserved[24];
}NVIC_Info_Typedef;


typedef struct{
/* ----------------------------------------------------------------------------------------------------------------------------- */
/* Interrupt Set-Enable Registers (NVIC_ISERx) - offset: 0x000 - 0x01C                                                           */
/* ----------------------------------------------------------------------------------------------------------------------------- */
    volatile uint32_t ISER[8];          // 0x000 - 0x01C
    volatile uint32_t _reserved0[24];   // 0x020 - 0x07C


/* ----------------------------------------------------------------------------------------------------------------------------- */
/* Interrupt Clear-Enable Registers (NVIC_ICERx) - offset: 0x080 - 0x09C                                                         */
/* ----------------------------------------------------------------------------------------------------------------------------- */
    volatile uint32_t ICER[8];          // 0x080 - 0x09C
    volatile uint32_t _reserved1[24];   // 0x0A0 - 0x0FC
   

/* ----------------------------------------------------------------------------------------------------------------------------- */
/* Interrupt Set-Pending Registers (NVIC_ISPRx) - offset: 0x100 - 0x11C                                                          */
/* ----------------------------------------------------------------------------------------------------------------------------- */
    volatile uint32_t ISPR[8];          // 0x100 - 0x11C
    volatile uint32_t _reserved2[24];   // 0x120 - 0x17C    
   

/* ----------------------------------------------------------------------------------------------------------------------------- */
/* Interrupt Clear-Pending Registers (NVIC_ICPRx) - offset: 0x180 - 0x19C                                                        */
/* ----------------------------------------------------------------------------------------------------------------------------- */
    volatile uint32_t ICPR[8];          // 0x180 - 0x19C
    volatile uint32_t _reserved3[24];   // 0x1A0 - 0x1FC


/* ----------------------------------------------------------------------------------------------------------------------------- */
/* Interrupt Active Bit Registers (NVIC_IABRx) - offset: 0x200 - 0x21C                                                           */
/* ----------------------------------------------------------------------------------------------------------------------------- */
    volatile uint32_t IABR[8];          // 0x200 - 0x21C
    volatile uint32_t _reserved4[56];   // 0x220 - 0x2FC   
    

/* ----------------------------------------------------------------------------------------------------------------------------- */
/* Interrupt Priority Registers (NVIC_IPRx) - offset: 0x300 - 0x3EF                                                              */
/* ----------------------------------------------------------------------------------------------------------------------------- */
    volatile uint8_t IPR[240];          // 0x300 - 0x3EF
} NVIC_TypeDef;


/*====================================================================================================================================================================
                        NVIC(Nested Vectored Interrupt Controller) BASE ADDRESS
 =====================================================================================================================================================================*/
#define NVIC_INFO_BASE  0xE000E000UL
#define NVIC_BASE       0xE000E100UL
#define NVIC_INFO      ((NVIC_Info_Typedef *)  NVIC_INFO_BASE)
#define NVIC            ((NVIC_TypeDef *)   NVIC_BASE)



/*====================================================================================================================================================================
 |                                           Mode Define
 =====================================================================================================================================================================*/

#define __NVIC_PRIO_BIT 4
/*------------------------ enum ------------------------*/
typedef enum
{
    NMI_IRQn            = -14,
    HardFault_IRQn      = -13,
    MemManage_IRQn      = -12,
    BusFault_IRQn       = -11,
    UsageFault_IRQn     = -10,
    SVC_IRQn            = -5,
    DebugMon_IRQn       = -4,
    PendSV_IRQn         = -2,
    SysTick_IRQn        = -1,
    WWDG_IRQn           = 0,
    PVD_IRQn            = 1,
    TAMPER_IRQn         = 2,
    RTC_IRQn            = 3,
    FLASH_IRQn          = 4,
    RCC_IRQn            = 5,
    EXTI0_IRQn          = 6,
    EXTI1_IRQn          = 7,
    EXTI2_IRQn          = 8,
    EXTI3_IRQn          = 9,
    EXTI4_IRQn          = 10,
    DMA1_Channel1_IRQn  = 11,
    DMA1_Channel2_IRQn  = 12,
    DMA1_Channel3_IRQn  = 13,
    DMA1_Channel4_IRQn  = 14,
    DMA1_Channel5_IRQn  = 15,
    DMA1_Channel6_IRQn  = 16,
    DMA1_Channel7_IRQn  = 17,
    ADC1_2_IRQn         = 18,    
    CAN1_TX_IRQn        = 19,
    CAN1_RX0_IRQn       = 20,
    CAN1_RX1_IRQn       = 21,
    CAN1_SCE_IRQn       = 22,
    EXTI9_5_IRQn        = 23,
    TIM1_BRK_IRQn       = 24,
    TIM1_UP_IRQn        = 25,
    TIM1_TRG_COM_IRQn   = 26,
    TIM1_CC_IRQn        = 27,
    TIM2_IRQn           = 28,
    TIM3_IRQn           = 29,
    TIM4_IRQn           = 30,
    I2C1_EV_IRQn        = 31,
    I2C1_ER_IRQn        = 32,
    I2C2_EV_IRQn        = 33,
    I2C2_ER_IRQn        = 34,
    SPI1_IRQn           = 35,
    SPI2_IRQn           = 36,    
    USART1_IRQn         = 37,    
    USART2_IRQn         = 38,    
    USART3_IRQn         = 39,
    EXTI15_10_IRQn      = 40
}NVIC_IRQn_Position_t;

/*====================================================================================================================================================================
|                             Inline Functions
 =====================================================================================================================================================================*/
void NVIC_EnableIRQ(NVIC_IRQn_Position_t IRQn);
void NVIC_DisableIRQ(NVIC_IRQn_Position_t IRQn);

void NVIC_SetPendingIRQ(NVIC_IRQn_Position_t IRQn);
void NVIC_ClearPendingIRQ(NVIC_IRQn_Position_t IRQn);
uint8_t NVIC_GetPendingIRQ(NVIC_IRQn_Position_t IRQn);

void NVIC_SetPriority(NVIC_IRQn_Position_t IRQn, uint16_t prio);
uint8_t NVIC_GetPriority(NVIC_IRQn_Position_t IRQn);
uint32_t NVIC_GetActive(NVIC_IRQn_Position_t IRQn);
#endif      /*   STM32F103xx_NVIC_H   */

