#include <I2CMotors_asukiaaa.h>

#define MOTORS_ADDRESS 0b00000000
I2CMotors_asukiaaa motors(MOTORS_ADDRESS);

void setup() {
  Serial.begin(115200);
  motors.begin();
}

void loop() {
  int result = motors.writeSpeed(0, 0);
  if (result != 0) {
    Serial.println("Cannot communicate with motors");
    delay(5000);
    return;
  }

  // Go forward
  motors.writeSpeed(200, 200);
  delay(500);
  motors.writeBrake();
  delay(1000);

  // Go back
  motors.writeSpeed(-200, -200);
  delay(500);
  motors.writeBrake();
  delay(1000);

  // Turn left
  motors.writeSpeed(-200, 200);
  delay(500);
  motors.writeBrake();
  delay(1000);

  // Turn right
  motors.writeSpeed(200, -200);
  delay(500);
  motors.writeBrake();
  delay(1000);
}
