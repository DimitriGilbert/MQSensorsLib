#include "./MQ7.h"
MQ7::MQ7(float Voltage_Resolution, int ADC_Bit_Resolution, int pin)
    : MQx(Voltage_Resolution, ADC_Bit_Resolution, pin) {}
float MQ7::readCO() {
  return this->readSensor(MQ7_CO_a, MQ7_CO_b, 0);
}