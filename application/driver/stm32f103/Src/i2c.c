/*********************************************************************************************************************************************************************
 * @file    i2c.c
 * @brief   I2C driver implementation for STM32F103xx
 *
 * @details
 * This file implements low-level I2C peripheral helper functions and configuration code used by the
 * STM32F103 I2C driver. It provides register-level helpers for enabling/disabling the peripheral,
 * generating start/stop conditions, reading and writing data, and controlling interrupt and status bits.
 *
 * @author  lu-05-cmd
 * @date    2026-05-07
 * @version 0.1.0
 *
 * @note
 * The implementation uses direct register access and assumes the peripheral register layout is defined
 * in the included STM32F103 header. These helpers are intended for low-level driver use and internal
 * control flow.
 *********************************************************************************************************************************************************************/

/*INCLUDE============================================================================================================================================================*/
#include <stdint.h>
#include <stddef.h>
#include "../Inc/i2c.h"
#include "../Inc/rcc.h"
#include "../Inc/gpio.h"



/**
 * @brief Timeout counter used by polling operations.
 */
static uint32_t timeout = 1000000U;

/**
 * @brief Temporary variable used to clear hardware flags and store register values.
 */
static uint32_t tmp;

/**
 * @brief General-purpose loop index.
 */
static uint32_t index;

/**
 * @brief Stores the status returned by I2C operations.
 */
static I2C_Status_t status;


/*********************************************************************************************************************************************************************
 * @brief Enable the I2C peripheral
 *
 * @param I2Cx: Pointer to the I2C peripheral instance (e.g., I2C1, I2C2, etc.)
 *
 * @note
 * This function sets the PE bit in the CR1 register to activate the peripheral.
 */
static inline void I2C_Enable(I2C_TypeDef *I2Cx)
{
    I2Cx->CR1 |= I2C_CR1_PE_MASK;
}

/*********************************************************************************************************************************************************************
 * @brief Disable the I2C peripheral
 *
 * @param I2Cx: Pointer to the I2C peripheral instance (e.g., I2C1, I2C2, etc.)
 *
 * @note
 * This function clears the PE bit in the CR1 register to stop the peripheral.
 */
static inline void I2C_Disable(I2C_TypeDef *I2Cx)
{
    I2Cx->CR1 &= ~I2C_CR1_PE_MASK;
}

/*********************************************************************************************************************************************************************
 * @brief Generate a START condition on the I2C bus
 *
 * @param I2Cx: Pointer to the I2C peripheral instance (e.g., I2C1, I2C2, etc.)
 *
 * @note
 * The START bit is set in the CR1 register to initiate a transaction.
 */
static inline void I2C_GenerateStart(I2C_TypeDef *I2Cx)
{
    I2Cx->CR1 |= I2C_CR1_START_MASK;
}

/*********************************************************************************************************************************************************************
 * @brief Generate a STOP condition on the I2C bus
 *
 * @param I2Cx: Pointer to the I2C peripheral instance (e.g., I2C1, I2C2, etc.)
 *
 * @note
 * The STOP bit is set in the CR1 register to terminate a transaction.
 */
static inline void I2C_GenerateStop(I2C_TypeDef *I2Cx)
{
    I2Cx->CR1 |= I2C_CR1_STOP_MASK;
}

/*********************************************************************************************************************************************************************
 * @brief Check whether a START condition has been generated
 *
 * @param I2Cx: Pointer to the I2C peripheral instance (e.g., I2C1, I2C2, etc.)
 *
 * @retval 1 if the START bit is set, otherwise 0.
 */
static inline uint8_t I2C_IsStartGenerated(I2C_TypeDef *I2Cx)
{
    return (uint8_t)((I2Cx->SR1 & I2C_SR1_SB_MASK) >> I2C_SR1_SB_POS);
}

/*********************************************************************************************************************************************************************
 * @brief Check whether a STOP condition has been detected
 *
 * @param I2Cx: Pointer to the I2C peripheral instance (e.g., I2C1, I2C2, etc.)
 *
 * @retval 1 if the STOPF flag is set, otherwise 0.
 */
static inline uint8_t I2C_IsStopDetected(I2C_TypeDef *I2Cx)
{
    return (uint8_t)((I2Cx->SR1 & I2C_SR1_STOPF_MASK) >> I2C_SR1_STOPF_POS);
}

/*********************************************************************************************************************************************************************
 * @brief Check whether the slave address has been matched
 *
 * @param I2Cx: Pointer to the I2C peripheral instance (e.g., I2C1, I2C2, etc.)
 *
 * @retval 1 if the address match flag is set, otherwise 0.
 */
static inline uint8_t I2C_IsAddressMatched(I2C_TypeDef *I2Cx)
{
    return (uint8_t)((I2Cx->SR1 & I2C_SR1_ADDR_MASK) >> I2C_SR1_ADDR_POS);
}

/*********************************************************************************************************************************************************************
 * @brief Write a byte to the I2C data register
 *
 * @param I2Cx: Pointer to the I2C peripheral instance (e.g., I2C1, I2C2, etc.)
 * @param data: Byte to send over the I2C bus
 */
static inline void I2C_SendData(I2C_TypeDef *I2Cx, uint8_t data)
{
    I2Cx->DR = data;
}

/*********************************************************************************************************************************************************************
 * @brief Read data from the I2C data register
 *
 * @param I2Cx: Pointer to the I2C peripheral instance (e.g., I2C1, I2C2, etc.)
 *
 * @note
 * This function reads the received byte from the I2C DR register.
 * It is typically used after a receive transaction has populated the data register.
 */
static inline uint8_t I2C_ReadData(I2C_TypeDef *I2Cx)
{
    return (uint8_t)(I2Cx->DR);
}

/*********************************************************************************************************************************************************************
 * @brief Check whether the transmit data register is empty
 *
 * @param I2Cx: Pointer to the I2C peripheral instance (e.g., I2C1, I2C2, etc.)
 *
 * @retval 1 if the TXE flag is set, otherwise 0.
 */
static inline uint8_t I2C_IsTXE(I2C_TypeDef *I2Cx)
{
    return (uint8_t)((I2Cx->SR1 & I2C_SR1_TxE_MASK) >> I2C_SR1_TxE_POS);
}

/*********************************************************************************************************************************************************************
 * @brief Check whether the receive data register is not empty
 *
 * @param I2Cx: Pointer to the I2C peripheral instance (e.g., I2C1, I2C2, etc.)
 *
 * @retval 1 if the RXNE flag is set, otherwise 0.
 */
static inline uint8_t I2C_IsRXNE(I2C_TypeDef *I2Cx)
{
    return (uint8_t)((I2Cx->SR1 & I2C_SR1_RxNE_MASK) >> I2C_SR1_RxNE_POS);
}

/*********************************************************************************************************************************************************************
 * @brief Check whether the byte transfer has finished
 *
 * @param I2Cx: Pointer to the I2C peripheral instance (e.g., I2C1, I2C2, etc.)
 *
 * @retval 1 if the BTF flag is set, otherwise 0.
 */
static inline uint8_t I2C_IsBTF(I2C_TypeDef *I2Cx)
{
    return (uint8_t)((I2Cx->SR1 & I2C_SR1_BTF_MASK) >> I2C_SR1_BTF_POS);
}

/*********************************************************************************************************************************************************************
 * @brief Check whether the I2C bus is currently busy
 *
 * @param I2Cx: Pointer to the I2C peripheral instance (e.g., I2C1, I2C2, etc.)
 *
 * @retval 1 if the bus is busy, otherwise 0.
 */
static inline uint8_t I2C_IsBusy(I2C_TypeDef *I2Cx)
{
    return (uint8_t)((I2Cx->SR2 & I2C_SR2_BUSY_MASK) >> I2C_SR2_BUSY_POS);
}

/*********************************************************************************************************************************************************************
 * @brief Check whether a bus error occurred
 *
 * @param I2Cx: Pointer to the I2C peripheral instance (e.g., I2C1, I2C2, etc.)
 *
 * @retval 1 if the BERR flag is set, otherwise 0.
 */
static inline uint8_t I2C_IsBusError(I2C_TypeDef *I2Cx)
{
    return (uint8_t)((I2Cx->SR1 & I2C_SR1_BERR_MASK) >> I2C_SR1_BERR_POS);
}

/*********************************************************************************************************************************************************************
 * @brief Check whether arbitration was lost
 *
 * @param I2Cx: Pointer to the I2C peripheral instance (e.g., I2C1, I2C2, etc.)
 *
 * @retval 1 if the ARLO flag is set, otherwise 0.
 */
static inline uint8_t I2C_IsArbitrationLost(I2C_TypeDef *I2Cx)
{
    return (uint8_t)((I2Cx->SR1 & I2C_SR1_ARLO_MASK) >> I2C_SR1_ARLO_POS);
}

/*********************************************************************************************************************************************************************
 * @brief Check whether an acknowledge failure occurred
 *
 * @param I2Cx: Pointer to the I2C peripheral instance (e.g., I2C1, I2C2, etc.)
 *
 * @retval 1 if the AF flag is set, otherwise 0.
 */
static inline uint8_t I2C_IsAcknowledgeFailed(I2C_TypeDef *I2Cx)
{
    return (uint8_t)((I2Cx->SR1 & I2C_SR1_AF_MASK) >> I2C_SR1_AF_POS);
}

/*********************************************************************************************************************************************************************
 * @brief Check whether an overrun condition occurred
 *
 * @param I2Cx: Pointer to the I2C peripheral instance (e.g., I2C1, I2C2, etc.)
 *
 * @retval 1 if the OVR flag is set, otherwise 0.
 */
static inline uint8_t I2C_IsOverrun(I2C_TypeDef *I2Cx)
{
    return (uint8_t)((I2Cx->SR1 & I2C_SR1_OVR_MASK) >> I2C_SR1_OVR_POS);
}

/*********************************************************************************************************************************************************************
 * @brief Check whether a timeout condition occurred
 *
 * @param I2Cx: Pointer to the I2C peripheral instance (e.g., I2C1, I2C2, etc.)
 *
 * @retval 1 if the TIMEOUT flag is set, otherwise 0.
 */
