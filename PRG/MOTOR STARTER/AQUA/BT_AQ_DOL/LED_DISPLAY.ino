void LED_DISPLAY() {
  switch (SCREEN) {
    case 1:
      switch (TIMER) {
        case 1:
          V_DISP.setDisplayToDecNumber(VOLT, 0, false);
          A_DISP.setDisplayToString("OFF");
          //A_DISP.setDisplayToDecNumber(ET, 0, false);
          break;
        case 2:
          V_DISP.setDisplayToString("odt");
          A_DISP.setDisplayToDecNumber(DELAY_TIME_CTD.CV, 0, false);
          break;
        case 3:
          V_DISP.setDisplayToDecNumber(VOLT, 0, false);
          A_DISP.setDisplayToDecNumber(AMP, 2, false);
          break;
        case 4:

          break;
      }
      break;

    case 2:
      switch (MENU_CTU.CV) {
        case 0:
          if ((AUTO_RTRIG.Q || (SCROL_TIMER_INC.Q == HIGH && SCROL_FAST.Q)) && SET_SS < SET_SS_MAX) SET_SS++;
          else if ((START_RTRIG.Q || (SCROL_TIMER_DEC.Q == HIGH && SCROL_FAST.Q)) && SET_SS > SET_SS_MIN) SET_SS--;
          if (SET_SS > SET_SS_MAX) SET_SS = SET_SS_MAX;
          else if (SET_SS < SET_SS_MIN) SET_SS = SET_SS_MIN;
          V_DISP.setDisplayToString("odS");
          A_DISP.setDisplayToDecNumber(SET_SS, 0, false);
          break;
        case 1:
          if ((AUTO_RTRIG.Q || (SCROL_TIMER_INC.Q == HIGH && SCROL_FAST.Q)) && SET_HV < SET_HV_MAX) SET_HV++;
          else if ((START_RTRIG.Q || (SCROL_TIMER_DEC.Q == HIGH && SCROL_FAST.Q)) && SET_HV > SET_HV_MIN) SET_HV--;
          if (SET_HV > SET_HV_MAX) SET_HV = SET_HV_MAX;
          else if (SET_HV < SET_HV_MIN) SET_HV = SET_HV_MIN;
          V_DISP.setDisplayToString("HUS");
          A_DISP.setDisplayToDecNumber(SET_HV, 0, false);
          break;
        case 2:
          if ((AUTO_RTRIG.Q || (SCROL_TIMER_INC.Q == HIGH && SCROL_FAST.Q)) && SET_LV < SET_LV_MAX) SET_LV++;
          else if ((START_RTRIG.Q || (SCROL_TIMER_DEC.Q == HIGH && SCROL_FAST.Q)) && SET_LV > SET_LV_MIN) SET_LV--;
          if (SET_LV > SET_LV_MAX) SET_LV = SET_LV_MAX;
          else if (SET_LV < SET_LV_MIN) SET_LV = SET_LV_MIN;
          V_DISP.setDisplayToString("LUS");
          A_DISP.setDisplayToDecNumber(SET_LV, 0, false);
          break;
        case 3:
          if (AUTO_RTRIG.Q && SET_HP < SET_HP_MAX) {
            SET_UC = 5;
            if (SET_HP == 1) SET_HP = 2, SET_OC = 40, SET_OC_MAX = 45;
            else if (SET_HP == 2) SET_HP = 3, SET_OC = 60, SET_OC_MAX = 65;
            else if (SET_HP == 3) SET_HP = 5, SET_OC = 90, SET_OC_MAX = 100;
            else if (SET_HP == 5) SET_HP = 7, SET_OC = 140, SET_OC_MAX = 150;
            else if (SET_HP == 7) SET_HP = 10, SET_OC = 190, SET_OC_MAX = 200;
          } else if (START_RTRIG.Q && SET_HP > SET_HP_MIN) {
            SET_UC = 5;
            if (SET_HP == 10) SET_HP = 7, SET_OC = 140, SET_OC_MAX = 150;
            else if (SET_HP == 7) SET_HP = 5, SET_OC = 90, SET_OC_MAX = 100;
            else if (SET_HP == 5) SET_HP = 3, SET_OC = 60, SET_OC_MAX = 65;
            else if (SET_HP == 3) SET_HP = 2, SET_OC = 40, SET_OC_MAX = 45;
            else if (SET_HP == 2) SET_HP = 1, SET_OC = 20, SET_OC_MAX = 25;
          }
          if (SET_HP > SET_HP_MAX) SET_HP = SET_HP_MAX;
          else if (SET_HP < SET_HP_MIN) SET_HP = SET_HP_MIN;
          V_DISP.setDisplayToString("HPS");
          A_DISP.setDisplayToDecNumber(SET_HP, 0, false);
          break;
        case 4:
          if ((AUTO_RTRIG.Q || (SCROL_TIMER_INC.Q == HIGH && SCROL_FAST.Q)) && SET_OC < SET_OC_MAX) SET_OC++;
          else if ((START_RTRIG.Q || (SCROL_TIMER_DEC.Q == HIGH && SCROL_FAST.Q)) && SET_OC > SET_OC_MIN) SET_OC--;
          if (SET_OC > SET_OC_MAX) SET_OC = SET_OC_MAX;
          else if (SET_OC < SET_OC_MIN) SET_OC = SET_OC_MIN;
          V_DISP.setDisplayToString("OCS");
          A_DISP.setDisplayToDecNumber(SET_OC, 2, false);
          break;
        case 5:
          SET_UC_MAX = (SET_OC * 80.0) / 100.0;
          if ((AUTO_RTRIG.Q || (SCROL_TIMER_INC.Q == HIGH && SCROL_FAST.Q)) && SET_UC < SET_UC_MAX) SET_UC++;
          else if ((START_RTRIG.Q || (SCROL_TIMER_DEC.Q == HIGH && SCROL_FAST.Q)) && SET_UC > SET_UC_MIN) SET_UC--;
          if (SET_UC > SET_UC_MAX) SET_UC = SET_UC_MAX;
          else if (SET_UC < SET_UC_MIN) SET_UC = SET_UC_MIN;
          V_DISP.setDisplayToString("UCS");
          A_DISP.setDisplayToDecNumber(SET_UC, 2, false);
          break;
      }
      break;

    case 3:
      module2.clearDisplay();
      switch (FAULT) {
        //case 0: V_DISP.setDisplayToString("   "); module1.setSegments(0b00000000, 3); break;
        case 1:
          V_DISP.setDisplayToString("HUF");
          module1.setSegments(0b00000001, 3);
          break;
        case 2:
          V_DISP.setDisplayToString("LUF");
          module1.setSegments(0b00010000, 3);
          break;
        case 3:
          V_DISP.setDisplayToString("PSF");
          module1.setSegments(0b00001000, 3);
          break;
        case 4:
          V_DISP.setDisplayToString("OCF");
          module1.setSegments(0b00100000, 3);
          break;
        case 5:
          V_DISP.setDisplayToString("UCF");
          module1.setSegments(0b00000100, 3);
          break;
        case 6:
          V_DISP.setDisplayToString("SCF");
          module1.setSegments(0b01000000, 3);
          break;
      }
      break;

    case 4:
      switch (CALIB_CTU.CV) {
        case 0:
          V_DISP.setDisplayToString(HWV, B0100);
          A_DISP.setDisplayToString(SWV, B0100);
          R_LED_FLAG = LOW;
          Y_LED_FLAG = LOW;
          B_LED_FLAG = LOW;
          break;
        case 1:
          V_DISP.setDisplayToString("FdS");
          if (AUTO_RTRIG.Q) FAC_DEF_SET = HIGH;
          else if (START_RTRIG.Q) FAC_DEF_SET = HIGH;
          if (FAC_DEF_SET == HIGH) A_DISP.setDisplayToString("yes");
          else A_DISP.setDisplayToString(" no");
          R_LED_FLAG = LOW;
          Y_LED_FLAG = LOW;
          B_LED_FLAG = LOW;
          break;
        case 2:
          GAIN[1] = (SET_RVGAIN / 1000.000) + 1.000;
          if ((AUTO_RTRIG.Q || (SCROL_TIMER_INC.Q == HIGH && SCROL_FAST.Q)) && SET_RVGAIN < SET_VGAIN_MAX) SET_RVGAIN++;
          else if ((START_RTRIG.Q || (SCROL_TIMER_DEC.Q == HIGH && SCROL_FAST.Q)) && SET_RVGAIN > SET_VGAIN_MIN) SET_RVGAIN--;
          if (SET_RVGAIN > SET_VGAIN_MAX) SET_RVGAIN = SET_VGAIN_MAX;
          else if (SET_RVGAIN < SET_VGAIN_MIN) SET_RVGAIN = SET_VGAIN_MIN;
          V_DISP.setDisplayToDecNumber(RMS[1], 0, false);
          A_DISP.setDisplayToDecNumber(SET_RVGAIN, 0, false);
          R_LED_FLAG = HIGH;
          Y_LED_FLAG = LOW;
          B_LED_FLAG = LOW;
          break;
        case 3:
          GAIN[3] = (SET_YVGAIN / 1000.000) + 1.000;
          if ((AUTO_RTRIG.Q || (SCROL_TIMER_INC.Q == HIGH && SCROL_FAST.Q)) && SET_YVGAIN < SET_VGAIN_MAX) SET_YVGAIN++;
          else if ((START_RTRIG.Q || (SCROL_TIMER_DEC.Q == HIGH && SCROL_FAST.Q)) && SET_YVGAIN > SET_VGAIN_MIN) SET_YVGAIN--;
          if (SET_YVGAIN > SET_VGAIN_MAX) SET_YVGAIN = SET_VGAIN_MAX;
          else if (SET_YVGAIN < SET_VGAIN_MIN) SET_YVGAIN = SET_VGAIN_MIN;
          V_DISP.setDisplayToDecNumber(RMS[3], 0, false);
          A_DISP.setDisplayToDecNumber(SET_YVGAIN, 0, false);
          R_LED_FLAG = LOW;
          Y_LED_FLAG = HIGH;
          B_LED_FLAG = LOW;
          break;
        case 4:
          GAIN[5] = (SET_BVGAIN / 1000.000) + 1.000;
          if ((AUTO_RTRIG.Q || (SCROL_TIMER_INC.Q == HIGH && SCROL_FAST.Q)) && SET_BVGAIN < SET_VGAIN_MAX) SET_BVGAIN++;
          else if ((START_RTRIG.Q || (SCROL_TIMER_DEC.Q == HIGH && SCROL_FAST.Q)) && SET_BVGAIN > SET_VGAIN_MIN) SET_BVGAIN--;
          if (SET_BVGAIN > SET_VGAIN_MAX) SET_BVGAIN = SET_VGAIN_MAX;
          else if (SET_BVGAIN < SET_VGAIN_MIN) SET_BVGAIN = SET_VGAIN_MIN;
          V_DISP.setDisplayToDecNumber(RMS[5], 0, false);
          A_DISP.setDisplayToDecNumber(SET_BVGAIN, 0, false);
          R_LED_FLAG = LOW;
          Y_LED_FLAG = LOW;
          B_LED_FLAG = HIGH;
          break;
        case 5:
          if (SET_HP_MAX <= 5) GAIN[0] = (SET_RAGAIN / 10000.0000);
          else if (SET_HP_MAX == 10) GAIN[0] = (SET_RAGAIN / 1000.000);
          if ((AUTO_RTRIG.Q || (SCROL_TIMER_INC.Q == HIGH && SCROL_FAST.Q)) && SET_RAGAIN < SET_AGAIN_MAX) SET_RAGAIN++;
          else if ((START_RTRIG.Q || (SCROL_TIMER_DEC.Q == HIGH && SCROL_FAST.Q)) && SET_RAGAIN > SET_AGAIN_MIN) SET_RAGAIN--;
          if (SET_RAGAIN > SET_AGAIN_MAX) SET_RAGAIN = SET_AGAIN_MAX;
          else if (SET_RAGAIN < SET_AGAIN_MIN) SET_RAGAIN = SET_AGAIN_MIN;
          V_DISP.setDisplayToDecNumber(RMS[0] * 100, B0100, false);
          A_DISP.setDisplayToDecNumber(SET_RAGAIN, 0, false);
          R_LED_FLAG = HIGH;
          Y_LED_FLAG = LOW;
          B_LED_FLAG = LOW;
          break;
        case 6:
          if (SET_HP_MAX <= 5) GAIN[2] = (SET_YAGAIN / 10000.0000);
          else if (SET_HP_MAX == 10) GAIN[2] = (SET_YAGAIN / 1000.000);
          if ((AUTO_RTRIG.Q || (SCROL_TIMER_INC.Q == HIGH && SCROL_FAST.Q)) && SET_YAGAIN < SET_AGAIN_MAX) SET_YAGAIN++;
          else if ((START_RTRIG.Q || (SCROL_TIMER_DEC.Q == HIGH && SCROL_FAST.Q)) && SET_YAGAIN > SET_AGAIN_MIN) SET_YAGAIN--;
          if (SET_YAGAIN > SET_AGAIN_MAX) SET_YAGAIN = SET_AGAIN_MAX;
          else if (SET_YAGAIN < SET_AGAIN_MIN) SET_YAGAIN = SET_AGAIN_MIN;
          V_DISP.setDisplayToDecNumber(RMS[2] * 100, B0100, false);
          A_DISP.setDisplayToDecNumber(SET_YAGAIN, 0, false);
          R_LED_FLAG = LOW;
          Y_LED_FLAG = HIGH;
          B_LED_FLAG = LOW;
          break;
        case 7:
          if (SET_HP_MAX <= 5) GAIN[4] = (SET_BAGAIN / 10000.0000);
          else if (SET_HP_MAX == 10) GAIN[4] = (SET_BAGAIN / 1000.000);
          if ((AUTO_RTRIG.Q || (SCROL_TIMER_INC.Q == HIGH && SCROL_FAST.Q)) && SET_BAGAIN < SET_AGAIN_MAX) SET_BAGAIN++;
          else if ((START_RTRIG.Q || (SCROL_TIMER_DEC.Q == HIGH && SCROL_FAST.Q)) && SET_BAGAIN > SET_AGAIN_MIN) SET_BAGAIN--;
          if (SET_BAGAIN > SET_AGAIN_MAX) SET_BAGAIN = SET_AGAIN_MAX;
          else if (SET_BAGAIN < SET_AGAIN_MIN) SET_BAGAIN = SET_AGAIN_MIN;
          V_DISP.setDisplayToDecNumber(RMS[4] * 100, B0100, false);
          A_DISP.setDisplayToDecNumber(SET_BAGAIN, 0, false);
          R_LED_FLAG = LOW;
          Y_LED_FLAG = LOW;
          B_LED_FLAG = HIGH;
          break;
        case 8:
          if ((AUTO_RTRIG.Q || (SCROL_TIMER_INC.Q == HIGH && SCROL_FAST.Q)) && SET_UBV < SET_UBV_MAX) SET_UBV++;
          else if ((START_RTRIG.Q || (SCROL_TIMER_DEC.Q == HIGH && SCROL_FAST.Q)) && SET_UBV > SET_UBV_MIN) SET_UBV--;
          if (SET_UBV > SET_UBV_MAX) SET_UBV = SET_UBV_MAX;
          else if (SET_UBV < SET_UBV_MIN) SET_UBV = SET_UBV_MIN;
          V_DISP.setDisplayToString("SPS");
          A_DISP.setDisplayToDecNumber(SET_UBV, 0, false);
          R_LED_FLAG = LOW;
          Y_LED_FLAG = LOW;
          B_LED_FLAG = LOW;
          break;
      }
      break;
  }
}
