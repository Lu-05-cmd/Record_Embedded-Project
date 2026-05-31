esp32_gateway/
│
├── main.cpp                  # entry point (ONLY file bắt buộc)
│
├── uart/
│   ├── uart_rx.h
│   └── uart_rx.cpp          # đọc STM32 UART stream
│
├── parser/
│   ├── parser.h
│   └── parser.cpp           # parse JSON / CSV
│
├── mqtt/
│   ├── mqtt_client.h
│   └── mqtt_client.cpp      # publish data to HiveMQ
│
├── config.h                 # WiFi, MQTT topic, device id
│
└── system/
    ├── system_monitor.h
    └── system_monitor.cpp   # optional debug status