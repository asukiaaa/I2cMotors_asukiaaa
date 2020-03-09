#include <I2CMotors_asukiaaa.h>

void I2CMotors_asukiaaa::setWire(TwoWire* wire) {
  this->wire = wire;
}

void I2CMotors_asukiaaa::begin() {
  if (this->wire == NULL) {
    Wire.begin();
    this->wire = &Wire;
  }
}

int I2CMotors_asukiaaa::writeSpeed(int speedL, int speedR) {
  uint8_t config = 0;
  if (speedL < 0) {
    speedL = - speedL;
  } else {
    config ^= 0b001;
  }
  if (speedR < 0) {
    speedR = - speedR;
  } else {
    config ^= 0b010;
  }
  uint8_t l = speedL > 0xff ? 0xff : speedL;
  uint8_t r = speedR > 0xff ? 0xff : speedR;
  wire->beginTransmission(address);
  wire->write(I2C_MOTORS_ASUKIAAA_ADDRESS_CONFIG);
  wire->write(config);
  wire->write(l);
  wire->write(r);
  return wire->endTransmission();
}

int I2CMotors_asukiaaa::readSpeed(int* speedL, int* speedR) {
  wire->beginTransmission(address);
  wire->write(I2C_MOTORS_ASUKIAAA_ADDRESS_CONFIG);
  int result = wire->endTransmission();
  if (result != 0) return result;
  static const int buffLen = 3;
  uint8_t buff[buffLen];
  uint8_t buffIndex = 0;
  wire->requestFrom(address, buffLen);
  int receivedLen = 0;

  while (wire->available() > 0) {
    uint8_t d = wire->read();
    if (buffIndex < buffLen) {
      buff[buffIndex] = d;
      ++buffIndex;
    }
#ifdef DEBUG_PRINT
  Serial.print(String(d) + " ");
#endif
    ++receivedLen;
  }

#ifdef DEBUG_PRINT
  Serial.println("receivedLen " + String(receivedLen));
#endif

  if (receivedLen != buffLen) {
    return I2C_MOTORS_ASUKIAAA_ERROR_READ_WRONG_LEN;
  }

  uint8_t config = buff[0];
  int l = buff[1];
  int r = buff[2];
  if ((config & 0b01) == 0) {
    l = -l;
  }
  if ((config & 0b10) == 0) {
    r = -r;
  }
  *speedL = l;
  *speedR = r;

  return 0;
}

int I2CMotors_asukiaaa::writeBrake() {
  wire->beginTransmission(address);
  wire->write(I2C_MOTORS_ASUKIAAA_ADDRESS_CONFIG);
  wire->write(0b100);
  return wire->endTransmission();
}
