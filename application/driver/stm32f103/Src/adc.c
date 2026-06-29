/**************************************************************************************************
 * @file    adc.c
 * @brief   ADC driver implementation for STM32F103xx
 *
 * @details
 * This file provides ADC peripheral control for STM32F103xx devices, including
 * ADC enable/disable, conversion start/stop, data reading, flag handling,
 * interrupt status checking, temperature sensor control, calibration, and
 * external trigger configuration.
 *
 * @author  lu-05-cmd
 * @date    2026-04-29
 * @version 0.1.0
 *
 * @note
 *   - Uses direct register access for ADC peripheral control.
 *   - Supports regular and injected conversions, alignment selection, and
 *     external trigger configuration.
 **************************************************************************************************/

 
/*INCLUDE============================================================================================================================================================*/
#include <stdint.h>
#include "../Inc/adc.h"
#include "../Inc/rcc.h"


/**
 * @brief   Enable ADC peripheral
 * @param   ADCx: Pointer to ADC instance (e.g., ADC1, ADC2)
 * @note    This function sets the ADON bit in CR2 to enable the ADC.
 *          ADC must be enabled before starting conversions.
 */
void ADC_Enable(ADC_TypeDef *ADCx)
{
    ADCx->CR2 |= ADC_CR2_ADON_MASK;
}

/**
 * @brief   Disable ADC peripheral
 * @param   ADCx: Pointer to ADC instance (e.g., ADC1, ADC2)
 * @note    This function clears the ADON bit in CR2 to disable the ADC.
 *          Disabling the ADC stops any ongoing conversions.
 */
void ADC_Disable(ADC_TypeDef *ADCx)
{
    ADCx->CR2 &= ~ADC_CR2_ADON_MASK;
}

/**
 * @brief   Start regular ADC conversion by software trigger
 * @param   ADCx: Pointer to ADC instance (e.g., ADC1, ADC2)
 * @retval  ADC_STATUS_OK if conversion start request was issued
 * @note    This function sets the SWSTART bit in CR2 to begin regular conversion.
 *          Regular conversions follow the configured regular sequence.
 */
ADC_Status_t ADC_StartRegularConversion(ADC_TypeDef *ADCx)
{
    ADCx->CR2 |= ADC_CR2_SWSTART_MASK;
    return ADC_STATUS_OK;
}

/**
 * @brief   Stop regular ADC conversion
 * @param   ADCx: Pointer to ADC instance (e.g., ADC1, ADC2)
 * @retval  ADC_STATUS_OK if the stop request was issued
 * @note    This function clears the CONT bit in CR2, stopping continuous regular conversions.
 */
ADC_Status_t ADC_StopRegularConversion(ADC_TypeDef *ADCx)
{
    ADCx->CR2 &= ~ADC_CR2_CONT_MASK;
    return ADC_STATUS_OK;
}

/**
 * @brief   Start injected ADC conversion by software trigger
 * @param   ADCx: Pointer to ADC instance (e.g., ADC1, ADC2)
 * @retval  ADC_STATUS_OK if conversion start request was issued
 * @note    This function sets the JSWSTART bit in CR2 to begin injected conversion.
 *          Injected conversions have higher priority and can be triggered independently of regular conversions.
 */
ADC_Status_t ADC_StartInjectedConversion(ADC_TypeDef *ADCx)
{
    ADCx->CR2 |= ADC_CR2_JSWSTART_MASK;
    return ADC_STATUS_OK;
}

/**
 * @brief   Stop injected ADC conversion
 * @param   ADCx: Pointer to ADC instance (e.g., ADC1, ADC2)
 * @retval  ADC_STATUS_OK if the stop request was issued
 * @note    This function clears the CONT bit in CR2, which also stops continuous injected conversions.
 */
ADC_Status_t ADC_StopInjectedConversion(ADC_TypeDef *ADCx)
{
    ADCx->CR2 &= ~ADC_CR2_CONT_MASK;
    return ADC_STATUS_OK;
}


/**
 * @brief   Read ADC regular conversion data
 * @param   ADCx: Pointer to ADC instance (e.g., ADC1, ADC2)
 * @retval  16-bit ADC conversion result
 * @note    This function returns the ADC data register value, taking into account
 *          right or left alignment based on the ALIGN bit in CR2.
 */
