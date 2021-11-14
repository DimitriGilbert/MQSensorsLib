#include "MQx.h"

MQx::MQx(float Voltage_Resolution, int ADC_Bit_Resolution, int pin) {
  this->pin = pin;
  this->voltage_resolution = Voltage_Resolution;
  this->ADC_Bit_Resolution = ADC_Bit_Resolution;
}
void MQx::init()
{
  pinMode(pin, INPUT);
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
void MQx::setADC(int value)
{
  this->sensor_volt = (value) * this->voltage_resolution / ((pow(2, this->ADC_Bit_Resolution)) - 1); 
  this->adc =  value;
}
void MQx::setVoltResolution(float voltage_resolution)
{
  this->voltage_resolution = voltage_resolution;
}
void MQx::setRegressionMethod(int regressionMethod)
{
  this->regressionMethod = regressionMethod;
}
float MQx::getR0() {
  return this->R0;
}
float MQx::getRL() {
  return this->RL;
}
float MQx::getVoltResolution()
{
  return this->voltage_resolution;
}
String MQx::getRegressionMethod()
{
  if(this->regressionMethod == 0) return "Exponential";
  else return "Linear";
}
void MQx::serialDebug(bool onSetup)
{
  if(onSetup)
  {
    Serial.println();
    Serial.println("************************************************************************************************************************************************");
    Serial.println("MQ sensor reading library for arduino");

    Serial.println("Note: remember that all the parameters below can be modified during the program execution with the methods:");
    Serial.println("setR0, setRL, setA, setB where you will have to send as parameter the new value, example: mySensor.setR0(20); //R0 = 20KΩ");

    Serial.println("Authors: Miguel A. Califa U - Yersson R. Carrillo A - Ghiordy F. Contreras C");
    Serial.println("Contributors: Andres A. Martinez - Juan A. Rodríguez - Mario A. Rodríguez O ");

    Serial.print("Sensor: "); Serial.println(_type);
    Serial.print("Supply voltage: "); Serial.print(this->voltage_resolution); Serial.println(" VDC");
    Serial.print("ADC Resolution: "); Serial.print(this->ADC_Bit_Resolution); Serial.println(" Bits");
    Serial.print("R0: "); Serial.print(this->R0); Serial.println(" KΩ");
    Serial.print("RL: "); Serial.print(this->RL); Serial.println(" KΩ");

    Serial.print("Model: "); if(this->regressionMethod == 1) Serial.println("Exponential"); else Serial.println("Linear");
    Serial.print(_type); Serial.print(" -> a: "); Serial.print(this->a); Serial.print(" | b: "); Serial.println(this->b);

    Serial.print("Development board: "); Serial.println(_placa);
  }
  else 
  {
    if(!_firstFlag)
    {
      Serial.print("| ********************************************************************"); Serial.print(_type); Serial.println("*********************************************************************|");
      Serial.println("|ADC_In | Equation_V_ADC | Voltage_ADC |        Equation_RS        | Resistance_RS  |    EQ_Ratio  | Ratio (RS/R0) | Equationthis->PPM |     PPM    |");
      _firstFlag = true;  //Headers are printed
    }
    else
    {
      Serial.print("|"); Serial.print(this->adc);  Serial.print("| v = ADC*"); Serial.print(this->voltage_resolution); Serial.print("/"); Serial.print((pow(2, this->ADC_Bit_Resolution)) - 1); Serial.print("  |    "); Serial.print(this->sensor_volt);
      Serial.print("     | RS = ((" ); Serial.print(this->voltage_resolution ); Serial.print("*RL)/Voltage) - RL|      "); Serial.print(this->RS_Calc); Serial.print("     | Ratio = RS/R0|    ");
      Serial.print(this->ratio);  Serial.print( "       |   ");
      if(this->regressionMethod == 1) Serial.print("ratio*a + b");
      else Serial.print("pow(10, (log10(ratio)-b)/a)");
      Serial.print("  |   "); Serial.print(this->PPM); Serial.println("  |");
    }
  }
}
void MQx::update()
{
  this->sensor_volt = this->getVoltage();
}
float MQx::readSensor()
{
  this->update();
  //More explained in: https://jayconsystems.com/blog/understanding-a-gas-sensor
  this->RS_Calc = ((this->voltage_resolution*this->RL)/this->sensor_volt)-this->RL; //Get value of RS in a gas
  if(this->RS_Calc < 0)  {this->RS_Calc = 0}; //No negative values accepted.
  this->ratio = this->RS_Calc / this->R0;   // Get ratio RS_gas/RS_air
  if(this->ratio <= 0)  {this->ratio = 0}; //No negative values accepted or upper datasheet recomendation.
  if(this->regressionMethod == 1) {
    this->PPM= this->a*pow(this->ratio, this->b)
  }; // <- Source excel analisis https://github.com/miguel5612/MQSensorsLib_Docs/tree/master/Internal_design_documents
  else {
    // https://jayconsystems.com/blog/understanding-a-gas-sensor <- Source of linear ecuation
    double ppm_log = (log10(this->ratio)-this->b)/this->a; //Get ppm value in linear scale according to the the ratio value  
    this->PPM = pow(10, ppm_log); //Convert ppm value to log scale  
  }
  if(this->PPM < 0)  {this->PPM = 0}; //No negative values accepted or upper datasheet recomendation.
  return this->PPM;
}
float MQx::readSensor(float a, float b, int regressionMethod = 0) {
  this->setA(a);
  this->setB(b);
  this->setRegressionMethod(regressionMethod);
  return this->readSensor();
}
float MQx::readSensorR0Rs()
{
  //More explained in: https://jayconsystems.com/blog/understanding-a-gas-sensor
  this->RS_Calc = ((this->voltage_resolution*this->RL)/this->sensor_volt)-this->RL; //Get value of RS in a gas
  if(this->RS_Calc < 0)  this->RS_Calc = 0; //No negative values accepted.
  this->ratio = this->R0/this->RS_Calc;   // Get ratio RS_air/RS_gas <- INVERTED for MQ-131 issue 28 https://github.com/miguel5612/MQSensorsLib/issues/28
  if(this->ratio <= 0)  this->ratio = 0; //No negative values accepted or upper datasheet recomendation.
  if(this->regressionMethod == 1) this->PPM= this->a*pow(this->ratio, this->b); // <- Source excel analisis https://github.com/miguel5612/MQSensorsLib_Docs/tree/master/Internal_design_documents
  else 
  {
    // https://jayconsystems.com/blog/understanding-a-gas-sensor <- Source of linear ecuation
    double ppm_log = (log10(this->ratio)-this->b)/this->a; //Get ppm value in linear scale according to the the ratio value  
    this->PPM = pow(10, ppm_log); //Convert ppm value to log scale  
  }
  if(this->PPM < 0)  this->PPM = 0; //No negative values accepted or upper datasheet recomendation.
  //if(this->PPM > 10000) this->PPM = 99999999; //No negative values accepted or upper datasheet recomendation.
  return this->PPM;
}
float MQx::calibrate(float ratioInCleanAir) {
  //More explained in: https://jayconsystems.com/blog/understanding-a-gas-sensor
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
  float RS_air; //Define variable for sensor resistance
  float R0; //Define variable for R0
  RS_air = ((this->voltage_resolution*this->RL)/this->sensor_volt)-this->RL; //Calculate RS in fresh air
  if(RS_air < 0)  RS_air = 0; //No negative values accepted.
  R0 = RS_air/ratioInCleanAir; //Calculate R0 
  if(R0 < 0)  R0 = 0; //No negative values accepted.
  return R0;
}
float MQx::getVoltage(int read) {
  float voltage;
  if(read)
  {
    float avg = 0.0;
    for (int i = 0; i < MQx_retries; i ++) {
      this->adc = analogRead(this->pin);
      avg += this->adc;
      delay(MQx_retry_interval);
    }
    voltage = (avg/ MQx_retries) * this->voltage_resolution / ((pow(2, this->ADC_Bit_Resolution)) - 1);
  }
  else
  {
    voltage = this->sensor_volt;
  }
  return voltage;
}