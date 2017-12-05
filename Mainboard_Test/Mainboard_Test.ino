/** Robo Mainboard 2.1.8 test code
*/

#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE0_PIN      38
#define X_MIN_PIN          3
#ifndef X_MAX_PIN
#define X_MAX_PIN          2
#endif

#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE0_PIN      56
#define Y_MIN_PIN          15
#define Y_MAX_PIN          14

#define Z_STEP_PIN         46
#define Z_DIR_PIN          48
#define Z_ENABLE0_PIN      62
#define Z_MIN_PIN          19
#define Z_MAX_PIN          18

#define E0_STEP_PIN        26
#define E0_DIR_PIN         28
#define E0_ENABLE0_PIN     24

#define E1_STEP_PIN        36
#define E1_DIR_PIN         34
#define E1_ENABLE0_PIN     30

#define SDPOWER            -1
#define SDSS               53
#define PS_ON_PIN          -1

#define FILWIDTH_PIN       -1 // ANALOG NUMBERING NOT USING IN R2 or C2

#define LED_PIN            -1
#define FAN1_PIN           6 // Pin used for the fan to cool controller
#define FAN_PIN            7 // PWM Fans for print cooling
//#define E0_AUTO_FAN_PIN  11 // Ext0 and Ext1 fan as defined in Configuration_adv.h

#define HEATER_0_PIN       10   // EXTRUDER 1
#define HEATER_1_PIN       9   // EXTRUDER 2 (FAN On Sprinter)

#define TEMP_0_PIN         13   // ANALOG NUMBERING default 13
#define TEMP_1_PIN         15   // ANALOG NUMBERING default 15

#define HEATER_BED_PIN     8    // BED

#define TEMP_BED_PIN       14   // ANALOG NUMBERING default 14

// I2C based DAC
#define DAC_STEPPER_CURRENT

// Channels available for DAC, For RoboMainboard 2.1.8 and up there are 4
#define DAC_STEPPER_ORDER   { 3, 2, 1, 0}
#define DAC_STEPPER_SENSE   0.05 // sense resistors on RoboMainboard A4988 stepper chips are .05 ohms
#define DAC_STEPPER_ADDRESS 0
#define DAC_STEPPER_MAX     4096 // was 5000 but max allowable value is actually 4096
#define DAC_STEPPER_VREF    1 // internal Vref, gain 2x = 4.096V
#define DAC_STEPPER_GAIN    0 // value of 1 here sets gain of 2
#define DAC_OR_ADDRESS      0x00

void setup() {
  pinMode(FAN_PIN , OUTPUT);
  pinMode(HEATER_0_PIN , OUTPUT);
  pinMode(HEATER_1_PIN , OUTPUT);
  pinMode(HEATER_BED_PIN , OUTPUT);
  pinMode(LED_PIN  , OUTPUT);

  pinMode(X_STEP_PIN  , OUTPUT);
  pinMode(X_DIR_PIN    , OUTPUT);
  pinMode(X_ENABLE0_PIN    , OUTPUT);

  pinMode(Y_STEP_PIN  , OUTPUT);
  pinMode(Y_DIR_PIN    , OUTPUT);
  pinMode(Y_ENABLE0_PIN    , OUTPUT);

  pinMode(Z_STEP_PIN  , OUTPUT);
  pinMode(Z_DIR_PIN    , OUTPUT);
  pinMode(Z_ENABLE0_PIN    , OUTPUT);

  pinMode(E0_STEP_PIN  , OUTPUT);
  pinMode(E0_DIR_PIN    , OUTPUT);
  pinMode(E0_ENABLE0_PIN    , OUTPUT);

  pinMode(E1_STEP_PIN  , OUTPUT);
  pinMode(E1_DIR_PIN    , OUTPUT);
  pinMode(E1_ENABLE0_PIN    , OUTPUT);

    digitalWrite(X_ENABLE0_PIN    , LOW);
    digitalWrite(Y_ENABLE0_PIN    , LOW);
    digitalWrite(Z_ENABLE0_PIN    , LOW);
    digitalWrite(E0_ENABLE0_PIN    , LOW);
    digitalWrite(E1_ENABLE0_PIN    , LOW);
}





void loop () {

  if (millis() %1000 <500)
    digitalWrite(LED_PIN, HIGH);
  else
   digitalWrite(LED_PIN, LOW);

  if (millis() %1000 <300) {
    digitalWrite(HEATER_0_PIN, HIGH);
    digitalWrite(HEATER_1_PIN, LOW);
    digitalWrite(HEATER_BED_PIN, HIGH);
    digitalWrite(FAN_PIN, LOW);
  } else if (millis() %1000 <600) {
    digitalWrite(HEATER_0_PIN, LOW);
    digitalWrite(HEATER_1_PIN, HIGH);
    digitalWrite(HEATER_BED_PIN, LOW);
    digitalWrite(FAN_PIN, LOW);
  } else  {
    digitalWrite(HEATER_0_PIN, LOW);
    digitalWrite(HEATER_1_PIN, LOW);
    digitalWrite(HEATER_BED_PIN, LOW);
    digitalWrite(FAN_PIN, HIGH);
  }

  if (millis() %10000 <5000) {
    digitalWrite(X_DIR_PIN    , HIGH);
    digitalWrite(Y_DIR_PIN    , HIGH);
    digitalWrite(Z_DIR_PIN    , HIGH);
    digitalWrite(E0_DIR_PIN    , HIGH);
    digitalWrite(E1_DIR_PIN    , HIGH);
  }
  else {
    digitalWrite(X_DIR_PIN    , LOW);
    digitalWrite(Y_DIR_PIN    , LOW);
    digitalWrite(Z_DIR_PIN    , LOW);
    digitalWrite(E0_DIR_PIN    , LOW);
    digitalWrite(E1_DIR_PIN    , LOW);
  }


    digitalWrite(X_STEP_PIN    , HIGH);
    digitalWrite(Y_STEP_PIN    , HIGH);
    digitalWrite(Z_STEP_PIN    , HIGH);
    digitalWrite(E0_STEP_PIN    , HIGH);
    digitalWrite(E1_STEP_PIN    , HIGH);
  delay(1);

    digitalWrite(X_STEP_PIN    , LOW);
    digitalWrite(Y_STEP_PIN    , LOW);
    digitalWrite(Z_STEP_PIN    , LOW);
    digitalWrite(E0_STEP_PIN    , LOW);
    digitalWrite(E1_STEP_PIN    , LOW);

}
