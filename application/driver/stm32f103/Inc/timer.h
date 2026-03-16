/**********************************************************************************************************************************************************************
 * @file    : timer.h
 * @brief   :      
 *              - Generic TIMER driver interface for STM32f103xx microcontroller.
 *              - This file provides register definitions, base address.
 *              - Configuration enum, add public ADIs intended tobe reused across multiple STM32F103xx.
 * @author  : lu-05-cmd
 * @date    : Junuary, 3 2026
 * @version : 0.1
 * 
 * @note    : 
 *              - Base on STM32F103xx Reference Manual (RM0008).
 *              - Implementation details may vary depending on the TIM instance.
 **********************************************************************************************************************************************************************/

#ifndef STM32F103xx_TIMER_H
#define STM32F103xx_TIMER_H

/*INCLUDE============================================================================================================================================================*/
#include <stdint.h>

/*====================================================================================================================================================================
 |                            Configuration Timer (TIM) 
 =====================================================================================================================================================================*/
typedef struct 
{
    volatile uint32_t CR1;          // Control Register 1                       - offset: 0x00
    volatile uint32_t CR2;          // Control Register 2                       - offset: 0x04
    volatile uint32_t SMCR;         // Slave Mode Control Register              - offset: 0x08
    volatile uint32_t DIER;         // DMA/Interrupt Enable Register            - offset: 0x0C
    volatile uint32_t SR;           // Status Register                          - offset: 0x10
    volatile uint32_t EGR;          // Event Generation Register                - offset: 0x14
    volatile uint32_t CCMR1;        // Capture/Compare Mode Register 1          - offset: 0x18
    volatile uint32_t CCMR2;        // Capture/Compare Mode Register 2          - offset: 0x1C
    volatile uint32_t CCER;         // Capture/Compare Enable Register          - offset: 0x20
    volatile uint32_t CNT;          // Counter Register                         - offset: 0x24
    volatile uint32_t PSC;          // Prescaler Register                       - offset: 0x28
    volatile uint32_t ARR;          // Auto-Relaod Register                     - offset: 0x2C
    volatile uint32_t RCR;          // Repetiton Couter Register                - offset: 0x30
    volatile uint32_t CCR1;         // Capture/Compare Register 1               - offset: 0x34
    volatile uint32_t CCR2;         // Capture/Compare Register 2               - offset: 0x38
    volatile uint32_t CCR3;         // Capture/Compare Register 3               - offset: 0x3C
    volatile uint32_t CCR4;         // Capture/Compare Register 4               - offset: 0x40
    volatile uint32_t BDTR;         // Break and Dead-tim Register              - offset: 0x44
    volatile uint32_t DCR;          // DMA Control Register                     - offset: 0x48
    volatile uint32_t DMAR;         // DMA Address for full Transfer            - offset: 0x4C
}TIM_TypeDef;

/*====================================================================================================================================================================
 |                                Reset and Clock Control (RCC) Base
 =====================================================================================================================================================================*/
#define TIM1 ((TIM_TypeDef *) 0x40013000UL)

#define TIM2 ((TIM_TypeDef *) 0x40000000UL)
#define TIM3 ((TIM_TypeDef *) 0x40000400UL)
#define TIM4 ((TIM_TypeDef *) 0x40000800UL)

 /*====================================================================================================================================================================
 |                                Mode Enum & define
 =====================================================================================================================================================================*/

/*TIMx Control Register 1*/
#define TIM_CR1_CEN_POS     (0U)
#define TIM_CR1_UDIS_POS    (1U)
#define TIM_CR1_DIR_POS     (4U)
#define TIM_CR1_ARPE_POS    (7U)
/*DMA/Interrupt Enable Register*/
#define TIM_DIER_UIE_POS    (0U)
/*TIMx Status Register*/   
#define TIM_SR_UIF_POS      (0U)
/*TIMx Event Generation Register*/
#define TIM_EGR_UG_POS      (0U)
/*TIMx Capture/Compare Output Mode Register 1*/
#define TIM_CCMR1_CC1S_POS  (0U)
#define TIM_CCMR1_OC1PE_POS (3U)
#define TIM_CCMR1_OC1M_POS  (4U)
#define TIM_CCMR1_CC2S_POS  (8U)
#define TIM_CCMR1_OC2PE_POS (11U)
#define TIM_CCMR1_OC2M_POS  (12U)
/*TIMx Capture/Compare Output Mode Register 2*/
#define TIM_CCMR2_CC3S_POS  (0U)
#define TIM_CCMR2_OC3PE_POS (3U)
#define TIM_CCMR2_OC3M_POS  (4U)
#define TIM_CCMR2_CC4S_POS  (8U)
#define TIM_CCMR2_OC4PE_POS (11U)
#define TIM_CCMR2_OC4M_POS  (12U)
/*TIMx Capture/Compare Enable Register*/
#define TIM_CCER_CC1E_POS   (0U)
#define TIM_CCER_CC1P_POS   (1U)
#define TIM_CCER_CC2E_POS   (4U)
#define TIM_CCER_CC2P_POS   (5U)
#define TIM_CCER_CC3E_POS   (8U)
#define TIM_CCER_CC3P_POS   (9U)
#define TIM_CCER_CC4E_POS   (9U)
#define TIM_CCER_CC4P_POS   (10U)
/*TIM1 Break and Dead-Time Register*/
#define TIM1_BDTR_MOE_POS   (0U)


