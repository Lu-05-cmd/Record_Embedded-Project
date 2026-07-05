/*********************************************************************************************************************************************************************
 * @file rcc.c
 * @brief RCC driver implementation for STM32F103.
 *
 * @details
 * This file provides RCC peripheral clock control and reset operations for
 * GPIO, AFIO, USART, ADC, DMA, SPI, I2C, TIM, and system clock sources.
 *
 * 
 * @author lu-05-cmd
 * @date 2026-03-29
 * @version 0.1.0
 * 
 * @note
 * The implementation uses direct RCC register access defined in the included
 * header and assumes the RCC structure and masks are provided there.
 *
 *********************************************************************************************************************************************************************/


// Include ============================================================================================================================================================
#include <stdint.h>
#include "../Inc/rcc.h"
#include "../Inc/timer.h"
#include "../Inc/usart.h"
#include "../Inc/spi.h"



/**
 * @brief Lookup table for PLL multiplication factors.
 *
 * @details Maps the RCC PLLMUL field encoding to the corresponding multiplication factor.
 */
static const uint16_t pll_mul_table[16] = 
{
    2, 3, 4, 5,
    6, 7, 8, 9,
    10, 11, 12, 13,
    14, 15, 16, 16 
};

/**
 * @brief Lookup table for AHB prescaler values.
 *
 * @details Maps the RCC HPRE field encoding to the effective AHB division factor.
 */
static const uint16_t ahb_prescaler[16] =
{
    1,1,1,1,
    1,1,1,1,
    2,4,8,16,
    64,128,256,512
};

/**
 * @brief Lookup table for APB prescaler values.
 *
 * @details Maps the RCC PPRE1/PPRE2 field encoding to the effective APB division factor.
 */
static const uint8_t apb_prescaler[8] = 
{
    1, 1, 1, 1,
    2, 4, 8, 16 
};








/*********************************************************************************************************************************************************************
 * @brief Enable/Disable the clock for a specific GPIO port
 * 
 * @param GPIOx: Enumeration value specifying which GPIO port to enable (e.g., RCC_GPIOA, RCC_GPIOB, etc.)
 * 
 * @note
 * This function enables/disables the clock for the specified GPIO port by setting the corresponding bit in the APB2ENR register of the RCC peripheral.
 * Enabling/Disabling the clock is necessary before configuring or using any GPIO pins on that port.
 */
void RCC_EnableGPIOClock(RCC_GPIO_t GPIOx)
{
    if(GPIOx == RCC_GPIOA)  RCC->APB2ENR |= RCC_APB2ENR_GPIOA_MASK;
    else if(GPIOx == RCC_GPIOB) RCC->APB2ENR |= RCC_APB2ENR_GPIOB_MASK;
    else if(GPIOx == RCC_GPIOC) RCC->APB2ENR |= RCC_APB2ENR_GPIOC_MASK;
    else if(GPIOx == RCC_GPIOD) RCC->APB2ENR |= RCC_APB2ENR_GPIOD_MASK;
    else if(GPIOx == RCC_GPIOE) RCC->APB2ENR |= RCC_APB2ENR_GPIOE_MASK;
}

void RCC_DisableGPIOClock(RCC_GPIO_t GPIOx)  
{
    if(GPIOx == RCC_GPIOA)         RCC->APB2ENR &= ~RCC_APB2ENR_GPIOA_MASK;
    else if(GPIOx == RCC_GPIOB)    RCC->APB2ENR &= ~RCC_APB2ENR_GPIOB_MASK;
    else if(GPIOx == RCC_GPIOC)    RCC->APB2ENR &= ~RCC_APB2ENR_GPIOE_MASK;
    else if(GPIOx == RCC_GPIOD)    RCC->APB2ENR &= ~RCC_APB2ENR_GPIOE_MASK;
    else if(GPIOx == RCC_GPIOE)    RCC->APB2ENR &= ~RCC_APB2ENR_GPIOE_MASK;
}


