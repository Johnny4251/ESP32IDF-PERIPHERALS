#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "freertos/task.h"
#include "rom/ets_sys.h"

#include "hcsr04.h"

#define TIMEOUT_MS 40
#define MAX_MEASUREMENT_MS 60

static int echo_gpio;
static int trig_gpio;

static HCSR04_Data hcsr04_data;

float _time_to_inches(int time) {
    return time / 148.0;
}

float _time_to_cm(int time) {
    return time / 58.0;
}

void init_hcsr04(const uint8_t echo_pin, const uint8_t trig_pin) {

    echo_gpio = echo_pin;
    trig_gpio = trig_pin;
    
    gpio_reset_pin(echo_gpio);
    gpio_reset_pin(trig_gpio);

    gpio_set_direction(echo_gpio, GPIO_MODE_INPUT);
    gpio_set_direction(trig_gpio, GPIO_MODE_OUTPUT);

    // just because I can :)
    gpio_set_level(trig_gpio, 0);
}

void _send_pulse() {
    gpio_set_level(trig_gpio, 1);
    ets_delay_us(10);
    gpio_set_level(trig_gpio, 0);
}

HCSR04_Data read_distance() {

    int start_time, end_time;
    start_time = 0;
    end_time = 0;

    int temp_time = esp_timer_get_time();

    // datasheet measurement recommendation time
    if(esp_timer_get_time() - hcsr04_data.last_read <= (MAX_MEASUREMENT_MS * 1000)) {
        hcsr04_data.status = HCSR04_ERROR;
        return hcsr04_data;
    }

    _send_pulse();
    
    temp_time = esp_timer_get_time();
    // wait for rising edge -> grab start time
    while (gpio_get_level(echo_gpio) == 0) {
        // timeout
        if(esp_timer_get_time() - temp_time > TIMEOUT_MS * 1000) {
            hcsr04_data.status = HCSR04_TIMEOUT;
            return hcsr04_data;
        }
    }
    start_time = esp_timer_get_time();

    // wait for falling edge -> grab end time
    while (gpio_get_level(echo_gpio) == 1) {
        // timeout
        if(esp_timer_get_time() - temp_time > TIMEOUT_MS * 1000) {
            hcsr04_data.status = HCSR04_TIMEOUT;
            return hcsr04_data;
        }
    };
    end_time = esp_timer_get_time();

    int diff_time = end_time - start_time;
    float dist_inches = _time_to_inches(diff_time);
    float dist_cm = _time_to_cm(diff_time);

    hcsr04_data.status = HCSR04_OK;

    // fill hcsr04 struct
    hcsr04_data.last_read = end_time;
    hcsr04_data.inches = dist_inches;
    hcsr04_data.cm = dist_cm;

    return hcsr04_data;
}
