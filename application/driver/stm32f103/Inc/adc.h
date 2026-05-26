/**************************************************************************************************
 * @file    adc.h
 * @brief   Bare-metal ADC driver for STM32F103xx microcontrollers
 *
 * @details
 * This file provides:
 *   - ADC register definitions
 *   - ADC configuration APIs
 *   - External trigger configuration
 *   - ADC interrupt and flag handling
 *   - ADC channel configuration support
 *
 * @author  lu-05-cmd
 * @date    2025-02-15
 * @version 0.1.0
 *
 * @target  STM32F103xx Series
 *
 * @note
 *   - Developed for embedded systems learning and practice
 *   - Fully register-level programming (Bare-metal)
 *   - Based on STM32F10x Reference Manual (RM0008)
 *   - Supports regular and injected conversion groups
 *
 **************************************************************************************************/

 
#ifndef STM32F103xx_ADC_H
#define STM32F103xx_ADC_H

/*INCLUDE============================================================================================================================================================*/
#include <stdint.h>
#include <stdbool.h>

/*====================================================================================================================================================================
|                             Configuration Analog to Digital Converter (ADC)
=====================================================================================================================================================================*/
typedef struct
    {
        volatile uint32_t SR;                   // Status Register                                  - offset: 0x00
        volatile uint32_t CR1;                  // Control Register 1                               - offset: 0x04
        volatile uint32_t CR2;                  // Control Register 2                               - offset: 0x08
        volatile uint32_t SMPR1;                // Sample Time Register 1                           - offset: 0x0C
        volatile uint32_t SMPR2;                // Sample Time Register 2                           - offset: 0x10
        volatile uint32_t JOFR1;                // Injected Channel Data Offset Register 1          - offset: 0x14
        volatile uint32_t JOFR2;                // Injected Channel Data Offset Register 2          - offset: 0x18
        volatile uint32_t JOFR3;                // Injected Channel Data Offset Register 3          - offset: 0x1C
        volatile uint32_t JOFR4;                // Injected Channel Data Offset Register 4          - offset: 0x20
        volatile uint32_t HTR;                  // Watchdog High Threshold Register                 - offset: 0x24
        volatile uint32_t LTR;                  // Watchdog Low Threshold Register                  - offset: 0x28
        volatile uint32_t SQR1;                 // Regular Sequence Register 1                      - offset: 0x2C
        volatile uint32_t SQR2;                 // Regular Sequence Register 2                      - offset: 0x30
        volatile uint32_t SQR3;                 // Regular Sequence Register 3                      - offset: 0x34
        volatile uint32_t JSQR;                 // Injected sequence Register                       - offset: 0x38
        volatile uint32_t JDR1;                 // Injected data Register 1                         - offset: 0x3C
        volatile uint32_t JDR2;                 // Injected data Register 2                         - offset: 0x40
        volatile uint32_t JDR3;                 // Injected data Register 3                         - offset: 0x44
        volatile uint32_t JDR4;                 // Injected data Register 4                         - offset: 0x48
        volatile uint32_t DR;                   // Regular Data Register                            - offset: 0x4C
    }ADC_TypeDef;


    /*====================================================================================================================================================================
    |                             ADC BASE ADDRESS
    =====================================================================================================================================================================*/
    #define ADC1 ((ADC_TypeDef *)0x40012400UL)
    #define ADC2 ((ADC_TypeDef *)0x40012800UL)