/*********************************************************************************************************************************************************************
 * @brief Enable/Disable the clock for AFIO peripheral
 * 
 * @note
 * This function enables/disables the clock for the AFIO peripheral by setting the corresponding bit in the APB2ENR register of the RCC peripheral.
 * Enabling/Disabling the clock is necessary before configuring or using any AFIO features.
 */
void RCC_EnableAFIOClock()
{
    RCC->APB2ENR &= ~RCC_APB2ENR_AFIO_MASK;
    RCC->APB2ENR |= RCC_APB2ENR_AFIO_MASK;
}

void RCC_DisableAFIOClock()
{
    RCC->APB2ENR &= ~RCC_APB2ENR_AFIO_MASK;
}



/*********************************************************************************************************************************************************************
 * @brief Enable/Disable the clock for a specific USART peripheral
 * 
 * @param USARTx: Enumeration value specifying which USART to enable (e.g., RCC_USART1, RCC_USART2, RCC_USART3).
 * @note
 * This function enables/disables the clock for the USART peripheral by setting the corresponding bit in the APB2ENR or APB1ENR register of the RCC peripheral.
 * Enabling/Disabling the clock is necessary before configuring or using any USART features.
 */
void RCC_EnableUSARTClock(RCC_USART_t USARTx)
{    
    if(USARTx == RCC_USART1)        RCC->APB2ENR |= RCC_APB2ENR_USART1_MASK;
    else if(USARTx == RCC_USART2)   RCC->APB1ENR |= RCC_APB1ENR_USART2_MASK;
    else if(USARTx == RCC_USART3)   RCC->APB1ENR |= RCC_APB1ENR_USART3_MASK;
}

void RCC_DisableUSARTClock(RCC_USART_t USARTx)
{
    if(USARTx == RCC_USART1)        RCC->APB2ENR &= ~RCC_APB2ENR_USART1_MASK;
    else if(USARTx == RCC_USART2)   RCC->APB1ENR &= ~RCC_APB1ENR_USART2_MASK;
    else if(USARTx == RCC_USART3)   RCC->APB1ENR &= ~RCC_APB1ENR_USART3_MASK;
}



/*********************************************************************************************************************************************************************
 * @brief Enable/Disable the clock for a specific ADC peripheral
 * 
 * @param ADCx: Enumeration value specifying which ADC to enable (e.g., RCC_ADC1, RCC_ADC2).
 * 
 * @note
 * This function enables/disables the clock for the AFIO peripheral by setting the corresponding bit in the APB2ENR register of the RCC peripheral.
 * Enabling/Disabling the clock is necessary before configuring or using any AFIO features.
 */
void RCC_EnableADCClock(RCC_ADC_t ADCx)
{
    if(ADCx == RCC_ADC1) RCC->APB2ENR |= RCC_APB2ENR_ADC1_MASK;         
    else if(ADCx == RCC_ADC2) RCC->APB2ENR |= RCC_APB2ENR_ADC2_MASK;    
}

void RCC_DisableADCClock(RCC_ADC_t ADCx)
{
    if(ADCx == RCC_ADC1) RCC->APB2ENR &= ~RCC_APB2ENR_ADC1_MASK;        
    else if(ADCx == RCC_ADC2) RCC->APB2ENR &= ~RCC_APB2ENR_ADC2_MASK;   
}

/*********************************************************************************************************************************************************************
 * @brief Enable/Disable the clock for a specific DMA peripheral
 * 
 * @param DMAx: Enumeration value specifying which DMA controller to enable (e.g., RCC_DMA1, RCC_DMA2).
 * 
 * @note
 * This function enables/disables the clock for the specified DMA controller by setting the corresponding bit in the AHBENR register of the RCC peripheral.
 * Enabling/Disabling the clock is necessary before configuring or using any DMA features.
 */
void RCC_EnableDMAClock(RCC_DMA_t DMAx)
{
    if(DMAx == RCC_DMA1) RCC->AHBENR |= RCC_AHBENR_DMA1_MASK;           
    else if(DMAx == RCC_DMA1) RCC->AHBENR |= RCC_AHBENR_DMA2_MASK;      
}

