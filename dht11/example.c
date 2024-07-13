#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "esp_log.h"
#include "freertos/task.h"
#include "rom/ets_sys.h"

#include "dht11.h"

void app_main(void) {
    DHT11_init(GPIO_NUM_19);

    while(1) {
        DHT11_Data data = dht11_read();
        if (data.status == DHT11_OK) {
            ESP_LOGI("DHT11", "Temperature: %.2f *F, Relative Humidity: %d%%", data.fahrenheit, data.humidity);
            //ESP_LOGI("DHT11", "Temperature: %.2f *C, Relative Humidity: %d%%", data.celsius, data.humidity);
        } else {
            ESP_LOGE("DHT11", "Failed to read data from DHT11 sensor");
        }
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

