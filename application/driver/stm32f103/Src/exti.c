/*********************************************************************************************************************************************************************
 * @file exti.c
 * @brief EXTI driver implementation for STM32F103xx.
 *
 * @details
 * This file provides register-level EXTI configuration for STM32F103xx devices,
 * including trigger selection, interrupt/event mode setup, callback registration,
 * and IRQ handling.
 *
 * @author lu-05-cmd
 * @date 2026-05-10
 * @version 0.1.0
 *
 * @note
 * The implementation uses direct register access and assumes the EXTI peripheral
 * register layout is defined in the included header.
 *********************************************************************************************************************************************************************/


/*INCLUDE============================================================================================================================================================*/
#include <stdint.h>
#include <stddef.h> 
#include "../Inc/exti.h"
#include "../Inc/afio.h"
#include "../Inc/rcc.h"
#include "../../../core/nvic/nvic.h"



/**
 * @brief Callback table indexed by EXTI line number.
 */
static void (*callback_table[EXTI_CALLBACK_TABLE_SIZE])(void);


/**
 * @brief Return the bit mask associated with a given EXTI line.
 *
 * @param line EXTI line number.
 * @return A 32-bit mask with the requested line bit set.
 */
static inline uint32_t EXTI_GetMask(EXTI_Line_t line)
{
    return (1UL << line);
}

/**
 * @brief Enable the interrupt request for the specified EXTI line.
 *
 * @param line EXTI line number to enable.
 */
static inline void EXTI_EnableInterrupt(EXTI_Line_t line)
{
    EXTI->IMR |= EXTI_GetMask(line);
}

/**
 * @brief Disable the interrupt request for the specified EXTI line.
 *
 * @param line EXTI line number to disable.
 */
static inline void EXTI_DisableInterrupt(EXTI_Line_t line)
{
    EXTI->IMR &= ~EXTI_GetMask(line);
}

/**
 * @brief Enable the event request for the specified EXTI line.
 *
 * @param line EXTI line number to enable.
 */
static inline void EXTI_EnableEvent(EXTI_Line_t line)
{
    EXTI->EMR |= EXTI_GetMask(line);
}

/**
 * @brief Disable the event request for the specified EXTI line.
 *
 * @param line EXTI line number to disable.
 */
static inline void EXTI_DisableEvent(EXTI_Line_t line)
{
    EXTI->EMR &= ~EXTI_GetMask(line);
}

/**
 * @brief Check whether the provided EXTI line number is valid.
 *
 * @param line EXTI line number to validate.
 * @return 1 if the line is valid, otherwise 0.
 */
static inline uint8_t EXTI_IsValidLine(EXTI_Line_t line)
{
    return (line <= EXTI_LINE_15);
}

/**
 * @brief Check whether the provided EXTI trigger configuration is valid.
 *
 * @param trigger Trigger mode to validate.
 * @return 1 if the trigger is valid, otherwise 0.
 */
static inline uint8_t EXTI_IsValidTrigger(EXTI_Trigger_t trigger)
{
    return (trigger == EXTI_TRIGGER_NONE ||
            trigger == EXTI_TRIGGER_RISING ||
            trigger == EXTI_TRIGGER_FALLING ||
            trigger == EXTI_TRIGGER_BOTH);
}

/**
 * @brief Check whether the provided EXTI mode is valid.
 *
 * @param mode Operating mode to validate.
 * @return 1 if the mode is valid, otherwise 0.
 */
static inline uint8_t EXTI_IsValidMode(EXTI_Mode_t mode)
{
    return (mode == EXTI_MODE_INTERRUPT || mode == EXTI_MODE_EVENT);
}

/**
 * @brief Check whether the pending bit is set for a specific EXTI line.
 *
 * @param line EXTI line number to inspect.
 * @return 1 if the pending bit is set, otherwise 0.
 */
static inline uint8_t EXTI_IsPending(EXTI_Line_t line)
{
    return (EXTI->PR & (1U << line)) ? 1U : 0U;
}