void RCC_DisableDMAClock(RCC_DMA_t DMAx)
{
    if(DMAx == RCC_DMA2)    RCC->AHBENR &= ~RCC_AHBENR_DMA2_MASK;       
    else if(DMAx == RCC_DMA2)   RCC->AHBENR &= ~RCC_AHBENR_DMA2_MASK;   
}


/*********************************************************************************************************************************************************************
 * @brief Enable/Disable the clock for a specific SPI peripheral
 * 
 * @param SPIx: Enumeration value specifying which SPI to enable (e.g., RCC_SPI1, RCC_SPI2).
 * 
 * @note
 * This function enables/disables the clock for the specified SPI peripheral by setting the corresponding bit in the APB2ENR or APB1ENR register of the RCC peripheral.
 * Enabling/Disabling the clock is necessary before configuring or using any DMA features.
 */
void RCC_EnableSPIClock(RCC_SPI_t SPIx)
{
    if(SPIx == RCC_SPI1)RCC->APB2ENR |= RCC_APB2ENR_SPI1_MASK;         
    else if(SPIx == RCC_SPI2)RCC->APB1ENR |= RCC_APB1ENR_SPI2_MASK;    
}

void RCC_DisableSPIClock(RCC_SPI_t SPIx)
{
    if(SPIx == RCC_SPI1)RCC->APB2ENR &= ~RCC_APB2ENR_SPI1_MASK;      
    else if(SPIx == RCC_SPI2)RCC->APB1ENR &= ~RCC_APB1ENR_SPI2_MASK; 
}

/*********************************************************************************************************************************************************************
 * @brief Enable/Disable the clock for a specific I2C peripheral
 * 
 * @param I2Cx: Enumeration value specifying which I2C to enable (e.g., RCC_I2C1, RCC_I2C2).
 * 
 * @note
 * This function enables/disables the clock for the specified I2C peripheral by setting the corresponding bit in the APB1ENR register of the RCC peripheral.
 * Enabling/Disabling the clock is necessary before configuring or using any I2C features.
 */
void RCC_EnableI2CClock(RCC_I2C_t I2Cx)
{
    
    if(I2Cx == RCC_I2C1)    RCC->APB1ENR |= RCC_APB1ENR_I2C1_MASK;      
    else if(I2Cx == RCC_I2C2)    RCC->APB1ENR |= RCC_APB1ENR_I2C2_MASK; 
}

void RCC_DisableI2CClock(RCC_I2C_t I2Cx)
{
        if(RCC_I2C1)    RCC->APB1ENR &= ~RCC_APB1ENR_I2C1_MASK;      
        else if(RCC_I2C2)   RCC->APB1ENR &= ~RCC_APB1ENR_I2C2_MASK;  
}

/*********************************************************************************************************************************************************************
 * @brief Reset the clock for a specific GPIO port
 * 
 * @param GPIOx: Enumeration value specifying which GPIO port to reset (e.g., RCC_GPIOA, RCC_GPIOB).
 * 
 * @note
 * This function resets the clock for the specified GPIO port by setting the corresponding bit in the APB2RSTR register of the RCC peripheral.
 * Resetting the clock is necessary before configuring or using any GPIO features.
 */
void RCC_ResetGPIO(RCC_GPIO_t GPIOx)
{
     if(GPIOx == RCC_GPIOA){
        RCC->APB2ENR |= RCC_APB2ENR_GPIOA_MASK;
        RCC->APB2RSTR |= RCC_APB2RSTR_GPIOA_RESET_MASK;
    }else if(GPIOx == RCC_GPIOB){
        RCC->APB2ENR |= RCC_APB2ENR_GPIOB_MASK;
        RCC->APB2RSTR |= RCC_APB2RSTR_GPIOB_RESET_MASK;
    }else if(GPIOx == RCC_GPIOC){
        RCC->APB2ENR |= RCC_APB2ENR_GPIOC_MASK;
        RCC->APB2RSTR |= RCC_APB2RSTR_GPIOC_RESET_MASK;
    }else if(GPIOx == RCC_GPIOD){
        RCC->APB2ENR |= RCC_APB2ENR_GPIOD_MASK;
        RCC->APB2RSTR |= RCC_APB2RSTR_GPIOD_RESET_MASK;
    }else if(GPIOx == RCC_GPIOE){
        RCC->APB2ENR |= RCC_APB2ENR_GPIOE_MASK;
        RCC->APB2RSTR |= RCC_APB2RSTR_GPIOE_RESET_MASK;
    }
}


