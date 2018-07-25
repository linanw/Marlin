#ifndef _CAP1188_ROBO_H_
#define _CAP1188_ROBO_H_
#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif
#include <Wire.h>
#include "serial.h"

// The default I2C address
#define CAP1188_I2CADDR 0x29

// Some registers we use
#define CAP1188_SENINPUTSTATUS 0x3
#define CAP1188_MTBLK 0x2A
#define CAP1188_LEDLINK 0x72
#define CAP1188_PRODID 0xFD
#define CAP1188_MANUID 0xFE
#define CAP1188_STANDBYCFG 0x41
#define CAP1188_REV 0xFF
#define CAP1188_MAIN 0x00
#define CAP1188_MAIN_INT 0x01
#define CAP1188_LEDPOL 0x73
#define BITS_OF_NIBBLE 4

//Recalibrate touch sensors
#define CAP1188_RECALIBRATE 0x26
#define CAP1188_SENSITIBITY 0x1F
#define CAP1188_LED 0x74
#define CAP1188_LED_LINK 0x72

class CAP1188_Robo
{
public:  
  // Hardware I2C
  CAP1188_Robo(int8_t resetpin = -1);

  boolean begin(uint8_t i2caddr = CAP1188_I2CADDR);
  uint8_t readRegister(uint8_t reg);
  void writeRegister(uint8_t reg, uint8_t value);
  uint8_t touched(void);
  void LEDpolarity(uint8_t x);

  // Added by Robo
  void resetCAP();
  int8_t read_Delta(int sensor);
  void readBytes(uint8_t reg, int length, uint8_t *data);
  void readByte(uint8_t reg, uint8_t *data);
  void writeBytes(uint8_t reg, int length, uint8_t* data);
  void writeByte(uint8_t reg, uint8_t* data);

  //calibration
  void recalibrate_touch();
  void adjust_Sensitivity(int delta_shift, int base_shift);

  //helpers
  void print_byte(uint8_t bytenum);
  void blink_led();
  void led_on();
  void led_off();

  //Variables
  const int CAP1188_DELTA[8] = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17};

 private:
  boolean _i2c;
  int8_t _i2caddr, _resetpin;
};

#endif