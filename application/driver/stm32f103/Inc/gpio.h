/**********************************************************************************************************************************************************************
 * @file    : gpio.h
 * @brief   : 
 *              - Generic GPIO driver interface for STM32F103xx devices.
 *              - This file provides register definitions, base addresses.
 * @author  : lu-05-cmd
 * @date    : January, 30 2026.
 * @version : 0.1
 * 
 * @note : 
 *          - This is file designed based on STM32F103xx reference manual (RM0008).
 *          - GPIO register layout follows the STM32F103 memory map and register description.
 *          - Implementation details may vary depending on the specific STM32F103xx devices.
 **********************************************************************************************************************************************************************/

#ifndef STM32F103xx_GPIO_H    
#define STM32F103xx_GPIO_H


/*INCLUDE============================================================================================================================================================*/
#include <stdint.h>




/*====================================================================================================================================================================
 |                                 Configuration General-Purpose Input/Output (GPIO)
 =====================================================================================================================================================================*/
typedef struct
{
    volatile uint32_t CRL;          // Configuration Register Low           - offset: 0x00
    volatile uint32_t CRH;          // Configuration Register High          - offset: 0x04
    volatile uint32_t IDR;          // Input Data Register                  - offset: 0x08
    volatile uint32_t ODR;          // Output Data Register                 - offset: 0x0C
    volatile uint32_t BSRR;         // Bit Set/Reset Register               - offset: 0x10
    volatile uint32_t BRR;          // Bit Reset Register                   - offset: 0x14
    volatile uint32_t LCKR;         // Configuration Lock Register          - offset: 0x18

}GPIO_TypeDef;

/*====================================================================================================================================================================
|                                 GPIO BASE ADDRESS
=====================================================================================================================================================================*/
#define GPIOA   ((GPIO_TypeDef *)0x40010800UL)
#define GPIOB   ((GPIO_TypeDef *)0x40010C00UL)
#define GPIOC   ((GPIO_TypeDef *)0x40011000UL)
#define GPIOD   ((GPIO_TypeDef *)0x40011400UL)
#define GPIOE   ((GPIO_TypeDef *)0x40011800UL)


/*====================================================================================================================================================================
 |                                          GPIO Mode Enum & Inline Functions
 =====================================================================================================================================================================*/
/* -------------------- GPIO mode enumeration -------------------- */
typedef enum{
    // Input modes
    GPIO_MODE_INPUT_ANALOG       = 0x0,    // GPIO Mode Input Analog    
    GPIO_MODE_INPUT_FLOATING     = 0x1,    // GPIO Mode Input Floating  
    GPIO_MODE_INPUT_PU           = 0x2,    // GPIO Mode Input Pull-Up   
    GPIO_MODE_INPUT_PD           = 0x3,    // GPIO Mode Input Pull-Down 
    // Out mode
    GPIO_MODE_OUTPUT_PP_10MHz    = 0x4,    // GPIO Mode Output Push-Pull 10MHz
    GPIO_MODE_OUTPUT_OD_10MHz    = 0x5,    // GPIO Mode Output Open-Drain 10MHz
    GPIO_MODE_OUTPUT_PP_2MHz     = 0x6,    // GPIO Mode Output Push-Pull 2MHz
    GPIO_MODE_OUTPUT_OD_2MHz     = 0x7,    // GPIO Mode Output Open-Drain 2MHz
    GPIO_MODE_OUTPUT_PP_50MHz    = 0x8,    // GPIO Mode Output Push-Pull 50MHz
    GPIO_MODE_OUTPUT_OD_50MHz    = 0x9,    // GPIO Mode Output Open-Drain 50MHz
    // Alternate Function mode
    GPIO_MODE_AF_OUTPUT_PP_10MHz = 0xA,    // GPIO Mode Alternate Function Push-Pull 10MHz
    GPIO_MODE_AF_OUTPUT_OD_10MHz = 0xB,    // GPIO Mode Alternate Function Open-Drain 10MHz
    GPIO_MODE_AF_OUTPUT_PP_2MHz  = 0xC,    // GPIO Mode Alternate Function Push-Pull 2MHz
    GPIO_MODE_AF_OUTPUT_OD_2MHz  = 0xD,    // GPIO Mode Alternate Function Open-Drain 2MHz
    GPIO_MODE_AF_OUTPUT_PP_50MHz = 0xE,    // GPIO Mode Alternate Function Push-Pull 50MHz
    GPIO_MODE_AF_OUTPUT_OD_50MHz = 0xF     // GPIO Mode Alternate Function Open-Drain 50MHz
}GPIO_MODE_TypeDef;