uint16_t ADC_ReadRegularData(ADC_TypeDef *ADCx)
{
    if(ADCx->CR2 & ADC_CR2_ALIGN_MASK)
    {
        return (uint16_t)(ADCx->DR >> 4);
    }else{
        return (uint16_t)ADCx->DR;
    }
}

/**
 * @brief   Read ADC injected conversion data for a specified rank
 * @param   ADCx: Pointer to ADC instance (e.g., ADC1, ADC2)
 * @param   rank: Injected rank identifier (ADC_InjectedRank_t enum)
 * @retval  16-bit ADC injected conversion result
 * @note    This function selects the correct JDRx register for the injected rank and
 *          handles alignment based on the ALIGN bit in CR2.
 */
uint16_t ADC_ReadInjectedData(ADC_TypeDef *ADCx, ADC_InjectedRank_t rank)
{
    uint16_t mask;
    switch(rank)
    {
        case ADC_INJECTED_RANK_1:   mask = ADCx->JDR1; break;
        case ADC_INJECTED_RANK_2:   mask = ADCx->JDR2; break;
        case ADC_INJECTED_RANK_3:   mask = ADCx->JDR3; break;
        case ADC_INJECTED_RANK_4:   mask = ADCx->JDR4; break;
        default:    mask = 0; break;
    }

    if(ADCx->CR2 & ADC_CR2_ALIGN_MASK)
    {
        return (uint16_t)(mask >> 4);
    }else{
        return mask;
    }
}


/**
 * @brief   Get ADC status flag state
 * @param   ADCx: Pointer to ADC instance (e.g., ADC1, ADC2)
 * @param   flag: Flag to check (ADC_FlagStatus_t enum)
 * @retval  true if the flag is set, false if not
 * @note    This function checks status flags in the ADC SR register without clearing them.
 */
bool ADC_GetFlagStatus(ADC_TypeDef *ADCx, ADC_FlagStatus_t flag)
{
    return (ADCx->SR & flag) ? true : false;
}

/**
 * @brief   Clear ADC status flags
 * @param   ADCx: Pointer to ADC instance (e.g., ADC1, ADC2)
 * @param   flag: Flags to clear (ADC_FlagStatus_t enum)
 * @note    This function clears non-EOC flags directly in SR. For EOC, it reads DR to clear the flag.
 */
void ADC_ClearFlag(ADC_TypeDef *ADCx, ADC_FlagStatus_t flag)
{
    volatile uint32_t tmp = 0;
    ADCx->SR &= ~(flag & (ADC_AWD_FLAG  |
                         ADC_JEOC_FLAG  |
                         ADC_JSTRT_FLAG |
                         ADC_STRT_FLAG));
    if(flag & (ADC_EOC_FLAG))   tmp = ADCx->DR; 
    (void)tmp;
}

/**
 * @brief   Get ADC interrupt flag status
 * @param   ADCx: Pointer to ADC instance (e.g., ADC1, ADC2)
 * @param   flag: Interrupt flag to check (ADC_InterruptFlag_t enum)
 * @retval  true if the interrupt flag is set, false if not
 * @note    This function reads the ADC SR register for interrupt-related status.
 */
bool ADC_GetInterruptFlagStatus(ADC_TypeDef *ADCx, ADC_InterruptFlag_t flag)
{
    return (ADCx->SR & flag) ? true : false;
}

/**
 * @brief   Check whether an ADC interrupt is enabled
 * @param   ADCx: Pointer to ADC instance (e.g., ADC1, ADC2)
 * @param   it_name: Interrupt enable mask (ADC_InterruptEnable_t enum)
 * @retval  true if interrupt is enabled, false if not
 * @note    This function tests the CR1 register for enabled ADC interrupts.
 */
bool ADC_IsInterruptEnable(ADC_TypeDef *ADCx, ADC_InterruptEnable_t it_name)
{
    return (ADCx->CR1 & it_name) ? true : false;
}

