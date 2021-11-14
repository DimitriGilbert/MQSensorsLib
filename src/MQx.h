#ifndef MQx_h
#define MQx_h

#include <Arduino.h>
#include <stdint.h>

#define MQx_retries 2
#define MQx_retry_interval 20

#define MQx_regression_method_exponential 0
#define MQx_regression_method_linear 1

class MQx {
 public:
  MQx(float Voltage_Resolution = 5, int ADC_Bit_Resolution = 10, int pin = 1);
  
  byte pin;
  byte firstFlag = false;
  // if 3.3v use 3.3
  byte voltage_resolution = 5.0;
  // Value in KiloOhms
  byte RL = 10;
  byte ADC_Bit_Resolution = 10;
  byte regressionMethod = 0;

  float adc, a, b, sensor_volt;
  float R0, RS_air, ratio, PPM, RS_Calc;


  // Functions to set values
  void init();
  void update();
  void setR0(float R0 = 10);
  void setRL(float RL = 10);
  void setA(float a);
  void setB(float b);
  void setRegressionMethod(int regressionMethod);
  void setVoltResolution(float voltage_resolution = 5);
  void serialDebug(
      bool onSetup = false);  // Show on serial port information about sensor
  void setADC(int value);     // For external ADC Usage

  // user functions
  float calibrate(float ratioInCleanAir);
  float readSensor();
  float readSensor(float a, float b);
  float readSensor(float a, float b, int regressionMethod);
  float readSensorR0Rs();
  float validateEcuation(float ratioInput = 0);

  // get function for info
  float getA();
  float getB();
  float getR0();
  float getRL();
  float getVoltResolution();
  String getRegressionMethod();

 private:
  /************************Private vars************************************/
};

#endif  // MQx
