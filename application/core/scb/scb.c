/**
 * @file    scb.c
 * @brief   System Control Block (SCB) helper implementation for STM32F103xx
 *
 * @details
 * This file implements basic SCB functions for configuring the priority
 * grouping used by the Cortex-M3 interrupt controller.
 *
 * @author  lu-05-cmd
 * @date    2026-04-19
 * @version 0.1.0
 */

/*INCLUDE============================================================================================================================================================*/
#include <stdint.h>
#include "../scb/scb.h"


/*********************************************************************************************************************************************************************
 * @brief Set the system priority grouping.
 *
 * @param group Priority group value to write to the AIRCR register.
 *
 * @note The vector key must be written to AIRCR before changing the priority
 *       grouping bits.
 */
void SCB_SetPriorityGroup(SCB_PriorityGroup_t group)
{ 
    SCB_AIRCR->AIRCR |= (__SCB_VECT_KEY << 16); 
    SCB_AIRCR->AIRCR &= ~SCB_AIRCR_PRIGROUP_MASK;
    SCB_AIRCR->AIRCR |= (group << 8);
}

/*********************************************************************************************************************************************************************
 * @brief Get the current system priority grouping value.
 *
 * @return Current priority grouping bits read from the AIRCR register.
 */
uint32_t SCB_GetPriorityGroup(void)
{
    return SCB_AIRCR->AIRCR & SCB_AIRCR_PRIGROUP_MASK;
}