static inline uint8_t I2C_IsTimeout(I2C_TypeDef *I2Cx)
{
    return (uint8_t)((I2Cx->SR1 & I2C_SR1_TIMEOUT_MASK) >> I2C_SR1_TIMEOUT_POS);
}

/*********************************************************************************************************************************************************************
 * @brief Enable ACK generation for I2C transfers
 *
 * @param I2Cx: Pointer to the I2C peripheral instance (e.g., I2C1, I2C2, etc.)
 *
 * @note
 * This function sets the ACK bit in the CR1 register.
 */
static inline void I2C_ACKEnable(I2C_TypeDef *I2Cx)
{
    I2Cx->CR1 |= I2C_CR1_ACK_MASK;
}

/*********************************************************************************************************************************************************************
 * @brief Disable ACK generation for I2C transfers
 *
 * @param I2Cx: Pointer to the I2C peripheral instance (e.g., I2C1, I2C2, etc.)
 *
 * @note
 * This function clears the ACK bit in the CR1 register.
 */
static inline void I2C_ACKDisable(I2C_TypeDef *I2Cx)
{
    I2Cx->CR1 &= ~I2C_CR1_ACK_MASK;
}

/*********************************************************************************************************************************************************************
 * @brief Enable the I2C event interrupt
 *
 * @param I2Cx: Pointer to the I2C peripheral instance (e.g., I2C1, I2C2, etc.)
 *
 * @note
 * This function sets the ITEVTEN bit in the CR2 register.
 */
static inline void I2C_EventIRQEnable(I2C_TypeDef *I2Cx)
{
    I2Cx->CR2 |= I2C_CR2_ITEVTEN_MASK;
}

/*********************************************************************************************************************************************************************
 * @brief Disable the I2C event interrupt
 *
 * @param I2Cx: Pointer to the I2C peripheral instance (e.g., I2C1, I2C2, etc.)
 *
 * @note
 * This function clears the ITEVTEN bit in the CR2 register.
 */
static inline void I2C_EventIRQDisable(I2C_TypeDef *I2Cx)
{
    I2Cx->CR2 &= ~I2C_CR2_ITEVTEN_MASK;
}

/*********************************************************************************************************************************************************************
 * @brief Enable the I2C buffer interrupt
 *
 * @param I2Cx: Pointer to the I2C peripheral instance (e.g., I2C1, I2C2, etc.)
 *
 * @note
 * This function sets the ITBUFEN bit in the CR2 register.
 */
static inline void I2C_BufferIRQEnable(I2C_TypeDef *I2Cx)
{
    I2Cx->CR2 |= I2C_CR2_ITBUFEN_MASK;
}

/*********************************************************************************************************************************************************************
 * @brief Disable the I2C buffer interrupt
 *
 * @param I2Cx: Pointer to the I2C peripheral instance (e.g., I2C1, I2C2, etc.)
 *
 * @note
 * This function clears the ITBUFEN bit in the CR2 register.
 */
static inline void I2C_BufferIRQDisable(I2C_TypeDef *I2Cx)
{
    I2Cx->CR2 &= ~I2C_CR2_ITBUFEN_MASK;
}

/*********************************************************************************************************************************************************************
 * @brief Enable the I2C error interrupt
 *
 * @param I2Cx: Pointer to the I2C peripheral instance (e.g., I2C1, I2C2, etc.)
 *
 * @note
 * This function sets the ITERREN bit in the CR2 register.
 */
static inline void I2C_ErrorIRQEnable(I2C_TypeDef *I2Cx)
{
    I2Cx->CR2 |= I2C_CR2_ITERREN_MASK;
}

/*********************************************************************************************************************************************************************
 * @brief Disable the I2C error interrupt
 *
 * @param I2Cx: Pointer to the I2C peripheral instance (e.g., I2C1, I2C2, etc.)
 *
 * @note
 * This function clears the ITERREN bit in the CR2 register.
 */
static inline void I2C_ErrorIRQDisable(I2C_TypeDef *I2Cx)
{
    I2Cx->CR2 &= ~I2C_CR2_ITERREN_MASK;
}

/*********************************************************************************************************************************************************************
 * @brief Enable general call support
 *
 * @param I2Cx: Pointer to the I2C peripheral instance (e.g., I2C1, I2C2, etc.)
 *
 * @note
 * This function sets the ENGC bit in the CR1 register.
 */
static inline void I2C_GeneralCallEnable(I2C_TypeDef *I2Cx)
{
    I2Cx->CR1 |= I2C_CR1_ENGC_MASK;
}

/*********************************************************************************************************************************************************************
 * @brief Disable general call support
 *
 * @param I2Cx: Pointer to the I2C peripheral instance (e.g., I2C1, I2C2, etc.)
 *
 * @note
 * This function clears the ENGC bit in the CR1 register.
 */
static inline void I2C_GeneralCallDisable(I2C_TypeDef *I2Cx)
{
    I2Cx->CR1 &= ~I2C_CR1_ENGC_MASK;
}

/*********************************************************************************************************************************************************************
 * @brief Enable clock stretching for I2C transfers
 *
 * @param I2Cx: Pointer to the I2C peripheral instance (e.g., I2C1, I2C2, etc.)
 *
 * @note
 * This function clears the NOSTRETCH bit in the CR1 register.
 */
static inline void I2C_ClockStretchEnable(I2C_TypeDef *I2Cx)
{
    I2Cx->CR1 &= ~I2C_CR1_NOSTRETCH_MASK;
}

/*********************************************************************************************************************************************************************
 * @brief Disable clock stretching for I2C transfers
 *
 * @param I2Cx: Pointer to the I2C peripheral instance (e.g., I2C1, I2C2, etc.)
 *
 * @note
 * This function sets the NOSTRETCH bit in the CR1 register.
 */
static inline void I2C_ClockStretchDisable(I2C_TypeDef *I2Cx)
{
    I2Cx->CR1 |= I2C_CR1_NOSTRETCH_MASK;
}

/*====================================================================================================================================================================
|                             I2C Config Functions
=====================================================================================================================================================================*/

/*********************************************************************************************************************************************************************
 * @brief Initialize the I2C peripheral with the specified configuration
 *
 * @param I2Cx: Pointer to the I2C instance (I2C1 or I2C2)
 * @param i2c_cfg: Pointer to the I2C configuration structure
 *
 * @retval I2C_STATUS_OK on success, or an error code on invalid input or configuration failure.
 */

