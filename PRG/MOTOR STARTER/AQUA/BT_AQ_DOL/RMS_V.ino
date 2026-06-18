void VOLT_RMS_MEASURE() {
  RMS_DATA[N_SAMPLES] = 0;
  int SAMPLING = 0;
  int RAW_ADC = 0;
  bool MEASUREMENT = HIGH;
  // unsigned long PT = millis();
  while (SAMPLING < 1000 && MEASUREMENT == HIGH) {
    RAW_ADC = analogRead(AMPLITUDE_PIN[SCAN]);
      if (RAW_ADC >= 500 && RAW_ADC <= 520) {
      while (MEASUREMENT == HIGH) {
        RMS_DATA[N_SAMPLES] = analogRead(AMPLITUDE_PIN[SCAN]) - analogRead(AMPLITUDE_PIN[SCAN + 2]);
        N_SAMPLES++;
        if (N_SAMPLES >= MAX_SAMPLES) {
          MEASUREMENT = LOW;
        }
      }

    } SAMPLING++;
  }
 //  ET = millis() - PT ;
  double RMS_SUM = 0;
  for (int i = 0; i < N_SAMPLES; i++) {
    RMS_SUM += (double)RMS_DATA[i] * RMS_DATA[i];
  }
  double RMS_SQRT = sqrt(RMS_SUM / N_SAMPLES);
  RMS[SCAN] = GAIN[SCAN] * RMS_SQRT; 
 if (RMS[SCAN] <= 25) RMS[SCAN] = 0;
  N_SAMPLES = 0;
}


/*
void VOLT_RMS_MEASURE() {
double RMS_DATA[MAX_SAMPLES];
  RMS_DATA[N_SAMPLES] = 0;
  bool volt_measurement = HIGH;
  unsigned long PT = millis();
  while (SCAN == 1 && volt_measurement == HIGH) {
    RMS_DATA[N_SAMPLES] = GAIN[1] * (analogRead(AMPLITUDE_PIN[1]) - analogRead(AMPLITUDE_PIN[3]));
    N_SAMPLES++;
    if (N_SAMPLES >= MAX_SAMPLES) {
      volt_measurement = LOW;
    }
  }
  while (SCAN == 3 && volt_measurement == HIGH) {
    RMS_DATA[N_SAMPLES] = GAIN[3] * (analogRead(AMPLITUDE_PIN[3]) - analogRead(AMPLITUDE_PIN[5]));
    N_SAMPLES++;
    if (N_SAMPLES >= MAX_SAMPLES) {
      volt_measurement = LOW;
    }
  }
  while (SCAN == 5 && volt_measurement == HIGH) {
    RMS_DATA[N_SAMPLES] = GAIN[5] * (analogRead(AMPLITUDE_PIN[5]) - analogRead(AMPLITUDE_PIN[1]));
    N_SAMPLES++;
    if (N_SAMPLES >= MAX_SAMPLES) {
      volt_measurement = LOW;
    }
  }
     ET = millis() - PT ;
  double RMS_OUTPUT = 0;
  for (int i = 0; i < N_SAMPLES; i++) {
    RMS_OUTPUT += RMS_DATA[i] * RMS_DATA[i];
  }
  RMS[SCAN] = sqrt(RMS_OUTPUT / N_SAMPLES);
  if (SCAN == 1) {
    RMS[1] = RYAVG.reading(RMS[1]);
    if (RMS[1] <= 25) RMS[1] = 0;
    else RMS[1];
  } else if (SCAN == 3) {
    RMS[3] = YBAVG.reading(RMS[3]);
    if (RMS[3] <= 25) RMS[3] = 0;
    else RMS[3];
  } else if (SCAN == 5) {
    RMS[5] = BRAVG.reading(RMS[5]);
    if (RMS[5] <= 25) RMS[5] = 0;
    else RMS[5];
  }
  N_SAMPLES = 0;
}*/