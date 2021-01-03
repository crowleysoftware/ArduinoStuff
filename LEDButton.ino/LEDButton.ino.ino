const int buttonPin = 2;
const int ledPin = 13;
int buttonPinValue = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
}

void loop() {
  buttonPinValue = digitalRead(buttonPin);

  //if(buttonPinValue == HIGH){
    String valAsStr = String(buttonPinValue);
    Serial.println(valAsStr);
  //}
  digitalWrite(ledPin, buttonPinValue);
}