I2C_Config_t i2c_cfg = 
{
    .cr1_cfg = 
    {
        .pe_enable = ENABLE,
        .smbus_mode = I2C_MODE,
        .smb_type = I2C_SMBUS_HOST,
        .general_call_enable = ENABLE,
        .no_stretch = ENABLE,
        .ack_enable = ENABLE,
        .ack_pos = I2C_ACK_NEXT_BYTE
    },

    .oar_cfg =
    {
        .oar1_cfg = 
        {
            .addr_mode  = OAR1_ADDMODE_7BIT_SLAVE,
            .own_address = 0x52
        },
        .oar2_cfg = 
        {
            .dual_mode = OAR2_ENDUAL_BOTH_7BIT,
            .own_address = 0X62
        }
    },

    .duty_cycle = I2C_DUTY_CYCLE_16_9,
    .scl_frequency_khz = 100
};
I2C_Status_t I2C_Init(I2C_TypeDef *I2Cx, I2C_Config_t *i2c_cfg)
{
    uint32_t pclk1_hz, pclk1_mhz;

    if((I2Cx == NULL) ||
        (i2c_cfg == NULL))
    {
        return I2C_STATUS_INVALID_PARAM;
    }

    if((I2Cx != I2C1) &&
        (I2Cx != I2C2))
    {
        return I2C_STATUS_INVALID_PARAM;
    }

    if(i2c_cfg->scl_frequency_khz == 0U || 
        i2c_cfg->scl_frequency_khz > 400U)
    {
        return I2C_STATUS_INVALID_PARAM;
    }

    /* Check bus busy */
    if(I2C_IsBusy(I2Cx))
    {
        return I2C_STATUS_BUSY;
    }

    /* Enable peripheral clock for the selected I2C */
    if(I2Cx == I2C1)
    {
        RCC->APB1ENR |= RCC_APB1ENR_I2C1_MASK;
    }
    else
    {
        RCC->APB1ENR |= RCC_APB1ENR_I2C2_MASK;
    }

    /* Disable I2C before configuration */
    I2C_Disable(I2Cx);

    /* Configure CR1 basic fields */
    I2Cx->CR1 &= ~(I2C_CR1_SMBUS_MASK | I2C_CR1_SMBTYPE_MASK |
                   I2C_CR1_ENGC_MASK | I2C_CR1_NOSTRETCH_MASK |
                   I2C_CR1_ACK_MASK | I2C_CR1_POS_MASK);

    I2Cx->CR1 |= (i2c_cfg->cr1_cfg.smbus_mode & I2C_CR1_SMBUS_MASK);
    I2Cx->CR1 |= (i2c_cfg->cr1_cfg.smb_type & I2C_CR1_SMBTYPE_MASK);
    I2Cx->CR1 |= (i2c_cfg->cr1_cfg.no_stretch & I2C_CR1_NOSTRETCH_MASK);
    I2Cx->CR1 |= (i2c_cfg->cr1_cfg.ack_enable & I2C_CR1_ACK_MASK);
    I2Cx->CR1 |= (i2c_cfg->cr1_cfg.ack_pos & I2C_CR1_POS_MASK);

    /* Configure CR2: frequency in MHz of PCLK1 */
    pclk1_hz = RCC_GetPCLK1();
    pclk1_mhz = pclk1_hz / 1000000U;

    if((pclk1_mhz < 2U) ||
        (pclk1_mhz > 50U))
    {
        return I2C_STATUS_INVALID_PARAM;
    }

    I2Cx->CR2 &= ~I2C_CR2_FREQ_MASK;
    I2Cx->CR2 |= (pclk1_mhz & 0x3FU);

    /* Interrupts configuration */
    I2Cx->CR2 &= ~(I2C_CR2_ITERREN_MASK | I2C_CR2_ITEVTEN_MASK | I2C_CR2_ITBUFEN_MASK);
    I2Cx->CR2 |= ((i2c_cfg->cr2_cfg.error_interrupt & 0x1U) << I2C_CR2_ITERREN_POS);
    I2Cx->CR2 |= ((i2c_cfg->cr2_cfg.event_interrupt & 0x1U) << I2C_CR2_ITEVTEN_POS);
    I2Cx->CR2 |= ((i2c_cfg->cr2_cfg.buffer_interrupt & 0x1U) << I2C_CR2_ITBUFEN_POS);

    /* Configure CCR and TRISE */
    {
        uint32_t ccr = 0U;
        I2Cx->CCR &= ~(I2C_CCR_CCR_MASK | I2C_CCR_DUTY_MASK | I2C_CCR_FS_MASK);

        if(i2c_cfg->scl_frequency_khz <= 100U)
        {
            /* Standard mode */
            uint32_t scl_hz = i2c_cfg->scl_frequency_khz * 1000U;
            ccr = pclk1_hz / (2U * scl_hz);
            if(ccr == 0U) ccr = 1U;
            I2Cx->CCR |= (ccr & I2C_CCR_CCR_MASK);
            /* ensure FS cleared */
            I2Cx->CCR &= ~I2C_CCR_FS_MASK;
            /* TRISE = pclk1_mhz + 1 (standard mode max rise 1000ns) */
            I2Cx->TRISE = (uint8_t)(pclk1_mhz + 1U);
        }
        else
        {
            /* Fast mode */
            uint32_t scl_hz = i2c_cfg->scl_frequency_khz * 1000U;
            I2Cx->CCR |= I2C_CCR_FS_MASK;
            if(i2c_cfg->duty_cycle == I2C_DUTY_CYCLE_16_9)
            {
                /* Duty 16/9 */
                I2Cx->CCR |= I2C_CCR_DUTY_MASK;
                ccr = pclk1_hz / (25U * scl_hz);
            }
            else
            {
                /* Duty = 2 */
                I2Cx->CCR &= ~I2C_CCR_DUTY_MASK;
                ccr = pclk1_hz / (3U * scl_hz);
            }
            if(ccr == 0U) ccr = 1U;
            I2Cx->CCR |= (ccr & I2C_CCR_CCR_MASK);
            /* TRISE for fast mode: 300ns */
            I2Cx->TRISE = (uint8_t)(((pclk1_mhz * 300U) / 1000U) + 1U);
        }
    }

    /* Configure OAR1 */
    I2Cx->OAR1 &= ~(I2C_OAR1_ADDMODE_MASK | I2C_OAR1_ADD_MASK | I2C_OAR1_ADD0_MASK);
    I2Cx->OAR1 |= (i2c_cfg->oar_cfg.oar1_cfg.addr_mode & I2C_OAR1_ADDMODE_MASK);

    /* Validate own address according to mode */
    if(i2c_cfg->oar_cfg.oar1_cfg.addr_mode == OAR1_ADDMODE_10BIT_SLAVE)
    {
        if(i2c_cfg->oar_cfg.oar1_cfg.own_address > 0x3FFU)
            return I2C_STATUS_INVALID_PARAM;
        I2Cx->OAR1 |= ((i2c_cfg->oar_cfg.oar1_cfg.own_address & 0x3FFU) << I2C_OAR1_ADD0_POS);
    }
    else
    {
        if(i2c_cfg->oar_cfg.oar1_cfg.own_address > 0x7FU)
            return I2C_STATUS_INVALID_PARAM;
        I2Cx->OAR1 |= ((i2c_cfg->oar_cfg.oar1_cfg.own_address & 0x7FU) << I2C_OAR1_ADD_POS);
    }

    /* Configure OAR2 */
    I2Cx->OAR2 &= ~(I2C_OAR2_ENDUAL_MASK | I2C_OAR2_ADD2_MASK);
    if(i2c_cfg->oar_cfg.oar2_cfg.own_address > 0x7FU)
        return I2C_STATUS_INVALID_PARAM;
    I2Cx->OAR2 |= (i2c_cfg->oar_cfg.oar2_cfg.dual_mode & I2C_OAR2_ENDUAL_MASK);
    I2Cx->OAR2 |= ((i2c_cfg->oar_cfg.oar2_cfg.own_address & 0x7FU) << I2C_OAR2_ADD2_POS);

    /* Finally enable peripheral if requested in config */
    if(i2c_cfg->cr1_cfg.pe_enable)
    {
        I2C_Enable(I2Cx);
    }

    return I2C_STATUS_OK;
}

/*********************************************************************************************************************************************************************
 * @brief Deinitialize the I2C peripheral
 *
 * @param I2Cx: Pointer to the I2C peripheral instance (e.g., I2C1, I2C2, etc.)
 *
 * @note
 * This function resets the selected I2C peripheral through the RCC reset interface.
 */
void I2C_DeInit(I2C_TypeDef *I2Cx)
{
    if(I2Cx == I2C1)
    {
        RCC_ResetI2C(RCC_I2C1);
    }else
    {
        RCC_ResetI2C(RCC_I2C2);
    }
}

/*********************************************************************************************************************************************************************
 * @brief Set the I2C slave own address
 *
 * @param I2Cx: Pointer to the I2C peripheral instance (e.g., I2C1, I2C2, etc.)
 * @param address: Own address value to be programmed into the OAR1 register
 *
 * @retval I2C_STATUS_OK on success, or I2C_STATUS_INVALID_PARAM for an invalid address.
 */
I2C_Status_t I2C_SetOwnAddress(I2C_TypeDef *I2Cx, uint16_t address)
{
    if( (I2Cx != I2C1) &&
        (I2Cx != I2C2))
    {
        return I2C_STATUS_INVALID_PARAM;
    }
    
    I2Cx->OAR1 &= ~(I2C_OAR1_ADD0_MASK | 
                    I2C_OAR1_ADD_MASK  |
                    I2C_OAR1_ADD8_MASK |
                    I2C_OAR1_ADD9_MASK);
    I2Cx->OAR1 |= I2C_OAR1_BIT14_MASK;
    

    if(!(I2Cx->OAR1 & I2C_OAR1_ADDMODE_MASK))
    {
        if(address > 0x7FU)
        {
            return I2C_STATUS_INVALID_PARAM;
        }else
        {
            I2Cx->OAR1 |= (address & 0x7F) << I2C_OAR1_ADD_POS;
        }
    }
    else
    {
        if(address > 0x3FFU)
        {
            return I2C_STATUS_INVALID_PARAM;
        }else
        {
            I2Cx->OAR1 |= (address & 0x3U) << I2C_OAR1_ADD0_POS;
            I2Cx->OAR1 |= (address & 0x7EU) << I2C_OAR1_ADD_POS;
            I2Cx->OAR1 |= (address & 0x3U) << I2C_OAR1_ADD8_POS;
        }
    }

    return I2C_STATUS_OK;
}

/*********************************************************************************************************************************************************************
 * @brief Configure the I2C clock speed
 *
 * @param I2Cx: Pointer to the I2C peripheral instance (e.g., I2C1, I2C2, etc.)
 * @param pclk1_hz: APB1 clock frequency in hertz
 * @param scl_hz: Desired SCL clock frequency in hertz
 * @param duty_cycle: I2C duty-cycle selection
 *
 * @note
 * This function updates the CCR register to control the I2C timing.
 */
void I2C_SetSpeed(I2C_TypeDef *I2Cx, uint32_t pclk1_hz, uint32_t scl_hz, I2C_DutyCycle_t duty_cycle)
{
    I2Cx->CCR &= ~(I2C_CCR_CCR_MASK |
                   I2C_CCR_DUTY_MASK | 
                   I2C_CCR_FS_MASK);
    I2Cx->CCR |= duty_cycle;

    static uint16_t ccr_value;

    ccr_value = (pclk1_hz) / (2* scl_hz);
    I2Cx->CCR |= ccr_value;
}


/*********************************************************************************************************************************************************************
 * @brief Send the target slave address on the I2C bus
 *
 * @param I2Cx: Pointer to the I2C peripheral instance (e.g., I2C1, I2C2, etc.)
 * @param address: 7-bit slave address to transmit
 * @param dir: Transfer direction (read or write)
 *
 * @retval I2C_STATUS_OK on success, or an error code if the operation fails.
 */
I2C_Status_t I2C_SendAddress(I2C_TypeDef *I2Cx, uint8_t address, I2C_Direction_t dir)
{

}

/*********************************************************************************************************************************************************************
 * @brief Clear the ADDR flag after an address match
 *
 * @param I2Cx: Pointer to the I2C peripheral instance (e.g., I2C1, I2C2, etc.)
 *
 * @note
 * This function reads SR1 and SR2 in the proper sequence to clear the ADDR flag.
 */
void I2C_ClearADDRFlag(I2C_TypeDef *I2Cx)
{
    volatile uint32_t dummy;

    dummy = I2Cx->SR1;
    dummy = I2Cx->SR2;
    (void)dummy;
}

/*********************************************************************************************************************************************************************
 * @brief Clear a specific I2C error flag
 *
 * @param I2Cx: Pointer to the I2C peripheral instance (e.g., I2C1, I2C2, etc.)
 * @param err_flag: Error flag bitmask to clear
 *
 * @note
 * This function clears the selected hardware error status bits from SR1.
 */
void I2C_ClearErrorFlag(I2C_TypeDef *I2Cx, I2C_ErrorFlag_t err_flag)
{
    I2Cx->SR1 &= ~(uint32_t)err_flag;
}


/*********************************************************************************************************************************************************************
 * @brief Set the ACK position for the next data byte
 *
 * @param I2Cx: Pointer to the I2C peripheral instance (e.g., I2C1, I2C2, etc.)
 * @param pos: ACK position selection
 *
 * @note
 * This function updates the POS bit in the CR1 register.
 */