/*====================================================================================================================================================================
|                                           DEFINE MASKS
=====================================================================================================================================================================*/
    /*--Status Register*/
    #define ADC_SR_AWD_POS          (0U)
    #define ADC_SR_EOC_POS          (1U)
    #define ADC_SR_JEOC_POS         (2U)
    #define ADC_SR_JSTRT_POS        (3U)
    #define ADC_SR_STRT_POS         (4U)
    /*--Control Register 1*/
    #define ADC_CR1_AWDCH_POS       (0U)
    #define ADC_CR1_EOCIE_POS       (5U)
    #define ADC_CR1_AWDIE_POS       (6U)
    #define ADC_CR1_JEOCIE_POS      (7U)
    #define ADC_CR1_SCAN_POS        (8U)
    #define ADC_CR1_AWDSGL_POS      (9U)
    #define ADC_CR1_JAUTO_POS       (10U)
    #define ADC_CR1_DISCEN_POS      (11U)
    #define ADC_CR1_JDISCEN_POS     (12U)
    #define ADC_CR1_DISCNUM_POS     (13U)
    #define ADC_CR1_DUALMODE_POS    (16U)
    #define ADC_CR1_JAWDEN_POS      (22U)
    #define ADC_CR1_AWDEN_POS       (23U)
    /*--Control Register 2*/
    #define ADC_CR2_ADON_POS        (0U)
    #define ADC_CR2_CONT_POS        (1U)
    #define ADC_CR2_CAL_POS         (2U)
    #define ADC_CR2_RSTCAL_POS      (3U)
    #define ADC_CR2_DMA_POS         (8U)
    #define ADC_CR2_ALIGN_POS       (11U)
    #define ADC_CR2_JEXTSEL_POS     (12U)
    #define ADC_CR2_JEXTTRIG_POS    (15U)
    #define ADC_CR2_EXTSEL_POS      (17U)
    #define ADC_CR2_EXTTRIG_POS     (20U)
    #define ADC_CR2_JSWSTART_POS    (21U)
    #define ADC_CR2_SWSTART_POS     (22U)
    #define ADC_CR2_TSVREFE_POS     (23U)
    /*--Injected Sequence Register*/
    #define ADC_JSQR_JL_POS         (20U)


    /*SR_MASK*/
    #define ADC_SR_AWD_MASK    (1U << ADC_SR_AWD_POS)
    #define ADC_SR_EOC_MASK    (1U << ADC_SR_EOC_POS)
    #define ADC_SR_JEOC_MASK   (1U << ADC_SR_JEOC_POS)
    #define ADC_SR_JSTRT_MASK  (1U << ADC_SR_JSTRT_POS)
    #define ADC_SR_STRT_MASK   (1U << ADC_SR_STRT_POS)
    /*CR1_MASK*/
    #define ADC_CR1_AWDCH_MASK      (0x1FU << ADC_CR1_AWDCH_POS)
    #define ADC_CR1_EOCIE_MASK      (1U << ADC_CR1_EOCIE_POS)
    #define ADC_CR1_AWDIE_MASK      (1U << ADC_CR1_AWDIE_POS)
    #define ADC_CR1_JEOCIE_MASK     (1U << ADC_CR1_JEOCIE_POS)
    #define ADC_CR1_SCAN_MASK       (1U << ADC_CR1_SCAN_POS)
    #define ADC_CR1_AWDSGL_MASK     (1U << ADC_CR1_AWDSGL_POS)
    #define ADC_CR1_JAUTO_MASK      (1U << ADC_CR1_JAUTO_POS)
    #define ADC_CR1_DISCEN_MASK     (1U << ADC_CR1_DISCEN_POS)
    #define ADC_CR1_JDISCEN_MASK    (1U << ADC_CR1_JDISCEN_POS)
    #define ADC_CR1_DISCNUM_MASK    (0x7U << ADC_CR1_DISCNUM_POS)
    #define ADC_CR1_DUALMODE_MASK   (0xFU << ADC_CR1_DUALMODE_POS)
    #define ADC_CR1_JAWDEN_MASK     (1U << ADC_CR1_JAWDEN_POS)
    #define ADC_CR1_AWDEN_MASK      (1U << ADC_CR1_AWDEN_POS)
    /*CR2_MASK*/
    #define ADC_CR2_ADON_MASK       (1U << ADC_CR2_ADON_POS)
    #define ADC_CR2_CONT_MASK       (1U << ADC_CR2_CONT_POS)
    #define ADC_CR2_CAL_MASK        (1U << ADC_CR2_CAL_POS)
    #define ADC_CR2_RSTCAL_MASK     (1U << ADC_CR2_RSTCAL_POS)
    #define ADC_CR2_DMA_MASK        (1U << ADC_CR2_DMA_POS)
    #define ADC_CR2_ALIGN_MASK      (1U << ADC_CR2_ALIGN_POS)
    #define ADC_CR2_JEXTSEL_MASK    (0x7U << ADC_CR2_JEXTSEL_POS)
    #define ADC_CR2_JEXTTRIG_MASK   (1U << ADC_CR2_JEXTTRIG_POS)
    #define ADC_CR2_EXTSEL_MASK     (0x7U << ADC_CR2_EXTSEL_POS)
    #define ADC_CR2_EXTTRIG_MASK    (1U << ADC_CR2_EXTTRIG_POS)
    #define ADC_CR2_JSWSTART_MASK   (1U << ADC_CR2_JSWSTART_POS)
    #define ADC_CR2_SWSTART_MASK    (1U << ADC_CR2_SWSTART_POS)
    #define ADC_CR2_TSVREFE_MASK    (1U << ADC_CR2_TSVREFE_POS)
    /*JSQR_MASK*/
    #define ADC_JSQR_JL_MASK        (0x3U << ADC_JSQR_JL_POS)




/*====================================================================================================================================================================
|                                           DEFINE ENENUMUMS
=====================================================================================================================================================================*/
/**
 * 
 * 
 * @brief   ADC Status Enumeration
 * @note    Used for function return status indication (e.g., success, error, timeout)
 *          - ADC_STATUS_OK: Operation completed successfully
 *          - ADC_STATUS_ERROR: An error occurred during the ADC operation (e.g., invalid parameters, hardware failure)
 *          - ADC_STATUS_TIMEOUT: The ADC operation timed out (e.g., waiting for conversion to complete)
 *          - ADC_STATUS_INVALID_RANK: Invalid rank specified for sequence configuration    
 *          - ADC_STATUS_INVALID_CONFIG: Invalid ADC configuration provided (e.g., unsupported mode, invalid channel)
 *          - ADC_STATUS_INVALID_CHANNEL: Invalid ADC channel specified for configuration or conversion
 */
typedef enum    // ADC status
{
    ADC_STATUS_OK = 0x00U,                      
    ADC_STATUS_ERROR = 0x01U,                   
    ADC_STATUS_TIMEOUT = 0x04U,                 
    ADC_STATUS_INVALID_RANK = 0x05U,            
    ADC_STATUS_INVALID_CONFIG = 0x03U,          
    ADC_STATUS_INVALID_CHANNEL = 0x02U          
} ADC_Status_t;