typedef enum {
    GPIO_SPEED_PP_10MHz         = 0x1,  // Output Push-Pull 10MHz
    GPIO_SPEED_OD_10MHz         = 0x5,  // Output Open-Drain 10MHz
    GPIO_SPEED_PP_2MHz          = 0x2,  // Output Push-Pull 2MHz
    GPIO_SPEED_OD_2MHz          = 0x6,  // Output Open-Drain 2MHz
    GPIO_SPEED_PP_50MHz         = 0x3,  // Output Push-Pull 50MHz
    GPIO_SPEED_OD_50MHz         = 0x7,  // Output Open-Drain 50MHz
    GPIO_SPEED_AF_PP_10MHz      = 0x9,  // Alternate Function Push-Pull 10MHz
    GPIO_SPEED_AF_OD_10MHz      = 0xA,  // Alternate Function Open-Drain 10MHz
    GPIO_SPEED_AF_PP_2MHz       = 0xB,  // Alternate Function Push-Pull 2MHz
    GPIO_SPEED_AF_OD_2MHz       = 0xD,  // Alternate Function Open-Drain 2MHz
    GPIO_SPEED_AF_PP_50MHz      = 0xE,  // Alternate Function Push-Pull 50MHz
    GPIO_SPEED_AF_OD_50MHz      = 0xF   // Alternate Function Open-Drain 50MHz
} GPIO_Cf_Speed;

typedef enum{
    LOW    = 0,
    HIGH   = 1
}GPIO_PinState_t;


/*====================================================================================================================================================================
 |                            GPIO Inline Functions
 =====================================================================================================================================================================*/
/*<--- Function that configures the initial state of the GPIOx --->*/
void GPIO_Init(GPIO_TypeDef *GPIOx, uint16_t pinmask, GPIO_MODE_TypeDef mode);

/*<--- Function that configutes the write pin/port of the GPIOx --->*/
void GPIO_WritePin(GPIO_TypeDef *GPIO, uint16_t pin, GPIO_PinState_t state);
void GPIO_WritePort(GPIO_TypeDef *GPIOx, uint32_t mask, uint32_t value);

/*<--- Function that configutes the read pin/port of the GPIOx --->*/
uint32_t GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t pin);
uint32_t GPIO_ReadPort(GPIO_TypeDef *GPIOx, uint32_t mask);

/*<--- Function that configures the toggle pin of the GPIOx --->*/
void GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t pin);

/*<--- Function that configures the input mode of the GPIOx --->*/
void GPIO_ConfigPull(GPIO_TypeDef *GPIOx, uint32_t pinmask, GPIO_MODE_TypeDef mode);

/*<--- Function that configures the output/alternate speed of the GPIOx --->*/
void GPIO_ConfigSpeed(GPIO_TypeDef *GPIOx,uint32_t pinmask, GPIO_Cf_Speed speed);
void GPIO_ConfigAF(GPIO_TypeDef *GPIOx,uint32_t mask, GPIO_MODE_TypeDef mdoe);

/*<--- Function that configures the pin lock of the GPIOx --->*/
void GPIO_LockPin(GPIO_TypeDef *GPIOx, uint16_t pinmask);


#endif  /*   STM32F103xx_GPIO_H    */