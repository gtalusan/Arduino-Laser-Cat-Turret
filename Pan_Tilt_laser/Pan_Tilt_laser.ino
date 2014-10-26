/**************************************************************************/
/*!
    @file     Pan_Tilt_laser.ino
    @author   Stuart Feichtinger
    @license  BSD (see license.txt)

    Main program running an automatic laser-turret cat toy using a pan-tilt
    rig (https://www.adafruit.com/products/1967) and laser diode
    (https://www.adafruit.com/products/1054). Random laser path, speed
    and pauses determined by homebrew Markov chain algorithm.


    @section  HISTORY
    v1.0.0 - First release
    v1.1.0 - Added Markov-based speed control
    v1.2.0 - Added Markov-based pause length/frequency
    v1.2.1 - Added Heartbeat during pause
    v1.3.0 - Added laser shake to enhance cat enjoyment
    v1.3.1 - Changed probability polling frequency to once a second
    v1.4.0 - Added ON/OFF Missile Switch from Sparkfun
*/
/**************************************************************************/



#include <Servo.h>
#include <math.h>
#include "stuServo.h"
#include "stuPanTilt.h"
#include "stuLaser.h"
#include "missileswitch.h"


#define BAUD_RATE 115200

#define MS_SWITCH_PIN 3
#define MS_LED_PIN 4

#define LASER_PIN 5

#define SERVO_X_PIN A0
#define SERVO_Y_PIN A1


#define DIRECTION_CHANGE_PROBABILITY 15

int sleepState = 0;


//X Position: lower numbers == Right
//Y Position: lower numbers == Up

panTiltPos_t panTiltX(0, 55, 125, -20);
panTiltPos_t panTiltY(0, 10, 45, 0, 10);


PanTilt panTilt(SERVO_X_PIN, &panTiltX, SERVO_Y_PIN, &panTiltY, 98);

StuLaser laser(LASER_PIN);

Missileswitch mSwitch(MS_SWITCH_PIN, MS_LED_PIN);

void setup() {
  delay(1000); // let things settle for stability

  Serial.begin(BAUD_RATE);
  mSwitch.begin();
  mSwitch.heartBeat(3);

  if(!mSwitch.switchState()) {
    mSwitch.ledState(0);
    while(!mSwitch.switchState()){
      delay(50);
      }
  }

  Serial.println(F("setup starting..."));
  mSwitch.ledState(1);

  panTilt.begin();
  laser.begin();

  panTiltX.angle = panTiltX.minAngle;
  panTiltY.angle = panTiltY.minAngle;
  panTilt.updateAngles();
  delay(1000);

  panTiltX.angle = panTiltX.maxAngle;
  panTiltY.angle = panTiltY.maxAngle;
  panTilt.updateAngles();
  delay(1000);


  panTiltX.angle = panTiltX.midAngle;
  panTiltY.angle = panTiltY.midAngle;
  panTilt.updateAngles();

  delay(2000);
  laser.fire(1);
  Serial.println(F("setup complete"));
//  attachInterrupt(0, sleepInt, FALLING);
}



void loop() {

  if(!mSwitch.switchState()){

    laser.fire(0);
    mSwitch.ledState(0);
    panTiltX.angle = panTiltX.midAngle - panTiltX.midOffset;
    panTiltY.angle = panTiltY.midAngle - panTiltY.midOffset;
    panTilt.updateAngles();

    delay(50);
//    attachInterrupt(0, wake, RISING);

    panTilt.detach();
    while(!mSwitch.switchState()){
      delay(50);

    }
    mSwitch.ledState(1);
    panTilt.begin();
    laser.fire(1);

  }
  //attachInterrupt(0, sleepInt, FALLING);
  randomSeed(analogRead(0));
  static unsigned long timePassed;
  static int changeVal;
  static int markovShakeState;
  delay(10);


  panTiltX.angle = getDeltaPosition(&panTiltX, changeVal, DIRECTION_CHANGE_PROBABILITY) + panTiltX.angle;

  panTiltY.angle = getDeltaPosition(&panTiltY, changeVal, DIRECTION_CHANGE_PROBABILITY) + panTiltY.angle;


  panTilt.updateAngles();

  if(markovShakeState == 2){
    shake();
  }

if(millis() - timePassed >= 1000){
    changeVal = getMarkovSpeed(changeVal);
    markovShakeState = markovState(10, 20);


    if(random(101) < 6){
      delay(markovPause());

    }


    if(random(5001) < 1){
      sleep(5, 10);

    }

    else if(random(10001) < 10){
      sleep(1800, 2400); //sleep between 30 and 40 minutes
    }
    timePassed = millis();
  }
  laser.fire(1);

}