/** 
 * 
 * 
 * @brief ADC Channel Selection Enumeration
 * @note  This enumeration defines the available ADC channels for conversion. Each channel corresponds to a specific input pin on the microcontroller that can be configured for analog-to-digital conversion. The enumeration values can be used to select the desired ADC channel when configuring the ADC for regular or injected conversions in the application.
 *      - ADC_CHANNEL_0: Corresponds to ADC channel 0 (e.g., PA0) 
 *      - ADC_CHANNEL_1: Corresponds to ADC channel 1 (e.g., PA1)
 *      - ADC_CHANNEL_2: Corresponds to ADC channel 2 (e.g., PA2)
 *      - ADC_CHANNEL_3: Corresponds to ADC channel 3 (e.g., PA3)
 *      - ADC_CHANNEL_4: Corresponds to ADC channel 4 (e.g., PA4)
 *      - ADC_CHANNEL_5: Corresponds to ADC channel 5 (e.g., PA5)
 *      - ADC_CHANNEL_6: Corresponds to ADC channel 6 (e.g., PA6)
 *      - ADC_CHANNEL_7: Corresponds to ADC channel 7 (e.g., PA7)
 *      - ADC_CHANNEL_8: Corresponds to ADC channel 8 (e.g., PB0)
 *      - ADC_CHANNEL_9: Corresponds to ADC channel 9 (e.g., PB1)
 *      - ADC_CHANNEL_10: Corresponds to ADC channel 10 (e.g., PC0)
 *      - ADC_CHANNEL_11: Corresponds to ADC channel 11 (e.g., PC1)
 *      - ADC_CHANNEL_12: Corresponds to ADC channel 12 (e.g., PC2)
 *      - ADC_CHANNEL_13: Corresponds to ADC channel 13 (e.g., PC3)
 *      - ADC_CHANNEL_14: Corresponds to ADC channel 14 (e.g., PC4)
 *      - ADC_CHANNEL_15: Corresponds to ADC channel 15 (e.g., PC5)
 *      - ADC_CHANNEL_16: Corresponds to ADC channel 16 (internal temperature sensor)
 *      - ADC_CHANNEL_17: Corresponds to ADC channel 17 (internal reference voltage)
 */
typedef enum 
{
    ADC_CHANNEL_0 = 0,
    ADC_CHANNEL_1,
    ADC_CHANNEL_2,
    ADC_CHANNEL_3,
    ADC_CHANNEL_4,
    ADC_CHANNEL_5,
    ADC_CHANNEL_6,
    ADC_CHANNEL_7,
    ADC_CHANNEL_8,
    ADC_CHANNEL_9,
    ADC_CHANNEL_10,
    ADC_CHANNEL_11,
    ADC_CHANNEL_12,
    ADC_CHANNEL_13,
    ADC_CHANNEL_14,
    ADC_CHANNEL_15,
    ADC_CHANNEL_16,
    ADC_CHANNEL_17
}ADC_Channel_t;


/**
 * 
 * 
 * @brief ADC Conversion Type Enumeration
 * @note This enumeration defines the types of ADC conversions that can be performed. It distinguishes between regular conversions, which are triggered by software or external events and follow a defined sequence of channels, and injected conversions, which can be triggered independently and allow for higher priority sampling of specific channels.
 *      - ADC_REGULAR_CONVERSION: Regular conversion mode, where the ADC performs conversions based on a predefined sequence of channels and can be triggered by software or external events. 
 *      - ADC_INJECTED_CONVERSION: Injected conversion mode, where the ADC can perform conversions independently of the regular sequence and can be triggered by specific events or software. 
 */
typedef enum 
{
    ADC_REGULAR_CONVERSION  = 0,
    ADC_INJECTED_CONVERSION
}ADC_Conversion_t;

/**
 * 
 * 
 * @brief ADC Scan Mode Enumeration
 * @note This enumeration defines the scan mode options for the ADC. Scan mode allows the ADC to automatically convert a sequence of channels in a predefined order.
 *      - ADC_MODE_SCAN_DISABLED: Scan mode is disabled.
 *      - ADC_MODE_SCAN_ENABLED: Scan mode is enabled.
 */
typedef enum 
{
    ADC_MODE_SCAN_DISABLED = 0,
    ADC_MODE_SCAN_ENABLED
}ADC_ScanMode_t;  

/**
 * @brief ADC Discontinuous Mode Enumeration for Regular Channels
 * @note This enumeration defines the options for configuring discontinuous mode on regular channels.
 *      - ADC_DIS_REGULAR_CHANNEL_DISABLE: Discontinuous mode is disabled for regular channels.
 *      - ADC_DIS_REGULAR_CHANNEL_ENABLE: Discontinuous mode is enabled for regular channels.
 */
typedef enum 
{
    ADC_DIS_REGULAR_CHANNEL_DISABLE = 0,
    ADC_DIS_REGULAR_CHANNEL_ENABLE
} ADC_DiscontinuousMode_Regular_t; 


