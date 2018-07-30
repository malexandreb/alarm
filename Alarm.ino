//program to trigger a nice little light sequence at
//pre-set time given by an external RTC

#include <DS3231.h>
#include <Wire.h>
#include<FastLED.h>

#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB

const int numLed    = 15;
const int ledPin    = 12;
const int buttonPin = 2;

byte increment = 0;

CRGB led[numLed];

DS3231 Clock;

void setup() {
  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, ledPin, COLOR_ORDER>(led, numLed).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(255);

  //setup button
  pinMode(buttonPin, INPUT_PULLUP);

  //setup DS3231 power
  pinMode(A2, OUTPUT); digitalWrite(A2, LOW); //ds3231 gnd
  pinMode(A3, OUTPUT); digitalWrite(A3, HIGH);//ds3231 vcc

  // Start the I2C interface
  Wire.begin();

}

void loop() {
  // put your main code here, to run repeatedly:
  if (buttonPressed()) {
    FastLED.clear(true);
    delay(1000);
    while (!buttonPressed()) {}
    delay(1000);
  }
  
  increment = Clock.getSecond()*4;
  bool a,b;
  for (int i = 0; i < numLed; i++) {
    led[i] = CHSV(Clock.getHour(a, b)*10, Clock.getMinute()*4, increment);
    increment += 5;
  }
  FastLED.show();
  FastLED.delay(1000);
}

boolean buttonPressed() {
  return digitalRead(buttonPin) == LOW;
}