/*CR1_MASK*/
#define TIM_CR1_CEN_MASK        (1U << TIM_CR1_CEN_POS)
#define TIM_CR1_UDIS_MASK       (1U << TIM_CR1_UDIS_POS)
#define TIM_CR1_DIR_MASK        (1U << TIM_CR1_DIR_POS)
#define TIM_CR1_ARPE_MASK       (1U << TIM_CR1_ARPE_POS)
/*DIER_MASK*/
#define TIM_DIER_UIE_MASK       (1U << TIM_DIER_UIE_POS)
/*SR_MASK*/ 
#define TIM_SR_UIF_MASK         (1U << TIM_SR_UIF_POS)
/*EGR_MASK*/    
#define TIM_EGR_UG_MASK         (1U << TIM_EGR_UG_POS)
/*CCMR1-Output_MASKS*/  
#define TIM_CCMR1_OC1PE_MASK    (1U << TIM_CCMR1_OC1PE_POS)
#define TIM_CCMR2_OC2PE_MASK    (1U << TIM_CCMR1_OC2PE_POS)
/*CCMR2-Output_MASKS*/  
#define TIM_CCMR2_OC3PE_MASK    (1U << TIM_CCMR2_OC3PE_POS)
#define TIM_CCMR2_OC4PE_MASK    (1U << TIM_CCMR2_OC4PE_POS)
/*CCER_MASK*/   
#define TIM_CCER_CC1E_MASK      (1U << TIM_CCER_CC1E_POS)
#define TIM_CCER_CC2E_MASK      (1U << TIM_CCER_CC2E_POS)
#define TIM_CCER_CC3E_MASK      (1U << TIM_CCER_CC3E_POS)
#define TIM_CCER_CC4E_MASK      (1U << TIM_CCER_CC4E_POS)

#define TIM_BDTR_MOE_MASK       (1U << TIM1_BDTR_MOE_POS)

/* ------------------------------------ TIM channel Mode Enum -------------------------------- */
typedef enum
{
    TIM_CH1 = 0,
    TIM_CH2,
    TIM_CH3,
    TIM_CH4
}TIM_Channel_t;
/*====================================================================================================================================================================
 |                             Inline Functions
 =====================================================================================================================================================================*/
void TIM_Init(TIM_TypeDef *TIMx, uint16_t psc, uint16_t arr);
void TIM_Init(TIM_TypeDef *TIMx, uint16_t psc, uint16_t arr);

void TIM_Start(TIM_TypeDef *TIMx);
void TIM_Start(TIM_TypeDef *TIMx);
void TIM_Stop(TIM_TypeDef *TIMx);
void TIM_Stop(TIM_TypeDef *TIMx);

void TIM_EnableIT(TIM_TypeDef *TIMx, uint32_t interrupt);
void TIM_EnableIT(TIM_TypeDef *TIMx, uint32_t interrupt);
void TIM_DisableIT(TIM_TypeDef *TIMx, uint32_t interrupt);
void TIM_DisableIT(TIM_TypeDef *TIMx, uint32_t interrupt);

void TIM_ClearFlag(TIM_TypeDef *TIMx, uint32_t flag_mask);
void TIM_ClearFlag(TIM_TypeDef *TIMx, uint32_t flag_mask);
uint32_t TIM_GetFlag(TIM_TypeDef *TIMx, uint32_t flag);
uint32_t TIM_GetFlag(TIM_TypeDef *TIMx, uint32_t flagx);

void TIM_SetCounter(TIM_TypeDef *TIMx, uint32_t cnt);
void TIM_SetCounter(TIM_TypeDef *TIMx, uint32_t cnt);
void TIM_SetPrescaler(TIM_TypeDef *TIMx, uint32_t psc);
void TIM_SetPrescaler(TIM_TypeDef *TIMx, uint32_t psc);


void TIM_PWM_Init(TIM_TypeDef *TIMx, uint32_t psc, uint32_t arr,TIM_Channel_t channel);
void TIM_PWM_Start(TIM_TypeDef *TIMx, TIM_Channel_t channel);
void TIM_PWM_Stop(TIM_TypeDef *TIMx, TIM_Channel_t channel);

#endif /*   STM32F103xx_TIMER_H   */