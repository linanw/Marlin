/*
 * Driver for the INA19x Current sensor.
 * Written by Matt Pedler NOV 2017.
 */
#include "MarlinConfig.h"

#ifdef INA19x_Input_pin // If the pin is even defined

#include "INA19x.h"

void setup_INA19x(){
    //Setup the pins
    pinMode(INA19x_Input_pin, INPUT);

}

float read_INA19x(){
    return ((float)analogRead(INA19x_Input_pin)) / (20* 0.033);
}


#endif