// proof of concept of sending over bits by serial
String incoming;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  incoming = Serial.readString();
  if (incoming.substring(0, 3) == "Lat") { // trigger if reading latitude data
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
  } else if (incoming.substring(0, 3) == "Lon") { // trigger if reading longitude data
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
  } else if (incoming.substring(0, 3) == "Alt") { // trigger if reading altitude data
    digitalWrite(LED_BUILTIN, HIGH);
  }

  digitalWrite(LED_BUILTIN, LOW);

}