/**
 * @brief   Clear pending ADC interrupt bit
 * @param   ADCx: Pointer to ADC instance (e.g., ADC1, ADC2)
 * @param   flag: Interrupt flag to clear (ADC_InterruptFlag_t enum)
 * @note    This function clears ADC AWDF and JEOC flags directly in SR, and clears EOC by reading DR.
 */
void ADC_ClearInterruptPendingBit(ADC_TypeDef *ADCx, ADC_InterruptFlag_t flag)
{
    volatile uint32_t tmp = 0;
    ADCx->SR &= ~(flag & (ADC_FLAG_AWD | ADC_FLAG_JEOC));
    if(flag & ADC_FLAG_EOC)  tmp = ADCx->DR;
    (void)tmp;
}

/**
 * @brief   Enable internal temperature sensor and Vrefint
 * @param   ADCx: Pointer to ADC instance (e.g., ADC1, ADC2)
 * @note    This function sets TSVREFE in CR2 to enable the internal temperature sensor and reference voltage.
 *          The temperature sensor output can then be sampled through the ADC channel 16.
 */
void ADC_EnableTemperatureSensor(ADC_TypeDef *ADCx)
{
    ADCx->CR2 |= ADC_CR2_TSVREFE_MASK;
}

/**
 * @brief   Disable internal temperature sensor and Vrefint
 * @param   ADCx: Pointer to ADC instance (e.g., ADC1, ADC2)
 * @note    This function clears TSVREFE in CR2 to disable the temperature sensor and internal reference voltage.
 */
void ADC_DisableTemperatureSensor(ADC_TypeDef *ADCx)
{
    ADCx->CR2 &= ~ADC_CR2_TSVREFE_MASK;
}


/*====================================================================================================================================================================
|                                   Static inline functions
=====================================================================================================================================================================*/
/**
 * @brief   Reset ADC calibration
 * @param   ADCx: Pointer to ADC instance (e.g., ADC1, ADC2)
 * @note    This function enables the ADC if necessary, sets the RSTCAL bit, and waits
 *          until the reset calibration completes.
 */
static inline void ADC_ResetCalibration(ADC_TypeDef *ADCx)
{
    ADC_Enable(ADCx);
    ADCx->CR2 |= ADC_CR2_RSTCAL_MASK;
    while(ADCx->CR2 & ADC_CR2_RSTCAL_MASK);
}

/**
 * @brief   Start ADC calibration
 * @param   ADCx: Pointer to ADC instance (e.g., ADC1, ADC2)
 * @note    This function sets the CAL bit in CR2 and waits until calibration completes.
 */
static inline void ADC_StartCalibration(ADC_TypeDef *ADCx)
{
    ADCx->CR2 |= ADC_CR2_CAL_MASK;
    while(ADCx->CR2 & ADC_CR2_CAL_MASK);
}

/**
 * @brief   Get ADC calibration status
 * @param   ADCx: Pointer to ADC instance (e.g., ADC1, ADC2)
 * @retval  true if calibration is in progress, false otherwise
 * @note    This function checks the CAL bit in CR2.
 */
static inline bool ADC_GetCalibrationStatus(ADC_TypeDef *ADCx)
{
    return (ADCx->CR2 & ADC_CR2_CAL_MASK) ? true : false;
}


/**
 * @brief   Configure ADC regular external trigger source
 * @param   ADCx: Pointer to ADC instance (e.g., ADC1, ADC2)
 * @param   regular_event: External event selection for regular group
 * @note    This function updates the EXTSEL field in CR2 to select the regular conversion trigger source.
 */
static inline void ADC_SetRegularExternalTrigger(ADC_TypeDef *ADCx, ADC_External_Event_Regular_Group regular_event)
{
    ADCx->CR2 &= ~ADC_CR2_EXTSEL_MASK;
    ADCx->CR2 |= ((regular_event) << ADC_CR2_EXTSEL_POS);
}

/**
 * @brief   Configure ADC injected external trigger source
 * @param   ADCx: Pointer to ADC instance (e.g., ADC1, ADC2)
 * @param   injected_event: External event selection for injected group
 * @note    This function updates the JEXTSEL field in CR2 to select the injected conversion trigger source.
 */