/*********************************************************************************************************************************************************************
 * @brief Reset the clock for AFIO peripheral
 * 
 * @note
 * This function resets the clock for the specified AFIO peripheral by setting the corresponding bit in the APB2RSTR register of the RCC peripheral.
 * Resetting the clock is necessary before configuring or using any AFIO features.
 */
void RCC_ResetAFIO()
{
    RCC->APB2RSTR |= RCC_APB2RSTR_AFIO_RESET_MASK;
}

/*********************************************************************************************************************************************************************
 * @brief Reset the clock for a specific USART peripheral
 * 
 * @param USARTx: Enumeration value specifying which USART controller to reset (e.g., RCC_USART1, RCC_USART2, RCC_USART3).
 * 
 * @note
 * This function resets the clock for the specified USART controller by setting the corresponding bit in the APB1RSTR or APB2RSTR register of the RCC peripheral.
 * Resetting the clock is necessary before configuring or using any USART features.
 */
void RCC_ResetUSART(RCC_USART_t USARTx)
{
    if(USARTx == RCC_USART1)
    {
        RCC->APB2RSTR |= RCC_APB2RSTR_USART1_RESET_MASK;         
        RCC->APB2RSTR &= ~RCC_APB2RSTR_USART1_RESET_MASK;         
    }
    else if(USARTx == RCC_USART2)
    { 
        RCC->APB1RSTR |= RCC_APB1RSTR_USART2_RESET_MASK;         
        RCC->APB1RSTR &= ~RCC_APB1RSTR_USART2_RESET_MASK;          
    }
    else if(USARTx == RCC_USART3)
    {
        RCC->APB1RSTR |= RCC_APB1RSTR_USART3_RESET_MASK;         
        RCC->APB1RSTR &= RCC_APB1RSTR_USART3_RESET_MASK;           
    }
}


/*********************************************************************************************************************************************************************
 * @brief Reset the clock for a specific ADC peripheral
 * 
 * @param ADCx: Enumeration value specifying which ADC controller to reset (e.g., RCC_ADC1, RCC_ADC2).
 * 
 * @note
 * This function resets the clock for the specified ADC controller by setting the corresponding bit in the APB2RSTR register of the RCC peripheral.
 * Resetting the clock is necessary before configuring or using any ADC features.
 */
void RCC_ResetADC(RCC_ADC_t ADCx)
{
    if(ADCx == RCC_ADC1)
    {
        RCC->APB2ENR  |= RCC_APB2ENR_ADC1_MASK;                 // Enable ADC1 Clock
        RCC->APB2RSTR |= RCC_APB2RSTR_ADC1_RESET_MASK;          // Reset ADC1
        RCC->APB2RSTR &= ~RCC_APB2RSTR_ADC1_RESET_MASK;         // Clear ADC1 reset bit
    }
    else if(ADCx == RCC_ADC2)
    {
        RCC->APB2ENR    |= RCC_APB2ENR_ADC2_MASK;               // Enable ADC2 Clock
        RCC->APB2RSTR   |= RCC_APB2RSTR_ADC2_RESET_MASK;        // Reset ADC2
        RCC->APB2RSTR   &= ~RCC_APB2RSTR_ADC2_RESET_MASK;       // Clear ADC2 reset bit
    }
}


