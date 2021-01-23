#include <FastLED.h>
#include "carstate.h"

const int run_duration = 5000;
const int maxBrightness = 100; // up to 255. More bright, more amps
const int runningBrightness = 255; // up to 255. More bright, more amps
const int led_cylcle_time = 100; //change led color every this many ms
const int LED_PIN = 7;
const int NUM_LEDS = 16;
CRGB leds[NUM_LEDS];

const int EN1 = 5; // Enable Pin for motor 1
const int IN1 = 4; // Control pin 1 for motor 1
const int IN2 = 9; // Control pin 2 for motor 1

const int EN2 = 8;  // Enable Pin for motor 2
const int IN3 = 11;  // Control 1 pin for motor 2
const int IN4 = 10; // Control 2 pin for motor 2

const int running_speed = 255; //speed to run motors
const int debounce = 10;

const int runTrigger = 2;
const int stopTrigger = 3;

carstate current_state = IDLING;

unsigned long starttime;
unsigned long looptime;
unsigned long lastIdleTimestamp;
unsigned long lastRunningTimestamp;
unsigned long lastFinishedTimestamp;
int finishLineLEDIteration = 0;

volatile byte startSwitchState = LOW;
volatile byte stopSwitchState = HIGH;

void setup() {
  Serial.begin(9600);
  Serial.println("Serial Starting....");

  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);

  pinMode(EN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN1, OUTPUT);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  pinMode(EN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(runTrigger, INPUT_PULLUP);
  pinMode(stopTrigger, INPUT_PULLUP);

  FastLED.setBrightness(maxBrightness);
  lastIdleTimestamp = millis();
  IdleLEDSequence();

  attachInterrupt(digitalPinToInterrupt(runTrigger), lights, CHANGE);
  attachInterrupt(digitalPinToInterrupt(stopTrigger), triggerStop, CHANGE);
}

void loop() {
  switch (current_state)
  {
    case IDLING:
    case READY:
      IdleLEDSequence();
      break;
    case RUNNING:
      runningLEDSequence();
      break;
    case FINISHED:
      StopMotor();
      finishLineLEDSequence();
      break;
  }
}

void triggerStop()
{
  stopSwitchState = digitalRead(stopTrigger);
  Serial.println("Stop Pin is: " + String(stopSwitchState));
  if (stopSwitchState == LOW) {
    current_state = FINISHED;
  }
}

void lights()
{
  //debounce doesn't seem necessary
  //unsigned long interrupt_time = millis();
  // if (interrupt_time - lastInterruptTimestamp > debounce) {

  startSwitchState = digitalRead(runTrigger);
  Serial.println("Pin is: " + String(startSwitchState));

  switch (current_state)
  {
    case IDLING:
      if (startSwitchState == HIGH) {
        current_state = READY;
        Serial.println("Transistion from IDLING to READY");
      }
      break;
    case READY:
      if (startSwitchState == LOW) {
        current_state = RUNNING;
        StartMotor();
        starttime = millis();
        Serial.println("Transistion from READY to RUNNING");
      }
      break;
    case RUNNING:
      Serial.println("No transition while running");
      break;
    case FINISHED:
      if (startSwitchState == HIGH) {
        current_state = READY;
        Serial.println("Transistion from FINISHED to READY");
      }
      break;
  }
  //  }
}

