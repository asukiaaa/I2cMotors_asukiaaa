#ifndef I2C_MOTORS_ASUKIAAA_H
#define I2C_MOTORS_ASUKIAAA_H

#include <Wire.h>

#define I2C_MOTORS_ASUKIAAA_ADDRESS_CONFIG 0 // bit1: l direction, bit2: r direction, bit3: brake
#define I2C_MOTORS_ASUKIAAA_ADDRESS_SPEED_L 1
#define I2C_MOTORS_ASUKIAAA_ADDRESS_SPEED_R 2
#define I2C_MOTORS_ASUKIAAA_MAX_SPEED 0xff

#define I2C_MOTORS_ASUKIAAA_ERROR_READ_WRONG_LEN 5

// #define DEBUG_PRINT

#ifdef DEBUG_PRINT
#include <Arduino.h>
#endif

class I2cMotors_asukiaaa {
 public:
  I2cMotors_asukiaaa(uint8_t address) {
    wire = NULL;
    this->address = address;
    reverseLeft = false;
    reverseRight = false;
  };

  void setWire(TwoWire* wire);
  void begin();
  bool reverseLeft;
  bool reverseRight;
  int writeSpeed(int speedL, int speedR);
  int readSpeed(int* speedL, int* speedR);
  int writeBrake();

 private:
  TwoWire* wire;
  uint8_t address;

  static int i2cReadBytes(uint8_t address);
  static int i2cWriteBytes();
};

#endif