/**
 * @brief ADC Discontinuous Mode Enumeration for Injected Channels
 * @note This enumeration defines the options for configuring discontinuous mode on injected channels.
 *      - ADC_DIS_INJECTED_CHANNEL_DISABLE: Discontinuous mode is disabled for injected channels.
 *      - ADC_DIS_INJECTED_CHANNEL_ENABLE: Discontinuous mode is enabled for injected channels.
 */
typedef enum 
{
    ADC_DIS_INJECTED_CHANNEL_DISABLE = 0,
    ADC_DIS_INJECTED_CHANNEL_ENABLE
} ADC_DiscontinuousMode_Injected_t;


/**
 * @brief ADC Discontinuous Mode Channel Count Enumeration
 * @note This enumeration defines the number of channels to be converted in discontinuous mode. It allows the user to specify how many channels will be converted in each discontinuous conversion sequence, providing flexibility in how the ADC handles multiple channels when discontinuous mode is enabled.
 *      - ADC_DIS_MODE_0_CHANNEL: 1 channel will be converted in discontinuous mode.
 *      - ADC_DIS_MODE_1_CHANNEL: 2 channels will be converted in discontinuous mode.
 *      - ADC_DIS_MODE_2_CHANNEL: 3 channels will be converted in discontinuous mode.
 *      - ADC_DIS_MODE_3_CHANNEL: 4 channels will be converted in discontinuous mode.
 *      - ADC_DIS_MODE_4_CHANNEL: 5 channels will be converted in discontinuous mode.
 *      - ADC_DIS_MODE_5_CHANNEL: 6 channels will be converted in discontinuous mode.
 *      - ADC_DIS_MODE_6_CHANNEL: 7 channels will be converted in discontinuous mode.
 *      - ADC_DIS_MODE_7_CHANNEL: 8 channels will be converted in discontinuous mode.
 */
typedef enum  
{
    ADC_DIS_MODE_0_CHANNEL = 0,     // 1 channel 
    ADC_DIS_MODE_1_CHANNEL,         // 2 channel 
    ADC_DIS_MODE_2_CHANNEL,         // 3 channel  
    ADC_DIS_MODE_3_CHANNEL,         // 4 channel  
    ADC_DIS_MODE_4_CHANNEL,         // 5 channel  
    ADC_DIS_MODE_5_CHANNEL,         // 6 channel  
    ADC_DIS_MODE_6_CHANNEL,         // 7 channel  
    ADC_DIS_MODE_7_CHANNEL          // 8 channel  
}ADC_DiscontinuousMode_Count_t;


/**
 * 
 * 
 * @brief ADC Dual Mode Enumeration
 * @note This enumeration defines the dual mode options for the ADC. Dual mode allows two ADCs to work together to perform conversions, which can increase the sampling rate and efficiency when using multiple channels.
 *      - ADC_DUAL_MODE_INDEPENDENT                             : Both ADCs operate independently, performing conversions separately without synchronization.
 *      - ADC_DUAL_MODE_REGULAR_INJECTED_SIMULTANEOUS           : Both ADCs perform regular and injected conversions simultaneously, with the same channels being converted at the same time.
 *      - ADC_DUAL_MODE_REGULAR_SIMULTANEOUS_ALTERNATE_TRIGGER  : Both ADCs perform regular conversions simultaneously, but with alternate triggers for each ADC.
 *      - ADC_DUAL_MODE_INJECTED_INTERLEAVED_FAST               : Both ADCs perform injected conversions in an interleaved manner, with one ADC starting its conversion immediately after the other finishes, resulting in a faster overall sampling rate.
 *      - ADC_DUAL_MODE_INJECTED_INTERLEAVED_SLOW               : Both ADCs perform injected conversions in an interleaved manner, but with a delay between conversions, resulting in a slower overall sampling rate.
 *      - ADC_DUAL_MODE_INJECTED_SIMULTANEOUS                   : Both ADCs perform injected conversions simultaneously, with the same channels being converted at the same time.
 *      - ADC_DUAL_MODE_REGULAR_SIMULTANEOUS                    : Both ADCs perform regular conversions simultaneously, with the same channels being converted at the same time.
 *      - ADC_DUAL_MODE_INTERLEAVED_FAST                        : Both ADCs perform regular conversions in an interleaved manner, with one ADC starting its conversion immediately after the other finishes, resulting in a faster overall sampling rate.
 *      - ADC_DUAL_MODE_INTERLEAVED_SLOW                        : Both ADCs perform regular conversions in an interleaved manner, but with a delay between conversions, resulting in a slower overall sampling rate.
 */
typedef enum
{
    ADC_DUAL_MODE_INDEPENDENT = 0,
    ADC_DUAL_MODE_REGULAR_INJECTED_SIMULTANEOUS,
    ADC_DUAL_MODE_REGULAR_SIMULTANEOUS_ALTERNATE_TRIGGER,
    ADC_DUAL_MODE_INJECTED_INTERLEAVED_FAST,
    ADC_DUAL_MODE_INJECTED_INTERLEAVED_SLOW,
    ADC_DUAL_MODE_INJECTED_SIMULTANEOUS,
    ADC_DUAL_MODE_REGULAR_SIMULTANEOUS,
    ADC_DUAL_MODE_INTERLEAVED_FAST,
    ADC_DUAL_MODE_INTERLEAVED_SLOW
}ADC_DualMode_t;