static inline void ADC_SetInjectedExternalTrigger(ADC_TypeDef *ADCx, ADC_External_Event_Injected_Group injected_event)
{
    ADCx->CR2 &= ~ADC_CR2_JEXTSEL_MASK;
    ADCx->CR2 |= ((injected_event) << ADC_CR2_JEXTSEL_POS);
}

/**
 * @brief   Configure ADC regular external event source
 * @param   ADCx: Pointer to ADC instance (e.g., ADC1, ADC2)
 * @param   regular_event: External event source for regular group
 * @note    This function sets the regular conversion trigger selection field in CR2.
 */
static inline void ADC_SetRegularEvent(ADC_TypeDef *ADCx, ADC_External_Event_Regular_Group regular_event)
{
    ADCx->CR2 &= ~ADC_CR2_EXTSEL_MASK;
    ADCx->CR2 |= ((regular_event) << ADC_CR2_EXTSEL_POS);
}

/**
 * @brief   Configure ADC injected external event source
 * @param   ADCx: Pointer to ADC instance (e.g., ADC1, ADC2)
 * @param   injected_event: External event source for injected group
 * @note    This function sets the injected conversion trigger selection field in CR2.
 */
static inline void ADC_SetInjectedEvent(ADC_TypeDef *ADCx, ADC_External_Event_Injected_Group injected_event)
{
    ADCx->CR2 &= ~ADC_CR2_JEXTSEL_MASK;
    ADCx->CR2 |= ((injected_event) << ADC_CR2_JEXTSEL_POS);
}

/**
 * @brief   Configure ADC scan mode
 * @param   ADCx: Pointer to ADC instance (e.g., ADC1, ADC2)
 * @param   scan_mode: Scan mode selection
 * @note    When scan mode is enabled, the ADC converts all channels in the regular sequence.
 */
static inline void ADC_SetScanMode(ADC_TypeDef *ADCx, ADC_ScanMode_t scan_mode)
{
    ADCx->CR1 &= ~ADC_CR1_SCAN_MASK;
    ADCx->CR1 |= (ADC_CR1_SCAN_MASK * scan_mode);
}

/**
 * @brief   Configure ADC conversion mode
 * @param   ADCx: Pointer to ADC instance (e.g., ADC1, ADC2)
 * @param   conv_mode: Conversion mode selection
 * @note    This function enables or disables continuous conversion mode.
 */
static inline void ADC_SetConversionMode(ADC_TypeDef *ADCx, ADC_ConversionMode_t conv_mode)
{
    ADCx->CR2 &= ~ADC_CR2_CONT_MASK;
    ADCx->CR2 |= (conv_mode << ADC_CR2_CONT_POS);
}

/**
 * @brief   Configure ADC dual mode
 * @param   ADCx: Pointer to ADC instance (e.g., ADC1, ADC2)
 * @param   dual: Dual ADC mode selection value
 * @retval  ADC_STATUS_OK on valid configuration
 * @retval  ADC_STATUS_ERROR if the dual mode value is invalid
 */
static inline ADC_Status_t ADC_SetDUALMode(ADC_TypeDef *ADCx, uint8_t dual)
{
    ADCx->CR2 &= ~ADC_CR1_DUALMODE_MASK;
    if(dual <= 0x9)
    {
        ADCx->CR2 |= dual;
    }
    else
    {
        return ADC_STATUS_ERROR;
    }
    return ADC_STATUS_OK;
}

/**
 * @brief   Enable ADC external trigger for a conversion group
 * @param   ADCx: Pointer to ADC instance (e.g., ADC1, ADC2)
 * @param   conv_name: Conversion group for trigger enable
 * @note    Enables the regular or injected external trigger bit in CR2.
 */
static inline void ADC_EnableExternalTrigger(ADC_TypeDef *ADCx, ADC_Conversion_t conv_name)
{
    switch(conv_name)
    {
        case ADC_REGULAR_CONVERSION:
            ADCx->CR2 |= ADC_CR2_EXTTRIG_MASK;
            break;
        case ADC_INJECTED_CONVERSION:
            ADCx->CR2 |= ADC_CR2_JEXTTRIG_MASK;
            break;
        default:
            break;
    }
}

