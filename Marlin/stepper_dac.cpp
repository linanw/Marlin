/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
  stepper_dac.cpp - To set stepper current via DAC

  Part of Marlin

  Copyright (c) 2016 MarlinFirmware

  Marlin is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Marlin is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Marlin.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Marlin.h"

#if ENABLED(DAC_STEPPER_CURRENT)

  #include "stepper_dac.h"

  bool dac_present = false;
  const uint8_t dac_order[NUM_AXIS] = DAC_STEPPER_ORDER;
  uint8_t dac_channel_pct[ABCD] = DAC_MOTOR_CURRENT_DEFAULT;

  int dac_init() {
    #if PIN_EXISTS(DAC_DISABLE)
      OUT_WRITE(DAC_DISABLE_PIN, LOW);  // set pin low to enable DAC
    #endif

    mcp4728_init();

    if (mcp4728_simpleCommand(RESET)) return -1;

    dac_present = true;

    mcp4728_setVref_all(DAC_STEPPER_VREF);
    mcp4728_setGain_all(DAC_STEPPER_GAIN);

    if (mcp4728_getDrvPct(0) < 1 || mcp4728_getDrvPct(1) < 1 || mcp4728_getDrvPct(2) < 1 || mcp4728_getDrvPct(3) < 1 ) {
      mcp4728_setDrvPct(dac_channel_pct);
      mcp4728_eepromWrite();
    }

    return 0;
  }

  void dac_current_percent(uint8_t channel, float val) {
    if (!dac_present) return;

    NOMORE(val, 100);

    mcp4728_analogWrite(dac_order[channel], val * 0.01 * (DAC_STEPPER_MAX));
    mcp4728_simpleCommand(UPDATE);
  }

  void dac_current_raw(uint8_t channel, uint16_t val) {
    if (!dac_present) return;

    NOMORE(val, DAC_STEPPER_MAX);

    mcp4728_analogWrite(dac_order[channel], val);
    mcp4728_simpleCommand(UPDATE);
  }

  static float dac_perc(int8_t n) { return 100.0 * mcp4728_getValue(dac_order[n]) * (1.0 / (DAC_STEPPER_MAX)); }
  static float dac_amps(int8_t n) { return mcp4728_getDrvPct(dac_order[n]) * (DAC_STEPPER_MAX) * 0.125 * (1.0 / (DAC_STEPPER_SENSE)); }

  #if RBV(R2) || RBV(C2) || RBV(R2_Dual)
  static float robo_dac_percent(AxisEnum axis){ return mcp4728_getDrvPct(dac_order[axis]);}

  //This calculates the Max Motor Current
  static float robo_dac_amps(AxisEnum axis){

    // volts = digital_vref * ( 1 / volts per unit)
    //5 represents 5 volts. I think that is the max VREF for our chip. This is just calculating the actual voltage that we are putting out.
    float volt_ref = mcp4728_getValue(dac_order[axis]) * (1.0 / ( (DAC_STEPPER_MAX) / 5.0)); 

    return  volt_ref / (8.0 * (DAC_STEPPER_SENSE)); //ITripMAX = VREF /( 8×RS ) page 9 of the A4988 Datasheet
  }
  #endif  

  uint8_t dac_current_get_percent(AxisEnum axis) { return mcp4728_getDrvPct(dac_order[axis]); }
  void dac_current_set_percents(const uint8_t pct[ABCD]) {
    LOOP_XYZE(i) dac_channel_pct[i] = pct[dac_order[i]];
    mcp4728_setDrvPct(dac_channel_pct);
  }


  void dac_print_values() {
    if (!dac_present) return;

    #if RBV(R2) || RBV(C2) || RBV(R2_Dual)
      SERIAL_ECHO_START();
      SERIAL_ECHOLNPGM("Stepper current values in % (Max motor Amps):");
      SERIAL_ECHO_START();
      SERIAL_ECHOPAIR(" (A)XY:",  robo_dac_percent(XY_DAC));
      SERIAL_ECHOPAIR(" (",   robo_dac_amps(XY_DAC));
      SERIAL_ECHOPAIR(") (B)Z:", robo_dac_percent(Z_DAC));
      SERIAL_ECHOPAIR(" (",   robo_dac_amps(Z_DAC));
      SERIAL_ECHOPAIR(") (C)E0:", robo_dac_percent(E0_DAC));
      SERIAL_ECHOPAIR(" (",   robo_dac_amps(E0_DAC));
      SERIAL_ECHOPAIR(") (D)E1:", robo_dac_percent(E1_DAC));
      SERIAL_ECHOPAIR(" (",   robo_dac_amps(E1_DAC));
      SERIAL_ECHOLN(")");

    #else

      SERIAL_ECHO_START();
      SERIAL_ECHOLNPGM("Stepper current values in % (Amps):");
      SERIAL_ECHO_START();
      SERIAL_ECHOPAIR(" (A)XY:",  dac_perc(XY_DAC));
      SERIAL_ECHOPAIR(" (",   dac_amps(XY_DAC));
      SERIAL_ECHOPAIR(") (B)Z:", dac_perc(Z_DAC));
      SERIAL_ECHOPAIR(" (",   dac_amps(Z_DAC));
      SERIAL_ECHOPAIR(") (C)E0:", dac_perc(E0_DAC));
      SERIAL_ECHOPAIR(" (",   dac_amps(E0_DAC));
      SERIAL_ECHOPAIR(") (D)E1:", dac_perc(E1_DAC));
      SERIAL_ECHOPAIR(" (",   dac_amps(E1_DAC));
      SERIAL_ECHOLN(")");
     
    #endif
    
  }

  void dac_commit_eeprom() {
    if (!dac_present) return;
    mcp4728_eepromWrite();
  }

#endif // DAC_STEPPER_CURRENT
