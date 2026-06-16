/*********************************************************************************************************************************************************************
 * @file timer.c
 * @brief Timer driver implementation for STM32F103.
 *
 * @details
 * This file provides timer initialization, start/stop, interrupt control,
 * PWM initialization and control, and flag utilities for general-purpose
 * and advanced timers.
 *
 * @author lu-05-cmd
 * @date 2026-04-05
 * @version 0.1.0
 * 
 * @note
 * Uses direct register access; assumes `TIM_TypeDef` and related masks are
 * defined in the included header.
 *
 *********************************************************************************************************************************************************************/

/*INCLUDE============================================================================================================================================================*/
#include <stdint.h>
#include "../Inc/timer.h"



/*********************************************************************************************************************************************************************
 * @brief Initialize TIMx with specified prescaler and auto-reload values
 * 
 * @param TIMx: Pointer to TIM peripheral (e.g., TIM1, TIM2, etc.)
 * @param psc: Prescaler value
 * @param arr: Auto-reload value
 * 
 * @note 
 * This function configures the specified TIM peripheral with the given prescaler and auto-reload values.
 */
void TIM_Init(TIM_TypeDef *TIMx, uint16_t psc, uint16_t arr)
{
    TIMx->CR1 &= ~TIM_CR1_CEN_MASK;
    TIMx->CR1 |= TIM_CR1_ARPE_MASK;

    TIMx->PSC = psc;
    TIMx->ARR = arr;
    
    TIMx->EGR |= TIM_EGR_UG_MASK;
    TIMx->CNT = 0;
}


/*********************************************************************************************************************************************************************
 * @brief DeInitialize TIMx to default state
 * 
 * @param TIMx: Pointer to TIM peripheral (e.g., TIM1, TIM2, etc.)
 * 
 * @note 
 * This function resets all registers of the specified TIM peripheral to their default values, effectively deinitializing it. 
 * It also generates an update event to apply the changes immediately.
 */
void TIM_DeInit(TIM_TypeDef *TIMx)
{
    TIMx->CR1   = 0;
    TIMx->CR2   = 0;

    TIMx->SMCR  = 0;
    TIMx->DIER  = 0;
    TIMx->CCMR1 = 0;
    TIMx->CCMR2 = 0;
    TIMx->CCER  = 0;

    TIMx->CCR1  = 0;
    TIMx->CCR2  = 0;
    TIMx->CCR3  = 0;
    TIMx->CCR4  = 0;

    TIMx->PSC   = 0;
    TIMx->ARR   = 0xFFFF;
    TIMx->CNT   = 0;

    TIMx->SR    = 0;
    TIMx->EGR   = TIM_EGR_UG_MASK;
}


/*********************************************************************************************************************************************************************
 * @brief Start/Stop TIMx counter
 * 
 * @param TIMx: Pointer to TIM peripheral (e.g., TIM1, TIM2, etc.)
 * 
 * @note 
 * This function starts or stops the specified TIM peripheral counter.
 */
void TIM_Start(TIM_TypeDef *TIMx)
{
    TIMx->CR1 |= TIM_CR1_CEN_MASK; 
}

void TIM_Stop(TIM_TypeDef *TIMx)
{
    TIMx->SR &= ~TIM_SR_UIF_MASK;
    TIMx->DIER &= ~TIM_DIER_UIE_MASK;
    TIMx->CCMR1  = TIM_CR1_CEN_MASK;
}


/*********************************************************************************************************************************************************************
 * @brief Enable/Disable TIMx interrupts
 * 
 * @param TIMx: Pointer to TIM peripheral (e.g., TIM1, TIM2, etc.)
 * @param interrupt: Interrupt to enable/disable (e.g., TIM_DIER_UIE_MASK for update interrupt)
 * 
 * @note 
 * This function enables or disables the specified interrupt for the given TIM peripheral.
 * The `interrupt` parameter should be a valid interrupt mask defined in the header (e.g., `TIM_DIER_UIE_MASK` for update interrupt).
 */
void TIM_EnableIT(TIM_TypeDef *TIMx, uint32_t interrupt)
{
    TIMx->DIER |= TIM_DIER_UIE_MASK;
}

void TIM_DisableIT(TIM_TypeDef *TIMx, uint32_t interrupt)
{
    TIMx->DIER |= ~TIM_DIER_UIE_MASK;
}

/*********************************************************************************************************************************************************************
 * @brief Clear/Get TIMx status flags
 * 
 * @param TIMx: Pointer to TIM peripheral (e.g., TIM1, TIM2, etc.)
 * @param flag_mask: Flag mask to clear or get (e.g., TIM_SR_UIF_MASK for update interrupt flag)
 * 
 * @note 
 * The `flag_mask` parameter should be a valid flag mask defined in the header (e.g., `TIM_SR_UIF_MASK` for update interrupt flag).
 * The `TIM_ClearFlag` function clears the specified flags in the status register, while the `TIM_GetFlag` function checks if the specified flags are set and returns 1 if they are, or 0 if they are not.
 */
void TIM_ClearFlag(TIM_TypeDef *TIMx, uint32_t flag_mask)
{

    TIMx->SR &=  ~flag_mask;
}

uint32_t TIM_GetFlag(TIM_TypeDef *TIMx, uint32_t flag_mask)
{
    return (TIMx->SR & (flag_mask)) ? 1U : 0U;
}

