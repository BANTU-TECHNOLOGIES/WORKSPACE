String SWV = "U23";

#include <EEPROM.h>
#include <TM16xxDisplay.h>
#include <TM1650.h>
#include <plclib.h>

#include "R10_10HP.h"  //4808 Chip

bool MAN_PB_FLAG;
bool STOP_PB_FLAG;
bool AUTO_PB_FLAG;
bool OC_TRIP;
bool TRIP;
bool MOTOR_CMD_FLAG;
bool AUTO_CMD_FLAG;
bool MAN_CMD_FLAG;
bool AUTO_DELAY_FLAG;
bool PULSE;
bool PULSE_1;
bool LED_AUTO_FLAG;
bool LED_MAN_FLAG;
bool LED_STOP_FLAG;
bool R_LED_FLAG;
bool Y_LED_FLAG;
bool B_LED_FLAG;
bool AUTO_MODE;
bool FAULT_RESET;
bool PHASE_SEQ;
bool BUZ_FLAG;
bool SCF_FLAG = LOW;
bool FAC_DEF_SET = LOW;
bool CALIB_CMD_FLAG = LOW;

uint16_t AUTO_MODE_MEM_FLAG = 00;
uint16_t SET_HH_MEM_FLAG = 10;
uint16_t SET_MM_MEM_FLAG = 20;
uint16_t SET_HV_MEM_FLAG = 30;
uint16_t SET_LV_MEM_FLAG = 40;
uint16_t SET_HP_MEM_FLAG = 50;
uint16_t SET_OC_MEM_FLAG = 60;
uint16_t SET_UC_MEM_FLAG = 70;
uint16_t SET_UBV_MEM_FLAG = 80;
uint16_t RV_GAIN_MEM_FLAG = 90;
uint16_t YV_GAIN_MEM_FLAG = 100;
uint16_t BV_GAIN_MEM_FLAG = 110;
uint16_t RA_GAIN_MEM_FLAG = 120;
uint16_t YA_GAIN_MEM_FLAG = 130;
uint16_t BA_GAIN_MEM_FLAG = 140;

int SET_HH_MIN = 1;
int SET_HH_MAX = 9;
int SET_MM_MIN = 1;
int SET_MM_MAX = 99;
int SET_HV_MIN = 470;
int SET_HV_MAX = 550;
int SET_LV_MIN = 270;
int SET_LV_MAX = 380;
int SET_HP_MIN = 1;
int SET_OC_MIN = 10;
int SET_UC_MIN = 05;
int SET_UC_MAX;
int SET_VGAIN_MIN = 400;
int SET_VGAIN_MAX = 550;
int SET_UBV_MIN = 1;
int SET_UBV_MAX = 100;
int SET_HH;
int SET_MM;
int SET_HV;
int SET_LV;
int SET_HP;
int SET_OC;
int SET_UC;
int SET_UBV;
int SET_RVGAIN;
int SET_YVGAIN;
int SET_BVGAIN;
int SET_RAGAIN;
int SET_YAGAIN;
int SET_BAGAIN;
int AUTO_MODE_MEM;
int AUTO_MODE_VALUE;
int MIN_DISPLAY;
int VOLT;
int AMP;
int UBV_TRIP_PERCENTAGE;
int MIN_VOLTAGE;
int MAX_VOLTAGE;
double AVG_VOLTAGE;
double MAX_DEVIATION;

double RMS[6];
double GAIN[6];
double OC_TRIP_100;
double OC_TRIP_500;
double OC_TRIP_700;

byte TIMER;
byte FAULT = 0;
byte SCREEN = 1;
byte DIS_SCROL = 0;
byte SCAN = 0;
byte DISPLAY_CHECK = 0;

unsigned long OC_TRIP_CURVE[6];
unsigned long OC_TRIP_TIME[6];

unsigned long ON_TIMER;
unsigned long OFF_TIMER;
unsigned long PULSE_TIMER;
unsigned long PULSE_TIMER_1;

CTUD HLV_TRIP_CTUD;
CTU MENU_CTU;
CTU CALIB_CTU;
CTD SCROL_SPEED;

