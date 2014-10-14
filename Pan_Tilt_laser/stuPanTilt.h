#ifndef _STUPANTILT_H_
#define _STUPANTILT_H_

#include "stuServo.h"
#include <Servo.h>
#include "Arduino.h"
#include <math.h>

#define DEFAULT_MIN 5
#define DEFAULT_MAX 170

struct panTiltPos_t {
  int
    pos,
    angle,
    dir;

  const int
    minAngle,
    maxAngle,
    midOffset,
    midAngle;


  panTiltPos_t(int p, int mn, int mx, int mdOff = 0): pos(p), dir(1), minAngle(mn), maxAngle(mx), midOffset(mdOff), midAngle(((mx-mn) >>1) + mn + midOffset){}

};

struct point_t{
  int
    x,
    y;

  point_t(int X=0, int Y=0):x(X), y(Y){}
};

class PanTilt {

public:

  PanTilt(uint8_t xPin, panTiltPos_t *xPos, uint8_t yPin, panTiltPos_t *yPos,  int height);

  void
    begin(),
    detach(),
    setPosition(int, int),
    updatePosition(),     //absolute
    updateAngles(),
    rMove(int dX, int dY); //relative

  int
    getHeight();



private:

  StuServo
    _xServo,
    _yServo;

  uint8_t
    _xPin,
    _yPin;

  void
    _setAnglesFromPosition(),
    _setPositionFromAngles(),
    _update();

  float
    _rad2Deg(float),
    _deg2Rad(float);

  panTiltPos_t
    *_Xpos,
    *_Ypos;


  const int
    _height; //in cm


};



#endif
