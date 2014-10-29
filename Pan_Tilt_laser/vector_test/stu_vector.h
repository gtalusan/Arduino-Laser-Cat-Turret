
/**************************************************************************/
/*!
    @file     stu_vector.h
    @author   Stuart Feichtinger
    @license  MIT (see license.txt)

    Simple library for working with vectors. I got the idea from the
    awesome book The Nature of Code: Simulating Natural Systems with
    Processing by Daniel Shiffman http://natureofcode.com. You can
    download it for free, but it's definitely worth buying!
    (http://www.amazon.com/gp/product/0985930802)


    @section  HISTORY
    v1.0 - First release

*/
/**************************************************************************/

#ifndef _STU_VECTOR_H_
#define _STU_VECTOR_H_

#if(ARDUINO > 100)
#include "Arduino.h"
#include <math.h>
#endif

class SVector {

public:

  SVector(int x, int y);

  SVector
    add(int x, int y),
    add(SVector *),
    sub(int x, int y),
    sub(SVector *);

  int
    x(),
    y();

  float
    getMag();

private:
  int
    _x,
    _y;

};



#endif
