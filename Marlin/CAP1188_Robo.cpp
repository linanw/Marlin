/*
* @Author: matt
* @Date:   2018-07-23 11:31:50
* @Last Modified by:   Matt Pedler
* @Last Modified time: 2018-07-25 09:46:43
*/

/*************************************************** 
  This is a library for the CAP1188 I2C/SPI 8-chan Capacitive Sensor

  Designed specifically to work with the CAP1188 sensor from Adafruit
  ----> https://www.adafruit.com/products/1602

  These sensors use I2C/SPI to communicate, 2+ pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include "CAP1188_Robo.h"


uint8_t mySPCR, SPCRback;

CAP1188_Robo::CAP1188_Robo(int8_t resetpin) {
  // I2C
  _resetpin = resetpin;
  _i2c = true;
  Wire.begin();

}


boolean CAP1188_Robo::begin(uint8_t i2caddr) {
  resetCAP(); // reset before trying to connect
  _i2caddr = i2caddr;

  // Check the Configuration Address to see if it is there
  Wire.beginTransmission(CAP_ADDR);
  byte error = Wire.endTransmission();

  if (error != 0){
    SERIAL_PROTOCOL("Supplied Address: ");
    SERIAL_PROTOCOL(CAP_ADDR);
    SERIAL_PROTOCOL(" does not exist on the I2C bus");
    SERIAL_EOL();
  }

  readRegister(CAP1188_PRODID);  

  if ( (readRegister(CAP1188_PRODID) != 0x50) ||
       (readRegister(CAP1188_MANUID) != 0x5D) ||
       (readRegister(CAP1188_REV) != 0x83)) {
    return false;
  }
  // allow multiple touches
  writeRegister(CAP1188_MTBLK, 0); 
  // Have LEDs follow touches
  writeRegister(CAP1188_LEDLINK, 0x80);
  // speed up a bit
  writeRegister(CAP1188_STANDBYCFG, 0x30);

  blink_led();

  return true;
}

void CAP1188_Robo::resetCAP(){
    if (_resetpin != -1) {
      pinMode(_resetpin, OUTPUT);
      digitalWrite(_resetpin, LOW);
      delay(100);
      digitalWrite(_resetpin, HIGH);
      delay(100);
      digitalWrite(_resetpin, LOW);
      delay(100);
    }
}

uint8_t  CAP1188_Robo::touched(void) {
  uint8_t t = readRegister(CAP1188_SENINPUTSTATUS);
  if (t) {
    writeRegister(CAP1188_MAIN, readRegister(CAP1188_MAIN) & ~CAP1188_MAIN_INT);
  }
  return t;
}

void CAP1188_Robo::LEDpolarity(uint8_t x) {
  writeRegister(CAP1188_LEDPOL, x);
}

int8_t CAP1188_Robo::read_Delta(int sensor) {
  uint8_t data;
  readByte(CAP1188_DELTA[sensor], data);

  return (int8_t)data;
}

void CAP1188_Robo::recalibrate_touch() {

  // Write 1 to all data stores to recalibrate all 
  uint8_t calibrate[1] = {0xFF};

  writeByte(CAP1188_RECALIBRATE, calibrate);
  // Bus wont be usable for 600 ms

}
  
// for this one the delta shift(3bit) and base shift(4bit) are binary numbers
// that we need to shift into a single 7bit number. I will have the user
// decide at what level to shift then turn that into the number we want

/*
  delta shift
  128x 000
  64x  001
  32x  010 (Default)
  16x  011
  8x   100
  4x   101
  2x   110
  1x   111

  base shift
  1x   0000
  2x   0001
  4x   0010
  8x   0011
  16x  0100
  32x  0101
  64x  0110
  128x 0111
  256x 1000 (Default)
*/
void CAP1188_Robo::adjust_Sensitivity(int delta_shift, int base_shift){
  uint8_t sensitivity_shift;
  uint8_t delta_shift_out;
  uint8_t base_shift_out;

  switch(delta_shift){
    case 128: delta_shift_out = 0; break;
    case 64:  delta_shift_out = 1; break;
    case 32:  delta_shift_out = 2; break;
    case 16:  delta_shift_out = 3; break;
    case 8:   delta_shift_out = 4; break;
    case 4:   delta_shift_out = 5; break;
    case 2:   delta_shift_out = 6; break;
    case 1:   delta_shift_out = 7; break;
    default: SERIAL_PROTOCOL("Delta Shift number is not valid\n"); break;


  }

  switch(base_shift){
    case 1:   base_shift_out = 0; break;
    case 2:   base_shift_out = 1; break;
    case 4:   base_shift_out = 2; break;
    case 8:   base_shift_out = 3; break;
    case 16:  base_shift_out = 4; break;
    case 32:  base_shift_out = 5; break;
    case 64:  base_shift_out = 6; break;
    case 128: base_shift_out = 7; break;
    case 256: base_shift_out = 8; break;
    default: SERIAL_PROTOCOL("Base Shift_out number is not valid\n"); break;
  }

  // This block will add the delta shift to the base shift to create 8 bytes
  delta_shift_out = delta_shift_out << 5;
  base_shift_out = base_shift_out << 1;
  sensitivity_shift = delta_shift_out | base_shift_out;

  writeByte(CAP1188_SENSITIBITY, sensitivity_shift);

}