/*********************************************************************************************************************************************************************
 * @brief Set TIMx counter and prescaler values
 * 
 * @param TIMx: Pointer to TIM peripheral (e.g., TIM1, TIM2, etc.)
 * @param cnt: Counter value to set
 * @param psc: Prescaler value to set
 * 
 * @note 
 * This function allows you to set the counter and prescaler values for the specified TIM peripheral. The `cnt` parameter sets the current counter value, while the `psc` parameter sets the prescaler value. This can be useful for dynamically adjusting the timer's counting behavior during runtime.
 * The `TIM_SetCounter` function sets the counter register (CNT) to the specified value, while the `TIM_SetPrescaler` function sets the prescaler register (PSC) to the specified value. After calling these functions, the timer will use the new counter and prescaler values for its counting operations.
 * The 'TIM_SetPrescaler' function allows you to change the timer's counting frequency by adjusting the prescaler value, while the 'TIM_SetCounter' function allows you to set the current count value, which can be useful for synchronizing timers or implementing specific timing behaviors.
 */
void TIM_SetCounter(TIM_TypeDef *TIMx, uint32_t cnt)
{
    TIMx->CNT = cnt;
}

void TIM_SetPrescaler(TIM_TypeDef *TIMx, uint32_t psc)
{
    TIMx->PSC = psc;
}

/*********************************************************************************************************************************************************************
 * @brief Initialize TIMx for PWM mode on a specific channel
 * 
 * @param TIMx: Pointer to TIM peripheral (e.g., TIM1, TIM2, etc.)
 * @param psc: Prescaler value
 * @param arr: Auto-reload value
 * @param channel: Timer channel (e.g., TIM_CH1, TIM_CH2, etc.)
 * 
 * @note 
 * This function configures the specified TIM peripheral for PWM mode on the given channel. It sets the prescaler and auto-reload values, configures the capture/compare mode for the specified channel to PWM mode 1, and enables the preload feature for that channel. The `channel` parameter should be one of the defined `TIM_Channel_t` values (e.g., `TIM_CH1`, `TIM_CH2`, etc.) to specify which channel to configure for PWM output.
 * After calling this function, the specified channel of the TIM peripheral will be configured for PWM mode, and you can start the PWM output using the `TIM_PWM_Start` function. The duty cycle of the PWM signal can be adjusted by changing the corresponding capture/compare register (e.g., CCR1 for TIM_CH1) to a value between 0 and `arr`, where `arr` is the auto-reload value set in this function.
 * The `TIM_PWM_Init` function sets up the timer for PWM output, but it does not start the timer or enable the PWM output on the specified channel. You will need to call `TIM_PWM_Start` after this function to begin generating the PWM signal on the configured channel.
 */
void TIM_PWM_Init(TIM_TypeDef *TIMx, uint32_t psc, uint32_t arr,TIM_Channel_t channel)
{ 
    TIMx->CR1 &= ~TIM_CR1_CEN_MASK ;
 
    TIMx->PSC = psc;
    TIMx->ARR = arr;

    if(channel == TIM_CH1)
    {
        TIMx->CCMR1 &= ~(1U << TIM_CCMR1_CC1S_POS);
        TIMx->CCMR1 |= (6U << TIM_CCMR1_OC1M_POS);      
        TIMx->CCMR1 |= (1U << TIM_CCMR1_OC1PE_POS);     
        TIMx->CCR1 = 0;
    }
    if(channel == TIM_CH2)
    {
        TIMx->CCMR1 &= ~(1U << TIM_CCMR1_CC2S_POS);
        TIMx->CCMR1 |=  (6U << TIM_CCMR1_OC2M_POS);       
        TIMx->CCMR1 |=  (1U << TIM_CCMR1_OC2PE_POS);      
        TIMx->CCR2 = 0; 
    }

    if(channel == TIM_CH3)
    {
        TIMx->CCMR2 &= ~(1U << TIM_CCMR2_CC3S_POS);
        TIMx->CCMR2 |=  (6U << TIM_CCMR2_OC3M_POS);  
        TIMx->CCMR2 |=  (1U << TIM_CCMR2_OC3PE_POS); 
        TIMx->CCR3 = 0;
    }

    if(channel == TIM_CH4)
    {
        TIMx->CCMR2 &= ~(1U << TIM_CCMR2_CC4S_POS);
        TIMx->CCMR2 |=  (6U << TIM_CCMR2_OC4M_POS);  
        TIMx->CCMR2 |=  (1U << TIM_CCMR2_OC4PE_POS); 
        TIMx->CCR4 = 0;
    }
    TIMx->EGR |= TIM_EGR_UG_MASK;
}


/*********************************************************************************************************************************************************************
 * @brief Start/Stop PWM output on TIMx for a specific channel
 * 
 * @param TIMx: Pointer to TIM peripheral (e.g., TIM1, TIM2, etc.)
 * @param channel: Timer channel (e.g., TIM_CH1, TIM_CH2, etc.)
 * 
 * @note 
 * This function starts or stops the PWM output on the specified channel of the TIM peripheral.
 */
void TIM_PWM_Start(TIM_TypeDef *TIMx, TIM_Channel_t channel)
{
    TIMx->BDTR |= TIM_BDTR_MOE_MASK;

    if(channel == TIM_CH1)      TIMx->CCER |= TIM_CCER_CC1E_MASK;
    else if(channel == TIM_CH2)     TIMx->CCER |= TIM_CCER_CC2E_MASK;
    else if(channel == TIM_CH3)     TIMx->CCER |= TIM_CCER_CC3E_MASK;
    else if(channel == TIM_CH4)     TIMx->CCER |= TIM_CCER_CC4E_MASK;

    TIMx->CR1 |= TIM_CR1_CEN_MASK;
}

void TIM_PWM_Stop(TIM_TypeDef *TIMx, TIM_Channel_t channel)
{
    if(channel == TIM_CH1)
    {
        TIMx->CR1 &= ~TIM_CR1_CEN_MASK;
        TIMx->CCER &= 0;
        TIMx->BDTR &= ~TIM_BDTR_MOE_MASK;
    }    
}

/* End of refactored TIME */