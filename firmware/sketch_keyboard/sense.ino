#include "nicola_keyboard.h"

int calibration_count = 10;
int adc_threshold = 200;

int adc_measured_val[MATRIX_COLS][MATRIX_ROWS];
int calibration_val[MATRIX_COLS][MATRIX_ROWS];
matrix_row_t raw_matrix[MATRIX_ROWS];
matrix_row_t matrix[MATRIX_ROWS];

void calibrate_adc(void) {
  for(int j=0; j<16; ++j) {
    for(int i=0; i<5; ++i) {
      calibration_val[j][i] = 0;
    }
  }
  
  for(int i=0; i<calibration_count; ++i) {
    acquire_adc(adc_measured_val);
  }

  for(int i=0; i<calibration_count; ++i) {
    acquire_adc(adc_measured_val);
    for(int j=0; j<16; ++j) {
      for(int k=0; k<5; ++k) {
        calibration_val[j][k] += adc_measured_val[j][k];
      }
    }
  }

  for(int j=0; j<16; ++j) {
    for(int i=0; i<5; ++i) {
      calibration_val[j][i] /= calibration_count;
    }
  }
}

uint8_t matrix_scan(void) {
    matrix_row_t curr_matrix[5] = {0, 0, 0, 0, 0};
    acquire_adc(adc_measured_val);
    for(int j=0; j<16; ++j) {
      for(int i=0; i<5; ++i) {
        int v = adc_measured_val[j][i] - calibration_val[j][i] - adc_threshold;
        if(v>0) {
          curr_matrix[i] |= (1<<j);
        }
      }
    }

    bool changed = memcmp(raw_matrix, curr_matrix, sizeof(curr_matrix)) != 0;
    if (changed) memcpy(raw_matrix, curr_matrix, sizeof(curr_matrix));

    debounce(raw_matrix, matrix, changed);

    return changed ? 1 : 0;
}


matrix_row_t matrix_get_row(uint8_t row) {
  return matrix[row];
}
