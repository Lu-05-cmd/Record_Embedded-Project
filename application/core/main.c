#include <stdint.h>

/* BASE ADDRESS*/
#define RCC_BASE    0x40021000UL
#define GPIOC_BASE  0x40011000UL

#define RCC_APB2ENR   (*(volatile uint32_t *)(RCC_BASE + 0x18))
#define GPIOC_CRH   (*(volatile uint32_t *)(GPIOC_BASE + 0x04))
#define GPIOC_ODR   (*(volatile uint32_t *)(GPIOC_BASE + 0x0C))

/*delay*/
void delay(uint32_t t){
    for(uint32_t i = 0; i < (t * 80000); i++);
}

int main(void){
    /*Enable GPOIC Clock*/
    RCC_APB2ENR |= (1U << 4);

    // config PC13: Output Push-Pull, 2 MHz
    GPIOC_CRH &= ~(0xFU << 20);  // clear bits
    GPIOC_CRH |=  (0x2U << 20);  // MODE=10, CNF=00

    while(1){
        GPIOC_ODR ^= (1 << 13);  //TOGGLE LED 
        delay(1000);
    }
}
