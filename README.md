
# ========================

# PROJEC TREE DIAGRAM   =

# ========================




bluepill-baremetal-framework/
│
├── application/
|   ├── config/
│   │   └── macros/
│   │       └── bit_ops.h
|   ├── core/
│   │   ├── linker/
│   │   │   └── stm32f103c8.ld
│   │   ├── startup/
│   │   │   └── startup_stm32f103.s
│   │   ├── system/
│   │   │   ├── system.c
│   │   │   └── system.h
│   │   └── main.c
│   │
|   ├── driver/
|   |   ├──stm32f103        
│   │   │   ├── Inc/
│   │   │   │    ├── gpio.h
│   │   │   │    ├── rcc.h
│   │   │   │    ├── timer.h
│   │   │   │    ├── flash.h
│   │   │   │    ├── afio.h
│   │   │   │    ├── nvic.h
│   │   │   │    ├── usart.h
│   │   │   │    ├── adc.h
│   │   │   │    ├── spi.h
│   │   │   │    ├── dma.h
│   │   │   │    ├── adc.h
│   │   │   │    └── exti.h
│   │   │   ├── Src/
│   │   │   │    ├── gpio.h
│   │   │   │    ├── rcc.h
│   │   │   │    ├── timer.h
│   │   │   │    ├── flash.h
│   │   │   │    ├── afio.h
│   │   │   │    ├── nvic.h
│   │   │   │    ├── usart.h
│   │   │   │    ├── adc.h
│   │   │   │    ├── spi.h
│   │   │   │    ├── dma.h
│   │   │   │    ├── adc.h
│   │   │   │    └── exti.h
|   |   |   └── stm32f103.h      
│   ├── middleware/
│   │   ├── scheduler/
│   │   │   ├── scheduler.c
│   │   │   └── scheduler.h
│   │   ├── protocol/
│   │   │   └── parser.c/h
│   │   ├── cli/
│   │   │   └── cli.c/h
│   │   └── log/
│   │       └── log.c/h
│   │
│   ├── services/
│   │   ├── led_service/
│   │   ├── sensor_service/
│   │   └── system_monitor/
│   │
│   ├── utils/
│   │   ├── ring_buffer/
│   │   ├── delay/
│   │   ├── memory/
│   │   └── debug/
│   │
│   ├── config/
│   │   ├── board.h
│   │   ├── pinout.h
│   │   ├── clocks.h
│   │   ├── macros/        
│   │   │   ├── bit_ops.h
│   │   │   ├── utils.h
│   │   │   └── system_macros.h
│   │   └── all_driver_config.h  
│   │
│   └── README.md
│
├── bootloader/
│   ├── inc/
│   ├── src/
│   ├── linker/
│   └── README.md
│
├── tools/
│   ├── flash/
│   ├── build/     
│   ├── python_tools/
│   └── README.md
|
├── tests/
│   ├── unit/
│   ├── integration/
│   └── hw_test/
│
├── examples/   
│   ├── blink/
│   ├── uart_echo/
│   ├── scheduler_demo/
│   └── sensor_demo/
│
├── docs/        
│   ├── architecture/
│   ├── drivers/
│   ├── scheduler/
│   ├── bootloader/
│   └── protocol/
│
├── .gitignore
├── makefile
└── README.md