/**
 * @brief   Set ADC data alignment
 * @param   ADCx: Pointer to ADC instance (e.g., ADC1, ADC2)
 * @param   align: Data alignment selection
 * @note    Right alignment places the result in the low bits of DR; left alignment places it in the high bits.
 */
static inline void ADC_SetDataAlignment(ADC_TypeDef *ADCx, ADC_Data_Alignment_t align)
{
    if(align == ADC_RIGHT_ALIGNMENT)
        ADCx->CR2 &= ~ADC_CR2_ALIGN_MASK;
    else if(align == ADC_LEFT_ALIGNMENT)
        ADCx->CR2 |= ADC_CR2_ALIGN_MASK;
}

/**
 * @brief   Configure ADC discontinuous mode
 * @param   ADC: Pointer to ADC instance (e.g., ADC1, ADC2)
 * @param   dis_mode_regular: Discontinuous mode settings for regular and injected groups
 * @param   dis_channel_number: Number of channels in discontinuous regular mode
 * @note    Programs both regular and injected discontinuous mode bits and the discontinuous channel count.
 */
static inline void ADC_SetDisRegularMode(ADC_TypeDef *ADC, 
                                         ADC_DiscontinuousMode_t dis_mode_regular,
                                         uint8_t dis_channel_number)
{
    ADC->CR1 &= ~(ADC_CR1_DISCEN_MASK | ADC_CR1_JDISCEN_MASK);
    ADC->CR1 &= ~ADC_CR1_DISCNUM_MASK;
    ADC->CR1 |= (dis_mode_regular.regular << ADC_CR1_DISCEN_POS)
              | (dis_mode_regular.injected << ADC_CR1_JDISCEN_POS);
    ADC->CR1 |= (dis_channel_number << ADC_CR1_DISCNUM_POS);
}



/**
 * @brief   Configure ADC DMA mode
 * @param   ADCx: Pointer to ADC instance (e.g., ADC1, ADC2)
 * @param   dma_config: DMA enable/disable configuration
 * @note    This function clears the existing DMA configuration bits and sets the new DMA mode in CR2.
 */
static inline void ADC_SetDMAStatus(ADC_TypeDef *ADCx, ADC_DMA_Config_t dma_config)
{
    ADCx->CR2 &= ~ADC_CR2_DMA_MASK;
    ADCx->CR2 |= (dma_config << ADC_CR2_DMA_POS);
}

/**
 * @brief   Configure ADC analog watchdog behavior
 * @param   ADCx: Pointer to ADC instance (e.g., ADC1, ADC2)
 * @param   conv_name: ADC conversion group type (regular or injected)
 * @param   highest: Analog watchdog high threshold
 * @param   lowest: Analog watchdog low threshold
 * @note    This function disables the ADC before configuring the watchdog thresholds and enables
 *          the correct watchdog channel based on the conversion group.
 */
static inline void ADC_AnalogWatchdogEnable(ADC_TypeDef *ADCx, ADC_Conversion_t conv_name)
{
    switch(conv_name)
    {
        case ADC_REGULAR_CONVERSION : ADCx->CR1 |= ADC_CR1_AWDEN_MASK;   break;
        case ADC_INJECTED_CONVERSION: ADCx->CR1 |= ADC_CR1_JAWDEN_MASK;  break;
        default:    break;
    }
}

static inline void ADC_AnalogWatchdogDisable(ADC_TypeDef *ADCx, ADC_Conversion_t conv_name)
{
    switch(conv_name)
    {
        case ADC_REGULAR_CONVERSION : ADCx->CR1 &= ~ADC_CR1_AWDEN_MASK;   break;
        case ADC_INJECTED_CONVERSION: ADCx->CR1 &= ~ADC_CR1_JAWDEN_MASK;  break;
        default:    break;
    }
}

