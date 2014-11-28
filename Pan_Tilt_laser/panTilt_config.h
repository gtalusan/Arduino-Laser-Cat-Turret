#pragma once

#define BAUD_RATE 115200


#include <Arduino.h>

#include <Servo.h>
#include <math.h>
#include "stuServo.h"
#include "stuPanTilt.h"
#include "stuLaser.h"
#include "stu_scheduler.h"
#include "missileswitch.h"
#include "stu_gauss.h"
#include <Gaussian.h>
#include "stutility.h"
#include <assert.h>
#include "stuMarkov.h"
#include "stu_dial.h"
#include "stu_display.h"

using namespace stu;

#define MS_SWITCH_PIN 3
#define MS_LED_PIN 4

#define LASER_PIN 5



#define SERVO_X_PIN A0
#define SERVO_Y_PIN A1
#define DIAL_PIN A2

#define DIRECTION_CHANGE_PROBABILITY 15