/**
 * 
 * 
 * @brief ADC Conversion Mode Enumeration
 * @note This enumeration defines the conversion mode options for the ADC. It allows the user to select between single conversion mode, where the ADC performs one conversion and then stops, and continuous conversion mode, where the ADC continuously performs conversions until stopped by software or an external event.
 *      - ADC_MODE_SINGLE_CONV: Single conversion mode, where the ADC performs one conversion and then stops.
 *      - ADC_MODE_CONTINUOUS_CONV: Continuous conversion mode, where the ADC continuously performs conversions until stopped by software or an external event.
 */
typedef enum    
{
    ADC_MODE_SINGLE_CONV = 0,
    ADC_MODE_CONTINUOUS_CONV
}ADC_ConversionMode_t;


/**
 * 
 * 
 * @brief ADC DMA Configuration Enumeration
 * @note This enumeration defines the DMA configuration options for the ADC. It allows the user to enable or disable DMA for ADC conversions.\
 */
typedef enum 
{
    ADC_DMA_DISABLE = 0,
    ADC_DMA_ENABLE
}ADC_DMA_Config_t;


/**
 * 
 * 
 * @brief ADC Data Alignment Enumeration
 * @note This enumeration defines the data alignment options for the ADC. It allows the user to select between right-aligned and left-aligned data output.
 *      - ADC_RIGHT_ALIGNMENT: Data is right-aligned in the result register.
 *      - ADC_LEFT_ALIGNMENT: Data is left-aligned in the result register.
 */
typedef enum  
{
    ADC_RIGHT_ALIGNMENT = 0,
    ADC_LEFT_ALIGNMENT
}ADC_Data_Alignment_t;


/**
 * @brief ADC external event selection for injected group
 * @note Selects the external trigger source for injected ADC conversions
 *       on STM32F103 ADC peripheral.
 *
 * Trigger sources:
 * - ADC_EXTERNAL_EVENT_INJECTED_TIM1_TRGO      : TIM1 TRGO event
 * - ADC_EXTERNAL_EVENT_INJECTED_TIM1_CC4       : TIM1 Capture Compare 4 event
 * - ADC_EXTERNAL_EVENT_INJECTED_TIM2_TRGO      : TIM2 TRGO event
 * - ADC_EXTERNAL_EVENT_INJECTED_TIM2_CC1       : TIM2 Capture Compare 1 event
 * - ADC_EXTERNAL_EVENT_INJECTED_TIM3_CC4       : TIM3 Capture Compare 4 event
 * - ADC_EXTERNAL_EVENT_INJECTED_TIM4_TRGO      : TIM4 TRGO event
 * - ADC_EXTERNAL_EVENT_INJECTED_EXTI_LINE_15   : EXTI Line 15 event
 * - ADC_EXTERNAL_EVENT_INJECTED_JSWSTART       : Software start
 */
typedef enum
{
    ADC_EXTERNAL_EVENT_INJECTED_TIM1_TRGO = 0,
    ADC_EXTERNAL_EVENT_INJECTED_TIM1_CC4,
    ADC_EXTERNAL_EVENT_INJECTED_TIM2_TRGO,
    ADC_EXTERNAL_EVENT_INJECTED_TIM2_CC1,
    ADC_EXTERNAL_EVENT_INJECTED_TIM3_CC4,
    ADC_EXTERNAL_EVENT_INJECTED_TIM4_TRGO,
    ADC_EXTERNAL_EVENT_INJECTED_EXTI_LINE_15,
    ADC_EXTERNAL_EVENT_INJECTED_JSWSTART
} ADC_External_Event_Injected_Group;


/**
 * @brief ADC external trigger control for injected group
 * @note Enables or disables external trigger for injected conversion.
 */
typedef enum
{
    ADC_EXT_TRIG_INJECTED_DISABLE = 0,
    ADC_EXT_TRIG_INJECTED_ENABLE  = 1
} ADC_ExternalTriggerInjected_t;


/**
 * @brief ADC external event selection for regular group
 * @note Selects the external trigger source for regular ADC conversions
 *       on STM32F103 ADC peripheral.
 *
 * Trigger sources:
 * - ADC_EXTERNAL_EVENT_REGULAR_TIM1_CC1      : TIM1 Capture Compare 1 event
 * - ADC_EXTERNAL_EVENT_REGULAR_TIM1_CC2      : TIM1 Capture Compare 2 event
 * - ADC_EXTERNAL_EVENT_REGULAR_TIM1_CC3      : TIM1 Capture Compare 3 event
 * - ADC_EXTERNAL_EVENT_REGULAR_TIM2_CC2      : TIM2 Capture Compare 2 event
 * - ADC_EXTERNAL_EVENT_REGULAR_TIM3_TRGO     : TIM3 TRGO event
 * - ADC_EXTERNAL_EVENT_REGULAR_TIM4_CC4      : TIM4 Capture Compare 4 event
 * - ADC_EXTERNAL_EVENT_REGULAR_EXTI_LINE_11  : EXTI Line 11 event
 * - ADC_EXTERNAL_EVENT_REGULAR_SWSTART       : Software start
 */
