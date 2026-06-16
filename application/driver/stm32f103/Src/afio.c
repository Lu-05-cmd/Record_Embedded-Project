/*********************************************************************************************************************************************************************
 * @file afio.c
 * @brief AFIO driver implementation for STM32F103.
 *
 * @details
 * This file provides AFIO peripheral initialization, debug port configuration,
 * EXTI line mapping to GPIO ports, and peripheral remapping for SPI, I2C,
 * USART, and timer functions.
 *
 * @author lu-05-cmd
 * @date January 2026
 * @version 0.1.0
 * 
 * @note
 * Bare-metal driver compatible with STM32F103xx series; uses direct register
 * access via `AFIO` structure and related masks defined in the included header.
 *********************************************************************************************************************************************************************/

/*INCLUDE============================================================================================================================================================*/
#include <stdio.h>
#include "../Inc/afio.h"





/*********************************************************************************************************************************************************************
 * @brief Configure the debug port settings for SWJ (Serial Wire/JTAG) functionality
 * 
 * @param mode: Desired debug port configuration mode (AFIO_DEBUG_FULL_SWJ, AFIO_DEBUG_NO_JTAG, or AFIO_DEBUG_DISABLE)
 * 
 * @note
 * This function allows you to configure the debug port settings for the SWJ (Serial Wire/JTAG) functionality on the STM32F103 microcontroller. The `mode` parameter specifies the desired configuration mode, which can be one of the following:
 * - `AFIO_DEBUG_FULL_SWJ`: Enables full SWJ (Serial Wire/JTAG) functionality, allowing both JTAG and SWD debugging.
 * - `AFIO_DEBUG_NO_JTAG`: Disables JTAG functionality while keeping SWD enabled, allowing for SWD debugging only.
 * - `AFIO_DEBUG_DISABLE`: Disables both JTAG and SWD debugging, effectively disabling the debug port.
 *********************************************************************************************************************************************************************/
void AFIO_DebugPortConfig(AFIO_Debug_Mode mode)
{
    AFIO->MAPR &= ~(0x3 << AFIO_MAPR_SWJ_CFG_POS);
    AFIO->MAPR |= ((uint32_t)mode << AFIO_MAPR_SWJ_CFG_POS);
}


/*********************************************************************************************************************************************************************
 * @brief Configure the EXTI line mapping to GPIO ports
 *
 * @param line: EXTI line number (0-15) to configure
 * @param port_mask: GPIO port mask to map the EXTI line to (e.g., 0 for GPIOA, 1 for GPIOB, etc.)
 *  
 * @note 
 * This function allows you to configure the mapping of EXTI lines to specific GPIO ports on the STM32F103 microcontroller. 
 * The `line` parameter specifies which EXTI line (0-15) you want to configure, while the `port_mask` parameter indicates which GPIO port to map that EXTI line to. For example, if you want to map EXTI line 0 to GPIOA, you would pass `line = EXTI_LINE_0` and `port_mask = 0`. 
 * If you want to map EXTI line 1 to GPIOB, you would pass `line = EXTI_LINE_1` and `port_mask = 1`, and so on. This configuration is essential for setting up external interrupts on specific pins of the microcontroller.
 * The function calculates the appropriate register and bit positions based on the EXTI line number and updates the AFIO->EXTICR registers to establish the desired mapping between EXTI lines and GPIO ports. 
 *********************************************************************************************************************************************************************/
void AFIO_EXTIConfig(AFIO_EXTI_LINE line, uint32_t port_mask)
{
  uint8_t reg = line >> 2;              
  uint16_t shift = (line & 0x3) << 2;

  AFIO->EXTICR[reg] &= ~(0xF << shift); 
  AFIO->EXTICR[reg] |= (port_mask << shift);
}


/*********************************************************************************************************************************************************************
 * @brief Configure peripheral remapping for SPI1, I2C1, USART1, USART2, USART3, TIM1, TIM2, and TIM3
 * 
 * @param func_2_t: Desired remapping configuration for the specified peripheral (e.g., AFIO_NO_REMAP, AFIO_REMAP, etc.)
 * 
 *  
 * @note
 * This set of functions allows you to configure the remapping of various peripherals (SPI1, I2C1, USART1, USART2, USART3, TIM1, TIM2, and TIM3) on the STM32F103 microcontroller. 
 * Each function corresponds to a specific peripheral and takes a parameter that specifies the desired remapping configuration. 
 * For example, the `AFIO_SPI1Remap` function allows you to remap the SPI1 peripheral to alternate pins, while the `AFIO_USART3Remap` function provides options for partial or full remapping of USART3. 
 * The `func_2_t` parameter can take values such as `AFIO_NO_REMAP`, `AFIO_REMAP`, or specific remapping options defined in the header for each peripheral.
 * The implementation of each function involves clearing the relevant bits in the AFIO->MAPR register and then setting the new remapping configuration based on the provided parameter. 
 * This allows you to customize the pin assignments for these peripherals according to your application's requirements.
 * The remapping options for each peripheral are defined in the header file and may include options for no remapping, partial remapping, or full remapping, depending on the specific peripheral and its capabilities. 
 * By using these functions, you can easily configure the pin assignments for your peripherals without needing to manually manipulate the AFIO registers directly in your application code.
 *********************************************************************************************************************************************************************/
