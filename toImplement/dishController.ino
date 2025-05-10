int pitch = 9;
int yaw = 10;

int t = 0;
float signalYaw;
float signalPitch;

void setup() {
  pinMode(pitch, OUTPUT);
  pinMode(yaw, OUTPUT);
}

void loop() {
  signalYaw = 45 * sin(2*PI * t / 50) + 205;
  signalPitch = 128 * sin(2 * PI * t / 200) + 128;
  analogWrite(pitch, signalPitch);
  analogWrite(yaw, signalYaw);
  t++;
  delay(100);
  t %= 100;
}