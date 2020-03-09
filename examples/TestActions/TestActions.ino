#include <I2CMotors_asukiaaa.h>

#define MOTORS_ADDRESS 0b00001000
I2CMotors_asukiaaa motors(MOTORS_ADDRESS);

void setup() {
  Serial.begin(115200);
  motors.begin();
}

void readAndPrintSpeed() {
  int l, r;
  int result = motors.readSpeed(&l, &r);
  if (result != 0) {
    Serial.println("Cannot read speed because error " + String(result));
  } else {
    Serial.println("speeds: " + String(l) + " " + String(r));
  }
}

void writeSpeedWithCheckingResult(int l, int r) {
  int result = motors.writeSpeed(l, r);
  if (result != 0) {
    Serial.println("Cannot write speed because error " + String(result));
  }
}

void loop() {
  int result = motors.writeSpeed(0, 0);
  if (result != 0) {
    Serial.println("Cannot communicate with motors at " + String(millis()));
    delay(5000);
    return;
  }

  Serial.println("left wheel forward");
  writeSpeedWithCheckingResult(100, 0);
  readAndPrintSpeed();
  delay(500);
  motors.writeBrake();
  delay(2000);

  Serial.println("left wheel back");
  writeSpeedWithCheckingResult(-100, 0);
  readAndPrintSpeed();
  delay(500);
  motors.writeBrake();
  delay(2000);

  Serial.println("right wheel forward");
  writeSpeedWithCheckingResult(0, 100);
  readAndPrintSpeed();
  delay(500);
  motors.writeBrake();
  delay(2000);

  Serial.println("right wheel back");
  writeSpeedWithCheckingResult(0, -100);
  readAndPrintSpeed();
  delay(500);
  motors.writeBrake();
  delay(2000);

  // Go forward
  Serial.println("go forward");
  writeSpeedWithCheckingResult(255, 255);
  readAndPrintSpeed();
  delay(500);
  motors.writeBrake();
  delay(2000);

  // Go back
  Serial.println("go back");
  writeSpeedWithCheckingResult(-255, -255);
  readAndPrintSpeed();
  delay(500);
  motors.writeBrake();
  delay(2000);
}