/**
 * @brief Configure the edge trigger selection for the specified EXTI line.
 *
 * @param trigger Trigger selection to apply.
 * @param line EXTI line number to configure.
 * @return EXTI_STATUS_OK on success, otherwise an error status.
 */
static inline EXTI_Status_t EXTI_ConfigTrigger(EXTI_Trigger_t trigger, EXTI_Line_t line)
{
    uint32_t mask = EXTI_GetMask(line);

    if(!EXTI_IsValidLine(line))
    {
        return EXTI_STATUS_INVALID_LINE;
    }

    switch(trigger)
    {
        case EXTI_TRIGGER_NONE:
            EXTI->RTSR &= ~mask;
            EXTI->FTSR &= ~mask;
            break;

        case EXTI_TRIGGER_RISING:
            EXTI->RTSR |= mask;
            EXTI->FTSR &= ~mask;
            break;

        case EXTI_TRIGGER_FALLING:
            EXTI->RTSR &= ~mask;
            EXTI->FTSR |= mask;
            break;

        case EXTI_TRIGGER_BOTH:
            EXTI->RTSR |= mask;
            EXTI->FTSR |= mask;
            break;

        default:
            return EXTI_STATUS_INVALID_TRIGGER;
    }

    return EXTI_STATUS_OK;
}
/*====================================================================================================================================================================
|                             ADC Configuration function global
=====================================================================================================================================================================*/
/**
 * @brief Initialize an EXTI line with the requested configuration.
 *
 * @param config Pointer to the EXTI configuration structure.
 * @return EXTI_STATUS_OK on success, or an error code if the input parameters are invalid.
 * @note The function configures the AFIO mapping, trigger mode, interrupt/event mode,
 *       clears pending flags, and enables the appropriate NVIC IRQ channel.
 */
EXTI_Status_t EXTI_Init(const EXTI_Config_t *config)
{
    uint32_t mask;
    EXTI_Status_t status;

    /* 1. Check NULL */
    if(config == NULL)
        return EXTI_STATUS_INVALID_PARAM;

    /* 2. Check line */
    if(!EXTI_IsValidLine(config->line))
        return EXTI_STATUS_INVALID_LINE;

    /* 3. Check trigger */
    if(!EXTI_IsValidTrigger(config->trigger))
        return EXTI_STATUS_INVALID_TRIGGER;

    /* 4. Check mode */
    if(!EXTI_IsValidMode(config->mode))
        return EXTI_STATUS_INVALID_MODE;

    /* 5. Enable AFIO clock */
    RCC_EnableAFIOClock();

    /* 6. Configure EXTICR */
    AFIO_EXTIConfig((AFIO_EXTI_LINE)config->line, config->port);

    /* 7. Disable line before configuration */
    mask = EXTI_GetMask(config->line);
    EXTI->IMR &= ~mask;
    EXTI->EMR &= ~mask;

    /* 8. Configure trigger */
    status = EXTI_ConfigTrigger(config->trigger, config->line);
    if(status != EXTI_STATUS_OK)
        return status;

    /* 9. Configure mode */
    if(config->mode == EXTI_MODE_INTERRUPT)
    {
        EXTI_EnableInterrupt(config->line);
        EXTI_DisableEvent(config->line);
    }
    else
    {
        EXTI_EnableEvent(config->line);
        EXTI_DisableInterrupt(config->line);
    }

    /* 10. Clear pending bit */
    EXTI->PR = mask;

    /* 11. Enable NVIC if interrupt mode */
    if(config->mode == EXTI_MODE_INTERRUPT)
    {
        switch(config->line)
        {
            case EXTI_LINE_0: NVIC_EnableIRQ(EXTI0_IRQn); break;
            case EXTI_LINE_1: NVIC_EnableIRQ(EXTI1_IRQn); break;
            case EXTI_LINE_2: NVIC_EnableIRQ(EXTI2_IRQn); break;
            case EXTI_LINE_3: NVIC_EnableIRQ(EXTI3_IRQn); break;
            case EXTI_LINE_4: NVIC_EnableIRQ(EXTI4_IRQn); break;
            case EXTI_LINE_5:
            case EXTI_LINE_6:
            case EXTI_LINE_7:
            case EXTI_LINE_8:
            case EXTI_LINE_9:
                NVIC_EnableIRQ(EXTI9_5_IRQn);
                break;
            case EXTI_LINE_10:
            case EXTI_LINE_11:
            case EXTI_LINE_12:
            case EXTI_LINE_13:
            case EXTI_LINE_14:
            case EXTI_LINE_15:
                NVIC_EnableIRQ(EXTI15_10_IRQn);
                break;
            default:
                break;
        }
    }

    return EXTI_STATUS_OK;
}

