#include "nicola_keyboard.h"

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
          Serial.print(r);
          Serial.print(' ');
          Serial.print(c);
          Serial.print(' ');
          Serial.print(matrix_row & col_mask ? '1' : '0');
          Serial.print(' ');
          Serial.print(matrix_prev[r] & col_mask ? '1' : '0');
          Serial.print(' ');
          Serial.print(matrix_change & col_mask ? '1' : '0');
          Serial.println(' ');

          // record a processed key
          matrix_prev[r] ^= col_mask;
        }
      }
    }
  }
}
