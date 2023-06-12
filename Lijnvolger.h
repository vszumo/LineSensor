#ifndef LINESENSOR_H
#define LINESENSOR_H

#include <Zumo32U4motors.h>
#include <Zumo32U4LineSensors.h>
#include <Zumo32U4.h>
#include <Motorcontroller.h>

#define NUM_SENSORS 5 

Class Lijnvolger {

public:
  Lijnvolger()
  void volgLijn();
  void calibrateSensors();
  void start();
  void init();


private:
  Zumo32U4Buzzer buzzer;
  Zumo32U4LineSensors lineSensors;
  Zumo32U4Motors motors;
  Zumo32U4ButtonA buttonA;

  unsigned int lineSensorValues[NUM_SENSORS];
  int16_t position;
  int16_t lastError;
  int16_t error;
  int16_t speedDifference;
  int16_t leftSpeed;
  int16_t rightSpeed;
  const uint16_t maxSpeed = 100;
}
#endif
