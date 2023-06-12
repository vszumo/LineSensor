#include "Lijnvolger.h"

Lijnvolger::Lijnvolger():
error(0),speedDifference(0),leftSpeed(0),rightSpeed(0),position(0),lastError(0);


void Lijnvolger::init(){
  
}

void Lijnvolger::start(){
    while(true){
  int16_t position = lineSensors.readLine(lineSensorValues);
  for(int i =0; i<5;i++){
    Serial.print(lineSensorValues[i]);
    Serial.print(", ");
  
  }
  Serial.println("");

  //De error die wie hier maken is hoe ver de Zumo van de midden van de lijn verwijderd is, dit komt overeen met de positie 2000.
  error = position - 2000;
  position = lineSensors.readLine(lineSensorValues);
  //Dit is een speciale formule om de speedDifference uit te rekenen.
  int16_t speedDifference = error / 4 + 6 * (error - lastError);
  lastError = error;

  //Hiermee pakken we de snelheden van links en rechts, de speedDifference bepaald of die links of rechts stuurt.
  int16_t leftSpeed = (int16_t)maxSpeed + speedDifference;
  int16_t rightSpeed = (int16_t)maxSpeed - speedDifference;

  //Hiermee beperken we de snelheden van 0 tot de maxSpeed die we eerder hebben ingesteld. Hij rijdt in principe altijd op de maximale snelheid tenzij die stilstaat.
  leftSpeed = constrain(leftSpeed, 0, (int16_t)maxSpeed);
  rightSpeed = constrain(rightSpeed, 0, (int16_t)maxSpeed);

  motors.setSpeeds(leftSpeed, rightSpeed);
    }
}

void Lijnvolger ::calibrateSensors(){
    delay(2000); 

  for(uint16_t i = 0; i < 185; i++) {
    if (i > 30 && i <= 90)
    {
      motors.setSpeeds(-200, 200);
    }
    else
    {
      motors.setSpeeds(200, -200);
    }

    lineSensors.calibrate();
  }
  motors.setSpeeds(0, 0);
}

void ::volgLijn(){
   Serial.begin(9600);
  lineSensors.initFiveSensors();
  //Zodra de A knop ingedrukt wordt gaat hij eerst kalibreren.
  buttonA.waitForButton(); 
  //Als die klaar is met kalibreren, maakt hij een geluidje zodat je weet dat hij gereed is.
  buzzer.play(">g32>>c32"); 
  calibrateSensors();
  //Als we nu op A drukken dan maakt hij eerst een geluidje en dan gaat hij de lijn volgen.
  buttonA.waitForButton(); 
  buzzer.play("L16 cdegreg8");
  //Hier maken we een while loop aan, zodat het geluidje afspeelt totdat het geluidje klaar is.
  while(buzzer.isPlaying());
}