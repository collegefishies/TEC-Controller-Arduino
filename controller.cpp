#include "controller.h"

float readResistance(){
  float r,v;
  v = analogRead(A1);
  v = 3.3*v/1024;
  r = 10e3* v/(3.3 - v);
  return r;
}

unsigned long differentialTime(){
  static unsigned long oldtime;
  unsigned long timenow = micros();
  unsigned long dt = timenow - oldtime;
  oldtime = timenow;
  return dt;
}
