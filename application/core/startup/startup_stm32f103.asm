/* ****************************************************************************
 * File        : startup.asm
 * MCU         : STM32F103xx (Cortex-M3, Medium Density)
 * Date        : December, 21 2025
 *
 * Note :
 *      - Full vector table
 *      - Copy .data from Flash to RAM
 *      - Clear .bss
 *      - Minimal SystemInit (HSI default)
 *      - Weak alias for all handlers
 * ****************************************************************************/

.syntax unified
.cpu cortex-m3
.thumb

/* ========================= Stack ========================= */
.global _estack
_estack = 0x20005000  

/* ========================= Vector Table ========================= */
.section .isr_vector, "a", %progbits
.type g_pfnVectors, %object
.global g_pfnVectors

g_pfnVectors:
    /*Core exceptions */
    .word _estack
    .word Reset_Handler
    .word NMI_Handler
    .word HardFault_Handler
    .word MemManage_Handler
    .word BusFault_Handler
    .word UsageFault_Handler
    .word 0
    .word 0
    .word 0
    .word 0
    .word SVC_Handler
    .word DebugMon_Handler
    .word 0
    .word PendSV_Handler
    .word SysTick_Handler

    /*IRQs (medium density) */
    .word WWDG_IRQHandler           
    .word PVD_IRQHandler            
    .word TAMPER_IRQHandler         
    .word RTC_IRQHandler            
    .word FLASH_IRQHandler          
    .word RCC_IRQHandler            
    .word EXTI0_IRQHandler          
    .word EXTI1_IRQHandler          
    .word EXTI2_IRQHandler          
    .word EXTI3_IRQHandler          
    .word EXTI4_IRQHandler          
    .word DMA1_Channel1_IRQHandler  
    .word DMA1_Channel2_IRQHandler  
    .word DMA1_Channel3_IRQHandler  
    .word DMA1_Channel4_IRQHandler  
    .word DMA1_Channel5_IRQHandler  
    .word DMA1_Channel6_IRQHandler  
    .word DMA1_Channel7_IRQHandler  
    .word ADC1_2_IRQHandler         
    .word USB_HP_CAN_TX_IRQHandler  
    .word USB_LP_CAN_RX0_IRQHandler 
    .word CAN_RX1_IRQHandler        
    .word CAN_SCE_IRQHandler        
    .word EXTI9_5_IRQHandler        
    .word TIM1_BRK_IRQHandler       
    .word TIM1_UP_IRQHandler        
    .word TIM1_TRG_COM_IRQHandler   
    .word TIM1_CC_IRQHandler        
    .word TIM2_IRQHandler           
    .word TIM3_IRQHandler           
    .word TIM4_IRQHandler           
    .word I2C1_EV_IRQHandler        
    .word I2C1_ER_IRQHandler        
    .word I2C2_EV_IRQHandler        
    .word I2C2_ER_IRQHandler        
    .word SPI1_IRQHandler           
    .word SPI2_IRQHandler           
    .word USART1_IRQHandler         
    .word USART2_IRQHandler         
    .word USART3_IRQHandler         
    .word EXTI15_10_IRQHandler      
    .word RTCAlarm_IRQHandler       
    .word USBWakeUp_IRQHandler      

/* ========================= Reset Handler ========================= */
.section .text
.thumb_func
.global Reset_Handler

Reset_Handler:
    /* Copy .data */
    LDR R0, =_sdata
    LDR R1, =_edata
    LDR R2, =_sidata
1:
    CMP R0, R1
    BCC 2f
    B   3f
2:
    LDR R3, [R2], #4
    STR R3, [R0], #4
    B   1b

3:
    /* Clear .bss */
    LDR R0, =_sbss
    LDR R1, =_ebss
    MOVS R2, #0
4:
    CMP R0, R1
    BCC 5f
    B   6f
5:
    STR R2, [R0], #4
    B   4b

6:
    /* System clock init */
    BL  SystemInit

    /* Call main */
    BL  main

7:
    B   7b

/* ========================= SystemInit ========================= */
.thumb_func
.global SystemInit
SystemInit:
    /* RCC base = 0x40021000 */
    LDR R0, =0x40021000
    /* Enable HSI */
    LDR R1, [R0]
    ORR R1, R1, #0x1
    STR R1, [R0]
