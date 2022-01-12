#include "sketch_keyboard.h"

#define ADC_RING_BUFFER_SIZE 16 

const int adc_press_threshold = 160 * ADC_RING_BUFFER_SIZE;
const int adc_release_threshold = 80 * ADC_RING_BUFFER_SIZE;
static int adc_ring_index = 0;

int adc_ring_buffer[ADC_RING_BUFFER_SIZE][MATRIX_COLS][MATRIX_ROWS];
int calibration_val[MATRIX_COLS][MATRIX_ROWS];
matrix_row_t matrix[MATRIX_ROWS];

void acquire_adc(int adc_measured_val[][5]) {
  for(int j=0; j<16; ++j) {
    digitalWrite(COLS[0], (j&1) ? HIGH : LOW);
    digitalWrite(COLS[1], (j&2) ? HIGH : LOW);
    digitalWrite(COLS[2], (j&4) ? HIGH : LOW);
    digitalWrite(COLS[3], (j&8) ? HIGH : LOW);
    for(int i=0; i<5; ++i) {
      digitalWrite(ROWS[i], HIGH);
      adc_measured_val[j][i] = analogRead(SENSE);
      digitalWrite(ROWS[i], LOW);
      pinMode(PURGE, OUTPUT);
      digitalWrite(PURGE, LOW);
      delayMicroseconds(4);
      pinMode(PURGE, INPUT);
    }
  }
}

void calibrate_adc(void) {
  int adc_measured_val[MATRIX_COLS][MATRIX_ROWS];

  for(int i=0; i<MATRIX_ROWS; ++i) {
    matrix[i] = 0;
    for(int j=0; j<MATRIX_COLS; ++j) {
      adc_measured_val[j][i] = 0;
      calibration_val[j][i] = 0;
      for(int k=0; k<ADC_RING_BUFFER_SIZE; ++k) {
        adc_ring_buffer[k][j][i] = 0;
      }
    }
  }
  
  for(int i=0; i<ADC_RING_BUFFER_SIZE; ++i) {
    acquire_adc(adc_measured_val);
  }

  for(int i=0; i<ADC_RING_BUFFER_SIZE; ++i) {
    acquire_adc(adc_measured_val);
    for(int j=0; j<16; ++j) {
      for(int k=0; k<5; ++k) {
        calibration_val[j][k] += adc_measured_val[j][k];
      }
    }
  }
}

uint8_t matrix_scan(void) {
    matrix_row_t curr_matrix[5] = {0, 0, 0, 0, 0};
    acquire_adc(adc_ring_buffer[adc_ring_index]);
    adc_ring_index = (adc_ring_index + 1) % ADC_RING_BUFFER_SIZE;

    for(int i=0; i<MATRIX_COLS; ++i) {
      for(int j=0; j<MATRIX_ROWS; ++j) {
        int adc_total = 0;
        for(int k=0; k<ADC_RING_BUFFER_SIZE; ++k) {
          adc_total += adc_ring_buffer[k][i][j];
        }

        if(adc_total > calibration_val[i][j] + adc_press_threshold) {
          curr_matrix[j] |= (1 << i);
        }
        else if(adc_total < calibration_val[i][j] + adc_release_threshold) {
          curr_matrix[j] &= ~(1 << i);
        }
      }
    }

    bool changed = memcmp(matrix, curr_matrix, sizeof(curr_matrix)) != 0;
    if (changed) memcpy(matrix, curr_matrix, sizeof(curr_matrix));

    return changed ? 1 : 0;
}


matrix_row_t matrix_get_row(uint8_t row) {
  return matrix[row];
}
