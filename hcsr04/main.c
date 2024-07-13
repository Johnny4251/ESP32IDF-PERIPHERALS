#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "esp_log.h"
#include "esp_rom_gpio.h"

#include "hcsr04.h"

#define TRIG_PIN 22
#define ECHO_PIN 23

const char* TAG = "hcsr04-example";
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
            ESP_LOGE(TAG, "Error! Status Code: %d", data.status);
        }
        // if read is to fast -> -1 status code is returned
        vTaskDelay(70 / portTICK_PERIOD_MS);
    }
}