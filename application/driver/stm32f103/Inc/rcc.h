// done

/**********************************************************************************************************************************************************************
 * @file    : rcc.h
 * @brief   : 
 *              - Generic RCC driver interface for STM32F103xx Series.
 *              - Based on STM32F103xx Reference Manual (RM0008).
 *              - This file provides register definitions and base addresses.
 *              - Configuration enums and public APIs, intended to be reused 
 *                across STM32103xx
 * @author  : lu-05-cmd
 * @date    : January, 1 2026
 * @version : 0.1
 * 
 * @note    : 
 *              - Applicable to STM32F103xx devices only.
 *              - Clock tree configuration is specific to  STM32F103xx series.
 *              - Peripheral clocks must be enabled before accessing peripheral register.
 *              - Provides peripheral clock enable and reset control.
 *              - Some peripheral clock domains may vary between device variants.
 **********************************************************************************************************************************************************************/

#ifndef STM32F103xx_RCC_H    
#define STM32F103xx_RCC_H

//Include ============================================================================================================================================================
#include <stdint.h>


/*====================================================================================================================================================================
 |                                 Configuration Reset and Clock Control (RCC) 
 =====================================================================================================================================================================*/
typedef struct
{
    volatile uint32_t CR;               // Control Register                                 offset: 0x00
    volatile uint32_t CFGR;             // Clock Configuration Register                     offset: 0x04
    volatile uint32_t CIR;              // Control Interrupt Register                       offset: 0x08
    volatile uint32_t APB2RSTR;         // PAB2 Peripheral Reset Register                   offset: 0x0C
    volatile uint32_t APB1RSTR;         // PAB1 Peripheral Reset Register                   offset: 0x10
    volatile uint32_t AHBENR;          // AHB Peripheral Clock Enable Register             offset: 0x14
    volatile uint32_t APB2ENR;          // APB2 Peripheral Clock Enabler Register           offset: 0x18
    volatile uint32_t APB1ENR;          // APB1 Peripheral Clock Enabler Register           offset: 0x1C
    volatile uint32_t BDCR;             // Backup Domain Control Register                   offset: 0x20
    volatile uint32_t CSR;              // Control/Status Register                          offset: 0x24
    volatile uint32_t AHBSTR;           // AHB Peripheral Clock Reset Register              offset: 0x28
    volatile uint32_t CFGR2;            // Clock Configuration Register 2                   offset: 0x2C

}RCC_TypeDef;

/*====================================================================================================================================================================
 |                                       RCC BASE ADDRESS
 =====================================================================================================================================================================*/
#define RCC ((RCC_TypeDef*)0x40021000UL) 




/*====================================================================================================================================================================
 |                                           Mode Define & enum
 =====================================================================================================================================================================*/

