#include "./MQ2.h"
float MQ2::readH2() {
    return this->readSensor(MQ2_H2_a, MQ2_H2_b, 0)
}
float MQ2::readLPG() {
    return this->readSensor(MQ2_LPG_a, MQ2_LPG_b, 0)
}
float MQ2::readCO() {
    return this->readSensor(MQ2_CO_a, MQ2_CO_b, 0)
}
float MQ2::readAlcohol() {
    return this->readSensor(MQ2_Alcohol_a, MQ2_Alcohol_b, 0)
}
float MQ2::readPropane() {
    return this->readSensor(MQ2_Propane_a, MQ2_Propane_b, 0)
}