/* Wait HSI ready */
1:
    LDR R1, [R0]
    TST R1, #0x2
    BEQ 1b
    /* SYSCLK = HSI (CFGR = 0) */
    MOVS R1, #0
    STR R1, [R0, #0x04]

    BX  LR

/* ========================= Default Handler ========================= */
.section .text
.thumb_func
Default_Handler:
    B .

/* ========================= Weak Aliases ========================= */
.weak NMI_Handler
.set  NMI_Handler, Default_Handler
.weak HardFault_Handler
.set  HardFault_Handler, Default_Handler
.weak MemManage_Handler
.set  MemManage_Handler, Default_Handler
.weak BusFault_Handler
.set  BusFault_Handler, Default_Handler
.weak UsageFault_Handler
.set  UsageFault_Handler, Default_Handler
.weak SVC_Handler
.set  SVC_Handler, Default_Handler
.weak DebugMon_Handler
.set  DebugMon_Handler, Default_Handler
.weak PendSV_Handler
.set  PendSV_Handler, Default_Handler
.weak SysTick_Handler
.set  SysTick_Handler, Default_Handler

/* IRQs */
.weak WWDG_IRQHandler        
.set  WWDG_IRQHandler, Default_Handler
.weak PVD_IRQHandler
.set  PVD_IRQHandler, Default_Handler
.weak TAMPER_IRQHandler
.set  TAMPER_IRQHandler, Default_Handler
.weak RTC_IRQHandler
.set  RTC_IRQHandler, Default_Handler
.weak FLASH_IRQHandler
.set  FLASH_IRQHandler, Default_Handler
.weak RCC_IRQHandler
.set  RCC_IRQHandler, Default_Handler
.weak EXTI0_IRQHandler
.set  EXTI0_IRQHandler, Default_Handler
.weak EXTI1_IRQHandler
.set  EXTI1_IRQHandler, Default_Handler
.weak EXTI2_IRQHandler
.set  EXTI2_IRQHandler, Default_Handler
.weak EXTI3_IRQHandler
.set  EXTI3_IRQHandler, Default_Handler
.weak EXTI4_IRQHandler
.set  EXTI4_IRQHandler, Default_Handler
.weak DMA1_Channel1_IRQHandler
.set  DMA1_Channel1_IRQHandler, Default_Handler
.weak DMA1_Channel2_IRQHandler
.set  DMA1_Channel2_IRQHandler, Default_Handler
.weak DMA1_Channel3_IRQHandler
.set  DMA1_Channel3_IRQHandler, Default_Handler
.weak DMA1_Channel4_IRQHandler
.set  DMA1_Channel4_IRQHandler, Default_Handler
.weak DMA1_Channel5_IRQHandler
.set  DMA1_Channel5_IRQHandler, Default_Handler
.weak DMA1_Channel6_IRQHandler
.set  DMA1_Channel6_IRQHandler, Default_Handler
.weak DMA1_Channel7_IRQHandler
.set  DMA1_Channel7_IRQHandler, Default_Handler
.weak ADC1_2_IRQHandler
.set  ADC1_2_IRQHandler, Default_Handler
.weak USB_HP_CAN_TX_IRQHandler
.set  USB_HP_CAN_TX_IRQHandler, Default_Handler
.weak USB_LP_CAN_RX0_IRQHandler
.set  USB_LP_CAN_RX0_IRQHandler, Default_Handler
.weak CAN_RX1_IRQHandler
.set  CAN_RX1_IRQHandler, Default_Handler
.weak CAN_SCE_IRQHandler
.set  CAN_SCE_IRQHandler, Default_Handler
.weak EXTI9_5_IRQHandler
.set  EXTI9_5_IRQHandler, Default_Handler
.weak TIM1_BRK_IRQHandler
.set  TIM1_BRK_IRQHandler, Default_Handler
.weak TIM1_UP_IRQHandler
.set  TIM1_UP_IRQHandler, Default_Handler
.weak TIM1_TRG_COM_IRQHandler
.set  TIM1_TRG_COM_IRQHandler, Default_Handler
.weak TIM1_CC_IRQHandler
.set  TIM1_CC_IRQHandler, Default_Handler
.weak TIM2_IRQHandler
.set  TIM2_IRQHandler, Default_Handler
.weak TIM3_IRQHandler
.set  TIM3_IRQHandler, Default_Handler
.weak TIM4_IRQHandler
.set  TIM4_IRQHandler, Default_Handler
.weak I2C1_EV_IRQHandler
.set  I2C1_EV_IRQHandler, Default_Handler
.weak I2C1_ER_IRQHandler
.set  I2C1_ER_IRQHandler, Default_Handler
.weak I2C2_EV_IRQHandler
.set  I2C2_EV_IRQHandler, Default_Handler
.weak I2C2_ER_IRQHandler
.set  I2C2_ER_IRQHandler, Default_Handler
.weak SPI1_IRQHandler
.set  SPI1_IRQHandler, Default_Handler
.weak SPI2_IRQHandler
.set  SPI2_IRQHandler, Default_Handler
.weak USART1_IRQHandler
.set  USART1_IRQHandler, Default_Handler
.weak USART2_IRQHandler
.set  USART2_IRQHandler, Default_Handler
.weak USART3_IRQHandler
.set  USART3_IRQHandler, Default_Handler
.weak EXTI15_10_IRQHandler
.set  EXTI15_10_IRQHandler, Default_Handler
.weak RTCAlarm_IRQHandler
.set  RTCAlarm_IRQHandler, Default_Handler
.weak USBWakeUp_IRQHandler
.set  USBWakeUp_IRQHandler, Default_Handler

.end