/*********************************************************************************************************************************************************************
 * @brief Reset the clock for a specific SPI peripheral
 * 
 * @param SPIx: Enumeration value specifying which SPI controller to reset (e.g., RCC_SPI1, RCC_SPI2).
 * 
 * @note
 * This function resets the clock for the specified SPI controller by setting the corresponding bit in the APB2RSTR or APB1RSTR register of the RCC peripheral.
 * Resetting the clock is necessary before configuring or using any SPI features.
 */
void RCC_ResetSPI(RCC_SPI_t SPIx)
{
    
    if(SPIx == RCC_SPI1){
        RCC->APB2RSTR |= RCC_APB2RSTR_SPI1_RESET_MASK;           // Reset SPI1
        RCC->APB2RSTR &= ~RCC_APB2RSTR_SPI1_RESET_MASK;          // Turn off Reset
    }
    else if(SPIx == RCC_SPI2){
        RCC->APB1RSTR = RCC_APB1RSTR_SPI2_RESET_MASK;            // Reset SPI2
        RCC->APB1RSTR = ~RCC_APB1RSTR_SPI2_RESET_MASK;           // Turn off Reset
    }
}

/*********************************************************************************************************************************************************************
 * @brief Reset the clock for a specific I2C peripheral
 * 
 * @param I2Cx: Enumeration value specifying which I2C controller to reset (e.g., RCC_I2C1, RCC_I2C2).
 * 
 * @note
 * This function resets the clock for the specified I2C controller by setting the corresponding bit in the APB1RSTR register of the RCC peripheral.
 * Resetting the clock is necessary before configuring or using any I2C features.
 */
void RCC_ResetI2C(RCC_I2C_t I2Cx)
{
    
    if(I2Cx == RCC_I2C1){
        RCC->APB1RSTR |= RCC_APB1RSTR_I2C1_RESET_MASK;             
        RCC->APB1RSTR &= ~RCC_APB1RSTR_I2C1_RESET_MASK;            
    }        
    else if(I2Cx == RCC_I2C2){
        RCC->APB1RSTR |= RCC_APB1RSTR_I2C1_RESET_MASK;             
        RCC->APB1RSTR &= ~RCC_APB1RSTR_I2C2_RESET_MASK;            
    }        
}


/*********************************************************************************************************************************************************************
 * @brief Enable/Disable the HSI/HSE (High-Speed Internal/External) clock
 * 
 * @note
 * These functions enable or disable the HSI and HSE clock sources by setting or clearing the corresponding bits in the CR register of the RCC peripheral.
 * Enabling the clock is necessary before using it as a system clock source or for PLL input, while disabling it can save power when it's not needed.
 */
void RCC_EnableHSI(void)
{
    RCC->CR |= RCC_CR_HSI_MASK;
    while(!RCC->CR & (RCC_CR_HSIRDY_MASK));
}
void RCC_DisableHSI(void)
{
    RCC->CR &= ~RCC_CR_HSI_MASK;
}

void RCC_EnableHSE(void)
{
    RCC->CR |= RCC_CR_HSE_MASK;
    while(!RCC->CR & RCC_CR_HSERDY_MASK);
}
void RCC_DisableHSE(void)
{
    RCC->CR &= ~RCC_CR_HSE_MASK;
}

/*********************************************************************************************************************************************************************
 * @brief Enable/Disable the PLL (Phase-Locked Loop) clock
 * 
 * 
 * @note
 * These functions enable or disable the PLL clock source by setting or clearing the corresponding bits in the CR register of the RCC peripheral.
 * Enabling the PLL is necessary before using it as a system clock source, while disabling it can save power when it's not needed. 
 * If the PLL is currently being used as the system clock source, it will be switched to HSI before disabling the PLL to ensure system stability.
 */
void RCC_EnablePLL(void)
{
    RCC->CR |= RCC_CR_PLL_MASK;
    while(!(RCC->CR & RCC_CR_PLLRDY_MASK));
}