void I2C_SetACKPosition(I2C_TypeDef *I2Cx, I2C_AckPosition_t pos)
{
    I2Cx->CR1 &= ~I2C_CR1_POS_MASK;
    I2Cx->CR1 |= pos;
}


/*********************************************************************************************************************************************************************
 * @brief Wait until the START bit is set
 *
 * @param I2Cx: Pointer to the I2C peripheral instance (e.g., I2C1, I2C2, etc.)
 * @param timeout: Maximum number of polling iterations before timeout
 *
 * @retval I2C_STATUS_OK when the START condition is generated, otherwise I2C_STATUS_TIMEOUT.
 */
I2C_Status_t I2C_WaitForSB(I2C_TypeDef *I2Cx, uint32_t timeout)
{
    while(!(I2C_IsStartGenerated(I2Cx)))
    {
        if(timeout-- == 0U)
        {
            return I2C_STATUS_TIMEOUT;
        }
    }
    
    return I2C_STATUS_OK;
}

/*********************************************************************************************************************************************************************
 * @brief Wait until the address match flag is set
 *
 * @param I2Cx: Pointer to the I2C peripheral instance (e.g., I2C1, I2C2, etc.)
 * @param timeout: Maximum number of polling iterations before timeout
 *
 * @retval I2C_STATUS_OK when the address is matched, otherwise I2C_STATUS_TIMEOUT.
 */
I2C_Status_t I2C_WaitForADDR(I2C_TypeDef *I2Cx, uint32_t timeout)
{
    while(!(I2C_IsAddressMatched(I2Cx)))
    {
        if(timeout-- == 0U)
        {
            return I2C_STATUS_TIMEOUT;
        }
    }
    
    return I2C_STATUS_OK;
}

/*********************************************************************************************************************************************************************
 * @brief Wait until the byte transfer finished flag is set
 *
 * @param I2Cx: Pointer to the I2C peripheral instance (e.g., I2C1, I2C2, etc.)
 * @param timeout: Maximum number of polling iterations before timeout
 *
 * @retval I2C_STATUS_OK when the transfer is complete, otherwise I2C_STATUS_TIMEOUT.
 */
I2C_Status_t I2C_WaitForBTF(I2C_TypeDef *I2Cx, uint32_t timeout)
{
    while(!(I2C_IsBTF(I2Cx)))
    {
        if(timeout-- == 0U)
        {
            return I2C_STATUS_TIMEOUT;
        }
    }
    
    return I2C_STATUS_OK;   
}

/*********************************************************************************************************************************************************************
 * @brief Wait until the transmit data register is empty
 *
 * @param I2Cx: Pointer to the I2C peripheral instance (e.g., I2C1, I2C2, etc.)
 * @param timeout: Maximum number of polling iterations before timeout
 *
 * @retval I2C_STATUS_OK when TXE is set, otherwise I2C_STATUS_TIMEOUT.
 */
I2C_Status_t I2C_WaitForTXE(I2C_TypeDef *I2Cx, uint32_t timeout)
{
    while(!(I2C_IsTXE(I2Cx)))
    {
        if(timeout-- == 0U)
        {
            return I2C_STATUS_TIMEOUT;
        }
    }
    
    return I2C_STATUS_OK;
}

/*********************************************************************************************************************************************************************
 * @brief Wait until the receive data register is not empty
 *
 * @param I2Cx: Pointer to the I2C peripheral instance (e.g., I2C1, I2C2, etc.)
 * @param timeout: Maximum number of polling iterations before timeout
 *
 * @retval I2C_STATUS_OK when RXNE is set, otherwise I2C_STATUS_TIMEOUT.
 */
I2C_Status_t I2C_WaitForRXNE(I2C_TypeDef *I2Cx, uint32_t timeout)
{
    while(!(I2C_IsRXNE(I2Cx)))
    {
        if(timeout-- == 0U)
        {
            return I2C_STATUS_TIMEOUT;
        }
    }

    return I2C_STATUS_OK;
}


/*********************************************************************************************************************************************************************
 * @brief Transmit data in master mode to a slave device
 *
 * @param I2Cx: Pointer to the I2C peripheral instance (e.g., I2C1, I2C2, etc.)
 * @param slave_addr: 7-bit address of the target slave
 * @param pData: Pointer to the data buffer to transmit
 * @param len: Number of bytes to transmit
 *
 * @retval I2C_STATUS_OK on success, or an error code for timeout, bus, or acknowledgment issues.
 */
I2C_Status_t I2C_MasterTransmit(I2C_TypeDef *I2Cx,
                                uint8_t slave_addr,
                                uint8_t *pData,
                                uint16_t len)
{
    /* Check error*/
    if((I2Cx != I2C1) &&
        I2Cx != I2C2)
        {
            return I2C_STATUS_INVALID_PARAM;
        } 
    if(pData == NULL ||
       slave_addr > 0x7FU ||
       len == 0U)
    {
        return I2C_STATUS_INVALID_PARAM;
    }

    /* Check busy*/
    uint32_t timeout = 1000000U;
    while(I2C_IsBusy(I2Cx))
    {
        if(timeout-- == 0U)
        {
            return I2C_STATUS_BUSY;
        }
    }

    /* Generate start bit*/
    I2C_GenerateStart(I2Cx);
    
    timeout = 1000000U;
    while(!(I2C_IsStartGenerated(I2Cx)))
    {
        if(timeout-- == 0U)
        {
            I2C_GenerateStop(I2Cx);
            return I2C_STATUS_TIMEOUT;
        }
    }

    /* Sent slave address*/
    I2Cx->DR = (slave_addr << 1) | 0U;

    /* Check ADDR in SR1*/
    timeout = 1000000U;
    while(!(I2C_IsAddressMatched(I2Cx)))
    {
        if(I2C_IsAcknowledgeFailed(I2Cx))
        {
            I2C_ClearErrorFlag(I2Cx, I2C_ERROR_ACK_FAIL);
            I2C_GenerateStop(I2Cx);
            return I2C_STATUS_ACK_FAIL;
        }
        else if(I2C_IsBusError(I2Cx))
        {
            I2C_ClearErrorFlag(I2Cx, I2C_ERROR_BUS);
            I2C_GenerateStop(I2Cx);
            return I2C_STATUS_BUS_ERROR;
        }else if(I2C_IsArbitrationLost(I2Cx))
        {
            I2C_ClearErrorFlag(I2Cx, I2C_ERROR_ARBITRATION_LOSS);
            I2C_GenerateStop(I2Cx);
            return I2C_STATUS_ARBITRATION_LOST;
        }
        else if(timeout-- == 0U)
        {
            I2C_GenerateStop(I2Cx);
            return I2C_STATUS_TIMEOUT;
        }
    }

    /* Clear ADDR flag*/
    I2C_ClearADDRFlag(I2Cx);


    for(index = 0U; index < len; index++)
    {
        timeout = 1000000U;
        while(!I2C_IsTXE(I2Cx))
        {
            if(timeout-- == 0U)
            {
                return I2C_STATUS_TIMEOUT;
            }
        }

        I2Cx->DR = *pData;
        pData++;
    }

    /* Wait BTF*/
    timeout = 1000000U;
    while(!I2C_IsBTF(I2Cx))
    {
        if(timeout-- == 0U)
        {
            return I2C_STATUS_TIMEOUT;
        }
    }

    /* Generate stop bit*/
    I2C_GenerateStop(I2Cx);

    return I2C_STATUS_OK;
}


/*********************************************************************************************************************************************************************
 * @brief Receive data in master mode from a slave device
 *
 * @param I2Cx: Pointer to the I2C peripheral instance (e.g., I2C1, I2C2, etc.)
 * @param slave_addr: 7-bit address of the target slave
 * @param pData: Pointer to the receive buffer
 * @param len: Number of bytes to receive
 *
 * @retval I2C_STATUS_OK on success, or an error code for timeout, bus, or acknowledgment issues.
 */
I2C_Status_t I2C_MasterReceive(I2C_TypeDef *I2Cx,
                               uint8_t slave_addr,
                               uint8_t *pData,
                               uint16_t len)
{

    if((I2Cx != I2C1) &&
        (I2Cx != I2C2))
    {
        return I2C_STATUS_INVALID_PARAM;
    }

    if((pData == NULL) ||
        (len == 0U) ||
        (slave_addr > 0x7FU))
    {
        return I2C_STATUS_INVALID_PARAM;
    }

    timeout = 1000000U;
    while(I2C_IsBusy(I2Cx))
    {
        if(timeout-- == 0U)
        {
            return I2C_STATUS_TIMEOUT;
        }
    }

    /* Generate start*/
    I2C_GenerateStart(I2Cx);

    timeout = 1000000U;
    while(!(I2C_IsStartGenerated(I2Cx)))
    {
        if(timeout-- == 0U)
        {
            I2C_GenerateStop(I2Cx);
            return I2C_STATUS_TIMEOUT;
        }
    }

    I2Cx->DR = (uint8_t)((slave_addr << 1U) | 0x01U);

    timeout = 1000000U;
    while(!(I2C_IsAddressMatched(I2Cx)))
    {
        if(I2C_IsAcknowledgeFailed(I2Cx))
        {
            I2C_ClearErrorFlag(I2Cx, I2C_ERROR_ACK_FAIL);
            I2C_GenerateStop(I2Cx);
            return I2C_STATUS_ACK_FAIL;
        }
        if(I2C_IsBusError(I2Cx))
        {
            I2C_ClearErrorFlag(I2Cx, I2C_ERROR_BUS);
            I2C_GenerateStop(I2Cx);
            return I2C_STATUS_BUS_ERROR;
        }
        if(I2C_IsArbitrationLost(I2Cx))
        {
            I2C_ClearErrorFlag(I2Cx, I2C_ERROR_ARBITRATION_LOSS);
            I2C_GenerateStop(I2Cx);
            return I2C_STATUS_ARBITRATION_LOST;
        }
        if(timeout-- == 0U)
        {
            I2C_GenerateStop(I2Cx);
            return I2C_STATUS_TIMEOUT;
        }
    }

    I2C_ClearADDRFlag(I2Cx);

    if(len == 1U)
    {
        I2C_ACKDisable(I2Cx);

        timeout = 1000000U;
        while(!I2C_IsRXNE(I2Cx))
        {
            if(timeout-- == 0U)
            {
                I2C_GenerateStop(I2Cx);
                return I2C_STATUS_TIMEOUT;
            }
        }

        pData[0] = I2C_ReadData(I2Cx);
        I2C_GenerateStop(I2Cx);
        return I2C_STATUS_OK;
    }

    I2C_ACKEnable(I2Cx);

    for (index = 0U; index < len; index++)
    {
        timeout = 1000000U;
        while(!I2C_IsRXNE(I2Cx))
        {
            if(timeout-- == 0U)
            {
                I2C_GenerateStop(I2Cx);
                return I2C_STATUS_TIMEOUT;
            }
        }

        pData[index] = I2C_ReadData(I2Cx);

        if(index == (len - 2U))
        {
            I2C_ACKDisable(I2Cx);
        }

        if(index == (len - 1U))
        {
            I2C_GenerateStop(I2Cx);
            break;
        }
    }

    return I2C_STATUS_OK;
}