typedef enum
{
    ADC_EXTERNAL_EVENT_REGULAR_TIM1_CC1 = 0,
    ADC_EXTERNAL_EVENT_REGULAR_TIM1_CC2,
    ADC_EXTERNAL_EVENT_REGULAR_TIM1_CC3,
    ADC_EXTERNAL_EVENT_REGULAR_TIM2_CC2,
    ADC_EXTERNAL_EVENT_REGULAR_TIM3_TRGO,
    ADC_EXTERNAL_EVENT_REGULAR_TIM4_CC4,
    ADC_EXTERNAL_EVENT_REGULAR_EXTI_LINE_11,
    ADC_EXTERNAL_EVENT_REGULAR_SWSTART
} ADC_External_Event_Regular_Group;


/**
 * @brief ADC external trigger control for regular group
 * @note Enables or disables external trigger for regular conversion.
 */
typedef enum
{
    ADC_EXT_TRIG_REGULAR_DISABLE = 0,
    ADC_EXT_TRIG_REGULAR_ENABLE  = 1
} ADC_ExternalTriggerRegular_t;


/**
 * @brief ADC sample time selection
 * @note Selects ADC channel sampling time.
 *
 * Sampling cycle options:
 * - ADC_SAMPLETIME_1_5_CYCLES    : 1.5 ADC cycles
 * - ADC_SAMPLETIME_7_5_CYCLES    : 7.5 ADC cycles
 * - ADC_SAMPLETIME_13_5_CYCLES   : 13.5 ADC cycles
 * - ADC_SAMPLETIME_28_5_CYCLES   : 28.5 ADC cycles
 * - ADC_SAMPLETIME_41_5_CYCLES   : 41.5 ADC cycles
 * - ADC_SAMPLETIME_55_5_CYCLES   : 55.5 ADC cycles
 * - ADC_SAMPLETIME_71_5_CYCLES   : 71.5 ADC cycles
 * - ADC_SAMPLETIME_239_5_CYCLES  : 239.5 ADC cycles
 */
typedef enum
{
    ADC_SAMPLETIME_1_5_CYCLES = 0,
    ADC_SAMPLETIME_7_5_CYCLES,
    ADC_SAMPLETIME_13_5_CYCLES,
    ADC_SAMPLETIME_28_5_CYCLES,
    ADC_SAMPLETIME_41_5_CYCLES,
    ADC_SAMPLETIME_55_5_CYCLES,
    ADC_SAMPLETIME_71_5_CYCLES,
    ADC_SAMPLETIME_239_5_CYCLES
} ADC_SampleTime_t;


/**
 * @brief ADC regular channel rank selection
 * @note Selects channel order in regular conversion sequence.
 *
 * Available ranks:
 * - ADC_REGULAR_RANK_1   : Rank 1
 * - ADC_REGULAR_RANK_2   : Rank 2
 * - ADC_REGULAR_RANK_3   : Rank 3
 * - ADC_REGULAR_RANK_4   : Rank 4
 * - ADC_REGULAR_RANK_5   : Rank 5
 * - ADC_REGULAR_RANK_6   : Rank 6
 * - ADC_REGULAR_RANK_7   : Rank 7
 * - ADC_REGULAR_RANK_8   : Rank 8
 * - ADC_REGULAR_RANK_9   : Rank 9
 * - ADC_REGULAR_RANK_10  : Rank 10
 * - ADC_REGULAR_RANK_11  : Rank 11
 * - ADC_REGULAR_RANK_12  : Rank 12
 * - ADC_REGULAR_RANK_13  : Rank 13
 * - ADC_REGULAR_RANK_14  : Rank 14
 * - ADC_REGULAR_RANK_15  : Rank 15
 * - ADC_REGULAR_RANK_16  : Rank 16
 */
typedef enum
{
    ADC_REGULAR_RANK_1 = 0,
    ADC_REGULAR_RANK_2,
    ADC_REGULAR_RANK_3,
    ADC_REGULAR_RANK_4,
    ADC_REGULAR_RANK_5,
    ADC_REGULAR_RANK_6,
    ADC_REGULAR_RANK_7,
    ADC_REGULAR_RANK_8,
    ADC_REGULAR_RANK_9,
    ADC_REGULAR_RANK_10,
    ADC_REGULAR_RANK_11,
    ADC_REGULAR_RANK_12,
    ADC_REGULAR_RANK_13,
    ADC_REGULAR_RANK_14,
    ADC_REGULAR_RANK_15,
    ADC_REGULAR_RANK_16
} ADC_RegularRank_t;


/**
 * @brief ADC injected channel rank selection
 * @note Selects channel order in injected conversion sequence.
 *
 * Available ranks:
 * - ADC_INJECTED_RANK_1 : Rank 1
 * - ADC_INJECTED_RANK_2 : Rank 2
 * - ADC_INJECTED_RANK_3 : Rank 3
 * - ADC_INJECTED_RANK_4 : Rank 4
 */
typedef enum
{
    ADC_INJECTED_RANK_1 = 0,
    ADC_INJECTED_RANK_2,
    ADC_INJECTED_RANK_3,
    ADC_INJECTED_RANK_4
} ADC_InjectedRank_t;


