#include "./MQ2.h"
MQ2::MQ2(float Voltage_Resolution, int ADC_Bit_Resolution, int pin)
    : MQx(Voltage_Resolution, ADC_Bit_Resolution, pin) {}
float MQ2::readCO() {
  return this->readSensor(MQ2_H2_a, MQ2_H2_b, 0);
}