/*--Control Register*/
#define RCC_CR_HSION_POS            (0U)
#define RCC_CR_HSIRDY_POS           (1U)
#define RCC_CR_HSEON_POS            (16U)
#define RCC_CR_HSERDY_POS           (16U)
#define RCC_CR_PLLON_POS            (24U)
#define RCC_CR_PLLRDY_POS            (24U)
/*--Clock Configuration Register*/
#define RCC_CFGR_SW_POS             (0U)
#define RCC_CFGR_SWS_POS            (2U)
#define RCC_CFGR_PLLSRC_POS         (16U)
#define RCC_CFGR_PLLXTPRE_POS       (17U)
#define RCC_CFGR_PLLMUL_POS         (18U)
/*--APB2 Peripheral Reset Register*/
#define RCC_APB2RSTR_AFIORST_POS    (0U)
#define RCC_APB2RSTR_IOPARST_POS    (2U)
#define RCC_APB2RSTR_IOPBRST_POS    (3U)
#define RCC_APB2RSTR_IOPCRST_POS    (4U)
#define RCC_APB2RSTR_IOPDRST_POS    (5U)
#define RCC_APB2RSTR_IOPERST_POS    (6U)
#define RCC_APB2RSTR_ADC1RST_POS    (9U)
#define RCC_APB2RSTR_ADC2RST_POS    (10U)
#define RCC_APB2RSTR_TIM1RST_POS    (11U)
#define RCC_APB2RSTR_SPI1RST_POS    (12U)
#define RCC_APB2RSTR_USART1RST_POS  (14U)
/*--APB1 Peripheral Reset Register*/
#define RCC_APB1RSTR_TIM2RST_POS    (0U)
#define RCC_APB1RSTR_TIM3RST_POS    (1U)
#define RCC_APB1RSTR_TIM4RST_POS    (2U)
#define RCC_APB1RSTR_TIM5RST_POS    (3U)
#define RCC_APB1RSTR_TIM6RST_POS    (4U)
#define RCC_APB1RSTR_TIM7RST_POS    (5U)
#define RCC_APB1RSTR_SPI2RST_POS    (14U)
#define RCC_APB1RSTR_SPI3RST_POS    (15U)
#define RCC_APB1RSTR_USART2RST_POS  (17U)
#define RCC_APB1RSTR_USART3RST_POS  (18U)
#define RCC_APB1RSTR_USART4RST_POS  (19U)
#define RCC_APB1RSTR_USART5RST_POS  (20U)
#define RCC_APB1RSTR_I2C1RST_POS    (21U)
#define RCC_APB1RSTR_I2C2RST_POS    (22U)
/*--APB Peripheral Clock Enable Register*/
#define RCC_AHBENR_DMA1EN_POS       (0U)
#define RCC_AHBENR_DMA2EN_POS       (1U)
/*--APB2 Peripheral Clock Enable Register*/
#define RCC_APB2ENR_AFIOEN_POS      (0U)
#define RCC_APB2ENR_IOPAEN_POS      (2U)
#define RCC_APB2ENR_IOPBEN_POS      (3U)
#define RCC_APB2ENR_IOPCEN_POS      (4U)
#define RCC_APB2ENR_IOPDEN_POS      (5U)
#define RCC_APB2ENR_IOPEEN_POS      (6U)
#define RCC_APB2ENR_ADC1EN_POS      (9U)
#define RCC_APB2ENR_ADC2EN_POS      (10U)
#define RCC_APB2ENR_TIM1EN_POS      (11U)
#define RCC_APB2ENR_SPI1EN_POS      (12U)
#define RCC_APB2ENR_USART1EN_POS    (14U)
/*--APB1 Peripheral Clock Enable Register*/
#define RCC_APB1ENR_TIM2EN_POS      (0U)
#define RCC_APB1ENR_TIM3EN_POS      (1U)
#define RCC_APB1ENR_TIM4EN_POS      (2U)
#define RCC_APB1ENR_TIM5EN_POS      (3U)
#define RCC_APB1ENR_TIM6EN_POS      (4U)
#define RCC_APB1ENR_TIM7EN_POS      (5U)
#define RCC_APB1ENR_SPI2EN_POS      (14U)
#define RCC_APB1ENR_SPI3EN_POS      (15U)
#define RCC_APB1ENR_USART2EN_POS    (17U)
#define RCC_APB1ENR_USART3EN_POS    (18U)
#define RCC_APB1ENR_I2C1EN_POS      (21U)
#define RCC_APB1ENR_I2C2EN_POS      (22U)


/*CR_MASK*/
#define RCC_CR_HSI_MASK             (1U << RCC_CR_HSION_POS)           
#define RCC_CR_HSIRDY_MASK          (1U << RCC_CR_HSIRDY_POS)
#define RCC_CR_HSE_MASK             (1U << RCC_CR_HSEON_POS)
#define RCC_CR_HSERDY_MASK          (1U << RCC_CR_HSERDY_POS)
#define RCC_CR_PLL_MASK             (1U << RCC_CR_PLLON_POS)
#define RCC_CR_PLLRDY_MASK          (1U << RCC_CR_PLLRDY_POS)

