#ifndef MQ2_h
#define MQ2_h

#include <Arduino.h>
#include "./MQx.h"

#define MQ7_CO_a 19.32
#define MQ7_CO_b -0.64

class MQ7 : public MQx {
 public:
  MQ7(float Voltage_Resolution, int ADC_Bit_Resolution, int pin);
  float readCO();
};

#endif