void RCC_DisablePLL(void)
{
    if((RCC->CFGR & RCC_CFGR_SWS_MASK) == 0x2){
        if((RCC->CR & RCC_CR_HSI_MASK) == 0){
            RCC_EnableHSI();
        }
        RCC->CFGR &= ~(1U << RCC_CFGR_SW_POS);
        while((RCC->CFGR & RCC_CFGR_SWS_MASK) == 0);
    }
    else
    {    
        RCC->CR &= ~RCC_CR_PLL_MASK;
    }    
}


/*********************************************************************************************************************************************************************
 * @brief Configure the PLL source, multiplication factor and select the system clock source
 * 
 * @param src: Enumeration value specifying the PLL source (e.g., PLLSRC_HSI, PLLSRC_HSE, PLLSRC_HSE_DIV2).
 * @param mul: Enumeration value specifying the PLL multiplication factor (e.g., MULx4, MULx5, etc.).
 * @param sysclk: Enumeration value specifying the system clock source (e.g., SYSCLK_HSI, SYSCLK_HSE, SYSCLK_PLL).
 * 
 * @note
 * This function configures the PLL source and multiplication factor by updating the CFGR register of the RCC peripheral.
 * It also selects the system clock source by updating the SW bits in the CFGR register and waits for the switch to complete by checking the SWS bits.
 * The PLL source can be selected from HSI, HSE, or HSE divided by 2, and the multiplication factor can be set according to the desired system clock frequency.
 */
void RCC_ConfigPLL(PLL_SRC_t src, RCC_Multiplier_t mul)
{
    /* If PLL is running, disable first */
    if(RCC->CR & RCC_CR_PLL_MASK) RCC_DisablePLL();
    /*Select PLL source */
    if(src == PLLSRC_HSI){
        if((RCC->CR & RCC_CR_HSI_MASK) == 0) RCC_EnableHSI();
        RCC->CFGR &= ~(1U << RCC_CFGR_PLLSRC_POS);  
        RCC->CFGR &= ~(1U << RCC_CFGR_PLLXTPRE_POS);
    }
    else if(src == PLLSRC_HSI){
        if((RCC->CR & RCC_CR_HSE_MASK) == 0) RCC_EnableHSE();
        RCC->CFGR |= (1U << RCC_CFGR_PLLSRC_POS);        
        RCC->CFGR &= ~(1U << RCC_CFGR_PLLXTPRE_POS);;    
    }   
    else if(src == PLLSRC_HSI){
        if((RCC->CR & RCC_CR_HSE_MASK) == 0) RCC_EnableHSE();
        RCC->CFGR |= (1U << RCC_CFGR_PLLSRC_POS);       
        RCC->CFGR |= (1U << RCC_CFGR_PLLXTPRE_POS);     
    }
    RCC->CFGR |= (mul << RCC_CFGR_PLLMUL_POS);
}

void RCC_SelectSYSCLK(RCC_SYSCLK_t sysclk)
{  
    RCC->CFGR &= ~(0x3 << 0);
    RCC->CFGR |= (sysclk << RCC_CFGR_SW_POS);
    while((RCC->CFGR & RCC_CFGR_SWS_MASK) != sysclk);
}


/*********************************************************************************************************************************************************************
 * @brief Enable/Disable the clock for a specific TIM peripheral
 * 
 * @note
 * These functions enable or disable the clock for the specified TIM peripheral by setting or clearing the corresponding bits in the APB1ENR or APB2ENR register of the RCC peripheral.
 * Enabling the clock is necessary before configuring or using any TIM features, while disabling it can save power when it's not needed.
 */
void RCC_EnableTIMClock(RCC_TIM_t TIMx)
{
    if(TIMx == RCC_TIM1)    RCC->APB2ENR |= RCC_APB2ENR_TIM1_MASK;           
    else if(TIMx == RCC_TIM2)  RCC->APB1ENR |= RCC_APB1ENR_TIM2_MASK;
    else if(TIMx == RCC_TIM3)  RCC->APB1ENR |= RCC_APB1ENR_TIM3_MASK;
    else if(TIMx == RCC_TIM4)  RCC->APB1ENR |= RCC_APB1ENR_TIM4_MASK;
    
}

