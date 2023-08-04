//wip
#define BUTTON_PIN 3

const int buzzerPin = 6;//13; //6
const int volume    = 3; // 0 to 255

int buzzerState = LOW;

unsigned long buttonLastPressedMillis = 0; // will store last time LED was updated

const long intervalMillis = 1000;

//wip
byte currentButtonState = 0;
byte previousButtonState = 0;

void setup() {
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(115200);

  //wip
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  unsigned long currentMillis = millis();

  if(
    currentMillis > intervalMillis && // so it doesn't activate before the first interval has elapsed
    (buttonLastPressedMillis + intervalMillis) > currentMillis) {
    buzzerState = volume;
  } else {

    //wip
    currentButtonState = digitalRead(BUTTON_PIN);


    if (currentButtonState != previousButtonState) {
      // button state has changed
      if (currentButtonState == LOW) {
        Serial.println("pressed");

        buttonLastPressedMillis = currentMillis;
      } else {
        // Serial.println("off");
      }

      // Delay a little bit to avoid bouncing
      delay(50);
    }

    // save the current state as the last state, for next time through the loop
    previousButtonState = currentButtonState;
  
    buzzerState = LOW;
  }

  analogWrite(buzzerPin, buzzerState);
}