/*********************************************************************************************************************************************************************
 * @brief Perform a combined transmit and receive transaction in master mode
 *
 * @param I2Cx: Pointer to the I2C peripheral instance (e.g., I2C1, I2C2, etc.)
 * @param slave_addr: 7-bit address of the target slave
 * @param txData: Pointer to the transmit buffer
 * @param txLen: Number of bytes to transmit
 * @param rxData: Pointer to the receive buffer
 * @param rxLen: Number of bytes to receive
 *
 * @retval I2C_STATUS_OK on success, or an error code for timeout, bus, or acknowledgment issues.
 */
I2C_Status_t I2C_MasterTransmitReceive(I2C_TypeDef *I2Cx,
                                       uint8_t slave_addr,
                                       uint8_t *txData,
                                       uint16_t txLen,
                                       uint8_t *rxData,
                                       uint16_t rxLen)
{
    uint32_t timeout;
    uint16_t index;

    if((I2Cx != I2C1) &&
        (I2Cx != I2C2))
    {
        return I2C_STATUS_INVALID_PARAM;
    }

    if((slave_addr > 0x7FU) ||
        ((txLen > 0U) && (txData == NULL)) ||
        ((rxLen > 0U) && (rxData == NULL)))
    {
        return I2C_STATUS_INVALID_PARAM;
    }

    timeout = 1000000U;
    while(I2C_IsBusy(I2Cx))
    {
        if(timeout-- == 0U)
        {
            return I2C_STATUS_TIMEOUT;
        }
    }

    I2C_GenerateStart(I2Cx);

    timeout = 1000000U;
    while(!(I2Cx->SR1 & I2C_SR1_SB_MASK))
    {
        if(timeout-- == 0U)
        {
            I2C_GenerateStop(I2Cx);
            return I2C_STATUS_TIMEOUT;
        }
    }

    I2Cx->DR = (uint8_t)(slave_addr << 1U);

    timeout = 1000000U;
    while(!(I2C_IsAddressMatched(I2Cx)))
    {
        if(I2C_IsAcknowledgeFailed(I2Cx))
        {
            I2C_ClearErrorFlag(I2Cx, I2C_ERROR_ACK_FAIL);
            I2C_GenerateStop(I2Cx);
            return I2C_STATUS_ACK_FAIL;
        }
        if(I2C_IsBusError(I2Cx))
        {
            I2C_ClearErrorFlag(I2Cx, I2C_ERROR_BUS);
            I2C_GenerateStop(I2Cx);
            return I2C_STATUS_BUS_ERROR;
        }
        if(I2C_IsArbitrationLost(I2Cx))
        {
            I2C_ClearErrorFlag(I2Cx, I2C_ERROR_ARBITRATION_LOSS);
            I2C_GenerateStop(I2Cx);
            return I2C_STATUS_ARBITRATION_LOST;
        }
        if(timeout-- == 0U)
        {
            I2C_GenerateStop(I2Cx);
            return I2C_STATUS_TIMEOUT;
        }
    }

    I2C_ClearADDRFlag(I2Cx);

    for (index = 0U; index < txLen; index++)
    {
        timeout = 1000000U;
        while(!I2C_IsTXE(I2Cx))
        {
            if(I2C_IsAcknowledgeFailed(I2Cx))
            {
                I2C_ClearErrorFlag(I2Cx, I2C_ERROR_ACK_FAIL);
                I2C_GenerateStop(I2Cx);
                return I2C_STATUS_ACK_FAIL;
            }
            if(I2C_IsBusError(I2Cx))
            {
                I2C_ClearErrorFlag(I2Cx, I2C_ERROR_BUS);
                I2C_GenerateStop(I2Cx);
                return I2C_STATUS_BUS_ERROR;
            }
            if(timeout-- == 0U)
            {
                I2C_GenerateStop(I2Cx);
                return I2C_STATUS_TIMEOUT;
            }
        }

        I2Cx->DR = txData[index];
    }

    if(txLen > 0U)
    {
        timeout = 1000000U;
        while(!I2C_IsBTF(I2Cx))
        {
            if(timeout-- == 0U)
            {
                I2C_GenerateStop(I2Cx);
                return I2C_STATUS_TIMEOUT;
            }
        }
    }

    I2C_GenerateStart(I2Cx);

    timeout = 1000000U;
    while(!(I2Cx->SR1 & I2C_SR1_SB_MASK))
    {
        if(timeout-- == 0U)
        {
            I2C_GenerateStop(I2Cx);
            return I2C_STATUS_TIMEOUT;
        }
    }

    I2Cx->DR = (uint8_t)((slave_addr << 1U) | 0x01U);

    timeout = 1000000U;
    while(!(I2C_IsAddressMatched(I2Cx)))
    {
        if(I2C_IsAcknowledgeFailed(I2Cx))
        {
            I2C_ClearErrorFlag(I2Cx, I2C_ERROR_ACK_FAIL);
            I2C_GenerateStop(I2Cx);
            return I2C_STATUS_ACK_FAIL;
        }
        if(I2C_IsBusError(I2Cx))
        {
            I2C_ClearErrorFlag(I2Cx, I2C_ERROR_BUS);
            I2C_GenerateStop(I2Cx);
            return I2C_STATUS_BUS_ERROR;
        }
        if(I2C_IsArbitrationLost(I2Cx))
        {
            I2C_ClearErrorFlag(I2Cx, I2C_ERROR_ARBITRATION_LOSS);
            I2C_GenerateStop(I2Cx);
            return I2C_STATUS_ARBITRATION_LOST;
        }
        if(timeout-- == 0U)
        {
            I2C_GenerateStop(I2Cx);
            return I2C_STATUS_TIMEOUT;
        }
    }

    I2C_ClearADDRFlag(I2Cx);

    if(rxLen == 0U)
    {
        I2C_GenerateStop(I2Cx);
        return I2C_STATUS_OK;
    }

    I2C_ACKEnable(I2Cx);

    for (index = 0U; index < rxLen; index++)
    {
        timeout = 1000000U;
        while(!I2C_IsRXNE(I2Cx))
        {
            if(timeout-- == 0U)
            {
                I2C_GenerateStop(I2Cx);
                return I2C_STATUS_TIMEOUT;
            }
        }

        rxData[index] = I2C_ReadData(I2Cx);

        if(index == (rxLen - 2U))
        {
            I2C_ACKDisable(I2Cx);
        }

        if(index == (rxLen - 1U))
        {
            I2C_GenerateStop(I2Cx);
            break;
        }
    }

    return I2C_STATUS_OK;
}


/*********************************************************************************************************************************************************************
 * @brief Transmit data in slave mode
 *
 * @param I2Cx: Pointer to the I2C peripheral instance (e.g., I2C1, I2C2, etc.)
 * @param pData: Pointer to the data buffer to transmit
 * @param len: Number of bytes to transmit
 *
 * @retval I2C_STATUS_OK on success, or an error code for bus or acknowledgment issues.
 */
I2C_Status_t I2C_SlaveTransmit(I2C_TypeDef *I2Cx,
                               uint8_t *pData,
                               uint16_t len)
{
    if((I2Cx != I2C1) &&
       (I2Cx != I2C2))
    {
        return I2C_STATUS_INVALID_PARAM;
    }
    if((len == 0U) ||
       (pData == NULL))
    {
        return I2C_STATUS_INVALID_PARAM;
    }

    /* Wait Adress matched*/
    timeout = 1000000U;
    while(!(I2C_IsAddressMatched(I2Cx)))
    {
        if(I2Cx->SR1 & I2C_SR1_BERR_MASK)
        {
            I2C_ClearErrorFlag(I2Cx, I2C_ERROR_BUS);
            return I2C_STATUS_BUS_ERROR;
        }

        if(I2Cx->SR1 & I2C_SR1_ARLO_MASK)
        {
            I2C_ClearErrorFlag(I2Cx, I2C_ERROR_ARBITRATION_LOSS);
            return I2C_STATUS_ARBITRATION_LOST;
        }

        if(I2Cx->SR1 & I2C_SR1_OVR_MASK)
        {
            I2C_ClearErrorFlag(I2Cx, I2C_ERROR_OVERRUN);
            return I2C_STATUS_OVERRUN;
        }
        if(timeout-- == 0U)
        {
            return I2C_STATUS_TIMEOUT;
        }
    }

    /* Clear adress matched*/
    I2C_ClearADDRFlag(I2Cx);

    /* Check Transmit/Receive*/
    if(!(I2Cx->SR2 & I2C_SR2_TRA_MASK))
    {
        return I2C_STATUS_ERROR;
    }

    /* Write data in data register*/
    for(index = 0U; index < len; index++)
    {
        timeout = 1000000U;
        while(!(I2C_IsTXE(I2Cx)))
        {
            if(I2C_IsAcknowledgeFailed(I2Cx))
            {
                I2C_ClearErrorFlag(I2Cx, I2C_ERROR_ACK_FAIL);
                return I2C_STATUS_OK;
            }
            if(I2Cx->SR1 & I2C_SR1_BERR_MASK)
            {
                I2C_ClearErrorFlag(I2Cx, I2C_ERROR_BUS);
                return I2C_STATUS_BUS_ERROR;
            }

            if(I2Cx->SR1 & I2C_SR1_ARLO_MASK)
            {
                I2C_ClearErrorFlag(I2Cx, I2C_ERROR_ARBITRATION_LOSS);
                return I2C_STATUS_ARBITRATION_LOST;
            }

            if(I2Cx->SR1 & I2C_SR1_OVR_MASK)
            {
                I2C_ClearErrorFlag(I2Cx, I2C_ERROR_OVERRUN);
                return I2C_STATUS_OVERRUN;
            }
            if(timeout-- == 0U)
            {
                return I2C_STATUS_TIMEOUT;
            }
        }
        I2Cx->DR = *pData;
        pData++;
    }

    /* Clear Acknowledge Failed flag*/
    timeout = 1000000U;
    while(I2C_IsAcknowledgeFailed(I2Cx))
    {
        if(timeout-- == 0U)
        {
            return I2C_STATUS_TIMEOUT;
        }
    }
    I2C_ClearErrorFlag(I2Cx, I2C_ERROR_ACK_FAIL);
    
    return I2C_STATUS_OK;
}