static inline void ADC_AnalogWatchdogConfig(ADC_TypeDef *ADCx, ADC_Conversion_t conv_name, uint16_t highest, uint16_t lowest)
{
    ADC_Disable(ADCx);
    if(conv_name == ADC_REGULAR_CONVERSION)
    {
        ADCx->CR1 |= ADC_CR1_AWDEN_MASK;
    }
    else
    {
        ADCx->CR1 |= ADC_CR1_JAWDEN_MASK;
    }
    if(highest >= lowest)
    {
        ADCx->HTR = highest;
        ADCx->LTR = lowest;
    }
    else
    {
        ADCx->HTR = lowest;
        ADCx->LTR = highest;
    }
}

/**
 * @brief   Select analog watchdog channel
 * @param   ADCx: Pointer to ADC instance (e.g., ADC1, ADC2)
 * @param   chl: ADC channel to monitor with watchdog
 * @param   conv_name: Conversion group used for watchdog (regular or injected)
 * @note    This function updates the AWDCH field and enables the corresponding watchdog mask.
 */
static inline void ADC_AnalogWatchdogSelectChannel(ADC_TypeDef *ADCx, ADC_Channel_t chl, ADC_Conversion_t conv_name)
{
    ADCx->CR1 &= ~ADC_CR1_AWDCH_MASK;
    ADCx->CR1 |= (chl << ADC_CR1_AWDCH_POS);
    if(conv_name == ADC_INJECTED_CONVERSION)
    {
        ADCx->CR1 &= ~ADC_CR1_JAWDEN_MASK;
        ADCx->CR1 |= ADC_CR1_JAWDEN_MASK;
    }
    else
    {
        ADCx->CR1 &= ~ADC_CR1_AWDEN_MASK;
        ADCx->CR1 |= ADC_CR1_AWDEN_MASK;
    }
}


/**
 * @brief   Enable ADC interrupt for a conversion group
 * @param   ADCx: Pointer to ADC instance (e.g., ADC1, ADC2)
 * @param   conv_name: Conversion group for interrupt enable
 * @note    This function enables the end-of-conversion interrupt for either regular or injected conversions.
 */
static inline void ADC_EnableInterrupt(ADC_TypeDef *ADCx, ADC_Conversion_t conv_name)
{
    switch(conv_name)
    {
        case ADC_REGULAR_CONVERSION:
            ADCx->CR1 |= ADC_CR1_EOCIE_MASK;
            break;
        case ADC_INJECTED_CONVERSION:
            ADCx->CR1 |= ADC_CR1_JEOCIE_MASK;
            break;
        default:
            break;
    }
}

/**
 * @brief   Disable ADC interrupt for a conversion group
 * @param   ADCx: Pointer to ADC instance (e.g., ADC1, ADC2)
 * @param   conv_name: Conversion group for interrupt disable
 * @note    This function disables the end-of-conversion interrupt for regular or injected conversions.
 */
static inline void ADC_DisableInterrupt(ADC_TypeDef *ADCx, ADC_Conversion_t conv_name)
{
    switch(conv_name)
    {
        case ADC_REGULAR_CONVERSION:
            ADCx->CR1 &= ~ADC_CR1_EOCIE_MASK;
            break;
        case ADC_INJECTED_CONVERSION:
            ADCx->CR1 &= ~ADC_CR1_JEOCIE_MASK;
            break;
        default:
            break;
    }
}

/**
 * @brief   Set ADC sample time for a channel
 * @param   ADCx: Pointer to ADC instance (e.g., ADC1, ADC2)
 * @param   channel: ADC channel to configure
 * @param   sample: Sample time selection
 * @note
 *   - Channels 0-9 use SMPR2; channels 10-17 use SMPR1.
 *   - ADC1 channel 16/17 map to temperature sensor and Vrefint respectively.
 */
static inline void ADC_SetSampleTime(ADC_TypeDef *ADCx, ADC_Channel_t channel, ADC_SampleTime_t sample)
{
    uint32_t shift;
    volatile uint32_t *reg = (channel < 10) ? &ADCx->SMPR2 : &ADCx->SMPR1;
    if(channel < 10)
    {
        shift = channel * 3;
    }
    else
    {
        shift = (channel - 10) * 3;
    }
    *reg &= ~(0x7U << shift);
    *reg |= (sample << shift);
}


