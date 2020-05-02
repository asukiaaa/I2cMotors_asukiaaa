#include <I2cMotors_asukiaaa.h>

#define MOTOR_A_1 4
#define MOTOR_A_2 5
#define MOTOR_A_PWM 9
#define MOTOR_B_1 6
#define MOTOR_B_2 7
#define MOTOR_B_PWM 10
#define MOTOR_STDBY 8

#define ACTIVE_LIFE_MS 1000

#define REGISTER_LEN 3
uint8_t registers[REGISTER_LEN];
uint8_t registerIndex;
unsigned long lastReceivedAt = 0;
bool isActive = false;

void onReceive(int howMany) {
  uint8_t receivedLen = 0;
  while (0 < Wire.available()) {
    uint8_t v = Wire.read();
    if (receivedLen == 0) {
      registerIndex = v;
    } else {
      if (registerIndex < REGISTER_LEN) {
        registers[registerIndex++] = v;
      }
    }
    ++receivedLen;
  }
  lastReceivedAt = millis();
  isActive = true;
}

void onRequest() {
  if (registerIndex < REGISTER_LEN) {
    Wire.write(&registers[registerIndex], REGISTER_LEN - registerIndex);
  } else {
    Wire.write(0);
  }
}

void setup() {
  pinMode(MOTOR_A_1, OUTPUT);
  pinMode(MOTOR_A_2, OUTPUT);
  pinMode(MOTOR_B_1, OUTPUT);
  pinMode(MOTOR_B_2, OUTPUT);
  pinMode(MOTOR_STDBY, OUTPUT);
  pinMode(MOTOR_A_PWM, OUTPUT);
  pinMode(MOTOR_B_PWM, OUTPUT);

  for (int i = 0; i < 8; ++i) {
    pinMode(addrPins[i], INPUT_PULLUP);
  }

  uint8_t address = 0;
  for (int i = 0; i < 8; ++i) {
    uint8_t pin = addrPins[7-i];
    address <<= 1;
    if (digitalRead(pin) == LOW) {
      address |= 1;
    }
  }

  Wire.begin(address);

  // Disable pullup by internal resisters
  pinMode(SDA, INPUT);
  pinMode(SCL, INPUT);

  Wire.onReceive(onReceive);
  Wire.onRequest(onRequest);
}

void setBreak() {
  digitalWrite(MOTOR_A_1, HIGH);
  digitalWrite(MOTOR_A_2, HIGH);
  analogWrite(MOTOR_A_PWM, 0xff);
  digitalWrite(MOTOR_B_1, HIGH);
  digitalWrite(MOTOR_B_2, HIGH);
  analogWrite(MOTOR_B_PWM, 0xff);
}

void setMotorsSpeed(uint8_t config, uint8_t l, uint8_t r) {
  bool lDir = (config & 0b01) != 0;
  bool rDir = (config & 0b10) != 0;
  digitalWrite(MOTOR_A_1, l != 0 && lDir);
  digitalWrite(MOTOR_A_2, l != 0 && !lDir);
  analogWrite(MOTOR_A_PWM, l);
  digitalWrite(MOTOR_B_1, r != 0 && rDir);
  digitalWrite(MOTOR_B_2, r != 0 && !rDir);
  analogWrite(MOTOR_B_PWM, r);
}

void handleRegisterValues() {
  uint8_t config = registers[0];
  uint8_t l = registers[1];
  uint8_t r = registers[2];
  if ((config & 0b100) != 0) {
    setBreak();
  } else {
    setMotorsSpeed(config, l, r);
  }
}

void loop() {
  if (isActive) {
    if (millis() - lastReceivedAt > ACTIVE_LIFE_MS) {
      digitalWrite(MOTOR_STDBY, LOW);
      isActive = false;
    } else {
      handleRegisterValues();
      digitalWrite(MOTOR_STDBY, HIGH);
    }
  }
  delay(2);
}
