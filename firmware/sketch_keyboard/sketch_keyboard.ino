#include "sketch_keyboard.h"
#include "keymap.h"

void setup() {
  //Serial.begin(9600);

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
          if(fn_pressed && keymap_fn[r][c] > 1) {
            keycode = keymap_fn[r][c];
          }
          else if(is_nicola_layer(keycode, pressed) && keymap_nicola[r][c] > 1) {
            keycode = keymap_nicola[r][c];
          }
          
          //Serial.print(pressed ? "P" : "R");
          //Serial.print(' ');
          //Serial.print(keycode);
          //Serial.print(' ');
          //Serial.print(r);
          //Serial.print(' ');
          //Serial.println(c);
          
          if(keycode == KEY_NICOLA_ON) {
            if(pressed) {
              nicola_on();
            }
            matrix_prev[r] ^= col_mask;
            continue;
          } else if(keycode == KEY_NICOLA_OFF) {
            if(pressed) {
              nicola_off();
            }
            matrix_prev[r] ^= col_mask;
            continue;
          }
          if(nicola_state()) {
            if(!process_nicola(keycode, pressed)) {
              matrix_prev[r] ^= col_mask;
              continue;
            }
          }
          if(keycode > 1 && keycode <= 0xff) {
            if(pressed) {
              Keyboard.press(keycode);
            } else {
              Keyboard.release(keycode);
            }
          }
          matrix_prev[r] ^= col_mask;
        }
      }
    }
  }
}