/**
 * @brief ADC status flag definition
 * @note Represents ADC status flags located in ADC_SR register.
 *
 * Available flags:
 * - ADC_AWD_FLAG    : Analog watchdog flag
 * - ADC_EOC_FLAG    : End of regular conversion flag
 * - ADC_JEOC_FLAG   : End of injected conversion flag
 * - ADC_JSTRT_FLAG  : Injected conversion started flag
 * - ADC_STRT_FLAG   : Regular conversion started flag
 */
typedef enum
{
    ADC_AWD_FLAG   = ADC_SR_AWD_MASK,
    ADC_EOC_FLAG   = ADC_SR_EOC_MASK,
    ADC_JEOC_FLAG  = ADC_SR_JEOC_MASK,
    ADC_JSTRT_FLAG = ADC_SR_JSTRT_MASK,
    ADC_STRT_FLAG  = ADC_SR_STRT_MASK
} ADC_FlagStatus_t;


/**
 * @brief ADC interrupt enable configuration
 * @note Enables ADC interrupt sources in ADC_CR1 register.
 *
 * Interrupt sources:
 * - ADC_IT_EOC_ENABLE   : End of conversion interrupt
 * - ADC_IT_AWD_ENABLE   : Analog watchdog interrupt
 * - ADC_IT_JEOC_ENABLE  : Injected conversion interrupt
 */
typedef enum
{
    ADC_IT_EOC_ENABLE  = ADC_CR1_EOCIE_MASK,
    ADC_IT_AWD_ENABLE  = ADC_CR1_AWDIE_MASK,
    ADC_IT_JEOC_ENABLE = ADC_CR1_JEOCIE_MASK
} ADC_InterruptEnable_t;


/**
 * @brief ADC interrupt flag definition
 * @note Defines ADC interrupt status flags from ADC_SR register.
 *
 * Interrupt flags:
 * - ADC_FLAG_EOC   : End of regular conversion flag
 * - ADC_FLAG_JEOC  : End of injected conversion flag
 * - ADC_FLAG_AWD   : Analog watchdog flag
 */
typedef enum
{
    ADC_FLAG_EOC  = ADC_SR_EOC_MASK,
    ADC_FLAG_JEOC = ADC_SR_JEOC_MASK,
    ADC_FLAG_AWD  = ADC_SR_AWD_MASK
} ADC_InterruptFlag_t;



/*====================================================================================================================================================================
|                                           DEFINE STRUCTURES
=====================================================================================================================================================================*/
/**
 * 
 * 
 * @brief Define a structure for ADC CR1 configuration
 * @note    This structure is used to configure the ADC CR1 register, which includes settings for scan mode, discontinuous mode, and dual mode.
 *     - The `ADC_CR1_Config_t`  : structure contains the following fields:
 *     - `adc_scan_mode`         : Specifies whether scan mode is enabled or disabled.
 *     - `adc_mode_discontinuous`: A nested structure that contains settings for discontinuous mode on regular and injected channels, as well as the number of channels in discontinuous mode.
 *     - `adc_dual_mode`         : Specifies the dual mode configuration for the ADC.
 * 
*/
typedef struct  // Discontinuous mode configuration
{
    ADC_DiscontinuousMode_Regular_t regular; 
    ADC_DiscontinuousMode_Injected_t injected;
} ADC_DiscontinuousMode_t;

typedef struct  // ADC mode CR1 configuration
{
    ADC_ScanMode_t                  adc_scan_mode;
    ADC_DiscontinuousMode_t         adc_mode_discontinuous;
    ADC_DiscontinuousMode_Count_t   dis_channel_count;
    ADC_DualMode_t                  adc_dual_mode;
}ADC_CR1_Config_t;



/**
 * 
 * 
 * @brief Define a structure for ADC CR2 configuration
 * @note    
 *      This structure is used to configure the ADC CR2 register, which includes settings for conversion mode, data alignment, external event selection, and external trigger enable/disable for both regular and injected groups.
 *          - The `ADC_CR2_Config_t`: structure contains the following fields:
 *          - The `adc_conversion_mode` : Specifies whether the ADC operates in single conversion mode or continuous conversion mode.
 *          - The `adc_data_alignment`  : Specifies the data alignment for ADC conversion results (right or left).
 *          - The `adc_external_event`  : A nested structure that contains the external event selection for both regular and injected groups.
 *          - The `adc_external_trigger`: A nested structure that contains the external trigger enable/disable settings for both regular and injected groups.
 */
typedef struct    // ADC external event selection
{
    ADC_External_Event_Regular_Group regular_external_event;
    ADC_External_Event_Injected_Group injected_external_event;
}ADC_ExternalEvent_t;

typedef struct  // ADC external trigger enable/disable
{
    ADC_ExternalTriggerRegular_t regular_external_trigger;
    ADC_ExternalTriggerInjected_t injected_external_trigger;
}ADC_ExternalTrigger_t;

typedef struct  // ADC mode CR2 configuration
{
    ADC_ConversionMode_t adc_conversion_mode;
    ADC_Data_Alignment_t adc_data_alignment;
    ADC_ExternalEvent_t adc_external_event;
    ADC_ExternalTrigger_t adc_external_trigger;

}ADC_CR2_Config_t;


