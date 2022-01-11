#include "nicola_keyboard.h"
#include "keymap.h"

void setup() {
  Serial.begin(9600);

  // multiplexer
  pinMode(COLS[0], OUTPUT);
  pinMode(COLS[1], OUTPUT);
  pinMode(COLS[2], OUTPUT);
  pinMode(COLS[3], OUTPUT);

  // purge pin
  pinMode(PURGE, INPUT);

  // sense pin
  pinMode(SENSE, INPUT);

  // trigger
  pinMode(ROWS[0], OUTPUT);
  pinMode(ROWS[1], OUTPUT);
  pinMode(ROWS[2], OUTPUT);
  pinMode(ROWS[3], OUTPUT);
  pinMode(ROWS[4], OUTPUT);

  debounce_init();
  calibrate_adc();
}

bool fn_pressed = false;

void loop() {
  static matrix_row_t matrix_prev[MATRIX_ROWS];
  matrix_row_t        matrix_row    = 0;
  matrix_row_t        matrix_change = 0;

  uint8_t matrix_changed = matrix_scan();

  for (uint8_t r = 0; r < MATRIX_ROWS; r++) {
    matrix_row    = matrix_get_row(r);
    matrix_change = matrix_row ^ matrix_prev[r];
    if (matrix_change) {
      matrix_row_t col_mask = 1;
      for (uint8_t c = 0; c < MATRIX_COLS; c++, col_mask <<= 1) {
        if (matrix_change & col_mask) {
          uint16_t keycode = keymap_base[r][c];
          bool pressed = matrix_row & col_mask;
          if(keycode == 0) {
            fn_pressed = pressed;
          }
          if(fn_pressed) {
            if(keymap_fn[r][c] > 1) {
              keycode = keymap_fn[r][c];
            }
          }
          if(keycode > 1 && keycode <= 0xff) {
            if(pressed) {
              // key pressed
              Serial.print("P");
              Serial.print(r);
              Serial.print(" ");
              Serial.print(c);
              Serial.print(" ");
              Serial.println(keycode);
              Keyboard.press(keycode);
            } else {
              // key released
              Serial.print("R");
              Serial.print(r);
              Serial.print(" ");
              Serial.print(c);
              Serial.print(" ");
              Serial.println(keycode);
              Keyboard.release(keycode);
            }
          }

          // record a processed key
          matrix_prev[r] ^= col_mask;
        }
      }
    }
  }
}
