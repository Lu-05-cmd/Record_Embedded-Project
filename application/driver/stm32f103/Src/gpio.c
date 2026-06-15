/*********************************************************************************************************************************************************************
 * @file gpio.c
 * @brief GPIO driver implementation for STM32F103.
 *
 * @details
 * This file provides GPIO initialization, read/write, toggle, pull/ speed
 * configuration, alternate function setup, and lock mechanisms for STM32F103
 * GPIO peripherals.
 * 
 * @author lu-05-cmd
 * @date 2026-03-25
 * @version 0.1.0
 * 
 * @note
 * The implementation uses direct register access and assumes the GPIO peripheral
 * base structures are defined in the included header.
 *
 ********************************************************************************************************************************************************************/


 /*INCLUDE============================================================================================================================================================*/
#include <stdint.h>
#include "../Inc/gpio.h"


/*********************************************************************************************************************************************************************
 * @brief Initialize GPIO pins with specified mode
 * 
 * @param GPIOx: Pointer to GPIO port (e.g., GPIOA, GPIOB, etc.)
 * @param pinmask: Bitmask specifying which pins to configure (e.g., 0x0001 for pin 0, 0x0003 for pins 0 and 1, etc.)
 * @param mode: GPIO mode configuration (e.g., GPIO_MODE_OUTPUT_PP_10MHz, GPIO_MODE_INPUT_PU, etc.)
 * 
 * @note 
 * This function configures the mode of the specified GPIO pins based on the provided mode parameter. 
 * It updates the CRL and CRH registers accordingly and sets the ODR for pull-up/pull-down configurations if needed.
 */
void GPIO_Init(GPIO_TypeDef *GPIOx, uint16_t pinmask, GPIO_MODE_TypeDef mode)
{
    uint32_t val;  
    switch(mode)
    {
        case GPIO_MODE_INPUT_ANALOG      :    val = 0x0; break;
        case GPIO_MODE_INPUT_FLOATING    :    val = 0x4; break;
        case GPIO_MODE_INPUT_PU          :    val = 0x8; break;
        case GPIO_MODE_INPUT_PD          :    val = 0x8; break;

        case GPIO_MODE_OUTPUT_PP_10MHz   :    val = 0x1; break;
        case GPIO_MODE_OUTPUT_OD_10MHz   :    val = 0x5; break;
        case GPIO_MODE_OUTPUT_PP_2MHz    :    val = 0x2; break;
        case GPIO_MODE_OUTPUT_OD_2MHz    :    val = 0x6; break;
        case GPIO_MODE_OUTPUT_PP_50MHz   :    val = 0x3; break;
        case GPIO_MODE_OUTPUT_OD_50MHz   :    val = 0x7; break;

        case GPIO_MODE_AF_OUTPUT_PP_10MHz:    val = 0x9; break;
        case GPIO_MODE_AF_OUTPUT_OD_10MHz:    val = 0xD; break;
        case GPIO_MODE_AF_OUTPUT_PP_2MHz :    val = 0xA; break;
        case GPIO_MODE_AF_OUTPUT_OD_2MHz :    val = 0xE; break;
        case GPIO_MODE_AF_OUTPUT_PP_50MHz:    val = 0xB; break;
        case GPIO_MODE_AF_OUTPUT_OD_50MHz:    val = 0xF; break;
        default : break;
    }

    for(uint8_t pin = 0; pin < 16; pin++)
    {
        if(pinmask & (1U << pin))
        {
            uint32_t shift = ((pin % 8) * 4);
            volatile uint32_t *reg = (pin < 8) ? &GPIOx->CRL : &GPIOx->CRH;  
            
            *reg &= ~(0xFU << shift);
            *reg |= (val << shift);
            
            if(mode == GPIO_MODE_INPUT_PU) 
            {
                GPIOx->ODR |= (1U << pin);
            }
            else if(mode == GPIO_MODE_INPUT_PD)           
            {
                GPIOx->ODR &= ~(1U << pin);
            }
        }
    }
}


/*********************************************************************************************************************************************************************
 * @brief Write to a specific GPIO pin or multiple pins using a mask
 * 
 * @param GPIOx: Pointer to GPIO port (e.g., GPIOA, GPIOB, etc.)
 * @param pin: Pin number to write (0-15) for GPIO_WritePin, or bitmask for GPIO_WritePort
 * @param state: Desired state for GPIO_WritePin (HIGH or LOW)
 * 
 * @note  
 * This function sets or resets the specified GPIO pin(s) based on the provided state. 
 * For GPIO_WritePin, it uses the BSRR register to set or reset the specific pin.
 * For GPIO_WritePort, it calculates the set and reset bits based on the provided mask and value, allowing for simultaneous updates to multiple pins.
 */
void GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t pin, GPIO_PinState_t state)
{
    if(state == HIGH)
    {
        GPIOx->BSRR = (1U << pin);
    }else
    {
        GPIOx->BSRR = (1U << (pin + 16));
    }
}

void GPIO_WritePort(GPIO_TypeDef *GPIOx,uint32_t mask, uint32_t value)
{
    uint32_t setBit   = (value & mask);
    uint32_t resetBit = (~value & mask);
    
    GPIOx->BSRR = (setBit) | (resetBit << 16);
}



/*********************************************************************************************************************************************************************
 * @brief Read the state of a specific GPIO pin or multiple pins using a mask
 * 
 * @param GPIOx: Pointer to GPIO port (e.g., GPIOA, GPIOB, etc.)
 * @param pin: Pin number to read (0-15) for GPIO_ReadPin, or bitmask for GPIO_ReadPort
 * 
 * @note 
 * This function reads the state of the specified GPIO pin(s) from the IDR register.
 * For GPIO_ReadPin, it returns the state of the specific pin (0 or 1).
 * For GPIO_ReadPort, it returns the state of the pins specified by the mask, allowing for simultaneous reading of multiple pins.
 */
uint32_t GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t pin)
{
    return ((GPIOx->IDR >> pin) & 1U);
}

uint32_t GPIO_ReadPort(GPIO_TypeDef *GPIOx, uint32_t mask)
{
    return (GPIOx->IDR & mask);
}


/*********************************************************************************************************************************************************************
 * @brief Read the state of a specific GPIO pin or multiple pins using a mask
 * 
 * @param GPIOx: Pointer to GPIO port (e.g., GPIOA, GPIOB, etc.)
 * @param pin: Pin number to toggle (0-15)
 * 
 * @note 
 * This function toggles the state of the specified GPIO pin(s) by XORing the ODR register with the pin mask. 
 * It allows for simultaneous toggling of multiple pins if a mask is provided.
 */
void GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t pin)
{
    GPIOx->ODR ^= (1U << pin);
}

/*********************************************************************************************************************************************************************
 * @brief Config input pull-up/pull-down settings for specific GPIO pins
 * 
 * @param GPIOx: Pointer to GPIO port (e.g., GPIOA, GPIOB, etc.)
 * @param pinmask: Bitmask specifying which pins to configure
 * @param mode: Desired mode (pull-up, pull-down, etc.)
 * 
 * @note 
 * This function configures the pull-up/pull-down settings for the specified GPIO pins.
 */
void GPIO_ConfigPull(GPIO_TypeDef *GPIOx,uint32_t pinmask, GPIO_MODE_TypeDef mode)
{
    uint32_t val;
    for(uint16_t pin = 0; pin < 16; pin++)
    {
        if(pinmask & (1U << pin))
        {
            uint32_t shift = (pin % 8) *4;
            volatile uint32_t *reg = (pin < 8) ? &GPIOx->CRL : &GPIOx->CRH;
            
            switch(mode)
            {
                case GPIO_MODE_INPUT_ANALOG   : val =  0x0; break;
                case GPIO_MODE_INPUT_FLOATING : val =  0x4; break;
                case GPIO_MODE_INPUT_PU       : val =  0x8; break;
                case GPIO_MODE_INPUT_PD       : val =  0x8; break;
                default : break;
            }
 
            *reg &= ~(0xF << shift);
            *reg |= (val << shift);
            if(mode == GPIO_MODE_INPUT_PU)
            {
                GPIOx->ODR |= (1U << pin);
            }
            else if(mode == GPIO_MODE_INPUT_PD)
            {
                GPIOx->ODR &= ~(1U << pin);
            }
        }    
    }
}




/*********************************************************************************************************************************************************************
 * @brief Configure the output speed for specific GPIO pins
 * 
 * @param GPIOx: Pointer to GPIO port (e.g., GPIOA, GPIOB, etc.)
 * @param pinmask: Bitmask specifying which pins to configure
 * @param speed: Desired output speed
 * 
 * @note 
 * This function configures the output speed for the specified GPIO pins.
 */
