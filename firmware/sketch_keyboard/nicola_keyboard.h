#ifndef __NICOLA_KEYBOARD_H__
#define __NICOLA_KEYBOARD_H__

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned short matrix_row_t;

const int MATRIX_ROWS = 5;
const int MATRIX_COLS = 16;

const int ROWS[] = { 7, 8, 6, 5, 4 };
const int COLS[] = { 3, 2, 1, 0 };
const int PURGE = 9;
const int SENSE = 10;

// asym_eager_defer_pk.ino
void debounce_init();
void debounce(matrix_row_t raw[], matrix_row_t cooked[], bool changed);

// adc.ino
void calibrate_adc(void);
void acquire_adc(int adc_measured_val[][5]);

// sense.ino
uint8_t matrix_scan(void);
matrix_row_t matrix_get_row(uint8_t row);

// nicola.ino
void nicola_on(void);
void nicola_off(void);
bool nicola_state(void);
bool nicola_layer_state(void);
void nicola_clear(void);
void nicola_mode(uint16_t keycode, bool pressed);
bool process_nicola(uint16_t keycode, bool pressed);

#endif