void RCC_DisableTIMClock(RCC_TIM_t TIMx)
{
    if(TIMx == RCC_TIM1)    RCC->APB2ENR &= ~RCC_APB2ENR_TIM1_MASK;
    else if(TIMx == RCC_TIM2)  RCC->APB1ENR &= ~RCC_APB1ENR_TIM2_MASK;
    else if(TIMx == RCC_TIM3)  RCC->APB1ENR &= ~RCC_APB1ENR_TIM3_MASK;
    else if(TIMx == RCC_TIM4)  RCC->APB1ENR &= ~RCC_APB1ENR_TIM4_MASK;
}


/*********************************************************************************************************************************************************************
 * @brief Read the current system clock frequency.
 *
 * @return Current SYSCLK frequency in hertz.
 *
 * @details Determines the active clock source from the SWS bits in RCC_CFGR and computes the
 * corresponding frequency for HSI, HSE, or PLL-based operation.
 */
uint32_t RCC_GetSYSCLK(void)
{
    uint32_t sysclk, pll_input, pllmul;
    pllmul =  pll_mul_table[((RCC->CFGR >> RCC_CFGR_PLLMUL_POS) & 0x0FU)];

    switch((RCC->CFGR & RCC_CFGR_SWS_MASK) >> RCC_CFGR_SWS_POS)
    {
    case 0x0U:
        sysclk = HSI_VALUE;
        break;

    case 0x1U:
        sysclk = HSE_VALUE;
        break;

    case 0x2U:
        if(!(RCC->CFGR & (1U << RCC_CFGR_PLLSRC_POS)))
        {
            pll_input = HSI_VALUE / 2U;
        }
        else
        {
            pll_input = HSE_VALUE;

            if((RCC->CFGR & (1U << RCC_CFGR_PLLXTPRE_POS)))
                pll_input /= 2U;
        }

        sysclk = pll_input * pllmul;
        break;
    default:
        return 0U;
    }
    return sysclk;
}


/*********************************************************************************************************************************************************************
 * @brief Read the current AHB clock frequency.
 *
 * @return Current HCLK frequency in hertz.
 *
 * @details Computes the AHB clock from the system clock and the HPRE prescaler value stored in RCC_CFGR.
 */
uint32_t RCC_GetHCLK(void)
{
    uint32_t sysclk, hpre;
    sysclk = RCC_GetSYSCLK();

    hpre = (RCC->CFGR & RCC_CFGR_HPRE_MASK) >> RCC_CFGR_HPRE_POS;
    
    return sysclk / ahb_prescaler[hpre];
}


/*********************************************************************************************************************************************************************
 * @brief Read the current APB1 peripheral clock frequency.
 *
 * @return Current APB1 clock frequency in hertz.
 *
 * @details Computes the APB1 clock from HCLK and the PPRE1 prescaler value stored in RCC_CFGR.
 */
uint32_t RCC_GetPCLK1(void)
{
    uint32_t hclk, ppre1;
    hclk = RCC_GetHCLK();

    ppre1 = (RCC->CFGR & RCC_CFGR_PPRE1_MASK) >> RCC_CFGR_PPRE1_POS;
    return hclk / apb_prescaler[ppre1];
}


/*********************************************************************************************************************************************************************
 * @brief Read the current APB2 peripheral clock frequency.
 *
 * @return Current APB2 clock frequency in hertz.
 *
 * @details Computes the APB2 clock from HCLK and the PPRE2 prescaler value stored in RCC_CFGR.
 */
uint32_t RCC_GetPCLK2(void)
{
    uint32_t hclk, ppre2;
    hclk = RCC_GetHCLK();

    ppre2 = (RCC->CFGR & RCC_CFGR_PPRE2_MASK) >> RCC_CFGR_PPRE2_POS;
    return hclk / apb_prescaler[ppre2];
}


/* End of refactored RCC */