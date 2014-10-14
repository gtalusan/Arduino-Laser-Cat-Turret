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
    v1.0 - First release

*/
/**************************************************************************/



#include <Servo.h>
#include <math.h>
#include "stuServo.h"
#include "stuPanTilt.h"
#include "stuLaser.h"

#define DIRECTION_CHANGE_PROBABILITY 15


point_t oldPoint, newPoint;



//X Position: lower numbers == Right
//Y Position: lower numbers == Up

panTiltPos_t panTiltX(0, 55, 125, -20);
panTiltPos_t panTiltY(0, 10, 45);


PanTilt panTilt(9, &panTiltX, 10, &panTiltY, 98);

StuLaser laser(5);

#define BAUD_RATE 115200

void setup() {

  Serial.begin(BAUD_RATE);

  panTilt.begin();

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
  laser.begin();
  laser.fire(1);
  delay(2000);



/*
  Serial.print(F("X min Value: "));
  Serial.println(panTiltX.minAngle);
  Serial.print(F("X max Value: "));
  Serial.println(panTiltX.maxAngle);
  Serial.print(F("X mid Value: "));
  Serial.println(panTiltX.midAngle);
  Serial.println();
  Serial.print(F("Y min Value: "));
  Serial.println(panTiltY.minAngle);
  Serial.print(F("Y max Value: "));
  Serial.println(panTiltY.maxAngle);
  Serial.print(F("Y mid Value: "));
  Serial.println(panTiltY.midAngle);
  Serial.println();

*/
  Serial.println("setup complete");


}



void loop() {
  int changeVal;
  delay(50);
  changeVal = getMarkovSpeed(changeVal);

  panTiltX.angle = getDeltaPosition(&panTiltX, changeVal, DIRECTION_CHANGE_PROBABILITY) + panTiltX.angle;



  panTiltY.angle = getDeltaPosition(&panTiltY, changeVal, DIRECTION_CHANGE_PROBABILITY) + panTiltY.angle;


  panTilt.updateAngles();

  if(random(1001) < 15){
    delay(markovPause());

  }


  if(random(10001) < 2){
    laser.fire(0);
    delay(random(1000, 5000));

  }

  else if(random(100001) < 10){
    laser.fire(0);
    panTilt.detach();
    int delayVal = 60000 * random(5, 30);
    delay(delayVal);
    panTilt.begin();
  }

  laser.fire(1);

}




int getMarkovDirection(panTiltPos_t *pt, int changeProb){
  int prob = changeProb;

  if(pt->dir == 0){
    pt->dir = 1;
  }



  if((pt->dir == 1 && pt->angle >= pt->midAngle) || (pt->dir == -1 && pt->angle <= pt->midAngle)){
    prob += abs(pt->midAngle - pt->angle);
/*
    Serial.print(F("Probability: "));
    Serial.println(prob);
*/
  }

  if(random(1001) <= prob << 1 || (pt->angle >= pt->maxAngle && pt->dir == 1) || pt->angle <= pt->minAngle && pt->dir == -1){
    pt->dir *= -1;
    Serial.println(F("##############CHANGE#################"));
    Serial.println(pt->dir);

  }

  return pt->dir;
}

int getDeltaPosition(panTiltPos_t *pt, int funcChangeVal, int changeProb){
  int tempVal = getMarkovDirection(pt, changeProb);

  tempVal *=funcChangeVal;
  Serial.print(F("Delta: "));
  Serial.println(tempVal);
  Serial.println();

  return tempVal;

}

int getMarkovSpeed(int oldSpeed){
  int val = random(101);

  if(oldSpeed == 1){
    if(val < 20){
      return 2;
    }
    else{
      return 1;
    }
  }

  else if(oldSpeed == 2){
    if(val < 20){
      return 3;
    }
    else if(val < 70){
      return 2;
    }
    else{
      return 1;
    }
  }

  else if(oldSpeed == 3){
    if(val < 20){
      return 2;
    }
    else{
      return 3;
    }
  }

  else{
    return constrain(oldSpeed, 1, 3);
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
