#include <FastLED.h>
#include "carstate.h"

const int run_duration = 3000;
const int maxBrightness = 60; // up to 255. More bright, more amps
const int runningBrightness = 50; // up to 255. More bright, more amps
const int LED_PIN = 7;
const int NUM_LEDS = 8;
CRGB leds[NUM_LEDS];

const int E1 = 5; // Enable Pin for motor
const int I1 = 3; // Control pin 1 for motor
const int I2 = 4; // Control pin 2 for motor
const int running_speed = 200; //speed to run motors

const int E2 = 8;
const int I3 = 9;
const int I4 = 10;
const int pulluppin = 2;

carstate current_state = IDLING;

unsigned long starttime;
unsigned long looptime;
unsigned long lastIdleTimestamp;
unsigned long lastRunningTimestamp;
unsigned long lastFinishedTimestamp;
static unsigned long lastInterruptTimestamp;
int finishLineLEDIteration = 0;

int switchstatus = LOW;
volatile byte switchstate = LOW;

void setup() {
  Serial.begin(9600);
  Serial.println("Serial Starting....");

  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);

  pinMode(E1, OUTPUT);
  pinMode(I1, OUTPUT);
  pinMode(I2, OUTPUT);

  pinMode(E2, OUTPUT);
  pinMode(I3, OUTPUT);
  pinMode(I4, OUTPUT);

  pinMode(pulluppin, INPUT_PULLUP);

  FastLED.setBrightness(maxBrightness);
  lastIdleTimestamp = millis();
  IdleLEDSequence();

  attachInterrupt(digitalPinToInterrupt(pulluppin), lights, CHANGE);
}

void loop() {

  looptime = millis();

  switch (current_state)
  {
    case IDLING:
    case READY:
      IdleLEDSequence();
      break;
    case RUNNING:
      if (looptime - starttime > run_duration) {
        StopMotor();
        lastFinishedTimestamp = millis();
        finishLineLEDSequence();
      } else {
        runningLEDSequence();
      }
      break;
    case FINISHED:
      finishLineLEDSequence();
      break;
  }
}

void lights()
{
  //unsigned long interrupt_time = millis();

  //if (interrupt_time - lastInterruptTimestamp > 200) {
  byte switchstatus = digitalRead(pulluppin);
  Serial.println("Pin is: " + String(switchstatus));

  switch (current_state)
  {
    case IDLING:
      if (switchstatus == HIGH) {
        current_state = READY;
        Serial.println("Transistion from IDLING to READY");
      }
      break;
    case READY:
      if (switchstatus == LOW) {
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
      if (switchstatus == HIGH) {
        current_state = READY;
        Serial.println("Transistion from FINISHED to READY");
      }
      break;
  }
}

void StopMotor()
{
  digitalWrite(E1, LOW);
  digitalWrite(E2, LOW);
  current_state = FINISHED;
}

void StartMotor()
{
  //Set motors speed
  analogWrite(E1, running_speed);
  analogWrite(E2, running_speed);

  //set rotation direction
  digitalWrite(I1, HIGH);
  digitalWrite(I2, LOW);
  digitalWrite(I3, HIGH);
  digitalWrite(I4, LOW);
}

void finishLineLEDSequence()
{
  //finishLineLEDIteration
  FastLED.setBrightness(maxBrightness);
  unsigned long now = millis();
  if (now - lastFinishedTimestamp > 100) {

    for (int i = 0; i < 8 / NUM_LEDS; i++) {
      int m = i * 8; //multiplier to shorten code

      switch (finishLineLEDIteration) {
        case 0:
          leds[0 + m] = leds[7 + m] = CRGB::Green;
          leds[1 + m] = leds[2 + m] = leds[3 + m] = leds[4 + m] = leds[5 + m] = leds[6 + m] = CRGB(0, 0, 0);
          finishLineLEDIteration = 1;
          break;
        case 1:
          leds[0 + m] = leds[1 + m] = leds[6 + m] = leds[7 + m] = CRGB::Green;
          leds[2 + m] = leds[3 + m] = leds[4 + m] = leds[5 + m] = CRGB(0, 0, 0);
          finishLineLEDIteration = 2;
          break;
        case 2:
          leds[0 + m] = leds[1 + m] = leds[2 + m] = leds[5 + m] = leds[6 + m] = leds[7 + m] = CRGB::Green;
          leds[3 + m] = leds[4 + m] = CRGB(0, 0, 0);
          finishLineLEDIteration = 3;
          break;
        case 3:
          leds[0 + m] = leds[1 + m] = leds[2 + m] = leds[3 + m] = leds[4 + m] = leds[5 + m] = leds[6 + m] = leds[7 + m] = CRGB::Green;
          finishLineLEDIteration = 4;
          break;
        case 4:
          leds[0 + m] = leds[1 + m] = leds[2 + m] = leds[5 + m] = leds[6 + m] = leds[7 + m] = CRGB(0, 0, 0);
          leds[3 + m] = leds[4 + m] = CRGB::Green;
          finishLineLEDIteration = 5;
          break;
        case 5:
          leds[0 + m] = leds[1 + m] = leds[3 + m] = leds[4 + m] = leds[6 + m] = leds[7 + m] = CRGB(0, 0, 0);
          leds[2 + m] = leds[5 + m] = CRGB::Green;
          finishLineLEDIteration = 6;
          break;
        case 6:
          leds[0 + m] = leds[2 + m] = leds[3 + m] = leds[4 + m] = leds[5 + m] = leds[7 + m] = CRGB(0, 0, 0);
          leds[1 + m] = leds[6 + m] = CRGB::Green;          
          finishLineLEDIteration = 7;
          break;
        case 7:
          leds[0 + m] = leds[7 + m] = CRGB::Green;
          leds[1 + m] = leds[2 + m] = leds[3 + m] = leds[4 + m] = leds[5 + m] = leds[6 + m] = CRGB(0, 0, 0);          
          finishLineLEDIteration = 8;
          break;
        case 8:
          leds[0] = leds[1] = leds[2] = leds[3] = leds[4] = leds[5] = leds[6] = leds[7] = CRGB(0, 0, 0);
          finishLineLEDIteration = 0;
          break;
      }
    }
    FastLED.show();
    lastFinishedTimestamp = millis();
  }
}

void runningLEDSequence()
{
  unsigned long now = millis();

  if (now - lastRunningTimestamp > 100) {
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
  if (millis() - lastIdleTimestamp > 100) {
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