/**
 * @brief   Configure a regular ADC channel rank in the sequence
 * @param   ADCx: Pointer to ADC instance (e.g., ADC1, ADC2)
 * @param   channel: ADC channel to assign
 * @param   rank: Rank position in the regular conversion sequence (1-16)
 * @retval  ADC_STATUS_OK on success
 * @retval  ADC_STATUS_ERROR on invalid channel or rank
 * @note    This function writes the channel number into the correct SQRx register field.
 */
static inline ADC_Status_t ADC_SetRegularChannel(ADC_TypeDef *ADCx, ADC_Channel_t channel, uint32_t rank)
{
    uint32_t shift;
    volatile uint32_t *reg;

    if(rank == 0 || rank > 16) return ADC_STATUS_ERROR;
    if(channel <= ADC_CHANNEL_6)
    {
        reg = &ADCx->SQR3;
        shift = (rank - 1) * 5;
    }
    else if(channel <= ADC_CHANNEL_12)
    {
        reg = &ADCx->SQR2;
        shift = (rank - 7) * 5;
    }
    else if(channel <= ADC_CHANNEL_17)
    {
        reg = &ADCx->SQR1;
        shift = (rank - 13) * 5;
    }
    else
    {
        return ADC_STATUS_ERROR;
    }

    *reg &= ~(0x1FU << shift);
    *reg |= (channel << shift);
    return ADC_STATUS_OK;
}


/**
 * @brief   Set the regular conversion sequence length
 * @param   ADCx: Pointer to ADC instance (e.g., ADC1, ADC2)
 * @param   length: Number of regular conversions in sequence (1-16)
 * @note    This function writes (length-1) into the JL field of SQR1.
 */
static inline void ADC_SetRegularSequenceLength(ADC_TypeDef *ADCx, uint32_t length)
{
    if(length == 0 || length > 16) return;
    ADCx->SQR1 &= ~(0xFU << 20);
    ADCx->SQR1 |= ((length - 1) << 20);
}


/**
 * @brief   Configure injected ADC channel and sequence rank
 * @param   ADCx: Pointer to ADC instance (e.g., ADC1, ADC2)
 * @param   channel: ADC channel to assign to injected sequence
 * @param   length: Injected rank position (1-4)
 * @note    This function writes the injected sequence length and channel into JSQR.
 */
static inline void ADC_SetInjectedChannel(ADC_TypeDef *ADCx, ADC_Channel_t channel, ADC_InjectedRank_t length)
{
    ADCx->JSQR &= ~ADC_JSQR_JL_MASK;
    ADCx->JSQR |= (length << ADC_JSQR_JL_POS);
    ADCx->JSQR |= channel;
}


/**
 * @brief   Set the injected conversion sequence length
 * @param   ADCx: Pointer to ADC instance (e.g., ADC1, ADC2)
 * @param   length: Number of injected conversions in sequence (1-4)
 * @note    This function writes the length into the JL field of JSQR.
 */
static inline void ADC_SetInjectedSequenceLength(ADC_TypeDef *ADCx, uint32_t length)
{
    if(length == 0 || length > 4) return;
    ADCx->JSQR &= ~ADC_JSQR_JL_MASK;
    ADCx->JSQR |= (length << ADC_JSQR_JL_POS);
}



/*====================================================================================================================================================================
                    ADC Initialization, De-initialization and Configuration Channel Functions
=====================================================================================================================================================================*/
/**
 * @brief   Initialize ADC peripheral with configuration data
 * @param   ADCx: Pointer to ADC instance (e.g., ADC1, ADC2)
 * @param   cfg: Pointer to ADC_Config_t structure containing ADC configuration
 * @retval  ADC_STATUS_OK on success
 * @retval  ADC_STATUS_ERROR if configuration values are invalid
 * @note    This function validates the regular and injected sequence lengths.
 */
