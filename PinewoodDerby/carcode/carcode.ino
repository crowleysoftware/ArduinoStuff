#include <FastLED.h>
#include "carstate.h"

const int run_duration = 3000;
const int maxBrightness = 60;
const int runningBrightness = 50;
const int LED_PIN = 7;
const int NUM_LEDS = 8;
CRGB leds[NUM_LEDS];

const int E1 = 5; // Enable Pin for motor
const int I1 = 3; // Control pin 1 for motor
const int I2 = 4; // Control pin 2 for motor

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
  Serial.println("Get your motor running..!");

  //Set motors speed
  analogWrite(E1, 255);
  analogWrite(E2, 255);

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
    switch (finishLineLEDIteration) {
      case 0:
        leds[0] = CRGB(0, 255, 0);
        leds[1] = CRGB(0, 0, 0);
        leds[2] = CRGB(0, 0, 0);
        leds[3] = CRGB(0, 0, 0);
        leds[4] = CRGB(0, 0, 0);
        leds[5] = CRGB(0, 0, 0);
        leds[6] = CRGB(0, 0, 0);
        leds[7] = CRGB(0, 255, 0);
        finishLineLEDIteration = 1;
        break;
      case 1:
        leds[0] = CRGB(0, 255, 0);
        leds[1] = CRGB(0, 255, 0);
        leds[2] = CRGB(0, 0, 0);
        leds[3] = CRGB(0, 0, 0);
        leds[4] = CRGB(0, 0, 0);
        leds[5] = CRGB(0, 0, 0);
        leds[6] = CRGB(0, 255, 0);
        leds[7] = CRGB(0, 255, 0);
        finishLineLEDIteration = 2;
        break;
      case 2:
        leds[0] = CRGB(0, 255, 0);
        leds[1] = CRGB(0, 255, 0);
        leds[2] = CRGB(0, 255, 0);
        leds[3] = CRGB(0, 0, 0);
        leds[4] = CRGB(0, 0, 0);
        leds[5] = CRGB(0, 255, 0);
        leds[6] = CRGB(0, 255, 0);
        leds[7] = CRGB(0, 255, 0);
        finishLineLEDIteration = 3;
        break;
      case 3:
        leds[0] = CRGB(0, 255, 0);
        leds[1] = CRGB(0, 255, 0);
        leds[2] = CRGB(0, 255, 0);
        leds[3] = CRGB(0, 255, 0);
        leds[4] = CRGB(0, 255, 0);
        leds[5] = CRGB(0, 255, 0);
        leds[6] = CRGB(0, 255, 0);
        leds[7] = CRGB(0, 255, 0);
        finishLineLEDIteration = 4;
        break;
      case 4:
        leds[0] = CRGB(0, 0, 0);
        leds[1] = CRGB(0, 0, 0);
        leds[2] = CRGB(0, 0, 0);
        leds[3] = CRGB(0, 255, 0);
        leds[4] = CRGB(0, 255, 0);
        leds[5] = CRGB(0, 0, 0);
        leds[6] = CRGB(0, 0, 0);
        leds[7] = CRGB(0, 0, 0);
        finishLineLEDIteration = 5;
        break;
      case 5:
        leds[0] = CRGB(0, 0, 0);
        leds[1] = CRGB(0, 0, 0);
        leds[2] = CRGB(0, 255, 0);
        leds[3] = CRGB(0, 0, 0);
        leds[4] = CRGB(0, 0, 0);
        leds[5] = CRGB(0, 255, 0);
        leds[6] = CRGB(0, 0, 0);
        leds[7] = CRGB(0, 0, 0);
        finishLineLEDIteration = 6;
        break;
      case 6:
        leds[0] = CRGB(0, 0, 0);
        leds[1] = CRGB(0, 255, 0);
        leds[2] = CRGB(0, 0, 0);
        leds[3] = CRGB(0, 0, 0);
        leds[4] = CRGB(0, 0, 0);
        leds[5] = CRGB(0, 0, 0);
        leds[6] = CRGB(0, 255, 0);
        leds[7] = CRGB(0, 0, 0);
        finishLineLEDIteration = 7;
        break;
      case 7:
        leds[0] = CRGB(0, 255, 0);
        leds[1] = CRGB(0, 0, 0);
        leds[2] = CRGB(0, 0, 0);
        leds[3] = CRGB(0, 0, 0);
        leds[4] = CRGB(0, 0, 0);
        leds[5] = CRGB(0, 0, 0);
        leds[6] = CRGB(0, 0, 0);
        leds[7] = CRGB(0, 255, 0);
        finishLineLEDIteration = 8;
      case 8:
        leds[0] = CRGB(0, 0, 0);
        leds[1] = CRGB(0, 0, 0);
        leds[2] = CRGB(0, 0, 0);
        leds[3] = CRGB(0, 0, 0);
        leds[4] = CRGB(0, 0, 0);
        leds[5] = CRGB(0, 0, 0);
        leds[6] = CRGB(0, 0, 0);
        leds[7] = CRGB(0, 0, 0);
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

  if (now - lastRunningTimestamp > 100) {
    FastLED.setBrightness(runningBrightness);
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB(255, 0, 0);
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
