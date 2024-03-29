// rp2040
const int PURGE = 28;
const int SENSE = 29;
const int ROWS[] = { 2, 4, 1, 0, 7 };
const int COLS[] = { 3, 6, 27, 26 };

// samd21
//const int PURGE = 2;
//const int SENSE = 3;
//const int ROWS[] = { 8, 9, 7, 6, 5 };
//const int COLS[] = { 10, 4, 1, 0 };

int calibration_count = 10;
int adc_threshold = 30;

int adc_measured_val[16][5];
int calibration_val[16][5];

void acquire_adc() {
  for(int j=0; j<16; ++j) {
    digitalWrite(COLS[0], (j&1) ? HIGH : LOW);
    digitalWrite(COLS[1], (j&2) ? HIGH : LOW);
    digitalWrite(COLS[2], (j&4) ? HIGH : LOW);
    digitalWrite(COLS[3], (j&8) ? HIGH : LOW);
    for(int i=0; i<5; ++i) {

      pinMode(PURGE, OUTPUT);
      digitalWrite(PURGE, LOW);
      delayMicroseconds(10);
      pinMode(PURGE, INPUT);

      digitalWrite(ROWS[i], HIGH);
      adc_measured_val[j][i] = analogRead(SENSE);
      digitalWrite(ROWS[i], LOW);
    }
  }
}

void setup() {
  Serial.begin(9600);

  // multiplexer
  pinMode(COLS[0], OUTPUT);
  pinMode(COLS[1], OUTPUT);
  pinMode(COLS[2], OUTPUT);
  pinMode(COLS[3], OUTPUT);

  // purge pin
  pinMode(PURGE, INPUT);
  //pinMode(PURGE, OUTPUT);
      
  // sense pin
  pinMode(SENSE, INPUT);

  // trigger
  pinMode(ROWS[0], OUTPUT);
  pinMode(ROWS[1], OUTPUT);
  pinMode(ROWS[2], OUTPUT);
  pinMode(ROWS[3], OUTPUT);
  pinMode(ROWS[4], OUTPUT);

  for(int j=0; j<16; ++j) {
    for(int i=0; i<5; ++i) {
      calibration_val[j][i] = 0;
    }
  }
  
  for(int i=0; i<calibration_count; ++i) {
    acquire_adc();
  }

  for(int i=0; i<calibration_count; ++i) {
    acquire_adc();
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


void loop() {
  
  //unsigned long start = micros();
  //for(int i=0; i<1000; ++i) {
    acquire_adc();
  //}
  //unsigned long end = micros();
  //unsigned long delta = end - start;
  //Serial.println(delta);

  for(int i=0; i<5; ++i) {
    for(int j=0; j<16; ++j) {
      int v = adc_measured_val[j][i] - calibration_val[j][i] - adc_threshold;
      if(v<0) v=0;
      //int v = adc_measured_val[j][i];
      Serial.print(v);
      Serial.print(" ");
    }
    Serial.println();
  }
  Serial.println();
}
