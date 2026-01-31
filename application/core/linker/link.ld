/****************************************************************************************
 * @file    link.ld
 * @brief   Created layout and sections for stm32f103xx.
 * @author  lu-05-cmd
 * @date    December, 20 2025
 *
 *@note     Define Flash and RAM, vector table location and
 *          placement of .text, .data, .bss sections.
 ****************************************************************************************/


ENTRY(Reset_Handler)

/* Memory regions-------------------------------------------------------------------------*/
MEMORY
{
    FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 64K 
    RAM   (rwx): ORIGIN = 0x20000000, LENGTH = 20K
}
_estack = ORIGIN(RAM) + LENGTH(RAM);

/* Sections--------------------------------------------------------------------------------*/
SECTIONS
{
    .isr_vector :
    {
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } > FLASH
    
    .text :
    {
        . = ALIGN(4);
        *(.text*)
        *(.rodata*)
        KEEP(*(.init))
        KEEP(*(.fini))
        . = ALIGN(4);
    } > FLASH

    .data :
    {
        . = ALIGN(4);
        _sdata = .;
        *(.data*)
        _edata = .;
        . = ALIGN(4);
    } > RAM AT> FLASH
    _sidata = LOADADDR(.data);

    .bss (NOLOAD) :
    {
        . = ALIGN(4);
        _sbss = .;
        *(.bss*)
        *(COMMON)
        _ebss = .;
        . = ALIGN(4);
    } > RAM
}
