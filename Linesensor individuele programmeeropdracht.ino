//Hier lezen we de bestanden in die we moeten gebruiken tijdens het programmeren van de Zumo robot.
#include <Wire.h>
#include <Zumo32U4.h>

//Hiermee geven we de max snelheid aan, we hebben hier gekozen voor 200 omdat dat snel genoeg is.
const uint16_t maxSpeed = 300; 

//Hiermee lezen we de button A en de linesensor in, ook lezen we de buzzer in zodat hij geluidjes maakt wanneer je op het knopje drukt en opstart.
//De motor wordt hier ook ingelezen zodat we de motors kunnen aansturen in de code.
Zumo32U4Buzzer buzzer;
Zumo32U4LineSensors lineSensors;
Zumo32U4Motors motors;
Zumo32U4ButtonA buttonA;


int16_t lastError = 0;
//Hier lezen we de 5 sensoren in die we gebruiken, er zijn namelijk 5 linesensoren aanwezig op de Zumo robot, deze zitten voorop de Zumo.
#define NUM_SENSORS 5 
//Een array aanmaken voor het aantal sensors die er zijn.
unsigned int lineSensorValues[NUM_SENSORS]; 
//Nu gaan we de sensors kalibreren door de robot eerst rond te laten draaien zodat hij de lijn kan lezen met alle sensors.
  void calibrateSensors() {
  //We zetten er een delay in van 2 seconden zodat de Zumo even de tijd krijgt voordat hij gaat kalibreren.
  delay(2000); 

  //Met deze for loop zorgen we ervoor dat de Zumo een korte tijd gaat ronddraaien totdat de waardes van i zijn bereikt.
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

//Hier gaan we de setup maken voor de robot.
void setup() { 
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

void loop() {
  //Zo bepalen we de positie van de robot.
  int16_t position = lineSensors.readLine(lineSensorValues);
  for(int i =0; i<5;i++){
    Serial.print(lineSensorValues[i]);
    Serial.print(", ");
  
  }
  Serial.println("");

  //De error die wie hier maken is hoe ver de Zumo van de midden van de lijn verwijderd is, dit komt overeen met de positie 2000.
  int16_t error = position - 2000;

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