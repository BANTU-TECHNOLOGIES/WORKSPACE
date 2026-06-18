#ifndef _R1_H
#define _R1_H

String HWV = "H01";

TM1650 module1(1, 0, 3);    // data, clock, digits
TM1650 module2(3, 2, 3);    // data, clock, digits
TM16xxDisplay V_DISP(&module1, 3);    // TM16xx object, 8 digits
TM16xxDisplay A_DISP(&module2, 3);    // TM16xx object, 8 digits

#define B_LED 4
#define Y_LED 5
#define R_LED 6
#define LED_AUTO 7
#define AUTO_PB_PIN 8
#define STOP_PB_PIN 9
#define LED_STOP 10
#define MAN_PB_PIN 11
#define LED_MAN 12
#define RELAY 20
#define BUZZER 21
#define SCF_PIN 13
//  AmplitudePins[6] =   RA  RV  YA  YV  BA  BV
int AMPLITUDE_PIN[8] = { A2, A6, A3, A5, A7, A4, A6, A6 }; 

bool LED = LOW;
int SET_AGAIN_MIN = 600;
int SET_AGAIN_MAX = 800;
int SET_OC_MAX = 65;
int SET_HP_MAX = 3;
double CT_MAX_RMS_AMP = 16.0;

#endif 
