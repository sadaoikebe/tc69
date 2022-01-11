/*
 * Copyright 2017 Alex Ong <the.onga@gmail.com>
 * Copyright 2020 Andrei Purdea <andrei@purdea.ro>
 * Copyright 2021 Simon Arlott
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
Basic symmetric per-key algorithm. Uses an 8-bit counter per key.
When no state changes have occured for DEBOUNCE milliseconds, we push the state.
*/
#include "nicola_keyboard.h"

#ifndef DEBOUNCE
#    define DEBOUNCE 5
#endif

// Maximum debounce: 127ms
#if DEBOUNCE > 127
#    undef DEBOUNCE
#    define DEBOUNCE 127
#endif

#define ROW_SHIFTER ((matrix_row_t)1)

typedef struct {
    bool    pressed : 1;
    uint8_t time : 7;
} debounce_counter_t;

static debounce_counter_t  debounce_counters[MATRIX_ROWS * MATRIX_COLS];
static unsigned long       last_time;
static bool                counters_need_update;
static bool                matrix_need_update;

#    define DEBOUNCE_ELAPSED 0

static void update_debounce_counters_and_transfer_if_expired(matrix_row_t raw[], matrix_row_t cooked[], uint8_t elapsed_time);
static void transfer_matrix_values(matrix_row_t raw[], matrix_row_t cooked[]);

void debounce_init() {
    int i             = 0;
    for (uint8_t r = 0; r < MATRIX_ROWS; r++) {
        for (uint8_t c = 0; c < MATRIX_COLS; c++) {
            debounce_counters[i++].time = DEBOUNCE_ELAPSED;
        }
    }
}

void debounce(matrix_row_t raw[], matrix_row_t cooked[], bool changed) {
    bool updated_last = false;

    if (counters_need_update) {
        unsigned long now          = millis();
        unsigned long elapsed_time = now - last_time;

        last_time    = now;
        updated_last = true;
        if (elapsed_time > 255) {
            elapsed_time = 255;
        }

        if (elapsed_time > 0) {
            update_debounce_counters_and_transfer_if_expired(raw, cooked, elapsed_time);
        }
    }

    if (changed || matrix_need_update) {
        if (!updated_last) {
            last_time = millis();
        }

        transfer_matrix_values(raw, cooked);
    }
}

static void update_debounce_counters_and_transfer_if_expired(matrix_row_t raw[], matrix_row_t cooked[], uint8_t elapsed_time) {
    debounce_counter_t *debounce_pointer = debounce_counters;

    counters_need_update = false;
    matrix_need_update   = false;

    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            matrix_row_t col_mask = (ROW_SHIFTER << col);

            if (debounce_pointer->time != DEBOUNCE_ELAPSED) {
                if (debounce_pointer->time <= elapsed_time) {
                    debounce_pointer->time = DEBOUNCE_ELAPSED;

                    if (debounce_pointer->pressed) {
                        // key-down: eager
                        matrix_need_update = true;
                    } else {
                        // key-up: defer
                        cooked[row] = (cooked[row] & ~col_mask) | (raw[row] & col_mask);
                    }
                } else {
                    debounce_pointer->time -= elapsed_time;
                    counters_need_update = true;
                }
            }
            debounce_pointer++;
        }
    }
}

static void transfer_matrix_values(matrix_row_t raw[], matrix_row_t cooked[]) {
    debounce_counter_t *debounce_pointer = debounce_counters;

    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        matrix_row_t delta = raw[row] ^ cooked[row];
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            matrix_row_t col_mask = (ROW_SHIFTER << col);

            if (delta & col_mask) {
                if (debounce_pointer->time == DEBOUNCE_ELAPSED) {
                    debounce_pointer->pressed = (raw[row] & col_mask);
                    debounce_pointer->time    = DEBOUNCE;
                    counters_need_update      = true;

                    if (debounce_pointer->pressed) {
                        // key-down: eager
                        cooked[row] ^= col_mask;
                    }
                }
            } else if (debounce_pointer->time != DEBOUNCE_ELAPSED) {
                if (!debounce_pointer->pressed) {
                    // key-up: defer
                    debounce_pointer->time = DEBOUNCE_ELAPSED;
                }
            }
            debounce_pointer++;
        }
    }
}
