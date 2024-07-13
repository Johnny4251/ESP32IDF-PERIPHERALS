
# ESP32IDF-PERIPHERALS

This repository contains drivers for various peripherals designed to be used with the ESP32 microcontroller and the ESP-IDF framework. Each driver is written in C and includes example usage.

## Available Drivers

- **DHT11**: Temperature and humidity sensor
- **HC-SR04**: Ultrasonic distance sensor

## Getting Started

### Prerequisites

- ESP32 development board
- ESP-IDF framework installed

### Adding Drivers to Your Project

1. Clone this repository or download the desired driver files.
2. Copy the driver files into your project's `components` directory.

Your project structure could look like this:
```
your_project/
├── components/
│   └── <driver_name>/
│       ├── <driver_name>.c
│       └── <driver_name>.h
├── main/
│   └── main.c
└── CMakeLists.txt
```

### Modifying CMakeLists.txt

Ensure your `CMakeLists.txt` includes the new component:
```cmake
set(EXTRA_COMPONENT_DIRS components/<driver_name>)
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

## Acknowledgments

Inspired by various open-source projects and the ESP-IDF documentation.

