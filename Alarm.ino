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

int alarmH = 5;
int alarmM = 27;
int divider = 1;

byte increment = 0;
bool a, b;

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
  
  FastLED.clear(true);
  
  Serial.begin(9600);
  Serial.print(Clock.getHour(a, b)); Serial.print(":"); Serial.println(Clock.getMinute());
  Serial.end();
  //alarmSequence();
}

void loop() {
  //wait for time to start
  int hour = Clock.getHour(a, b);
  int minute = Clock.getMinute();
  if (hour == alarmH && minute == alarmM) {
    alarmSequence();
    while (hour == alarmH && minute == alarmM) {
      minute = Clock.getMinute();
      delay(10000);
    }
  }
}

void alarmSequence() {

  //turn all led a pale blue 1.5min
  for (int i = 0 ; i < 15; i++) {
    led[i] = CRGB(0, 0, 1);
    FastLED.show();
    FastLED.delay(10000 / divider);
  }

  //then make it red 2min
  for (int j = 1; j < 10; j++) {
    for (int i = 0 ; i < 15; i++) {
      led[i] = CRGB(j << 1, 0, 2 - min(2, j));
      FastLED.show();
      FastLED.delay(800 / divider);
    }
  }

  //then gold 2min
  for (int j = 1; j < 10; j++) {
    for (int i = 0 ; i < 15; i++) {
      led[i] = CRGB(20 + j << 1, j << 2, 0);
      FastLED.show();
      FastLED.delay(800 / divider);
    }
  }

  //then white 2min
  for (int j = 1; j < 40; j++) {
    for (int i = 0 ; i < 15; i++) {
      led[i] = CRGB(40 + j, 40 + j, j << 1);
      FastLED.show();
      FastLED.delay(200 / divider);
    }
  }

  //finally, rainbow! 5min
  for (int j = 1; j < 3000; j++) {
    for (int i = 0 ; i < 15; i++) {
      led[i] = ColorFromPalette( RainbowColors_p, j+i*5, 128, LINEARBLEND);;
      FastLED.show();
    }
    FastLED.delay(100 / divider);
  }
  
  FastLED.clear(true);
  

/*
  // put your main code here, to run repeatedly:
  if (buttonPressed()) {
    FastLED.clear(true);
    delay(1000);
    while (!buttonPressed()) {}
    delay(1000);
  }

  // displayTime();

  FastLED.show();
  FastLED.delay(1000);
  */
}

boolean buttonPressed() {
  return digitalRead(buttonPin) == LOW;
}

//sadly can't display full min and second over 5 bits ...
void displayTime() {
  FastLED.clear(true);
  CRGB color = ColorFromPalette( RainbowColors_p, Clock.getDate(), 255, LINEARBLEND);
  byte data[3];
  data[2] = Clock.getHour(a, b);
  data[1] = Clock.getMinute();
  data[0] = Clock.getSecond();
  for (int j = 0; j < 3; j++) {
    for (int i = 0; i < 5; i++) {
      if (data[j] & 0x01) {
        led[i + j * 5] = color;
      }
      data[j] = data[j] >> 1;
    }
  }
}
