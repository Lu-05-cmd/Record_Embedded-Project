/**
 * @file    bit_ops.h
 * @brief   configulation single bit-field macro for stm32f10x
 * @author  lu-05-cmd
 * @date    December 25,2025
 * 
 * @note    Macro only maniplument singple bit
 */
#ifndef STM32F10x_BIT_OPS_H
#define STM32F10x_BIT_OPS_H

/****************************************************************************
 *                  BASIC GROUP
 ****************************************************************************/
#define BIT(pos)                        (1U << (pos))
#define SET_BIT(reg, pos)               ((reg) |= BIT(pos))
#define CLEAR_BIT(reg, pos)             ((reg) &= ~(BIT(pos)))
#define READ_BIT(reg, pos)              (1U & ((reg) >> (pos)))
#define WRITE_BIT(reg, pos, val)    \
    ((reg) = ((reg) & ~(BIT(pos))) | ((val & 1) << pos))

/****************************************************************************
 *                  BASIC MASK FOR MACRO GROUP
 ****************************************************************************/
#define SET_MASK(reg, mask)             ((reg) |= (mask))
#define CLEAR_MASK(reg, mask)           ((reg) &= ~(mask))
#define READ_MASK(reg, mask)            ((reg) & (mask))

/****************************************************************************
 *                  BASIC MASK FOR MACRO GROUP
 ****************************************************************************/
#define FIELD_MASK(width)                       ((1U << width) -1)           
#define FIELD_VALUE(value, width, shift)        ((value & FIELD_MASK(width)) << shift)
#define CLEAR_FIELD(reg, field_mask)((reg) &= ~(field_mask))
#define WRITE_FIELD(reg, field_mask, value_shifted)                             \
    do{                                                                         \
        ((reg) = ((reg) & ~(field_mask)) | ((value_shifted) & (field_mask)));       \
    }while(0)
#define READ_FIELD(reg, field_mask, shift)   (((reg) & (field_mask)) >> shift)                     
/** other sections for define WIRE_FIELD and REAGFIELD
 *      static inline uint32_t WRITE_FILED(uint32_t *reg, uint32_t field_mask, uint32_t value_shifted){
 *          return (*reg = (*reg & ~(field_mask) | (value_shifted & field_mask)));
 *      }
 * 
 *      static inline uint32_t READ_FIELD(uint32_t reg, uint32_t field_mask, uint32_t shift){
 *          return ((reg & field_mask) >> shift);
 *      }
 */

#endif    /* STM32F10x_BIT_OPS_H  */