TON MENU_SCREEN_TIMER(5000);
TON MENU_EXIT_TIMER(30000);
TON CALIB_SCREEN_TIMER(5000);
TON SCROL_TIMER_INC(1000);
TON SCROL_TIMER_DEC(1000);
TON HV_TRIP_TIMER(2000);
TON LV_TRIP_TIMER(5000);
TON UBV_TRIP_TIMER(5000);
TON UC_TRIP_TIMER(6000);
TON HLV_TRIP_AUTO_RST_TON(5000);
TON HLV_TRIP_HOUR_RST_TON(3600000);

R_TRIG STOP_RTRIG;
R_TRIG START_RTRIG;
R_TRIG AUTO_RTRIG;
R_TRIG MENU_SCREEN_RTRIG;
R_TRIG CALIB_SCREEN_RTRIG;
R_TRIG SCROL_FAST;
R_TRIG HV_TRIP_RTRIG;
R_TRIG LV_TRIP_RTRIG;
R_TRIG OC_TRIP_RTRIG;
R_TRIG UC_TRIP_RTRIG;
R_TRIG FAULT_RESET_RTRIG;
R_TRIG TRIP_RTRIG;
R_TRIG PULSE_RTRIG;
R_TRIG ON_TIMER_RTRIG;
R_TRIG OFF_TIMER_RTRIG;
F_TRIG PULSE_FTRIG;

//const double VMultiplier = 5 / 1024.0 * 300;                                               // Amplitude multiplier
const double nCyclesToMeasure = 1;  //The number of cycles to measure across
const int nSamplesPerCycle = 300;   //The number of samples per cycle
//const double sineFrequency = 50;                                                           //The frequency of the sine wave
const int MAX_SAMPLES = nSamplesPerCycle * nCyclesToMeasure;  //The number of samples in a sample cycle
//const int samplePeriod = ((1 / sineFrequency) * nCyclesToMeasure * 1000000) / MAX_SAMPLES;  //Frequency in microseconds of sampling
int N_SAMPLES = 0;  //
int RMS_DATA[MAX_SAMPLES];

