#include "MQx.h"

MQx::MQx(float Voltage_Resolution, int ADC_Bit_Resolution, int pin) {
  this->pin = pin;
  this->voltage_resolution = Voltage_Resolution;
  this->ADC_Bit_Resolution = ADC_Bit_Resolution;
}
void MQx::init() {
  pinMode(pin, INPUT);
  float calcR0 = 0;
  for (int i = 1; i <= 10; i++) {
    this->update();
    calcR0 += this->calibrate(9.83);
  }
  this->setR0(calcR0 / 10);
}
void MQx::setA(float a) {
  this->a = a;
}
void MQx::setB(float b) {
  this->b = b;
}
void MQx::setR0(float R0) {
  this->R0 = R0;
}
void MQx::setRL(float RL) {
  this->RL = RL;
}
void MQx::setADC(int value) {
  this->sensor_volt = (value) * this->voltage_resolution /
                      ((pow(2, this->ADC_Bit_Resolution)) - 1);
  this->adc = value;
}
void MQx::setVoltResolution(float voltage_resolution) {
  this->voltage_resolution = voltage_resolution;
}
void MQx::setRegressionMethod(int regressionMethod) {
  this->regressionMethod = regressionMethod;
}
float MQx::getR0() {
  return this->R0;
}
float MQx::getRL() {
  return this->RL;
}
float MQx::getVoltResolution() {
  return this->voltage_resolution;
}
String MQx::getRegressionMethod() {
  if (this->regressionMethod == 0)
    return "Exponential";
  else
    return "Linear";
}
void MQx::serialDebug(bool onSetup) {
  if (onSetup) {
    Serial.print("Supply voltage: ");
    Serial.print(this->voltage_resolution);
    Serial.println("");
    Serial.print("ADC Resolution: ");
    Serial.print(this->ADC_Bit_Resolution);
    Serial.println(" Bits");
    Serial.print("R0: ");
    Serial.print(this->R0);
    Serial.println(" KΩ");
    Serial.print("RL: ");
    Serial.print(this->RL);
    Serial.println(" KΩ");

    Serial.print("Model: ");
    if (this->regressionMethod == MQx_regression_method_exponential)
      Serial.println("Exponential");
    else
      Serial.println("Linear");
    Serial.print(" -> a: ");
    Serial.print(this->a);
    Serial.print(" | b: ");
    Serial.println(this->b);
  } else {
    Serial.print("adc: ");
    Serial.println(this->adc);
    Serial.print("voltage resolution : ");
    Serial.println(this->voltage_resolution);
    Serial.print("ADC bit resolution : ");
    Serial.println(this->ADC_Bit_Resolution);
    Serial.print("sensor voltage : ");
    Serial.println(this->sensor_volt);
    Serial.print("RS : ");
    Serial.println(this->RS_Calc);
    Serial.print("ratio : ");
    Serial.println(this->ratio);
  }
}
void MQx::update() {
  float avg = 0.0;
  for (int i = 0; i < MQx_retries; i++) {
    this->adc = analogRead(this->pin);
    avg += this->adc;
    delay(MQx_retry_interval);
  }
  this->sensor_volt = (avg / MQx_retries) * this->voltage_resolution /
                      ((pow(2, this->ADC_Bit_Resolution)) - 1);
}
float MQx::readSensor() {
  this->update();
  // More explained in:
  // https://jayconsystems.com/blog/understanding-a-gas-sensor
  // Get value of RS in a gas
  this->RS_Calc =
      ((this->voltage_resolution * this->RL) / this->sensor_volt) - this->RL;
  // No negative values accepted.
  if (this->RS_Calc < 0) {
    this->RS_Calc = 0;
  }
  // Get ratio RS_gas/RS_air
  this->ratio = this->RS_Calc / this->R0;
  // No negative values accepted or upper datasheet recomendation.
  if (this->ratio <= 0) {
    this->ratio = 0;
  }
  if (this->regressionMethod == 0) {
    this->PPM = this->a * pow(this->ratio, this->b);
  }
  // <- Source excel analisis
  // https://github.com/miguel5612/MQSensorsLib_Docs/tree/master/Internal_design_documents
  else {
    // https://jayconsystems.com/blog/understanding-a-gas-sensor <- Source of
    // linear ecuation
    // Get ppm value in linear scale according to the ratio value
    double ppm_log = (log10(this->ratio) - this->b) / this->a;
    // Convert ppm value to log scale
    this->PPM = pow(10, ppm_log);
  }
  // No negative values accepted or upper datasheet recomendation.
  if (this->PPM < 0) {
    this->PPM = 0;
  };
  return this->PPM;
}
float MQx::readSensor(float a, float b) {
  return this->readSensor(a, b, 0);
}
float MQx::readSensor(float a, float b, int regressionMethod) {
  this->setA(a);
  this->setB(b);
  this->setRegressionMethod(regressionMethod);
  return this->readSensor();
}
float MQx::readSensorR0Rs() {
  // More explained in:
  // https://jayconsystems.com/blog/understanding-a-gas-sensor Get value of RS
  // in a gas
  this->RS_Calc =
      ((this->voltage_resolution * this->RL) / this->sensor_volt) - this->RL;
  // No negative values accepted.
  if (this->RS_Calc < 0) {
    this->RS_Calc = 0;
  }
  // Get ratio RS_air/RS_gas <- INVERTED for MQ-131 issue 28
  // https://github.com/miguel5612/MQSensorsLib/issues/28
  this->ratio = this->R0 / this->RS_Calc;
  // No negative values accepted or upper datasheet recomendation.
  if (this->ratio <= 0) {
    this->ratio = 0;
  }
  // <- Source excel analisis
  // https://github.com/miguel5612/MQSensorsLib_Docs/tree/master/Internal_design_documents
  if (this->regressionMethod == 1) {
    this->PPM = this->a * pow(this->ratio, this->b);
  }
  // https://jayconsystems.com/blog/understanding-a-gas-sensor <- Source of
  // linear ecuation
  else {
    double ppm_log = (log10(this->ratio) - this->b) /
                     this->a;  // Get ppm value in linear scale according to the
                               // the ratio value
    this->PPM = pow(10, ppm_log);  // Convert ppm value to log scale
  }
  // No negative values accepted or upper datasheet recomendation.
  if (this->PPM < 0) {
    this->PPM = 0;
  }
  return this->PPM;
}
float MQx::calibrate(float ratioInCleanAir) {
  // More explained in:
  // https://jayconsystems.com/blog/understanding-a-gas-sensor
  /*
  V = I x R
  VRL = [VC / (RS + RL)] x RL
  VRL = (VC x RL) / (RS + RL)
  Así que ahora resolvemos para RS:
  VRL x (RS + RL) = VC x RL
  (VRL x RS) + (VRL x RL) = VC x RL
  (VRL x RS) = (VC x RL) - (VRL x RL)
  RS = [(VC x RL) - (VRL x RL)] / VRL
  RS = [(VC x RL) / VRL] - RL
  */
  // Define variable for sensor resistance
  float RS_air;
  // Define variable for R0
  float R0;
  // Calculate RS in fresh air
  RS_air =
      ((this->voltage_resolution * this->RL) / this->sensor_volt) - this->RL;

  // No negative values accepted.
  if (RS_air < 0) {
    RS_air = 0;
  }

  // Calculate R0
  R0 = RS_air / ratioInCleanAir;
  // No negative values accepted.
  if (R0 < 0) {
    R0 = 0;
  }
  return R0;
}
