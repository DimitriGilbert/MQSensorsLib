#ifndef MQ2
#define MQ2

#include <Arduino.h>
#include "./MQx.h"

#define MQ2_
#define MQ2_H2_a 987.99
#define MQ2_H2_b -2.162
#define MQ2_LPG_a 574.25
#define MQ2_LPG_b -2.222
#define MQ2_CO_a 36974
#define MQ2_CO_b -3.109
#define MQ2_Alcohol_a 3616.1
#define MQ2_Alcohol_b -2.675
#define MQ2_Propane_a 658.71
#define MQ2_Propane_b -2.168

class MQ2 ; public MQx {
public:
  float readH2();
  float readLPG();
  float readCO();
  float readAlcohol();
  float readPropane();
}
