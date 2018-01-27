/*
 * Driver for the INA19x Current sensor.
 * Written by Matt Pedler NOV 2017.
 */

#ifndef INA19x_h
#define INA19x_h

#include "Arduino.h"
#include "Wire.h"

void setup_INA19x();
float read_INA19x();

#endif
