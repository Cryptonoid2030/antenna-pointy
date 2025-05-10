#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

static const int RXPin = 4, TXPin = 3; //RXPin is TX on gps
static const uint32_t GPSBaud = 9600;

// The TinyGPSPlus object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void setup()
{
  Serial.begin(19200); // set to what need on terminal
  ss.begin(GPSBaud);

  Serial.println("starting up...");
}

void loop()
{
  // Dispatch incoming characters
  while (ss.available() > 0)
    gps.encode(ss.read());

  if (gps.location.isUpdated() || gps.altitude.isUpdated())
  {
    Serial.print(F("Lat="));
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(" Long="));
    Serial.print(gps.location.lng(), 6);

    Serial.print(F(" | ALTITUDE Meters="));
    Serial.println(gps.altitude.meters());
  }


}