// -------------------------------------------------
// Copyright (c) 2022 HiBit <https://www.hibit.dev>
// -------------------------------------------------

#include "Wire.h"
#include "I2C.h"
#include "inputs.h"  // <-- target_angle should be defined here
#include <Servo.h>

Servo myservo;

// PID controller state
float PID_error;
float integral = 0;
float derivative;
float previous_error = 0;

float kP = 0.15;
float kI = 0.09;
float kD = 0.2;

float dt = 0.5;
unsigned long lastTime = 0;

int pwm = 0;
int ledPin = 9;

void setup()
{
  Wire.begin();
  Serial.begin(9600);

  I2CwriteByte(MPU9250_IMU_ADDRESS, 55, 0x02); // Bypass mode for magnetometers
  I2CwriteByte(MPU9250_IMU_ADDRESS, 56, 0x01); // Enable interrupt pin

  setMagnetometerAdjustmentValues();

  // Start magnetometer
  I2CwriteByte(MPU9250_MAG_ADDRESS, 0x0A, 0x12); // Continuous mode, 16-bit

  myservo.attach(9);
  lastTime = millis();
}

void loop()
{
  unsigned long now = millis();
  dt = (now - lastTime) / 1000.0;
  lastTime = now;

  float angle = 0;
  int a, b;

  if (isMagnetometerReady()) {
    readRawMagnetometer();
    normalize(magnetometer);
  }

  // Compute heading angle
  angle = atan2(normalized.magnetometer.y, normalized.magnetometer.x) * 180 / 3.14159;
  if (angle < 0) {
    angle += 360;
  }

  // Calculate PID error with angle wrapping
  PID_error = angle - target_angle;
  if (PID_error > 180) PID_error -= 360;
  if (PID_error < -180) PID_error += 360;

  // PID calculations
  integral += PID_error * dt;
  integral = constrain(integral, -100, 100); // Anti-windup

  derivative = (PID_error - previous_error) / dt;
  previous_error = PID_error;

  pwm = (int)(kP * PID_error + kI * integral + kD * derivative);
  pwm = constrain(pwm, 0, 180);

  // Drive servo and output debug info
  myservo.write(pwm);
  Serial.print("angle: ");
  Serial.print(angle);
  Serial.print(" error: ");
  Serial.print(PID_error);
  Serial.print(" PWM: ");
  Serial.println(pwm);

  delay(20); // Small delay to allow servo to settle
}