ADC_Status_t ADC_Init(ADC_TypeDef *ADCx, ADC_Config_t *cfg)
{
    if((ADCx == NULL) || (cfg == NULL))
    {
        return ADC_STATUS_ERROR;
    }

    if(cfg->regular_length == 0 || cfg->regular_length > 16)
    {
        return ADC_STATUS_ERROR;
    }

    if(cfg->injected_length == 0 || cfg->injected_length > 4)
    {
        return ADC_STATUS_ERROR;
    }

    ADC_Disable(ADCx);

    ADC_SetScanMode(ADCx, cfg->mode.adc_cr1_config.adc_scan_mode);
    ADC_SetDisRegularMode(ADCx,
                          cfg->mode.adc_cr1_config.adc_mode_discontinuous,
                          cfg->mode.adc_cr1_config.dis_channel_count);

    ADCx->CR1 &= ~ADC_CR1_DUALMODE_MASK;
    ADCx->CR1 |= (cfg->mode.adc_cr1_config.adc_dual_mode << ADC_CR1_DUALMODE_POS);

    ADC_SetConversionMode(ADCx, cfg->mode.adc_cr2_config.adc_conversion_mode);
    ADC_SetDataAlignment(ADCx, cfg->mode.adc_cr2_config.adc_data_alignment);
    ADC_SetRegularEvent(ADCx, cfg->mode.adc_cr2_config.adc_external_event.regular_external_event);
    ADC_SetInjectedEvent(ADCx, cfg->mode.adc_cr2_config.adc_external_event.injected_external_event);

    if(cfg->mode.adc_cr2_config.adc_external_trigger.regular_external_trigger == ADC_EXT_TRIG_REGULAR_ENABLE)
    {
        ADCx->CR2 |= ADC_CR2_EXTTRIG_MASK;
    }
    else
    {
        ADCx->CR2 &= ~ADC_CR2_EXTTRIG_MASK;
    }

    if(cfg->mode.adc_cr2_config.adc_external_trigger.injected_external_trigger == ADC_EXT_TRIG_INJECTED_ENABLE)
    {
        ADCx->CR2 |= ADC_CR2_JEXTTRIG_MASK;
    }
    else
    {
        ADCx->CR2 &= ~ADC_CR2_JEXTTRIG_MASK;
    }

    ADC_SetDMAStatus(ADCx, cfg->dma_config);
    ADC_SetRegularSequenceLength(ADCx, cfg->regular_length);
    ADC_SetInjectedSequenceLength(ADCx, cfg->injected_length);

    return ADC_STATUS_OK;
}

/**
 * @brief   De-initialize ADC peripheral
 * @param   ADCx: Pointer to ADC instance (e.g., ADC1, ADC2)
 * @retval  ADC_STATUS_OK on success
 * @note    This function resets the ADC peripheral configuration to its default state.
 */
ADC_Status_t ADC_DeInit(ADC_TypeDef *ADCx)
{
    if(ADCx == NULL)
    {
        return ADC_STATUS_ERROR;
    }

    ADC_Disable(ADCx);

    RCC_ResetADC(ADCx);

    return ADC_STATUS_OK;
}


/**
 * @brief   Configure a regular ADC channel and sample time
 * @param   ADCx: Pointer to ADC instance (e.g., ADC1, ADC2)
 * @param   channel_config: Regular channel configuration structure
 * @retval  ADC_STATUS_OK on success
 * @note    This function assigns a channel to the regular sequence and programs its sample time.
 */
ADC_Status_t ADC_RegularChannelConfig(ADC_TypeDef *ADCx, ADC_RegularChannel_t *channel_config)
{
    ADC_SetRegularChannel(ADCx, channel_config->channel, channel_config->rank);
    ADC_SetSampleTime(ADCx, channel_config->channel, channel_config->sample_time);
    return ADC_STATUS_OK;
}

/**
 * @brief   Configure an injected ADC channel and sample time
 * @param   ADCx: Pointer to ADC instance (e.g., ADC1, ADC2)
 * @param   channel_config: Injected channel configuration structure
 * @retval  ADC_STATUS_OK on success
 * @note    This function assigns a channel to the injected sequence and programs its sample time.
 */
ADC_Status_t ADC_InjectedChannelConfig(ADC_TypeDef *ADCx, ADC_InjectedChannel_t *channel_config)
{
    ADC_SetInjectedChannel(ADCx, channel_config->channel, channel_config->rank);
    ADC_SetSampleTime(ADCx, channel_config->channel, channel_config->sample_time);
    return ADC_STATUS_OK;
}
/* End of refactored ADC */