#ifndef HCSR04_H_
#define HCSR04_H_

enum HCSR04_Status {
    HCSR04_OK = 0,
    HCSR04_ERROR = -1,
    HCSR04_TIMEOUT = -2,
};

/**
 * Struct to keep track of sensor data. 
 * 
 * status:    Status code
 * last_read: last read time
 * cm: last   distance reading converted to cm
 * inches:    last distance reading converted to inches
 */
typedef struct HCSR04_Data {
    enum HCSR04_Status status;
    int64_t last_read;
    float cm;
    float inches;
} HCSR04_Data;

/**
 * Initalize GPIO pins. Only need to call this once.
 */
void init_hcsr04(const uint8_t echo_pin, const uint8_t trig_pin);

/**
 * Read distance into HCSR04_Data struct. Returns
 * and sets struct data. Will perform conversion to cm/inches.
 * 
 * NOTE: This is a busy wait function!
 */
HCSR04_Data read_distance();

#endif