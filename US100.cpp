//*********************************************************
//-- US100: Library(cpp file)
//*********************************************************
#include "US100.h"
#include "Arduino.h"

void US100::init(int trig,int echo){
  _trig = trig;
  _echo = echo;
  pinMode(_trig, OUTPUT);
  pinMode(_echo, INPUT);
}

float US100::getDistance(){
    digitalWrite(_trig, LOW);
    delayMicroseconds(2);
    digitalWrite(_trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(_trig, LOW);
    // Compute distance
    float dist = pulseIn(_echo, HIGH);
    dist = dist / 58.2;
    return(dist);
}
