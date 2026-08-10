#include <TinyGPS++.h>
#include <SoftwareSerial.h>

// -------- GPS SETUP --------
TinyGPSPlus gps;
SoftwareSerial gpsSerial(4, 3); // RX, TX (GPS)

// -------- GSM SETUP --------
SoftwareSerial gsmSerial(7, 5); // RX, TX (GSM)

// -------- PINS --------
#define VIBRATION_PIN A1
#define SWITCH_PIN A0

void setup() {
  Serial.begin(9600);

  gpsSerial.begin(9600);
  gsmSerial.begin(9600);

  pinMode(SWITCH_PIN, INPUT);

  Serial.println("GPS & GSM System Started...");
}

void loop() {

  // Read GPS data
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }

  double lat = gps.location.isValid() ? gps.location.lat() : 0.0;
  double lng = gps.location.isValid() ? gps.location.lng() : 0.0;

  int vibration = analogRead(VIBRATION_PIN);
  int switchState = digitalRead(SWITCH_PIN);

  Serial.print("Vibration: ");
  Serial.println(vibration);

  Serial.print("Switch: ");
  Serial.println(switchState);

  // If switch is ON and vibration is high → send alert
  if (switchState == 1 || vibration > 50) {

    String googleMapsLink = "https://maps.google.com/?q=" + String(lat, 6) + "," + String(lng, 6);
    String message = "Alert! Abnormal Vibration Detected.\nLocation: " + googleMapsLink;

    gsmSerial.println("AT");
    delay(1000);
    gsmSerial.println("AT+CMGF=1");
    delay(1000);

    gsmSerial.println("AT+CMGS=\"+919952076443\"");
    delay(1000);
    gsmSerial.println(message);
    delay(1000);
    gsmSerial.write(26);
    delay(4000);
  }

  delay(500);
}