#include "nicola_keyboard.h"

void acquire_adc(int adc_measured_val[][5]) {
  for(int j=0; j<16; ++j) {
    digitalWrite(COLS[0], (j&1) ? HIGH : LOW);
    digitalWrite(COLS[1], (j&2) ? HIGH : LOW);
    digitalWrite(COLS[2], (j&4) ? HIGH : LOW);
    digitalWrite(COLS[3], (j&8) ? HIGH : LOW);
    for(int i=0; i<5; ++i) {
      digitalWrite(ROWS[i], HIGH);
      adc_measured_val[j][i] = analogRead(SENSE);
      adc_measured_val[j][i] += analogRead(SENSE);
      adc_measured_val[j][i] += analogRead(SENSE);
      adc_measured_val[j][i] += analogRead(SENSE);
      adc_measured_val[j][i] += analogRead(SENSE);
      digitalWrite(ROWS[i], LOW);
      pinMode(PURGE, OUTPUT);
      digitalWrite(PURGE, LOW);
      pinMode(PURGE, INPUT);
    }
  }
}
