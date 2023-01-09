#include "./MQ135.h"
MQ135::MQ135(float Voltage_Resolution, int ADC_Bit_Resolution, int pin)
    : MQx(Voltage_Resolution, ADC_Bit_Resolution, pin) {}
float MQ135::readCO() { return this->readSensor(MQ135_CO_a, MQ135_CO_b, 0); }
float MQ135::readAlcohol() {
  return this->readSensor(MQ135_Alcohol_a, MQ135_Alcohol_b, 0);
}
float MQ135::readCO2() { return this->readSensor(MQ135_CO2_a, MQ135_CO2_b, 0); }
float MQ135::readToluen() {
  return this->readSensor(MQ135_Toluen_a, MQ135_Toluen_b, 0);
}
float MQ135::readNH4() { return this->readSensor(MQ135_NH4_a, MQ135_NH4_b, 0); }
float MQ135::readAceton() {
  return this->readSensor(MQ135_Aceton_a, MQ135_Aceton_b, 0);
}