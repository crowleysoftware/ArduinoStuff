const int led = 3;

void setup() {
  // put your setup code here, to run once:
  // Serial.begin(115200);
  // Serial.println("blah, blah, blah");
  delay(1000);

  pinMode(led, OUTPUT);
}

void loop() {

  digitalWrite(led, HIGH);
  delay(600);
  digitalWrite(led, LOW);
  delay(600);

}
