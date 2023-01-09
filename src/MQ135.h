#ifndef MQ135_h
#define MQ135_h

#include <Arduino.h>
#include "./MQx.h"

#define MQ135_CO_a 605.18
#define MQ135_CO_b -3.937
#define MQ135_Alcohol_a 77.255
#define MQ135_Alcohol_b -3.18
#define MQ135_CO2_a 110.47
#define MQ135_CO2_b -2.862
#define MQ135_Toluen_a 44.947
#define MQ135_Toluen_b -3.445
#define MQ135_NH4_a 102.2
#define MQ135_NH4_b -2.473
#define MQ135_Aceton_a 34.668
#define MQ135_Aceton_b -3.369

class MQ135 : public MQx {
 public:
  MQ135(float Voltage_Resolution, int ADC_Bit_Resolution, int pin);
  float readCO();
  float readAlcohol();
  float readCO2();
  float readToluen();
  float readNH4();
  float readAceton();
};

#endif
