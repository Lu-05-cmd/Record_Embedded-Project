/**********************************************************************************************************************************************************************
 * @file    scb.h
 * @brief   System Control Block (SCB) definitions for STM32F103xx
 *
 * @details
 * This file provides SCB register definitions and API declarations for
 * configuring the application interrupt and reset control register (AIRCR)
 * and priority grouping on STM32F103xx devices.
 *
 * @author  lu-05-cmd
 * @date    2026-04-19
 * @version 0.1.0
 *
 * @note
 *   - Based on ARM Cortex-M3 SCB register layout.
 *   - Used for system interrupt priority grouping and reset control.
 **********************************************************************************************************************************************************************/


#ifndef STM32F103xx_SCB_H
#define STM32F103xx_SCB_H

#include <stdint.h>

/*====================================================================================================================================================================
 |                      Application interrupt and reset control register (AIRCR)
 =====================================================================================================================================================================*/
typedef struct
{
    volatile uint32_t AIRCR;   /* Application Interrupt and Reset Control Register */
} SCB_AIRCR_REG_t;


/*====================================================================================================================================================================
 |                       NVIC(Nested Vectored Interrupt Controller) BASE ADDRESS
 =====================================================================================================================================================================*/
#define SCB_BASE 0xE000ED0C
#define SCB_AIRCR ((volatile SCB_AIRCR_REG_t *) SCB_BASE) 


/*====================================================================================================================================================================
 |                                           Mode Define
 =====================================================================================================================================================================*/
#define __SCB_VECT_KEY 0x5FA 

#define SCB_AIRCR_PRIGROUP_POS (8U)

#define SCB_AIRCR_PRIGROUP_MASK (0x7 << SCB_AIRCR_PRIGROUP_POS)


 typedef enum
{
    SCB_PRIORITYGROUP_0 = 0,
    SCB_PRIORITYGROUP_1,
    SCB_PRIORITYGROUP_2,
    SCB_PRIORITYGROUP_3,
    SCB_PRIORITYGROUP_4,
    SCB_PRIORITYGROUP_5,
    SCB_PRIORITYGROUP_6,
    SCB_PRIORITYGROUP_7
} SCB_PriorityGroup_t;


/*====================================================================================================================================================================
|                             Inline Functions
 =====================================================================================================================================================================*/
/* Set the system priority grouping */
void SCB_SetPriorityGroup(SCB_PriorityGroup_t group);

/* Get the current system priority grouping */
uint32_t SCB_GetPriorityGroup(void);


#endif  /*  STM32F103xx_SCB_H  */