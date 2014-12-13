/**************************************************************************/
/*!
    @file     stu_gauss.cpp
    @author   Stuart Feichtinger
    @license  MIT (see license.txt)

    Wrapper library for Gaussian library
    (https://github.com/ivanseidel/Gaussian) to create random number
    generator with custom distribution.


    @section  HISTORY
    v0.0.1 - First release

*/
/**************************************************************************/

#include "stu_gauss.h"


StuGauss gauss;

StuGauss::StuGauss():_gauss(){


}

unsigned long StuGauss::gRandom(unsigned long zero, double variance){
  _gauss.setVariance(variance);
  _gauss.setMean(zero);
  double temp = _gauss.random();

  #ifdef GAUSS_DEBUG
    MY_SERIAL.println(temp);
  #endif

  return round(max(temp, 2));
}
