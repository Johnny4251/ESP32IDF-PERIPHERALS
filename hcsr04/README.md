
# HC-SR04 Sensor Driver for ESP32-IDF

This repository contains a driver for the HC-SR04 ultrasonic distance sensor, designed to be used with the ESP32 microcontroller and the ESP-IDF framework. The driver is written in C and includes an example usage in the `main.c` file.

## Features

- Measure distance using ultrasonic waves
- Compatible with ESP32 and ESP-IDF

## Getting Started

### Prerequisites

- ESP32 development board
- ESP-IDF framework installed
- HC-SR04 sensor

### Adding the Driver to Your Project

1. Clone this repository or download the `hcsr04.c` and `hcsr04.h` files.
2. Copy the `hcsr04.c` and `hcsr04.h` files into your project's `components` directory.

Your project structure could look like this:
```
your_project/
├── components/
│   └── hcsr04/
│       ├── hcsr04.c
│       └── hcsr04.h
├── main/
│   └── main.c
└── CMakeLists.txt
```

### Modifying CMakeLists.txt

Ensure your `CMakeLists.txt` includes the new component:
```cmake
set(EXTRA_COMPONENT_DIRS components/hcsr04)
```

### Example Usage

Here's an example to demonstrate how to use the HC-SR04 driver:

```c
#include "hcsr04.h"

#define TRIG_PIN 22
#define ECHO_PIN 23

void app_main() {
    
    init_hcsr04(ECHO_PIN, TRIG_PIN);
    
    while (1) {
        HCSR04_Data data = read_distance();
        if(data.status == HCSR04_OK) {
            printf("INCHES:\t%f\n", data.inches);
            printf("CM:\t%f\n", data.cm);
        } else {
            // -1 = ERROR
            // -2 = TIMEOUT
            ESP_LOGE("HCSR04", "Error! Status Code: %d", data.status);
        }
        // if read is to fast -> -1 status code is returned
        vTaskDelay(70 / portTICK_PERIOD_MS);
    }
}
```

### Building and Flashing

1. Build your project:
    ```sh
    idf.py build
    ```

2. Flash the project to your ESP32:
    ```sh
    idf.py flash
    ```

3. Monitor the output:
    ```sh
    idf.py monitor
    ```

## License

This project is licensed under the MIT License.