void StopMotor()
{
  digitalWrite(EN1, LOW);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(EN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void StartMotor()
{
  //Set motors speed
  analogWrite(EN1, running_speed);
  analogWrite(EN2, running_speed);

  //set rotation direction
  digitalWrite(IN2, HIGH);
  digitalWrite(IN1, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void finishLineLEDSequence()
{
  //finishLineLEDIteration
  FastLED.setBrightness(maxBrightness);
  unsigned long now = millis();
  if (now - lastFinishedTimestamp > led_cylcle_time) {

    int m = 0; //multiplier to shorten code

    switch (finishLineLEDIteration) {
      case 0:
        for (int i = 0; i < (NUM_LEDS / 8); i++) {
          m = i * 8;
          leds[0 + m] = leds[7 + m] = CRGB::Green;
          leds[1 + m] = leds[2 + m] = leds[3 + m] = leds[4 + m] = leds[5 + m] = leds[6 + m] = CRGB(0, 0, 0);
        }
        finishLineLEDIteration = 1;
        break;
      case 1:
        for (int i = 0; i < (NUM_LEDS / 8); i++) {
          m = i * 8;
          leds[0 + m] = leds[1 + m] = leds[6 + m] = leds[7 + m] = CRGB::Green;
          leds[2 + m] = leds[3 + m] = leds[4 + m] = leds[5 + m] = CRGB(0, 0, 0);
        }
        finishLineLEDIteration = 2;
        break;
      case 2:
        for (int i = 0; i < (NUM_LEDS / 8); i++) {
          m = i * 8;
          leds[0 + m] = leds[1 + m] = leds[2 + m] = leds[5 + m] = leds[6 + m] = leds[7 + m] = CRGB::Green;
          leds[3 + m] = leds[4 + m] = CRGB(0, 0, 0);
        }
        finishLineLEDIteration = 3;
        break;
      case 3:
        for (int i = 0; i < (NUM_LEDS / 8); i++) {
          m = i * 8;
          leds[0 + m] = leds[1 + m] = leds[2 + m] = leds[3 + m] = leds[4 + m] = leds[5 + m] = leds[6 + m] = leds[7 + m] = CRGB::Green;
        }
        finishLineLEDIteration = 4;
        break;
      case 4:
        for (int i = 0; i < (NUM_LEDS / 8); i++) {
          m = i * 8;
          leds[0 + m] = leds[1 + m] = leds[2 + m] = leds[5 + m] = leds[6 + m] = leds[7 + m] = CRGB(0, 0, 0);
          leds[3 + m] = leds[4 + m] = CRGB::Green;
        }
        finishLineLEDIteration = 5;
        break;
      case 5:
        for (int i = 0; i < (NUM_LEDS / 8); i++) {
          m = i * 8;
          leds[0 + m] = leds[1 + m] = leds[3 + m] = leds[4 + m] = leds[6 + m] = leds[7 + m] = CRGB(0, 0, 0);
          leds[2 + m] = leds[5 + m] = CRGB::Green;
        }
        finishLineLEDIteration = 6;
        break;
      case 6:
        for (int i = 0; i < (NUM_LEDS / 8); i++) {
          m = i * 8;
          leds[0 + m] = leds[2 + m] = leds[3 + m] = leds[4 + m] = leds[5 + m] = leds[7 + m] = CRGB(0, 0, 0);
          leds[1 + m] = leds[6 + m] = CRGB::Green;
        }
        finishLineLEDIteration = 7;
        break;
      case 7:
        for (int i = 0; i < (NUM_LEDS / 8); i++) {
          m = i * 8;
          leds[0 + m] = leds[7 + m] = CRGB::Green;
          leds[1 + m] = leds[2 + m] = leds[3 + m] = leds[4 + m] = leds[5 + m] = leds[6 + m] = CRGB(0, 0, 0);
        }
        finishLineLEDIteration = 8;
        break;
      case 8:
        for (int i = 0; i < (NUM_LEDS / 8); i++) {
          m = i * 8;
          leds[0 + m] = leds[1 + m] = leds[2 + m] = leds[3 + m] = leds[4 + m] = leds[5 + m] = leds[6 + m] = leds[7 + m] = CRGB(0, 0, 0);
        }
        finishLineLEDIteration = 0;
        break;
    }

    FastLED.show();
    lastFinishedTimestamp = millis();
  }
}

void runningLEDSequence()
{
  unsigned long now = millis();

  if (now - lastRunningTimestamp > led_cylcle_time) {
    FastLED.setBrightness(runningBrightness);
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Red;
    }
    lastRunningTimestamp = millis();
  } else {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB(0, 0, 0);
    }
  }
  FastLED.show();
}

void IdleLEDSequence()
{
  if (millis() - lastIdleTimestamp > led_cylcle_time) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = GetRandomColor();
    }
    lastIdleTimestamp = millis();
  }
  FastLED.show();
}

CRGB GetRandomColor() {
  int seed = analogRead(12);
  randomSeed(seed);
  return CRGB(random(255), random(255), random(255));
}