/**
 * @brief Reset EXTI configuration and clear all registered callbacks.
 *
 * @return EXTI_STATUS_OK when the de-initialization completes.
 * @note This function disables all EXTI lines, clears pending bits, disables
 *       related NVIC interrupts, and removes all registered callbacks.
 */
EXTI_Status_t EXTI_DeInit(void)
{
    uint32_t mask = 0x0000FFFFU;

    EXTI->IMR &= ~mask;
    EXTI->EMR &= ~mask;
    EXTI->RTSR &= ~mask;
    EXTI->FTSR &= ~mask;
    EXTI->PR = mask;

    NVIC_DisableIRQ(EXTI0_IRQn);
    NVIC_DisableIRQ(EXTI1_IRQn);
    NVIC_DisableIRQ(EXTI2_IRQn);
    NVIC_DisableIRQ(EXTI3_IRQn);
    NVIC_DisableIRQ(EXTI4_IRQn);
    NVIC_DisableIRQ(EXTI9_5_IRQn);
    NVIC_DisableIRQ(EXTI15_10_IRQn);

    for(uint16_t index = 0; index < EXTI_CALLBACK_TABLE_SIZE; index++)
    {
        callback_table[index] = NULL;       
    }
    return EXTI_STATUS_OK;
}

/**
 * @brief Register a callback function for a specific EXTI line.
 *
 * @param line EXTI line number to associate with the callback.
 * @param callback Function pointer to invoke when the EXTI line triggers.
 * @return EXTI_STATUS_OK on success, or EXTI_STATUS_INVALID_PARAM/EXTI_STATUS_INVALID_LINE on failure.
 * @note The callback is stored in a line-based lookup table and is invoked by the IRQ handler.
 */
EXTI_Status_t EXTI_RegisterCallback(EXTI_Line_t line, void (*callback)(void))
{
    if(callback == NULL)
    {
        return EXTI_STATUS_INVALID_PARAM;
    }
    if(!(EXTI_IsValidLine(line)))
    {
        return EXTI_STATUS_INVALID_LINE;
    }
    callback_table[line] = callback;
    return EXTI_STATUS_OK;
}

/**
 * @brief Remove the callback associated with a specific EXTI line.
 *
 * @param line EXTI line number whose callback should be cleared.
 * @return EXTI_STATUS_OK on success, or EXTI_STATUS_INVALID_LINE on failure.
 * @note This only clears the callback entry; it does not disable the EXTI line itself.
 */
EXTI_Status_t EXTI_UnregisterCallback(EXTI_Line_t line)
{

    if(!(EXTI_IsValidLine(line)))
    {
        return EXTI_STATUS_INVALID_LINE;
    }
    callback_table[line] = NULL;

    return EXTI_STATUS_OK;
}

/**
 * @brief Handle an EXTI interrupt for the specified line.
 *
 * @param line EXTI line number that triggered the interrupt.
 * @return EXTI_STATUS_OK on success, or EXTI_STATUS_INVALID_LINE if the line is invalid.
 * @note The pending bit is cleared before invoking the registered callback to prevent repeated handling.
 */
EXTI_Status_t EXTI_IRQ_Handler(EXTI_Line_t line)
{
    if(!(EXTI_IsValidLine(line)))
    {
        return EXTI_STATUS_INVALID_LINE;
    }
    if(!(EXTI_IsPending(line)))
    {
        return EXTI_STATUS_OK;
    }
    EXTI->PR = (1U << line);

    if(!(callback_table[line] == NULL))
    {
        callback_table[line]();
    }
    return EXTI_STATUS_OK;
}

/* End of refactored EXTI */