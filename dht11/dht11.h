#ifndef DHT11_H_
#define DHT11_H_

/**
 *  DHT11_OK = 0
    DHT11_ERROR = -1
    DHT11_TIMEOUT_ERROR = -2
 */
enum DHT11_Status {
    DHT11_OK = 0,
    DHT11_ERROR = -1,
    DHT11_TIMEOUT_ERROR = -2,
};
/**
 * Holds read data from struct
 */
typedef struct {
    enum DHT11_Status status;
    int64_t last_read;
    float fahrenheit;
    float celsius;
    int humidity;
} DHT11_Data;

/**
 * Initializes GPIO pin for DHT11 sensor
 */
void DHT11_init(const uint8_t gpio_pin);

/**
 * Reads data from signal pin into DHT11_Data struct.
 * Check DHT11_Status for error handling. 
 * 
 * NOTE: This is a busy-wait function. 
 */
DHT11_Data dht11_read();

#endif