void setup() {

  pinMode(STOP_PB_PIN, INPUT_PULLUP);
  pinMode(AUTO_PB_PIN, INPUT_PULLUP);
  pinMode(MAN_PB_PIN, INPUT_PULLUP);

  pinMode(LED_MAN, OUTPUT);
  pinMode(LED_AUTO, OUTPUT);
  pinMode(LED_STOP, OUTPUT);
  pinMode(RELAY, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(SCF_PIN, INPUT);

  module1.begin();
  module1.setupDisplay(true, 1);
  module2.begin();
  module2.setupDisplay(true, 1);
  
  if (digitalRead(STOP_PB_PIN) != HIGH) {
    V_DISP.setDisplayToString(HWV, B0100);
    A_DISP.setDisplayToString(SWV, B0100);
  } else {
    V_DISP.setDisplayToString("888", B0111);
    A_DISP.setDisplayToString("888", B0111);
  }

  EEPROM.get(AUTO_MODE_MEM_FLAG, AUTO_MODE_MEM);
  EEPROM.get(SET_HH_MEM_FLAG, SET_HH);
  EEPROM.get(SET_MM_MEM_FLAG, SET_MM);
  EEPROM.get(SET_HV_MEM_FLAG, SET_HV);
  EEPROM.get(SET_LV_MEM_FLAG, SET_LV);
  EEPROM.get(SET_HP_MEM_FLAG, SET_HP);
  EEPROM.get(SET_OC_MEM_FLAG, SET_OC);
  EEPROM.get(SET_UC_MEM_FLAG, SET_UC);
  EEPROM.get(SET_UBV_MEM_FLAG, SET_UBV);

  EEPROM.get(RV_GAIN_MEM_FLAG, SET_RVGAIN);
  EEPROM.get(YV_GAIN_MEM_FLAG, SET_YVGAIN);
  EEPROM.get(BV_GAIN_MEM_FLAG, SET_BVGAIN);
  EEPROM.get(RA_GAIN_MEM_FLAG, SET_RAGAIN);
  EEPROM.get(YA_GAIN_MEM_FLAG, SET_YAGAIN);
  EEPROM.get(BA_GAIN_MEM_FLAG, SET_BAGAIN);

  GAIN[1] = (SET_RVGAIN / 1000.000) + 1.000;
  GAIN[3] = (SET_YVGAIN / 1000.000) + 1.000;
  GAIN[5] = (SET_BVGAIN / 1000.000) + 1.000;
  if (SET_HP_MAX <= 5) {
    GAIN[0] = (SET_RAGAIN / 10000.0000);
    GAIN[2] = (SET_YAGAIN / 10000.0000);
    GAIN[4] = (SET_BAGAIN / 10000.0000);
  } else if (SET_HP_MAX == 10) {
    GAIN[0] = (SET_RAGAIN / 1000.000);
    GAIN[2] = (SET_YAGAIN / 1000.000);
    GAIN[4] = (SET_BAGAIN / 1000.000);
  }

  SCROL_SPEED.PV = 10;
  MENU_CTU.PV = 7;
  CALIB_CTU.PV = 9;
  HLV_TRIP_CTUD.PV = 3;
  AUTO_MODE_VALUE = AUTO_MODE_MEM;

 
  for (int i = 0; i <= 17; i++) {
    if (SCAN == 1 || SCAN == 3 || SCAN == 5) VOLT_RMS_MEASURE();
    SCAN++;
    if (SCAN > 5) SCAN = 0;
  }

  attachInterrupt(SCF_PIN, SCF, RISING);
  interrupts();

  int8_t PHASE_SEQ_SCAN = 0;
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j <= 1000; j++) {
      int B = analogRead(AMPLITUDE_PIN[5]);
      if (B > 700 && B < 900) {
        delayMicroseconds(6666);
        int R = analogRead(AMPLITUDE_PIN[1]);
        int Y = analogRead(AMPLITUDE_PIN[3]);
        if (R > 600 && R < 1000) PHASE_SEQ_SCAN++;
        else if (Y > 600 && Y < 1000) PHASE_SEQ_SCAN--;
        break;
      }
    }
  }
  if (PHASE_SEQ_SCAN >= 3) PHASE_SEQ = LOW;
  else if (PHASE_SEQ_SCAN <= -3) PHASE_SEQ = HIGH;


  module1.setSegments(0b00000001, 3);
  delay(1000);
  module1.setSegments(0b00010000, 3);
  delay(1000);
  module1.setSegments(0b00001000, 3);
  delay(1000);
  module1.setSegments(0b00100000, 3);
  delay(1000);
  module1.setSegments(0b00000100, 3);
  delay(1000);
  module1.setSegments(0b01000000, 3);
  delay(1000);
  module1.setSegments(0b00000000, 3);

  PULSE_TIMER = millis();
  PULSE_TIMER_1 = millis();
}

void (*resetFunc)(void) = 0;  //declare reset function at address 0

void SCF() {
  if (digitalRead(SCF_PIN) == HIGH) {
    digitalWriteFast(RELAY, LOW);
    MOTOR_CMD_FLAG = LOW;
    SCF_FLAG = HIGH;
    TRIP = HIGH;
  }
}