void AFIO_SPI1Remap(Function_2Remap_t func_2_t)
{
    AFIO->MAPR &= ~AFIO_MAPR_SPI1_REMAP_MASK;
    switch(func_2_t)
    {
        case AFIO_NO_REMAP: break;
        case AFIO_REMAP   : AFIO->MAPR |= AFIO_MAPR_SPI1_REMAP_MASK;    break;
        default : break;
    }
}

void AFIO_I2C1Remap(Function_2Remap_t status)
{
    AFIO->MAPR &= ~AFIO_MAPR_I2C1_REMAP_MASK;                 
    switch(status)
    {
        case AFIO_NO_REMAP: break;
        case AFIO_REMAP   : AFIO->MAPR |= AFIO_MAPR_I2C1_REMAP_MASK;    break;
        default : break;
    }
}

void AFIO_USART1Remap(Function_2Remap_t status)
{
    AFIO->MAPR &= ~AFIO_MAPR_USART1_REMAP_MASK; 
    switch(status)
    {
        case AFIO_NO_REMAP: break;
        case AFIO_REMAP   : AFIO->MAPR |= AFIO_MAPR_USART1_REMAP_MASK;  break;
        default : 
            break;
    }
}

void AFIO_USART2Remap(Function_2Remap_t status)
{
    AFIO->MAPR &= ~AFIO_MAPR_USART2_REMAP_MASK; 
    switch(status)
    {
        case AFIO_NO_REMAP: break;
        case AFIO_REMAP   : AFIO->MAPR |= AFIO_MAPR_USART2_REMAP_MASK;     break;
        default : break;
    }
}

void AFIO_USART3Remap(Function_RemapUSART3_t func_usart3_t)
{
    AFIO->MAPR &= ~(0x3U << AFIO_MAPR_USART3_REMAP_POS); 
    switch(func_usart3_t)
    {
        case AFIO_USART3_NO_REMAP     : break;
        case AFIO_USART3_PARTIAL_REMAP: AFIO->MAPR |= (0x1U << AFIO_MAPR_USART3_REMAP_POS);  break;
        case AFIO_USART3_FULL_REMAP   : AFIO->MAPR |= (0x3U << AFIO_MAPR_USART3_REMAP_POS);  break;
        default :
            break;
    }
}


void AFIO_TIM1Remap(Function_RemapTIM1_t func_tim1_t)
{
    AFIO->MAPR &= ~(0x3U << AFIO_MAPR_TIM1_REMAP_POS);
    switch(func_tim1_t)
    {
        case AFIO_TIM1_NO_REMAP      : break;
        case AFIO_TIM1_PARTIAL_REMAP : AFIO->MAPR |= (0x1U << AFIO_MAPR_TIM1_REMAP_POS);    break;
        case AFIO_TIM1_FULL_REMAP    : AFIO->MAPR |= (0x3U << AFIO_MAPR_TIM1_REMAP_POS);    break;
        default : 
            break;
    }
}

void AFIO_TIM2Remap(Function_RemapTIM2_t func_tim2_t)
{ 
    AFIO->MAPR &= ~(0x3U << AFIO_MAPR_TIM2_REMAP_POS);
    switch(func_tim2_t)
    {
        case AFIO_TIM2_NO_REMAP       : break;
        case AFIO_TIM2_PARTIAL1_REMAP : AFIO->MAPR |= (0x1U << AFIO_MAPR_TIM2_REMAP_POS);    break;
        case AFIO_TIM2_PARTIAL2_REMAP : AFIO->MAPR |= (0x2U << AFIO_MAPR_TIM2_REMAP_POS);    break;
        case AFIO_TIM2_FULL_REMAP     : AFIO->MAPR |= (0x3U << AFIO_MAPR_TIM2_REMAP_POS);    break;
        default:    
            break;
    }
}

void AFIO_TIM3Remap(Function_RemapTIM3_t func_tim3_t)
{
    AFIO->MAPR &= ~(0x3U << AFIO_MAPR_TIM3_REMAP_POS);
    switch(func_tim3_t)
    {
        case AFIO_TIM3_NO_REMAP     :   break;
        case AFIO_TIM3_PARTIAL_REMAP:   AFIO->MAPR |= (0x2U << AFIO_MAPR_TIM3_REMAP_POS);   break;
        case AFIO_TIM3_FULL_REMAP   :   AFIO->MAPR |= (0x3U << AFIO_MAPR_TIM3_REMAP_POS);   break;
        default:
            break;
    }
}

void AFIO_TIM4Remap(Function_2Remap_t func_tim3_t)
{ 
    AFIO->MAPR &= ~(AFIO_MAPR_TIM4_REMAP_MASK); 
    switch(func_tim3_t)
    {
        case AFIO_NO_REMAP:  break;
        case AFIO_REMAP   :  AFIO->MAPR |= (AFIO_MAPR_TIM4_REMAP_MASK); break;
        default:
            break;
    }
}

/* End of refactored AFIO  */