/*********************************************************************************************************************************************************************
 * @brief Receive data in slave mode
 *
 * @param I2Cx: Pointer to the I2C peripheral instance (e.g., I2C1, I2C2, etc.)
 * @param pData: Pointer to the receive buffer
 * @param len: Number of bytes to receive
 *
 * @retval I2C_STATUS_OK on success, or an error code for bus or overrun issues.
 */
I2C_Status_t I2C_SlaveReceive(I2C_TypeDef *I2Cx,
                              uint8_t *pData,
                              uint16_t len)
{
    /* Check parameter*/
    if((I2Cx != I2C1) &&
       (I2Cx != I2C2))
    {
        return I2C_STATUS_INVALID_PARAM;
    }
    if((len == 0U) ||
       (pData == NULL))
    {
        return I2C_STATUS_INVALID_PARAM;
    }

    /* Wait Address Match*/
    timeout = 1000000U;
    while(!(I2C_IsAddressMatched(I2Cx)))
    {
        if(I2C_IsAcknowledgeFailed(I2Cx))
        {
            I2C_ClearErrorFlag(I2Cx, I2C_ERROR_ACK_FAIL);
            return I2C_STATUS_ACK_FAIL;
        }
        if(timeout-- == 0U)
        {
            return I2C_STATUS_TIMEOUT;
        }
    }

    /* Clear Address matched flag*/
    I2C_ClearADDRFlag(I2Cx);

    if(I2Cx->SR2 & I2C_SR2_TRA_MASK)
    {
        return I2C_STATUS_ERROR;
    }

    /* Read data in DR*/
    for(index = 0U; index < len; index++)
    {
        timeout = 1000000U;
        while(!(I2C_IsRXNE(I2Cx)))
        {
            if(I2Cx->SR1 & I2C_SR1_BERR_MASK)
            {
                I2C_ClearErrorFlag(I2Cx, I2C_ERROR_BUS);
                return I2C_STATUS_BUS_ERROR;
            }
 
            if(I2Cx->SR1 & I2C_SR1_OVR_MASK)
            {
                I2C_ClearErrorFlag(I2Cx, I2C_ERROR_OVERRUN);
                return I2C_STATUS_OVERRUN;
            }
            if(timeout-- == 0U)
            {
                return I2C_STATUS_TIMEOUT;
            }
        }
        
        *pData = (uint8_t)I2Cx->DR;
        pData++;
    }

    timeout = 1000000U;
    while(!(I2C_IsStopDetected(I2Cx)))
    {
        if(I2Cx->SR1 & I2C_SR1_BERR_MASK)
        {
            I2C_ClearErrorFlag(I2Cx, I2C_ERROR_BUS);
            return I2C_STATUS_BUS_ERROR;
        }

        if(I2Cx->SR1 & I2C_SR1_OVR_MASK)
        {
            I2C_ClearErrorFlag(I2Cx, I2C_ERROR_OVERRUN);
            return I2C_STATUS_OVERRUN;
        }
        if(timeout-- == 0U)
        {
            return I2C_STATUS_TIMEOUT;
        }
    }

    /*Clear STOFF*/
    (void)I2Cx->SR1;
    I2Cx->CR1 |= I2Cx->SR1;
    
    return I2C_STATUS_OK;
}


/*********************************************************************************************************************************************************************
 * @brief Write data to an internal memory address on an I2C slave device
 *
 * @param I2Cx: Pointer to the I2C peripheral instance (e.g., I2C1, I2C2, etc.)
 * @param slave_addr: 7-bit address of the target slave
 * @param mem_addr: Internal memory/register address to write
 * @param pData: Pointer to the data buffer to write
 * @param len: Number of bytes to write
 *
 * @retval I2C_STATUS_OK on success, or an error code for timeout or acknowledgment issues.
 */
I2C_Status_t I2C_MemWrite(I2C_TypeDef *I2Cx,
                          uint8_t slave_addr,
                          uint8_t mem_addr,
                          uint8_t *pData,
                          uint16_t len)
{
    /* Check parameter*/
    if((I2Cx != I2C1) &&
       (I2Cx != I2C2))
    {
        return I2C_STATUS_INVALID_PARAM;
    }
    if((len == 0U) ||
       (pData == NULL))
    {
        return I2C_STATUS_INVALID_PARAM;
    }

    /* Wait busy = 0*/
    timeout = 1000000U;
    while(I2C_IsBusy(I2Cx))
    {
        if(timeout-- == 0U)
        {
            return I2C_STATUS_BUSY;
        }
    }

    /* Generate start*/
    I2C_GenerateStart(I2Cx);

    /* Wait start bit*/
    timeout = 1000000U;
    while(!(I2C_IsStartGenerated(I2Cx)))
    {
        if(timeout-- == 0U)
        {
            return I2C_STATUS_TIMEOUT;
        }
    }

    /* Send slave address + Write*/
    I2Cx->DR = (slave_addr << 1U) | 0U;

    /* Wait address matched flag*/
    timeout = 1000000U;
    while(!(I2C_IsAddressMatched(I2Cx)))
    {
        if(I2C_IsAcknowledgeFailed(I2Cx))
        {
            I2C_ClearErrorFlag(I2Cx, I2C_ERROR_ACK_FAIL);
            I2C_GenerateStop(I2Cx);
            return I2C_STATUS_ACK_FAIL;
        }
        if(timeout-- == 0U)
        {
            return I2C_STATUS_TIMEOUT;
        }
    }

    /* Clear address matched flag*/
    tmp = I2Cx->SR1;
    tmp = I2Cx->SR2;
    (void)tmp;

    timeout = 1000000U;
    while(!(I2C_IsTXE(I2Cx)))
    {
        if(timeout-- == 0U)
        {
            return I2C_STATUS_TIMEOUT;
        }
    }

    /* Send memory address*/
    I2Cx->DR = mem_addr;

    /* Check byte transfer finished*/
    timeout = 1000000U;
    while(!I2C_IsBTF(I2Cx))
    {
    if(I2C_IsAcknowledgeFailed(I2Cx))
    {
        I2C_ClearErrorFlag(I2Cx, I2C_ERROR_ACK_FAIL);
        I2C_GenerateStop(I2Cx);
        return I2C_STATUS_ACK_FAIL;
    }

    if(timeout-- == 0U)
    {
        return I2C_STATUS_TIMEOUT;
    }
}
    /* Send data*/
    for(index = 0U; index < len; index++)
    {
        timeout = 1000000U;
        while(!(I2C_IsTXE(I2Cx)))
        {
            if(I2C_IsAcknowledgeFailed(I2Cx))
            {
                I2C_ClearErrorFlag(I2Cx, I2C_ERROR_ACK_FAIL);
                I2C_GenerateStop(I2Cx);
                return I2C_STATUS_ACK_FAIL;
            }
            if(timeout-- == 0U)
            {
                return I2C_STATUS_TIMEOUT;
            }
        }

        I2Cx->DR = *pData;
        pData++;
    }

    timeout = 1000000U;
    while(!I2C_IsBTF(I2Cx))
    {
        if(timeout-- == 0U)
        {
            return I2C_STATUS_TIMEOUT; 
        }
    }

    I2C_GenerateStop(I2Cx);

    return I2C_STATUS_OK;
}

/*********************************************************************************************************************************************************************
 * @brief Read data from an internal memory address on an I2C slave device
 *
 * @param I2Cx: Pointer to the I2C peripheral instance (e.g., I2C1, I2C2, etc.)
 * @param slave_addr: 7-bit address of the target slave
 * @param mem_addr: Internal memory/register address to read
 * @param pData: Pointer to the receive buffer
 * @param len: Number of bytes to read
 *
 * @retval I2C_STATUS_OK on success, or an error code for timeout or acknowledgment issues.
 */