/*CFGR_MASK*/
#define RCC_CFGR_SW_MASK            (1U << RCC_CFGR_SW_POS)
#define RCC_CFGR_SWS_MASK           (0x3U << RCC_CFGR_SW_POS)
/*APB2RSTR_MASK*/
#define RCC_APB2RSTR_AFIO_RESET_MASK    (1U << RCC_APB2RSTR_AFIORST_POS)
#define RCC_APB2RSTR_GPIOA_RESET_MASK   (1U << RCC_APB2RSTR_IOPARST_POS)       
#define RCC_APB2RSTR_GPIOB_RESET_MASK   (1U << RCC_APB2RSTR_IOPBRST_POS)
#define RCC_APB2RSTR_GPIOC_RESET_MASK   (1U << RCC_APB2RSTR_IOPCRST_POS)
#define RCC_APB2RSTR_GPIOD_RESET_MASK   (1U << RCC_APB2RSTR_IOPDRST_POS)
#define RCC_APB2RSTR_GPIOE_RESET_MASK   (1U << RCC_APB2RSTR_IOPERST_POS)
#define RCC_APB2RSTR_ADC1_RESET_MASK    (1U << RCC_APB2RSTR_ADC1RST_POS)
#define RCC_APB2RSTR_ADC2_RESET_MASK    (1U << RCC_APB2RSTR_ADC2RST_POS)
#define RCC_APB2RSTR_TIM1_RESET_MASK    (1U << RCC_APB2RSTR_TIM1RST_POS)
#define RCC_APB2RSTR_SPI1_RESET_MASK    (1U << RCC_APB2RSTR_SPI1RST_POS)
#define RCC_APB2RSTR_USART1_RESET_MASK  (1U << RCC_APB2RSTR_USART1RST_POS)
/*APB1RSTR_MASK*/
#define RCC_APB1RSTR_TIM2_RESET_MASK    (1U << RCC_APB1RSTR_TIM2RST_POS)      
#define RCC_APB1RSTR_TIM3_RESET_MASK    (1U << RCC_APB1RSTR_TIM2RST_POS)
#define RCC_APB1RSTR_TIM4_RESET_MASK    (1U << RCC_APB1RSTR_TIM2RST_POS)
#define RCC_APB1RSTR_SPI2_RESET_MASK    (1U << RCC_APB1RSTR_SPI2RST_POS)
#define RCC_APB1RSTR_USART2_RESET_MASK  (1U << RCC_APB1RSTR_USART2RST_POS)
#define RCC_APB1RSTR_USART3_RESET_MASK  (1U << RCC_APB1RSTR_USART3RST_POS)
#define RCC_APB1RSTR_I2C1_RESET_MASK    (1U << RCC_APB1RSTR_I2C1RST_POS)
#define RCC_APB1RSTR_I2C2_RESET_MASK    (1U << RCC_APB1RSTR_I2C2RST_POS)
/*APNENR_MASK*/
#define RCC_AHBENR_DMA1_MASK        (1U << RCC_AHBENR_DMA1EN_POS)
#define RCC_AHBENR_DMA2_MASK        (1U << RCC_AHBENR_DMA2EN_POS)
/*APB2ENR_MASK*/
#define RCC_APB2ENR_AFIO_MASK       (1U << RCC_APB2ENR_AFIOEN_POS)       
#define RCC_APB2ENR_GPIOA_MASK      (1U << RCC_APB2ENR_IOPAEN_POS)          
#define RCC_APB2ENR_GPIOB_MASK      (1U << RCC_APB2ENR_IOPBEN_POS)
#define RCC_APB2ENR_GPIOC_MASK      (1U << RCC_APB2ENR_IOPCEN_POS)
#define RCC_APB2ENR_GPIOD_MASK      (1U << RCC_APB2ENR_IOPDEN_POS)
#define RCC_APB2ENR_GPIOE_MASK      (1U << RCC_APB2ENR_IOPEEN_POS)
#define RCC_APB2ENR_ADC1_MASK       (1U << RCC_APB2ENR_ADC1EN_POS)
#define RCC_APB2ENR_ADC2_MASK       (1U << RCC_APB2ENR_ADC1EN_POS)
#define RCC_APB2ENR_TIM1_MASK       (1U << RCC_APB2ENR_TIM1EN_POS)
#define RCC_APB2ENR_SPI1_MASK       (1U << RCC_APB2ENR_SPI1EN_POS)
#define RCC_APB2ENR_USART1_MASK     (1U << RCC_APB2ENR_USART1EN_POS)
/*APB1ENR_MASK*/
#define RCC_APB1ENR_TIM2_MASK     (1U << RCC_APB1ENR_TIM2EN_POS)        
#define RCC_APB1ENR_TIM3_MASK     (1U << RCC_APB1ENR_TIM3EN_POS)
#define RCC_APB1ENR_TIM4_MASK     (1U << RCC_APB1ENR_TIM4EN_POS)
#define RCC_APB1ENR_SPI2_MASK     (1U << RCC_APB1ENR_SPI2EN_POS)
#define RCC_APB1ENR_USART2_MASK   (1U << RCC_APB1ENR_USART2EN_POS)
#define RCC_APB1ENR_USART3_MASK   (1U << RCC_APB1ENR_USART3EN_POS)
#define RCC_APB1ENR_I2C1_MASK     (1U << RCC_APB1ENR_I2C1EN_POS)
#define RCC_APB1ENR_I2C2_MASK     (1U << RCC_APB1ENR_I2C2EN_POS)


