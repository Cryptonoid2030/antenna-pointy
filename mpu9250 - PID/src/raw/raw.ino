// -------------------------------------------------
// Copyright (c) 2022 HiBit <https://www.hibit.dev>
// -------------------------------------------------

#include "Wire.h"
#include "I2C.h"
#include "inputs.h"
#include <Servo.h>

Servo myservo;

// Servo control setup
unsigned long lastPrintMillis = 0;
int pwm = 0; // reset PWM value
int ledPin = 9;

// PID controller setup
float error;
float integral;
float dt = 0.5; // time between each read (s)
float derivative;
float previous_error = 0;

float kP = 0.15;
float kI = 0.09;
float kD = 0.2;

void setup()
{
  Wire.begin();
  Serial.begin(9600);

  I2CwriteByte(MPU9250_IMU_ADDRESS, 55, 0x02); // Set by pass mode for the magnetometers
  I2CwriteByte(MPU9250_IMU_ADDRESS, 56, 0x01); // Enable interrupt pin for raw data

  setMagnetometerAdjustmentValues();

  //Start magnetometer
  I2CwriteByte(MPU9250_MAG_ADDRESS, 0x0A, 0x12); // Request continuous magnetometer measurements in 16 bits (mode 1)
  // pinMode(ledPin, OUTPUT);
  myservo.attach(9);
}

void loop()
{
  unsigned long currentMillis = millis();
  float angle;
  int a; int b;

  if (isMagnetometerReady()) {
    readRawMagnetometer();

    normalize(magnetometer);
  }


  angle = atan(normalized.magnetometer.y / normalized.magnetometer.x) * 180 / 3.14159;

  a = normalized.magnetometer.x > 0; // x direction comparator
  b = normalized.magnetometer.y > 0; // y direction comparator

  // step 1: implement conversion to full circle
  if (a && b) {// Q1
      angle = angle;
  } else if (!a && b) {// Q2
      angle = 180. + angle;
  } else if (!a && !b) {// Q3
      angle = 180. + angle;
  } else if (a && !b) {// Q4
      angle = 360. + angle;
  }

  // Calculate PID terms
  error = ((int) angle % 360) - target_angle; // calculate the error
  integral += error * dt; // dt is the time difference between loops
  derivative = (error - previous_error) / dt;
  previous_error = error;

  pwm = (int) (kP * error + kI * integral + kD * derivative) % 180; // PID sum cast as int

  // Write results to serial - Remove if takes processing power
  // analogWrite(ledPin, pwm);
  myservo.write(pwm);
  Serial.print("angle: ");
  Serial.print(angle);
  Serial.print(" error: ");
  Serial.print(error);
  Serial.print(" Writing PWM: ");
  Serial.println(pwm);
  delay(dt*1000);
}