void loop() {

 
  if (millis() - PULSE_TIMER >= 500) {
    PULSE_TIMER += 500;  // Keeps absolute interval integrity
    PULSE = !PULSE;
  }
  if ((SCREEN == 2 || SCREEN == 4) && millis() >= PULSE_TIMER_1) {
    PULSE_TIMER_1 += (SCROL_SPEED.CV * 20) + 1;
    PULSE_1 = !PULSE_1;
  }

    PULSE_RTRIG.process(PULSE);
    //PULSE_FTRIG.process(PULSE);

    AUTO_PB_FLAG = !digitalRead(AUTO_PB_PIN);
    MAN_PB_FLAG = !digitalRead(MAN_PB_PIN);
    STOP_PB_FLAG = !digitalRead(STOP_PB_PIN);

    STOP_RTRIG.process(STOP_PB_FLAG == HIGH);
    START_RTRIG.process(MAN_PB_FLAG == HIGH);
    AUTO_RTRIG.process(AUTO_PB_FLAG == HIGH);

    if (MAN_PB_FLAG == LOW && AUTO_PB_FLAG == LOW && STOP_PB_FLAG == HIGH) AUTO_MODE_VALUE = 50, MAN_CMD_FLAG = LOW;
    else if (SCREEN == 1 && MAN_PB_FLAG == HIGH && AUTO_PB_FLAG == LOW && STOP_PB_FLAG == LOW) AUTO_MODE_VALUE = 50, MAN_CMD_FLAG = HIGH;
    else if (SCREEN == 1 && MAN_PB_FLAG == LOW && AUTO_PB_FLAG == HIGH && STOP_PB_FLAG == LOW) AUTO_MODE_VALUE = 100, MAN_CMD_FLAG = LOW;

    if (SCREEN == 1 && AUTO_MODE_MEM != AUTO_MODE_VALUE) EEPROM.put(AUTO_MODE_MEM_FLAG, AUTO_MODE_VALUE);
    EEPROM.get(AUTO_MODE_MEM_FLAG, AUTO_MODE_MEM);
    if (AUTO_MODE_MEM == 100) AUTO_MODE = HIGH;
    else AUTO_MODE = LOW;

    SCROL_TIMER_INC.process(AUTO_PB_FLAG == HIGH);
    SCROL_TIMER_DEC.process(MAN_PB_FLAG == HIGH);
    SCROL_SPEED.process((SCROL_TIMER_INC.Q == HIGH || SCROL_TIMER_DEC.Q == HIGH) && PULSE == HIGH, SCROL_TIMER_INC.Q == LOW && SCROL_TIMER_DEC.Q == LOW);
    SCROL_FAST.process(PULSE_1 == HIGH);

    MENU_SCREEN_TIMER.process(STOP_PB_FLAG == HIGH);
    MENU_SCREEN_RTRIG.process(MENU_SCREEN_TIMER.Q);
    MENU_CTU.process((STOP_RTRIG.Q == HIGH && SCREEN == 2), MENU_CTU.Q == HIGH || MENU_SCREEN_RTRIG.Q == HIGH);
    MENU_EXIT_TIMER.process((SCREEN == 2 || SCREEN == 4) && AUTO_PB_FLAG == LOW && STOP_PB_FLAG == LOW && MAN_PB_FLAG == LOW);

    CALIB_SCREEN_TIMER.process(AUTO_PB_FLAG == HIGH && MAN_PB_FLAG == HIGH);
    CALIB_SCREEN_RTRIG.process(CALIB_SCREEN_TIMER.Q);
    CALIB_CTU.process((STOP_RTRIG.Q == HIGH && SCREEN == 4), CALIB_CTU.Q == HIGH);

    if (SCREEN == 4 && (CALIB_CTU.CV >= 5 && CALIB_CTU.CV <= 7)) CALIB_CMD_FLAG = HIGH;
    else CALIB_CMD_FLAG = LOW;

    OC_TRIP_100 = (SET_OC / 10.0);        //LOAD CURRENT
    OC_TRIP_500 = (SET_OC / 10.0) * 5.0;  //OVER LOAD CURRENT
    OC_TRIP_700 = constrain(OC_TRIP_500, 1.0, CT_MAX_RMS_AMP);

    if (SCREEN != 2 && (SCAN == 0 || SCAN == 2 || SCAN == 4)) {
      CURRENT_RMS_MEASURE();
      if (RMS[SCAN] >= OC_TRIP_100 && RMS[SCAN] <= OC_TRIP_700) OC_TRIP_CURVE[SCAN] = map(RMS[SCAN], OC_TRIP_100, OC_TRIP_700, 20000, 8000);
      else if (RMS[SCAN] > OC_TRIP_700) OC_TRIP_CURVE[SCAN] = 7000;
      if (RMS[SCAN] <= OC_TRIP_100) OC_TRIP_TIME[SCAN] = millis();
      else if (RMS[SCAN] > OC_TRIP_100 && ((millis() - OC_TRIP_TIME[SCAN]) >= OC_TRIP_CURVE[SCAN])) OC_TRIP = HIGH;
    } else if (SCREEN != 2 && (SCAN == 1 || SCAN == 3 || SCAN == 5)) {
      VOLT_RMS_MEASURE();
    }

    HV_TRIP_TIMER.process(MOTOR_CMD_FLAG == HIGH && (RMS[1] > SET_HV || RMS[3] > SET_HV || RMS[5] > SET_HV));
    LV_TRIP_TIMER.process(MOTOR_CMD_FLAG == HIGH && (RMS[1] < SET_LV || RMS[3] < SET_LV || RMS[5] < SET_LV));
    UC_TRIP_TIMER.process(MOTOR_CMD_FLAG == HIGH && (RMS[0] < SET_UC / 10.0 || RMS[2] < SET_UC / 10.0 || RMS[4] < SET_UC / 10.0));
    AVG_VOLTAGE = (RMS[1] + RMS[3] + RMS[5]) / 3;
    if (RMS[1] > RMS[3] && RMS[1] > RMS[5]) MAX_VOLTAGE = RMS[1];
    else if (RMS[3] > RMS[5] && RMS[3] > RMS[1]) MAX_VOLTAGE = RMS[3];
    else if (RMS[5] > RMS[1] && RMS[5] > RMS[3]) MAX_VOLTAGE = RMS[5];
    if (RMS[1] < RMS[3] && RMS[1] < RMS[5]) MIN_VOLTAGE = RMS[1];
    else if (RMS[3] < RMS[5] && RMS[3] < RMS[1]) MIN_VOLTAGE = RMS[3];
    else if (RMS[5] < RMS[1] && RMS[5] < RMS[3]) MIN_VOLTAGE = RMS[5];
    MAX_DEVIATION = MAX_VOLTAGE - MIN_VOLTAGE;
    UBV_TRIP_PERCENTAGE = (MAX_DEVIATION / AVG_VOLTAGE) * 100;
    UBV_TRIP_TIMER.process(MOTOR_CMD_FLAG == HIGH && UBV_TRIP_PERCENTAGE > SET_UBV);

    HLV_TRIP_AUTO_RST_TON.process(HLV_TRIP_CTUD.QU == LOW && (FAULT == 1 || FAULT == 2) && AUTO_MODE == HIGH && TRIP == HIGH && RMS[1] < SET_HV && RMS[3] < SET_HV && RMS[5] < SET_HV && RMS[1] > SET_LV && RMS[3] > SET_LV && RMS[5] > SET_LV && UBV_TRIP_PERCENTAGE < SET_UBV);
    HLV_TRIP_CTUD.CU = HLV_TRIP_AUTO_RST_TON.Q;
    HLV_TRIP_CTUD.CD = HLV_TRIP_HOUR_RST_TON.Q;
    HLV_TRIP_CTUD.RESET = STOP_PB_FLAG == HIGH;
    HLV_TRIP_CTUD.process();
    HLV_TRIP_HOUR_RST_TON.process(HLV_TRIP_CTUD.QU);

    if ((SCREEN == 1) && HV_TRIP_TIMER.Q == HIGH || LV_TRIP_TIMER.Q == HIGH || OC_TRIP == HIGH || UC_TRIP_TIMER.Q == HIGH || SCF_FLAG == HIGH || PHASE_SEQ == HIGH || UBV_TRIP_TIMER.Q == HIGH) TRIP = HIGH;

    if (((millis() - ON_TIMER) < SET_HH * 3600000) && AUTO_MODE == HIGH && TRIP == LOW && AUTO_DELAY_FLAG == LOW) AUTO_CMD_FLAG = HIGH;
    else AUTO_CMD_FLAG = LOW, ON_TIMER = millis();

    if (((millis() - OFF_TIMER) < SET_MM * 60000) && AUTO_MODE == HIGH && TRIP == LOW && AUTO_CMD_FLAG == LOW) AUTO_DELAY_FLAG = HIGH;
    else AUTO_DELAY_FLAG = LOW, OFF_TIMER = millis();
    MIN_DISPLAY = ((SET_MM + 1) * 60000 - (millis() - OFF_TIMER)) / 60000;

    TRIP_RTRIG.process(TRIP == HIGH);
    if (TRIP_RTRIG.Q) SCREEN = 3;

    if (TRIP == LOW) FAULT = 0, module1.setSegments(0b00000000, 3);
    else if (HV_TRIP_TIMER.Q) FAULT = 1;
    else if (LV_TRIP_TIMER.Q || UBV_TRIP_TIMER.Q) FAULT = 2;
    else if (PHASE_SEQ == HIGH) FAULT = 3;
    else if (OC_TRIP == HIGH) FAULT = 4;
    else if (UC_TRIP_TIMER.Q) FAULT = 5;
    else if (SCF_FLAG == HIGH) FAULT = 6;

    if (TRIP == HIGH) {
      LED_MAN_FLAG = LOW;
      LED_STOP_FLAG = PULSE;
      LED_AUTO_FLAG = LOW;
      BUZ_FLAG = PULSE;
      TIMER = 4;
    } else if (SCREEN == 4) {
      LED_MAN_FLAG = LOW;
      LED_STOP_FLAG = LOW;
      LED_AUTO_FLAG = LOW;
      BUZ_FLAG = LOW;
    } else if (AUTO_CMD_FLAG == LOW && MAN_CMD_FLAG == LOW && AUTO_MODE == LOW) {
      LED_MAN_FLAG = LOW;
      LED_STOP_FLAG = HIGH;
      LED_AUTO_FLAG = LOW;
      BUZ_FLAG = LOW;
      TIMER = 1;
    } else if (AUTO_CMD_FLAG == LOW && MAN_CMD_FLAG == LOW && AUTO_MODE == HIGH) {
      LED_MAN_FLAG = LOW;
      LED_STOP_FLAG = LOW;
      LED_AUTO_FLAG = PULSE;
      BUZ_FLAG = LOW;
      TIMER = 2;
    } else if (AUTO_CMD_FLAG == HIGH && MAN_CMD_FLAG == LOW && AUTO_MODE == HIGH) {
      LED_MAN_FLAG = LOW;
      LED_STOP_FLAG = LOW;
      LED_AUTO_FLAG = HIGH;
      BUZ_FLAG = LOW;
      TIMER = 3;
    } else if (AUTO_CMD_FLAG == LOW && MAN_CMD_FLAG == HIGH && AUTO_MODE == LOW) {
      LED_MAN_FLAG = HIGH;
      LED_STOP_FLAG = LOW;
      LED_AUTO_FLAG = LOW;
      BUZ_FLAG = LOW;
      TIMER = 3;
    } else {
      LED_MAN_FLAG = LOW;
      LED_STOP_FLAG = LOW;
      LED_AUTO_FLAG = LOW;
      BUZ_FLAG = LOW;
    }

    if ((SCREEN == 2 && MENU_SCREEN_RTRIG.Q == HIGH) || MENU_EXIT_TIMER.Q == HIGH) {
      EEPROM.put(SET_HH_MEM_FLAG, SET_HH);
      EEPROM.put(SET_MM_MEM_FLAG, SET_MM);
      EEPROM.put(SET_HV_MEM_FLAG, SET_HV);
      EEPROM.put(SET_LV_MEM_FLAG, SET_LV);
      EEPROM.put(SET_HP_MEM_FLAG, SET_HP);
      EEPROM.put(SET_OC_MEM_FLAG, SET_OC);
      EEPROM.put(SET_UC_MEM_FLAG, SET_UC);
      SCREEN = 1;
    } else if (((AUTO_RTRIG.Q || START_RTRIG.Q) && SCREEN == 4 && CALIB_CTU.CV == 0) || MENU_EXIT_TIMER.Q == HIGH) {
      EEPROM.put(RV_GAIN_MEM_FLAG, SET_RVGAIN);
      EEPROM.put(YV_GAIN_MEM_FLAG, SET_YVGAIN);
      EEPROM.put(BV_GAIN_MEM_FLAG, SET_BVGAIN);
      EEPROM.put(RA_GAIN_MEM_FLAG, SET_RAGAIN);
      EEPROM.put(YA_GAIN_MEM_FLAG, SET_YAGAIN);
      EEPROM.put(BA_GAIN_MEM_FLAG, SET_BAGAIN);
      EEPROM.put(SET_UBV_MEM_FLAG, SET_UBV);
      resetFunc();
    } else if ((AUTO_RTRIG.Q || START_RTRIG.Q) && SCREEN == 4 && CALIB_CTU.CV == 1 && FAC_DEF_SET == 0) {
      EEPROM.put(RV_GAIN_MEM_FLAG, 464);//464
      EEPROM.put(YV_GAIN_MEM_FLAG, 464);//464
      EEPROM.put(BV_GAIN_MEM_FLAG, 464);//464
      EEPROM.put(SET_HH_MEM_FLAG, 6);
      EEPROM.put(SET_MM_MEM_FLAG, 30);
      EEPROM.put(SET_HV_MEM_FLAG, 500);
      EEPROM.put(SET_LV_MEM_FLAG, 300);
      EEPROM.put(SET_HP_MEM_FLAG, 5);
      EEPROM.put(SET_OC_MEM_FLAG, 90);
      EEPROM.put(SET_UC_MEM_FLAG, 5);
      EEPROM.put(SET_UBV_MEM_FLAG, 12);
      if (SET_HP_MAX <= 5) {
        EEPROM.put(RA_GAIN_MEM_FLAG, 690);
        EEPROM.put(YA_GAIN_MEM_FLAG, 690);
        EEPROM.put(BA_GAIN_MEM_FLAG, 690);
      } else if (SET_HP_MAX == 10) {
        EEPROM.put(RA_GAIN_MEM_FLAG, 132);//140
        EEPROM.put(YA_GAIN_MEM_FLAG, 132);//140
        EEPROM.put(BA_GAIN_MEM_FLAG, 132);//140
      }
      resetFunc();
    } else if (CALIB_SCREEN_RTRIG.Q == HIGH && (SCREEN == 1 || SCREEN == 2 || SCREEN == 3)) {
      CALIB_CTU.CV = 0;
      SCREEN = 4;
      TRIP = LOW;
    } else if (MENU_SCREEN_RTRIG.Q == HIGH && (SCREEN == 1 || SCREEN == 3 || SCREEN == 4)) {
      MENU_CTU.CV = 0;
      SCREEN = 2;
    }

    if (PULSE_RTRIG.Q && DIS_SCROL <= 2) DIS_SCROL++;
    if (DIS_SCROL > 2) DIS_SCROL = 0;

    if (SCREEN == 1 && TRIP == LOW) {
      switch (DIS_SCROL) {
        case 0:
          R_LED_FLAG = HIGH;
          Y_LED_FLAG = LOW;
          B_LED_FLAG = LOW;
          if (PULSE_RTRIG.Q) AMP = RMS[0] * 10, VOLT = RMS[1];
          break;
        case 1:
          R_LED_FLAG = LOW;
          Y_LED_FLAG = HIGH;
          B_LED_FLAG = LOW;
          if (PULSE_RTRIG.Q) AMP = RMS[2] * 10, VOLT = RMS[3];
          break;
        case 2:
          R_LED_FLAG = LOW;
          Y_LED_FLAG = LOW;
          B_LED_FLAG = HIGH;
          if (PULSE_RTRIG.Q) AMP = RMS[4] * 10, VOLT = RMS[5];
          break;
      }
    } else if (SCREEN == 2) {
      R_LED_FLAG = LOW;
      Y_LED_FLAG = LOW;
      B_LED_FLAG = LOW;
      BUZ_FLAG = LOW;
    } else if (SCREEN == 4) {
      BUZ_FLAG = LOW;
    } else {
      R_LED_FLAG = LOW;
      Y_LED_FLAG = LOW;
      B_LED_FLAG = LOW;
    }

    if (((SCREEN == 1 && (AUTO_CMD_FLAG == HIGH || MAN_CMD_FLAG == HIGH)) || (SCREEN == 4 && CALIB_CMD_FLAG == HIGH)) && TRIP == LOW) MOTOR_CMD_FLAG = HIGH;
    else MOTOR_CMD_FLAG = LOW;

    FAULT_RESET_RTRIG.process(STOP_PB_FLAG == HIGH || HLV_TRIP_AUTO_RST_TON.Q == HIGH);
    if (FAULT_RESET_RTRIG.Q && SCREEN == 3) SCREEN = 1, OC_TRIP = LOW, SCF_FLAG = LOW, TRIP = LOW;
    else if (TRIP == HIGH && SCREEN == 1) SCREEN = 3;

    LED_DISPLAY();

    digitalWriteFast(RELAY, MOTOR_CMD_FLAG);
    digitalWriteFast(LED_MAN, LED_MAN_FLAG);
    digitalWriteFast(LED_AUTO, LED_AUTO_FLAG);
    digitalWriteFast(LED_STOP, LED_STOP_FLAG);
    digitalWriteFast(BUZZER, BUZ_FLAG);

    if (R_LED_FLAG == HIGH) module2.setSegments(0b00000001, 3);
    else if (Y_LED_FLAG == HIGH) module2.setSegments(0b00100000, 3);
    else if (B_LED_FLAG == HIGH) module2.setSegments(0b00000010, 3);
    else module2.setSegments(0b00000000, 3);
  
 SCAN++;
  if (SCAN > 5) SCAN = 0;

}