void GPIO_ConfigSpeed(GPIO_TypeDef *GPIOx,uint32_t pinmask, GPIO_Cf_Speed speed)
{
    uint32_t val;
    for(uint16_t pin = 0; pin < 16; pin++)
    {
        uint32_t shift = (pin % 8) * 4;
        volatile uint32_t *reg = (pin < 8) ? &GPIOx->CRL : &GPIOx->CRH;
        
        if((*reg & 0x3) != 0)
        {    
            switch(speed){
                case GPIO_SPEED_PP_10MHz :  val = 0x1; break;   
                case GPIO_SPEED_OD_10MHz :  val = 0x5; break;   
                case GPIO_SPEED_PP_2MHz  :  val = 0x2; break;   
                case GPIO_SPEED_OD_2MHz  :  val = 0x6; break;   
                case GPIO_SPEED_PP_50MHz :  val = 0x3; break;   
                case GPIO_SPEED_OD_50MHz :  val = 0x7; break;   
    
                case GPIO_SPEED_AF_PP_10MHz    :  val = 0x9; break;
                case GPIO_SPEED_AF_OD_10MHz    :  val = 0xD; break;
                case GPIO_SPEED_AF_PP_2MHz     :  val = 0xA; break;
                case GPIO_SPEED_AF_OD_2MHz     :  val = 0xE; break;
                case GPIO_SPEED_AF_PP_50MHz    :  val = 0xB; break;
                case GPIO_SPEED_AF_OD_50MHz    :  val = 0xF; break;
                default : break;
            }
            
            if(pinmask & (1U << pin))
            {
                *reg &= ~(0xFU << shift);   
                *reg |= (val << shift);
            }  
        }
    }  
}



/*********************************************************************************************************************************************************************
 * @brief Read the state of a specific GPIO pin or multiple pins using a mask
 * 
 * @param GPIOx: Pointer to GPIO port (e.g., GPIOA, GPIOB, etc.)
 * @param pinmask: Bitmask specifying which pins to configure
 * @param mode: Desired alternate function mode
 * 
 * @note 
 * This function configures the alternate function settings for the specified GPIO pins.
 * It updates the CRL and CRH registers accordingly based on the provided mode parameter.
 */
void GPIO_ConfigAF(GPIO_TypeDef *GPIOx,uint32_t pinmask, GPIO_MODE_TypeDef mode)
{
    uint32_t value;
    
    switch(mode)
    {                                                           
        case GPIO_MODE_AF_OUTPUT_PP_10MHz : value = 0x9; break; 
        case GPIO_MODE_AF_OUTPUT_OD_10MHz : value = 0xD; break; 
        case GPIO_MODE_AF_OUTPUT_PP_2MHz  : value = 0xA; break; 
        case GPIO_MODE_AF_OUTPUT_OD_2MHz  : value = 0xE; break; 
        case GPIO_MODE_AF_OUTPUT_PP_50MHz : value = 0xB; break; 
        case GPIO_MODE_AF_OUTPUT_OD_50MHz : value = 0xF; break; 
        default: break;
    }
    for(uint32_t pin = 0; pin < 16; pin++){
        if(pinmask & (1U << pin))
        {    
            uint32_t shift = (pin % 8) * 4;
            volatile uint32_t *reg = (pin < 8) ? &GPIOx->CRL : &GPIOx->CRH;
            
            *reg &= ~(0xFU << shift);
            *reg |= (value << shift);
        }    
    }
}


/*********************************************************************************************************************************************************************
 * @brief Read the state of a specific GPIO pin or multiple pins using a mask
 * 
 * @param GPIOx: Pointer to GPIO port (e.g., GPIOA, GPIOB, etc.)
 * @param pinmask: Bitmask specifying which pins to configure.
 * 
 * @note 
 * This function locks the configuration of the specified GPIO pins, preventing any further changes until the next reset.
 * It follows the locking sequence as defined in the STM32F103 reference manual, which involves writing to the LCKR register in a specific order to lock the desired pins. 
 * Once locked, the configuration of these pins cannot be modified until a system reset occurs.
 */
void GPIO_LockPin(GPIO_TypeDef *GPIOx, uint16_t pinmask)
{
    // Set LCKx and LCKK
    GPIOx->LCKR = pinmask | (1U << 16);
    // Reset LCKK
    GPIOx->LCKR = pinmask;
    // Again set LCKK
    GPIOx->LCKR = pinmask | (1U << 16);

    volatile uint32_t tmp = GPIOx->LCKR;
    tmp = GPIOx->LCKR;
    (void)tmp;
}

/* End of refactored GPIO */