/* ------------------------------------ GPIO Mode Enum -------------------------------- */
typedef enum
{
    RCC_GPIOA = 0,
    RCC_GPIOB,
    RCC_GPIOC,
    RCC_GPIOD,
    RCC_GPIOE
}RCC_GPIO_t;

/* ----------------------------------- USART Mode Enum -------------------------------- */
typedef enum
{
    RCC_USART1 = 0,
    RCC_USART2,
    RCC_USART3
}RCC_USART_t;

/* ----------------------------------- ADC Mode Enum ---------------------------------- */
typedef enum
{
    RCC_ADC1 = 0,
    RCC_ADC2
}RCC_ADC_t;

/* ----------------------------------- SPI Mode Enum ---------------------------------- */
typedef enum
{
    RCC_SPI1 = 0,
    RCC_SPI2
}RCC_SPI_t;

/* ----------------------------------- I2C Mode Enum ---------------------------------- */
typedef enum
{
    RCC_I2C1 = 0,
    RCC_I2C2
}RCC_I2C_t;


/* ----------------------------------- PLL Mode Enum ---------------------------------- */
typedef enum
{
    PLLSRC_HSI = 0,
    PLLSRC_HSE,
    PLLSRC_HSE_DIV2
}PLL_SRC_t;

typedef enum
{
    SYSCLK_HSI = 0x0,
    SYSCLK_HSE = 0x1,
    SYSCLK_PLL = 0x2
}RCC_SYSCLK_t;

typedef enum{
    MULx4 = 0x2,
    MULx5 = 0x3,
    MULx6 = 0x4,
    MULx7 = 0x5,
    MULx8 = 0x6,
    MULx9 = 0x7,
    MULx6_5 = 0xD
}RCC_Multiplier_t;

/* ----------------------------------- TIM Mode Enum ---------------------------------- */
typedef enum
{
    RCC_TIM1 = 0,
    RCC_TIM2,
    RCC_TIM3,
    RCC_TIM4
}RCC_TIM_t;

/* ----------------------------------- DMA Mode Enum ---------------------------------- */
typedef enum
{
    RCC_DMA1 = 0,
    RCC_DMA2
}RCC_DMA_t;

/*====================================================================================================================================================================
 |                            RCC Inline Functions
 =====================================================================================================================================================================*/
void RCC_EnableGPIOClock(RCC_GPIO_t GPIOx);
void RCC_DisableGPIOClock(RCC_GPIO_t GPIOx);

void RCC_EnableAFIOClock(void);
void RCC_DisableAFIOClock(void);

void RCC_EnableUSARTClock(RCC_USART_t USARTx);
void RCC_DisableUSARTClock(RCC_USART_t USARTx);

void RCC_EnableADCClock(RCC_ADC_t ADCx);
void RCC_DisableADCClock(RCC_ADC_t ADCx);

void RCC_EnableDMAClock(RCC_DMA_t DMAx);
void RCC_DisableDMAClock(RCC_DMA_t DMAx);

void RCC_EnableSPIClock(RCC_SPI_t SPIx);
void RCC_DisableSPIClock(RCC_SPI_t SPIx);

void RCC_EnableI2CClock(RCC_I2C_t I2Cx);
void RCC_DisableI2CClock(RCC_I2C_t I2Cx);

void RCC_ResetGPIO(RCC_GPIO_t GPIOx);
void RCC_ResetAFIO(void);
void RCC_ResetUSART(RCC_USART_t USARTx);
void RCC_ResetADC(RCC_ADC_t ADCx);
void RCC_ResetSPI(RCC_SPI_t SPIx);
void RCC_ResetI2C(RCC_I2C_t I2Cx);

void RCC_EnableHSI(void);
void RCC_DisableHSI(void);
void RCC_EnableHSE(void);
void RCC_DisableHSE(void);
void RCC_EnablePLL(void);
void RCC_DisablePLL(void);
void RCC_ConfigPLL(PLL_SRC_t src, RCC_Multiplier_t mul);
void RCC_SelectSYSCLK(RCC_SYSCLK_t src);

void RCC_EnableTIMClock(RCC_TIM_t TIMx);
void RCC_DisableTIMClock(RCC_TIM_t TIMx);

#endif  /*   STM32F103xx_RCC_H    */