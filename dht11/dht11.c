#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "esp_log.h"
#include "freertos/task.h"
#include "rom/ets_sys.h"
#include "esp_rom_gpio.h"

#include "dht11.h"

#define MAX_MEASUREMENT_MS 150

const char* TAG = "DHT11_DRIVER";
static uint8_t signal_pin;
static DHT11_Data dht11_data;

void DHT11_init(const uint8_t gpio_pin) {
    dht11_data.last_read = 0;

    // device has one second initial unstable status
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    signal_pin = gpio_pin;
    gpio_set_direction(signal_pin, GPIO_MODE_OUTPUT);
}

int _wait_for_response(const uint8_t time_us, const int level) {
    uint8_t us_count = 0;
    while(gpio_get_level(signal_pin) == level) {
        if(us_count > time_us) 
            return -1; 
        us_count++;
        ets_delay_us(1);
    }
    return us_count;
}


float _celsius_to_farenheit(const float celsius) {
    return (float) ((celsius) * 9.0/5.0) + 32.0;
}

int _check_data(const uint8_t data[5]) {
    // checksum!
    if(data[0] + data[1] + data[2] + data[3] == data[4])
        return 0;
    return 1;
}


void _send_start() {
    // Set GPIO to output and start signal
    gpio_set_direction(signal_pin, GPIO_MODE_OUTPUT);
    gpio_set_level(signal_pin, 0);
    ets_delay_us(18 * 1000);
    gpio_set_level(signal_pin, 1);
    ets_delay_us(35);
    gpio_set_direction(signal_pin, GPIO_MODE_INPUT);
}

DHT11_Data dht11_read() {
    
    if(esp_timer_get_time() - dht11_data.last_read <= (MAX_MEASUREMENT_MS * 1000)) 
        return dht11_data;

    uint8_t data[5] = {0,0,0,0,0};

    _send_start();

    // wait for sensor to be low->high for 80us
    if(_wait_for_response(80, 0) == -1) {
        ESP_LOGE(TAG, "Low reading timeout");
        dht11_data.status = DHT11_TIMEOUT_ERROR;
        return dht11_data;
    }
    if(_wait_for_response(80, 1) == -1) {
        ESP_LOGE(TAG, "High reading timeout");
        dht11_data.status = DHT11_TIMEOUT_ERROR;
        return dht11_data;
    }

    for(int i=0; i<40; i++) {
        if(_wait_for_response(50,0) == -1) {
            ESP_LOGE(TAG, "Low bit reading timeout");
            dht11_data.status = DHT11_TIMEOUT_ERROR;
            return dht11_data;
        }
        int bit_response = _wait_for_response(70,1);
        if (bit_response == -1) {
            ESP_LOGE(TAG, "Bit read error");
            dht11_data.status = DHT11_ERROR;
            return dht11_data;
        }
        /* To explain this witchcraft.. */
        /* fyi - dht11 reads MSB first */
        /* In data[i / 8], turn on the bit (7 - (i % 8)) by OR-ing with 1 << (7 - (i % 8)). */
        if (bit_response > 28)
            data[i / 8] |= (1 << (7 - (i % 8)));
    }

    if(_check_data(data) == 1) {
        ESP_LOGE(TAG, "Checksum failed!");
        dht11_data.status = DHT11_ERROR;
        return dht11_data;
    }

    // Fill struct
    dht11_data.last_read = esp_timer_get_time();
    dht11_data.humidity = data[0] + (data[1] / 10.0);
    dht11_data.celsius = data[2] + (data[3] / 10.0);
    dht11_data.fahrenheit = _celsius_to_farenheit(dht11_data.celsius);
    dht11_data.status = DHT11_OK;

    return dht11_data;
}
