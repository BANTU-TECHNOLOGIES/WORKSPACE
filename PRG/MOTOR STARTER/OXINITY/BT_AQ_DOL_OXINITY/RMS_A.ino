void CURRENT_RMS_MEASURE() {
  RMS_DATA[N_SAMPLES] = 0;
  int SAMPLING = 0;
  int LAST_ADC = 20;
  int RAW_ADC = 0;
  bool MEASUREMENT = HIGH;
  double RMS_SUM = 0;
  while (SAMPLING < 1000 && MEASUREMENT == HIGH) {
    RAW_ADC = analogRead(AMPLITUDE_PIN[SCAN]);
    if (RAW_ADC >= 0 && RAW_ADC <= 20) {
      if (RAW_ADC - LAST_ADC > 0) {
        while (MEASUREMENT == HIGH) {
          RMS_DATA[N_SAMPLES] = analogRead(AMPLITUDE_PIN[SCAN]);
          N_SAMPLES++;          
          if (N_SAMPLES >= MAX_SAMPLES) {
            MEASUREMENT = LOW;
          }
        }
      } else LAST_ADC = RAW_ADC;
    }
    SAMPLING++;
  }
  if (MEASUREMENT == HIGH) RMS[SCAN] = 0.00;
  else {
    for (int i = 0; i < N_SAMPLES; i++) {
      RMS_SUM += (double)RMS_DATA[i] * RMS_DATA[i];
    }
    double RMS_SQRT = sqrt(RMS_SUM / N_SAMPLES);
    RMS[SCAN] = GAIN[SCAN] * RMS_SQRT;
    N_SAMPLES = 0;
  }
}


/*void CURRENT_RMS_MEASURE() {

  RMS_DATA[N_SAMPLES] = 0;
  int sampling = 0;
  int last_adc = 20;
  int raw_adc = 0;
  bool amp_measurement = HIGH;
  double RMS_OUTPUT = 0;
  while (sampling < 1000 && amp_measurement == HIGH) {
    raw_adc = analogRead(AMPLITUDE_PIN[SCAN]);
    if (raw_adc >= 0 && raw_adc <= 20) {
      if (raw_adc - last_adc > 0) {
        while (amp_measurement == HIGH) {
          RMS_DATA[N_SAMPLES] = (GAIN[SCAN] * double(analogRead(AMPLITUDE_PIN[SCAN])));
          N_SAMPLES++;
          if (N_SAMPLES >= MAX_SAMPLES) {
            amp_measurement = LOW;
          }
        }
      } else last_adc = raw_adc;
    }
    sampling++;
  }
  if (amp_measurement == HIGH) RMS[SCAN] = 0.00;
  else{
    for (int i = 0; i < N_SAMPLES; i++) {
      RMS_OUTPUT += RMS_DATA[i] * RMS_DATA[i];
    }
  RMS[SCAN] = sqrt(RMS_OUTPUT / N_SAMPLES);
  N_SAMPLES = 0;
  }
  
}*/