I2C_Status_t I2C_MemRead(I2C_TypeDef *I2Cx,
                         uint8_t slave_addr,
                         uint8_t mem_addr,
                         uint8_t *pData,
                         uint16_t len)
{
    /* Check parameter*/
    if((I2Cx != I2C1) &&
       (I2Cx != I2C2))
    {
        return I2C_STATUS_INVALID_PARAM;
    }
    if((pData == NULL) ||
       (len == 0U))
    {
        return I2C_STATUS_INVALID_PARAM;
    }

    /* Write busy = 0*/
    timeout = 1000000U;
    while(I2C_IsBusy(I2Cx))
    {
        if(timeout-- == 0U)
        {
            return I2C_STATUS_TIMEOUT;
        }
    }

    /* case len = 1*/
    if(len == 1)
    {
        /* Generate start*/
        I2C_GenerateStart(I2Cx);
        
        /* Check bit start*/
        timeout = 1000000U;
        while(!(I2C_IsStartGenerated(I2Cx)))
        {
            if(timeout-- == 0U)
            {
                return I2C_STATUS_TIMEOUT;
            }
        }

        /* Sent slave address and Write */
        I2Cx->DR = (slave_addr << 1U) | 0U;

        /* Wait address mztched flag*/  
        timeout = 1000000U;
        while(!(I2C_IsAddressMatched(I2Cx)))
        {
            if(I2C_IsAcknowledgeFailed(I2Cx))
            {
                I2C_ClearErrorFlag(I2Cx, I2C_ERROR_ACK_FAIL);
                return I2C_STATUS_ACK_FAIL;
            }
            if(timeout-- == 0U)
            {
                return I2C_STATUS_TIMEOUT;
            }
        }

        /* Clear address sent/match*/
        tmp = I2Cx->SR1;
        tmp = I2Cx->SR2;
        (void)tmp;

        /* Wait TxE*/
        timeout = 1000000U;
        while(!(I2C_IsTXE(I2Cx)))
        {
            if(timeout-- == 0U)
            {
                return I2C_STATUS_TIMEOUT;
            }
        }

        /* Send memory address*/
        I2Cx->DR = mem_addr;

        /*Wait BTF */
        timeout = 1000000U;
        while(!(I2C_IsBTF(I2Cx)))
        {
            if(timeout-- == 0U)
            {
                return I2C_STATUS_TIMEOUT;
            }
        }

        /* Repeated Start*/
        I2C_GenerateStart(I2Cx);

        /* Check bit start*/
        timeout = 1000000U;
        while(!(I2C_IsStartGenerated(I2Cx)))
        {
            if(timeout-- == 0U)
            {
                return I2C_STATUS_TIMEOUT;
            }
        }

        /*Sent slave address + Read*/
        I2Cx->DR = (slave_addr << 1U) | 1U;

        /* Wait address mztched flag*/  
        timeout = 1000000U;
        while(!(I2C_IsAddressMatched(I2Cx)))
        {
            if(I2C_IsAcknowledgeFailed(I2Cx))
            {
                I2C_ClearErrorFlag(I2Cx, I2C_ERROR_ACK_FAIL);
                I2C_GenerateStop(I2Cx);
                return I2C_STATUS_ACK_FAIL;
            }
            if(timeout-- == 0U)
            {
                return I2C_STATUS_TIMEOUT;
            }
        }
        
        /* Disable ACK*/
        I2C_ACKDisable(I2Cx);

        /* Clear address sent/match*/
        tmp = I2Cx->SR1;
        tmp = I2Cx->SR2;
        (void)tmp;

        /* Generate stop*/
        I2C_GenerateStop(I2Cx);

        /* Read data*/
        timeout = 1000000U;
        while(!(I2C_IsRXNE(I2Cx)))
        {
            if(timeout-- == 0U)
            {
                return I2C_STATUS_TIMEOUT;
            }
        }
        *pData = I2Cx->DR;

        /* Restore ACK*/
        I2C_ACKEnable(I2Cx);
        
        return I2C_STATUS_OK;
    }

    /* case len = 2*/
    if(len == 2)
    {

        /* ACK/PEC Position*/
        I2Cx->CR1 |= I2C_CR1_POS_MASK;
        I2Cx->CR1 &= ~I2C_CR1_ACK_MASK;
        
        /* Generate start*/
        I2C_GenerateStart(I2Cx);
        
        /* Check start bit*/
        timeout = 1000000U;
        while(!(I2C_IsStartGenerated(I2Cx)))
        {
            if(timeout-- == 0U)
            {
                return I2C_STATUS_TIMEOUT;
            }
        }

        /* Sent slave address and Write*/
        I2Cx->DR = (slave_addr << 1U) | 0U;

        /* Wait address match flag*/
        timeout = 1000000U;
        while(!(I2C_IsAddressMatched(I2Cx)))
        {
            if(I2C_IsAcknowledgeFailed(I2Cx))
            {
                I2C_ClearErrorFlag(I2Cx, I2C_ERROR_ACK_FAIL);
                return I2C_STATUS_ACK_FAIL;
            }
            if(timeout-- == 0U)
            {
                return I2C_STATUS_TIMEOUT;
            }
        }

        /* Clear address match flag*/
        tmp = I2Cx->SR1;
        tmp = I2Cx->SR2;
        (void)tmp;

        /* Wait TxE*/
        timeout = 1000000U;
        while(!(I2C_IsTXE(I2Cx)))
        {
            if(timeout-- == 0U)
            {
                return I2C_STATUS_TIMEOUT;
            }
        }

        /* Send memory address*/
        I2Cx->DR = mem_addr;

        /* Wait data register empty*/
        timeout = 1000000U;
        while(!(I2C_IsBTF(I2Cx)))
        {

            if(timeout-- == 0U)
            {
                return I2C_STATUS_TIMEOUT;
            }
        }

        /* Repeated start*/
        I2C_GenerateStart(I2Cx);

        /* Check start bit*/
        timeout = 1000000U;
        while(!(I2C_IsStartGenerated(I2Cx)))
        {
            if(timeout-- == 0U)
            {
                return I2C_STATUS_TIMEOUT;
            }
        }

        /* Sent Slave address and Read*/
        I2Cx->DR = (slave_addr << 1U) | 1U;

        /* Wait address mztched flag*/  
        timeout = 1000000U;
        while(!(I2C_IsAddressMatched(I2Cx)))
        {
            if(I2C_IsAcknowledgeFailed(I2Cx))
            {
                I2C_ClearErrorFlag(I2Cx, I2C_ERROR_ACK_FAIL);
                return I2C_STATUS_ACK_FAIL;
            }
            if(timeout-- == 0U)
            {
                return I2C_STATUS_TIMEOUT;
            }
        }

        /* Clear ACK*/
        I2C_ACKDisable(I2Cx);

        /* Clear address sent/match*/
        tmp = I2Cx->SR1;
        tmp = I2Cx->SR2;
        (void)tmp;

        /* Generate stop*/
        I2C_GenerateStop(I2Cx);
    
        timeout = 1000000U;
        while(!(I2C_IsBTF(I2Cx)))
        {
            if(I2C_IsAcknowledgeFailed(I2Cx))
            {
                I2C_ClearErrorFlag(I2Cx, I2C_ERROR_ACK_FAIL);
                return I2C_STATUS_ACK_FAIL;
            }
            if(timeout-- == 0U)
            {
                return I2C_STATUS_TIMEOUT;
            }
        }

        pData[0] = I2Cx->DR;
        pData[1] = I2Cx->DR;

        /* Disable ACK/PEC Position*/
        I2Cx->CR1 &= ~I2C_CR1_POS_MASK;

        /* Enable ACK*/
        I2C_ACKEnable(I2Cx);
        
        return I2C_STATUS_OK;
    }


    /* Read data (len >= 3) */
    /* Generate START */
    I2C_GenerateStart(I2Cx);

    /* Wait SB */
    timeout = 1000000U;
    while(!I2C_IsStartGenerated(I2Cx))
    {
        if(timeout-- == 0U)
            return I2C_STATUS_TIMEOUT;
    }

    /* Send Slave Address + Write */
    I2Cx->DR = (slave_addr << 1U);

    /* Wait ADDR */
    timeout = 1000000U;
    while(!I2C_IsAddressMatched(I2Cx))
    {
        if(I2C_IsAcknowledgeFailed(I2Cx))
        {
            I2C_ClearErrorFlag(I2Cx, I2C_ERROR_ACK_FAIL);
            return I2C_STATUS_ACK_FAIL;
        }

        if(timeout-- == 0U)
            return I2C_STATUS_TIMEOUT;
    }

    /* Clear ADDR */
    tmp = I2Cx->SR1;
    tmp = I2Cx->SR2;
    (void)tmp;

    /* Wait TXE */
    timeout = 1000000U;
    while(!I2C_IsTXE(I2Cx))
    {
        if(timeout-- == 0U)
            return I2C_STATUS_TIMEOUT;
    }

    /* Send Memory Address */
    I2Cx->DR = mem_addr;

    /* Wait BTF */
    timeout = 1000000U;
    while(!I2C_IsBTF(I2Cx))
    {
        if(timeout-- == 0U)
            return I2C_STATUS_TIMEOUT;
    }

    /* Repeated START */
    I2C_GenerateStart(I2Cx);

    /* Wait SB */
    timeout = 1000000U;
    while(!I2C_IsStartGenerated(I2Cx))
    {
        if(timeout-- == 0U)
            return I2C_STATUS_TIMEOUT;
    }

    /* Send Slave Address + Read */
    I2Cx->DR = (slave_addr << 1U) | 1U;

    /* Wait ADDR */
    timeout = 1000000U;
    while(!I2C_IsAddressMatched(I2Cx))
    {
        if(I2C_IsAcknowledgeFailed(I2Cx))
        {
            I2C_ClearErrorFlag(I2Cx, I2C_ERROR_ACK_FAIL);
            return I2C_STATUS_ACK_FAIL;
        }

        if(timeout-- == 0U)
            return I2C_STATUS_TIMEOUT;
    }

    /* Enable ACK */
    I2C_ACKEnable(I2Cx);

    /* Clear ADDR */
    tmp = I2Cx->SR1;
    tmp = I2Cx->SR2;
    (void)tmp;

    /* Read bytes until 3 bytes remain */
    while(len > 3U)
    {
        timeout = 1000000U;
        while(!I2C_IsRXNE(I2Cx))
        {
            if(I2C_IsAcknowledgeFailed(I2Cx))
            {
                I2C_ClearErrorFlag(I2Cx, I2C_ERROR_ACK_FAIL);
                I2C_GenerateStop(I2Cx);
                return I2C_STATUS_ACK_FAIL;
            }
            if(timeout-- == 0U)
            {
                I2C_GenerateStop(I2Cx);
                return I2C_STATUS_TIMEOUT;
            }
        }

        *pData++ = I2Cx->DR;
        len--;
    }

    /* Wait BTF (3 bytes remaining) */
    timeout = 1000000U;
    while(!I2C_IsBTF(I2Cx))
    {
        if(timeout-- == 0U)
        {
            I2C_GenerateStop(I2Cx);
            return I2C_STATUS_TIMEOUT;
        }
    }

    /* Disable ACK (NACK byte N) */
    I2C_ACKDisable(I2Cx);

    /* Read byte N-2 */
    *pData++ = I2Cx->DR;

    /* Generate STOP */
    I2C_GenerateStop(I2Cx);

    /* Read byte N-1 */
    *pData++ = I2Cx->DR;

    /* Wait RXNE */
    timeout = 1000000U;
    while(!I2C_IsRXNE(I2Cx))
    {
        if(I2C_IsAcknowledgeFailed(I2Cx))
        {
            I2C_ClearErrorFlag(I2Cx, I2C_ERROR_ACK_FAIL);
            I2C_GenerateStop(I2Cx);
            return I2C_STATUS_ACK_FAIL;
        }
        if(timeout-- == 0U)
        {
            I2C_GenerateStop(I2Cx);
            return I2C_STATUS_TIMEOUT;
        }
    }

    /* Read byte N */
    *pData = I2Cx->DR;

    /* Restore ACK */
    I2C_ACKEnable(I2Cx);

    return I2C_STATUS_OK;

}


