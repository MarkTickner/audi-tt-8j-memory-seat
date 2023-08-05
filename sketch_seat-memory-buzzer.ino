#include <CAN.h>

const int buzzerPin = 6;
const int volume    = 5; // 0 to 255

int buzzerState = LOW;

unsigned long currentMillis = 0;

unsigned long beepEndMillis = 0;
const int beepDurationMillis = 300;

enum ButtonState {
  NONE_PRESSED, MEMORY_1, MEMORY_2, MEMORY_3
};

ButtonState currentButtonState = NONE_PRESSED;

void setup() {
  pinMode(buzzerPin, OUTPUT);

  Serial.begin(115200);

  while (!Serial);

  Serial.println("Seat memory stored confirmation");

  const long convenienceBitrate = 100E3;
  if (!CAN.begin(convenienceBitrate)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }

  const int memorySeatAddress = 0x511;
  CAN.filter(memorySeatAddress);

  CAN.onReceive(onReceive);
}

void loop() {
  currentMillis = millis();

  if (currentButtonState != NONE_PRESSED) {
    Serial.println(currentButtonState);

    buzzerState = volume;
    beepEndMillis = currentMillis + beepDurationMillis;

    currentButtonState = NONE_PRESSED;
  }

  if (beepEndMillis <= currentMillis) {
    buzzerState = LOW;
  }

  analogWrite(buzzerPin, buzzerState);
}

void onReceive(int packetSize) {
  uint8_t packet[8];
  uint8_t len = 0;

  uint16_t id = CAN.packetId();

  if (id == 0x511) {
    while (CAN.available()) {
      packet[len++] = CAN.read();
    }

    switch (packet[0]) {
      case 0xA1:
        currentButtonState = MEMORY_1;
        break;
      case 0xA2:
        currentButtonState = MEMORY_2;
        break;
      case 0xA4:
        currentButtonState = MEMORY_3;
        break;
    }
  }

}
