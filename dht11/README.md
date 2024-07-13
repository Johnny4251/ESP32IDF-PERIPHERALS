
# DHT11 Sensor Driver for ESP32-IDF

This repository contains a driver for the DHT11 temperature and humidity sensor, designed to be used with the ESP32 microcontroller and the ESP-IDF framework. The driver is written in C and includes an example usage in the `main.c` file.

## Features

- Read temperature and humidity from the DHT11 sensor
- Compatible with ESP32 and ESP-IDF

## Getting Started

### Prerequisites

- ESP32 development board
- ESP-IDF framework installed
- DHT11 sensor

### Adding the Driver to Your Project

1. Clone this repository or download the `dht11.c` and `dht11.h` files.
2. Copy the `dht11.c` and `dht11.h` files into your project's `components` directory.

Your project structure could look like this:
```
your_project/
├── components/
│   └── dht11/
│       ├── dht11.c
│       └── dht11.h
├── main/
│   └── main.c
└── CMakeLists.txt
```

### Modifying CMakeLists.txt

Ensure your `CMakeLists.txt` includes the new component:
```cmake
set(EXTRA_COMPONENT_DIRS components/dht11)
```

### Example Usage

Here's an example `main.c` file to demonstrate how to use the DHT11 driver:

```c
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "dht11.h"

void app_main(void)
{
    printf("DHT11 sensor example
");

    // Initialize the DHT11 sensor
    dht11_init(GPIO_NUM_4); // Use GPIO 4 for the DHT11 data pin

    while (1)
    {
        int16_t temperature = dht11_read().temperature;
        int16_t humidity = dht11_read().humidity;

        if (temperature != DHT11_ERROR && humidity != DHT11_ERROR)
        {
            printf("Temperature: %d°C
", temperature);
            printf("Humidity: %d%%
", humidity);
        }
        else
        {
            printf("Failed to read from DHT11 sensor
");
        }

        vTaskDelay(2000 / portTICK_PERIOD_MS); // Delay for 2 seconds
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