/*********************************************************************************************************************************************************************
 * @brief Check whether a slave device responds on the I2C bus
 *
 * @param I2Cx: Pointer to the I2C peripheral instance (e.g., I2C1, I2C2, etc.)
 * @param slave_addr: 7-bit address of the target slave
 * @param trials: Number of probing attempts to perform
 *
 * @retval I2C_STATUS_OK if the device acknowledges, otherwise an error status.
 */
I2C_Status_t I2C_IsDeviceReady(I2C_TypeDef *I2Cx,
                               uint8_t slave_addr,
                               uint32_t trials)
{
    uint32_t attempt;

    /* Check parameter*/
    if((I2Cx != I2C1) &&
       (I2Cx != I2C2))
    {
        return I2C_STATUS_INVALID_PARAM;
    }

    if(slave_addr > 0x7FU)
    {
        return I2C_STATUS_INVALID_PARAM;
    }

    if(trials == 0U)
    {
        return I2C_STATUS_TIMEOUT;
    }

    for(attempt = 0U; attempt < trials; attempt++)
    {
        /* Wait busy*/
        timeout = 1000000U;
        while(I2C_IsBusy(I2Cx))
        {
            if(timeout-- == 0U)
            {
                return I2C_STATUS_BUSY;
            }
        }

        /* Generate start*/
        I2C_GenerateStart(I2Cx);

        /* Wait start bit*/
        timeout = 1000000U;
        while(!(I2C_IsStartGenerated(I2Cx)))
        {
            if(timeout-- == 0U)
            {
                I2C_GenerateStop(I2Cx);
                return I2C_STATUS_TIMEOUT;
            }
        }

        I2Cx->DR = (uint8_t)(slave_addr << 1U) | 0U;

        /* Wait address mztched flag*/  
        timeout = 1000000U;
        while(!(I2C_IsAddressMatched(I2Cx)))
        {
            if(I2C_IsAcknowledgeFailed(I2Cx))
            {
                I2C_ClearErrorFlag(I2Cx, I2C_ERROR_ACK_FAIL);
                I2C_GenerateStop(I2Cx);
                break;
            }

            if(I2C_IsBusError(I2Cx))
            {
                I2C_ClearErrorFlag(I2Cx, I2C_ERROR_BUS);
                I2C_GenerateStop(I2Cx);
                return I2C_STATUS_BUS_ERROR;
            }

            if(I2C_IsArbitrationLost(I2Cx))
            {
                I2C_ClearErrorFlag(I2Cx, I2C_ERROR_ARBITRATION_LOSS);
                I2C_GenerateStop(I2Cx);
                return I2C_STATUS_ARBITRATION_LOST;
            }
            if(timeout-- == 0U)
            {
                I2C_GenerateStop(I2Cx);
                break;
            }
        }

        if(I2C_IsAddressMatched(I2Cx))
        {
            I2C_ClearADDRFlag(I2Cx);
            I2C_GenerateStop(I2Cx);
            return I2C_STATUS_OK;
        }
    }

    return I2C_STATUS_TIMEOUT;
}


/*********************************************************************************************************************************************************************
 * @brief Scan the I2C bus for connected slave devices
 *
 * @param I2Cx: Pointer to the I2C peripheral instance (e.g., I2C1, I2C2, etc.)
 * @param found_list: Buffer to store the addresses of discovered devices
 * @param found_count: Pointer to the number of discovered devices
 *
 * @retval I2C_STATUS_OK on success, or an error code if the scan fails.
 */
I2C_Status_t I2C_ScanBus(I2C_TypeDef *I2Cx,
                         uint8_t *found_list,
                         uint8_t *found_count)
{

    /* Check parameter*/
    if(I2Cx != I2C1 &&
       I2Cx != I2C2)
    {
        return I2C_STATUS_INVALID_PARAM;
    }    
    if(found_list == NULL ||
       found_count == NULL)
    {
        return I2C_STATUS_INVALID_PARAM;
    }
    *found_count = 0U;

    /* Scan bus*/
    for(index = 0x08U; index <= 0x77U; index++)
    {
        status = I2C_IsDeviceReady(I2Cx, index, 10U);
        switch(status)
        {
            case I2C_STATUS_ARBITRATION_LOST:   break;
            
            case I2C_STATUS_ACK_FAIL:           break;
            
            case I2C_STATUS_TIMEOUT:            break;
            
            case I2C_STATUS_BUSY:               break;
            
            case I2C_STATUS_OK:
            found_list[*found_count] = index;
            (*found_count)++;
            break;
            
            case I2C_STATUS_BUS_ERROR:          return status;
            
            default:
                return status;
        }
    }
    return I2C_STATUS_OK;
}

/*********************************************************************************************************************************************************************
 * @brief Recover the I2C bus after an error or stuck condition
 *
 * @param I2Cx: Pointer to the I2C peripheral instance (e.g., I2C1, I2C2, etc.)
 *
 * @retval I2C_STATUS_OK on success, or I2C_STATUS_INVALID_PARAM if the peripheral is invalid.
 *
 * @note
 * This function disables and re-enables the peripheral, clears error flags, and generates a recovery sequence on the SCL/SDA lines.
 */
I2C_Status_t I2C_BusRecover(I2C_TypeDef *I2Cx)
{
    GPIO_TypeDef *gpio = GPIOB;
    uint16_t scl_pin;
    uint16_t sda_pin;
    uint16_t pin_mask;
    volatile uint32_t delay;
    uint32_t i;

    /* Check parameter */
    if((I2Cx != I2C1) && (I2Cx != I2C2))
    {
        return I2C_STATUS_INVALID_PARAM;
    }

    if(I2Cx == I2C1)
    {
        scl_pin = 6U;
        sda_pin = 7U;
    }
    else
    {
        scl_pin = 10U;
        sda_pin = 11U;
    }

    pin_mask = (uint16_t)((1U << scl_pin) | (1U << sda_pin));

    /* Disable I2C peripheral */
    I2C_Disable(I2Cx);

    /* Enable GPIO clock */
    RCC->APB2ENR |= RCC_APB2ENR_GPIOB_MASK;

    /* Configure SCL/SDA as GPIO Open-Drain */
    GPIO_Init(gpio, pin_mask, GPIO_MODE_OUTPUT_OD_2MHz);

    /* Release both lines */
    GPIO_WritePin(gpio, scl_pin, HIGH);
    GPIO_WritePin(gpio, sda_pin, HIGH);

    for(delay = 0U; delay < 1000U; delay++);

    /* Generate up to 9 clock pulses */
    for(i = 0U; i < 9U; i++)
    {
        GPIO_WritePin(gpio, scl_pin, LOW);
        for(delay = 0U; delay < 1000U; delay++);

        GPIO_WritePin(gpio, scl_pin, HIGH);
        for(delay = 0U; delay < 1000U; delay++);

        /* Slave has released SDA */
        if(GPIO_ReadPin(gpio, sda_pin) == HIGH)
        {
            break;
        }
    }

    /* SDA still held low */
    if(GPIO_ReadPin(gpio, sda_pin) == LOW)
    {
        GPIO_Init(gpio, pin_mask, GPIO_MODE_AF_OUTPUT_OD_2MHz);
        I2C_Enable(I2Cx);

        return I2C_STATUS_BUSY;
    }

    /* Generate STOP condition
     * SDA LOW -> SCL HIGH -> SDA HIGH
     */
    GPIO_WritePin(gpio, sda_pin, LOW);
    for(delay = 0U; delay < 1000U; delay++);

    GPIO_WritePin(gpio, scl_pin, HIGH);
    for(delay = 0U; delay < 1000U; delay++);

    GPIO_WritePin(gpio, sda_pin, HIGH);
    for(delay = 0U; delay < 1000U; delay++);

    /* Restore Alternate Function Open-Drain */
    GPIO_Init(gpio, pin_mask, GPIO_MODE_AF_OUTPUT_OD_2MHz);

    /* Reset I2C peripheral */
    if(I2Cx == I2C1)
    {
        RCC_ResetI2C(RCC_I2C1);
    }
    else
    {
        RCC_ResetI2C(RCC_I2C2);
    }

    /* Clear error flags */
    I2C_ClearErrorFlag(I2Cx, I2C_ERROR_BUS);
    I2C_ClearErrorFlag(I2Cx, I2C_ERROR_ARBITRATION_LOSS);
    I2C_ClearErrorFlag(I2Cx, I2C_ERROR_ACK_FAIL);
    I2C_ClearErrorFlag(I2Cx, I2C_ERROR_OVERRUN);

    /* Enable I2C peripheral */
    I2C_Enable(I2Cx);

    return I2C_STATUS_OK;
}



