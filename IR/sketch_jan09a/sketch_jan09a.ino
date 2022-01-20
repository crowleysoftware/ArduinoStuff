
#include <IRremote.h>

const int IR_RECEIVE_PIN = 11;
const int led = 7;
int ledState = LOW;

void setup()   /*----( SETUP: RUNS ONCE )----*/
{
  Serial.begin(115200);
  delay(1000);

  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

  Serial.println("IR Receiver Button Decode");
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Start the receiver

}/*--(end setup )---*/


void loop()   /*----( LOOP: RUNS CONSTANTLY )----*/
{
  if (IrReceiver.decode()) {  // Grab an IR code
    if (IrReceiver.decodedIRData.decodedRawData == 0xBA45FF00)
    {
      Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
      delay(100);

      if (digitalRead(led) == HIGH)
      {
        digitalWrite(led, LOW);
      } else {
        digitalWrite(led, HIGH);
      }
    }

    IrReceiver.resume();
  }

}/* --(end main loop )-- */