/**
 * 
 * 
 * @brief Define a structure for ADC mode configuration
 * @note This structure is used to configure the ADC mode, including scan mode, discontinuous mode, dual mode, conversion mode, data alignment, and external trigger settings.
 *      - The `ADC_Mode_t` structure contains two nested structures: 
 *                         +) `ADC_CR1_Config_t` for CR1 configuration 
 *                         +) `ADC_CR2_Config_t` for CR2 configuration.
*/
typedef struct
{
    /* ADC mode CR1 configuration */
    ADC_CR1_Config_t adc_cr1_config;
    /* ADC mode CR2 configuration */
    ADC_CR2_Config_t adc_cr2_config;
}ADC_Mode_t;


/**
 * 
 * 
 * @brief ADC configuration structure
 * @note 
 *      - This structure is used to configure the ADC peripheral. It includes the mode configuration, regular and injected sequence lengths, and DMA configuration.
 *      - The `mode` field is a nested structure that contains the CR1 and CR2 configuration settings for the ADC.
 *             +) The `regular_length` and `injected_length` : fields specify the number of conversions in the regular and injected sequences, respectively. 
 *             +) The `dma_config` : field specifies whether DMA is enabled or disabled for the ADC.
 */
typedef struct
{
    ADC_Mode_t mode;    
    
    uint8_t regular_length; 
    uint8_t injected_length;

    ADC_DMA_Config_t dma_config;    
}ADC_Config_t;


/**
 * 
 * 
 * @brief ADC channel configuration structure
 * @note This structure is used to configure individual ADC channels, including the channel number, sample time, and rank in the conversion sequence.
 *      - The `channel`     : field specifies the ADC channel to be configured (e.g., ADC_CHANNEL_0, ADC_CHANNEL_1, etc.).
 *      - The `sample_time` : field specifies the sample time for the ADC channel, which determines how long the ADC samples the input signal before converting it (e.g., ADC_1_5_CYCLES, ADC_7_5_CYCLES, etc.).
 *      - The `rank`        : field specifies the rank of the channel in the conversion sequence, which determines the order in which channels are converted during regular or injected conversions (e.g., ADC_REGULAR_RANK_1, ADC_REGULAR_RANK_2, etc. for regular channels, and ADC_INJECTED_CONVERSION_1, ADC_INJECTED_CONVERSIONS_2, etc. for injected channels).
*/
typedef struct
{
    ADC_Channel_t channel;
    ADC_SampleTime_t sample_time;

    ADC_RegularRank_t rank;
}ADC_RegularChannel_t;

typedef struct
{
    ADC_Channel_t channel;
    ADC_SampleTime_t sample_time;

    ADC_InjectedRank_t rank;
}ADC_InjectedChannel_t;


/*====================================================================================================================================================================
|                             Inline Functions
=====================================================================================================================================================================*/
/*<---ADC Init/DeInit--->*/
ADC_Status_t ADC_Init(ADC_TypeDef *ADCx, ADC_Config_t *config);
ADC_Status_t ADC_DeInit(ADC_TypeDef *ADCx);

/*<---ADC Channel Configuration--->*/
ADC_Status_t ADC_RegularChannelConfig(ADC_TypeDef *ADCx, ADC_RegularChannel_t *channel_config);
ADC_Status_t ADC_InjectedChannelConfig(ADC_TypeDef *ADCx, ADC_InjectedChannel_t *channel_config);

/*<---ADC Enable/Disable--->*/
void ADC_Enable(ADC_TypeDef *ADCx);
void ADC_Disable(ADC_TypeDef *ADCx);

/*<---ADC Conversion Control--->*/
ADC_Status_t ADC_StartRegularConversion(ADC_TypeDef *ADCx);
ADC_Status_t ADC_StopRegularConversion(ADC_TypeDef *ADCx);
ADC_Status_t ADC_StartInjectedConversion(ADC_TypeDef *ADCx);
ADC_Status_t ADC_StopInjectedConversion(ADC_TypeDef *ADCx);

/*<---ADC Data Read--->*/
uint16_t ADC_ReadRegularData(ADC_TypeDef *ADCx);
uint16_t ADC_ReadInjectedData(ADC_TypeDef *ADCx, ADC_InjectedRank_t rank);

/*<---ADC Flag and Interrupt Control--->*/
bool ADC_GetFlagStatus(ADC_TypeDef *ADCx, ADC_FlagStatus_t flag);
bool ADC_GetInterruptFlagStatus(ADC_TypeDef *ADCx, ADC_InterruptFlag_t flag);
bool ADC_IsInterruptEnable(ADC_TypeDef *ADCx, ADC_InterruptEnable_t it_name);

/*<---ADC Flag and Interrupt Clear--->*/
void ADC_ClearFlag(ADC_TypeDef *ADCx, ADC_FlagStatus_t flag);
void ADC_ClearInterruptPendingBit(ADC_TypeDef *ADCx, ADC_InterruptFlag_t flag);

/*<---ADC Temperature Sensor Control--->*/
void ADC_EnableTemperatureSensor(ADC_TypeDef *ADCx);
void ADC_DisableTemperatureSensor(ADC_TypeDef *ADCx);


#endif  /*  STM32F103xx_ADC_H   */

