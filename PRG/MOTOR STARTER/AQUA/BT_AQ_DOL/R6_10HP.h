#ifndef _R6_10HP_H
#define _R6_10HP_H

String HWV = "H06";

TM1650 module1(4, 3, 3);    // data, clock, digits
TM1650 module2(5, 3, 3);    // data, clock, digits
TM16xxDisplay V_DISP(&module1, 3);    // TM16xx object, 8 digits
TM16xxDisplay A_DISP(&module2, 3);    // TM16xx object, 8 digits

#define MAN_PB_PIN 11
#define STOP_PB_PIN 9
#define AUTO_PB_PIN 7

#define LED_MAN 12
#define LED_STOP 10
#define LED_AUTO 8

#define RELAY 6
#define BUZZER 2

#define SCF_PIN 16

#define R_LED 23
#define Y_LED 24
#define B_LED 25

//  AmplitudePins[6] =   RA  RV  YA  YV  BA  BV
int AMPLITUDE_PIN[8] = { A2, A5, A1, A6, A3, A7, A5, A5 };

bool LED = HIGH;
int SET_AGAIN_MIN = 100;
int SET_AGAIN_MAX = 200;
int SET_OC_MAX = 200;
int SET_HP_MAX = 10;
double CT_MAX_RMS_AMP = 35.0;

#endif 