void CAP1188_Robo::blink_led(){
 uint8_t led_on = 0x01;
 uint8_t led_off = 0x00;
 writeByte(CAP1188_LED, led_on);
 delay(100);
 writeByte(CAP1188_LED, led_off);
 delay(100);
}

void CAP1188_Robo::led_on(){
  uint8_t led_on = 0x01;
  writeByte(CAP1188_LED, led_on);
}

void CAP1188_Robo::led_off(){
  uint8_t led_off = 0x00;
  writeByte(CAP1188_LED, led_off);
}


void CAP1188_Robo::print_byte(uint8_t bytenum){
  for (int x = 8; x>0 ; x--){
    SERIAL_PROTOCOL(bitRead(bytenum, x));
  }
}
/*********************************************************************/

/**************************************************************************/
/*!
    @brief  Abstract away platform differences in Arduino wire library
*/
/**************************************************************************/
static uint8_t i2cread(void) {
  #if ARDUINO >= 100
  return Wire.read();
  #else
  return Wire.receive();
  #endif
}

/**************************************************************************/
/*!
    @brief  Abstract away platform differences in Arduino wire library
*/
/**************************************************************************/
static void i2cwrite(uint8_t x) {
  #if ARDUINO >= 100
  Wire.write((uint8_t)x);
  #else
  Wire.send(x);
  #endif
}

/**************************************************************************/
/*!
    @brief  Reads 8-bits from the specified register
*/
/**************************************************************************/

uint8_t CAP1188_Robo::readRegister(uint8_t reg) {
  
  Wire.beginTransmission(_i2caddr);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(_i2caddr, 1);
  return (i2cread());
  
}

void CAP1188_Robo::readBytes(uint8_t reg, int length, uint8_t *data) {
  
  Wire.beginTransmission(_i2caddr);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(_i2caddr, length);
  data = Wire.read();    
  
}

void CAP1188_Robo::readByte(uint8_t reg, uint8_t *data){
   return readBytes(reg, (uint8_t)1, data);
}

void CAP1188_Robo::writeBytes(uint8_t reg, int length, uint8_t* data){
  uint8_t buf[128];

  buf[0] = reg ; // Add Register to write
  memcpy(buf+1, data, length); // copy bytes into buf

  if(_i2c){
    Wire.beginTransmission(_i2caddr);
    Wire.write(buf, length+1);
    Wire.endTransmission();
  }

}

void CAP1188_Robo::writeByte(uint8_t reg, uint8_t* data){
  writeBytes(reg, 1, data);
}

/**************************************************************************/
/*!
    @brief  Writes 8-bits to the specified destination register
*/
/**************************************************************************/
void CAP1188_Robo::writeRegister(uint8_t reg, uint8_t value) {
  if (_i2c) {
    Wire.beginTransmission(_i2caddr);
    i2cwrite((uint8_t)reg);
    i2cwrite((uint8_t)(value));
    Wire.endTransmission();
  }
}