int getMarkovDirection(panTiltPos_t *pt, int changeProb){

    int prob = changeProb;

    if(pt->dir == 0){
      pt->dir = 1;
    }


    if((pt->dir == 1 && pt->angle >= pt->midAngle) || (pt->dir == -1 && pt->angle <= pt->midAngle)){
      prob += abs(pt->midAngle - pt->angle) * max(pt->probOffset, 1);

    }

    if(random(1001) <= prob << 1 || (pt->angle >= pt->maxAngle && pt->dir == 1) || pt->angle <= pt->minAngle && pt->dir == -1){
      pt->dir *= -1;
    }
    return pt->dir;
  }


int getDeltaPosition(panTiltPos_t *pt, int funcChangeVal, int changeProb){
  int tempVal = getMarkovDirection(pt, changeProb);

  tempVal *= funcChangeVal;

  return tempVal;

}

int getMarkovSpeed(int oldSpeed){
  int probability = random(101);
  int lowVal = 1;
  int midVal = 2;
  int hiVal = 3;

  if(oldSpeed == lowVal){
    if(probability < 60){
      return midVal;
    }
    else{
      return lowVal;
    }
  }

  else if(oldSpeed == midVal){
    if(probability < 30){
      return hiVal;
    }
    else if(probability < 80){
      return midVal;
    }
    else{
      return lowVal;
    }
  }

  else if(oldSpeed == hiVal){
    if(probability < 60){
      return midVal;
    }
    else{
      return hiVal;
    }
  }

  else{
    return constrain(oldSpeed, lowVal, hiVal);
  }
}

int markovPause(){
  int val = random(101);

  if(val < 35){
    return random(1000, 2000);
  }
  else if(val < 80){
    return random(750, 1000);
  }
  else{
    return random(500, 750);
  }
}

void sleep(unsigned long minSec, unsigned long maxSec){
  unsigned int delayVal = random(minSec, maxSec);
  laser.fire(0);
  panTilt.detach();

  unsigned long startTime = millis();
  for(unsigned long i = 0; i < delayVal; i++){
    while(millis() - startTime < 1000){
      heartBeat(millis(), 10000);
      if(startTime > millis()){ //check for rollovers
        startTime = millis();
        break;
      }
    }
    startTime = millis();
  }
  panTilt.begin();
  laser.fire(1);

}


void heartBeat(unsigned long mSeconds, int hbInterval){
  static unsigned long oldTime;
  if(mSeconds - oldTime > hbInterval){


    mSwitch.heartBeat(3);
    oldTime = mSeconds;
  }
}

void shake(){
  int moveVal = 10;
  panTiltX.angle += moveVal;
  panTilt.updateAngles();
  delay(10);
  panTiltX.angle -= 2*moveVal;
  panTilt.updateAngles();
  delay(10);
  panTiltX.angle += moveVal;
  panTilt.updateAngles();
  delay(10);
}


int markovState(int prob1, int prob2){
  static int markovState;
  int probVal = random(101);
  if(!markovState){
    markovState = 1;
  }
  else if(markovState == 1){
    if(probVal <= prob1){
      markovState = 2;
    }
  }
  else if(markovState == 2){
    if(probVal <= prob2){
      markovState = 1;
    }
  }

  return markovState;
}


void sleepInt(){
  detachInterrupt(0);
  sleepState = 1;
  mSwitch.ledState(0);


}

void wake(){
  detachInterrupt(0);
  sleepState = 0;
  mSwitch.ledState(1);
}
