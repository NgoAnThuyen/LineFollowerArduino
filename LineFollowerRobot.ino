#include <QTRSensors.h>

QTRSensors qtr;

int IN1 = 3; 
int IN2 = 5;  // Right motor 

int IN3 = 10; 
int IN4 = 11;  // Left motor 

const uint8_t SensorCount = 5;
int PID_out;
uint16_t sensorValues[SensorCount];
bool manualCalibration = 1;
int  lastError;
void setup()
{
  pinMode(IN1, OUTPUT);  // sets the pin as output
  pinMode(IN2, OUTPUT);  // sets the pin as output
  pinMode(IN3, OUTPUT);  // sets the pin as output
  pinMode(IN4, OUTPUT);  // sets the pin as output

  // configure the sensors
  qtr.setTypeRC();
  qtr.setSensorPins((const uint8_t[]){A0, A1, A2, A3, A4}, SensorCount);
  qtr.setEmitterPin(2);

  delay(500);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); // turn on Arduino's LED to indicate we are in calibration mode
  Serial.begin(115200);
if (manualCalibration)
{
  uint16_t minVal[5] = {444,420,416,412,352};
  uint16_t maxVal[5] = {2500,2500,2500,2500,2500};
  qtr.calibrationOn.initialized = true;
  qtr.calibrationOn.minimum = (uint16_t *)realloc(qtr.calibrationOn.minimum,sizeof(uint16_t) * 5);
  qtr.calibrationOn.maximum = (uint16_t *)realloc(qtr.calibrationOn.maximum,sizeof(uint16_t) * 5);
  for (uint8_t i = 0; i < SensorCount; i++)
  {
    qtr.calibrationOn.minimum[i] = minVal[i];
    qtr.calibrationOn.maximum[i] = maxVal[i];
  }
}
else
{
  // 2.5 ms RC read timeout (default) * 10 reads per calibrate() call
  // = ~25 ms per calibrate() call.
  // Call calibrate() 400 times to make calibration take about 10 seconds.
  for (uint16_t i = 0; i < 400; i++)
  {
    qtr.calibrate();
  }
}
  digitalWrite(LED_BUILTIN, LOW); // turn off Arduino's LED to indicate we are through with calibration
  
  // print the calibration minimum values measured when emitters were on

  Serial.print("Minimum: ");
  for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(qtr.calibrationOn.minimum[i]);
    Serial.print('\t');
  }
  Serial.println();
  //print the calibration maximum values measured when emitters were on
  Serial.print("Maximun: ");
  for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(qtr.calibrationOn.maximum[i]);
    Serial.print('\t');
  }

  Serial.println();

  delay(1000);
}

void loop()
{
  uint16_t position = qtr.readLineBlack(sensorValues);
  int error = position - 2000;
  PID_out = 0.1 * error + 0.001 * (error - lastError)/0.01;
  lastError = error;
  
  int Speed0 = 250;
  int Left_Out = max(min(Speed0 + PID_out,255),-255);
  int Right_Out = max(min(Speed0 - PID_out,255),-255);
  if (Left_Out >= 0)
  {
    Motor_Left(Left_Out,1);
  }
  else 
  {
    Motor_Left(abs(Left_Out),0);
  }
  if (Right_Out >=0)
  {
    Motor_Right(Right_Out,1);
  }
  else
  {
    Motor_Right(abs(Right_Out),0);
  }
  
    delay(10);
  }
  //Serial.print("SENSOR VALUE = ");
  //Serial.print(position);
  //Serial.print(" PID OUT = ");
  //Serial.println(PID_out);


void Motor_Right(int turnspeed, bool direction) // 1 forward, 0 reverse
{
  if (direction)
  {
    analogWrite(IN1 , 0);
    analogWrite(IN2 , turnspeed);
  }
  else
  {
    analogWrite(IN1 , turnspeed);
    analogWrite(IN2 , 0);
  }
}

void Motor_Left(int speed, bool direction) // 1 forward, 0 reverse
{
  if (direction)
  {
    analogWrite(IN3 , speed);
    analogWrite(IN4 , 0);
  }
  else
  {
    analogWrite(IN3 , 0);
    analogWrite(IN4 , speed);
  }
}

void Turn_Right(int speed)
{
  Motor_Right(speed,1);
  Motor_Left(speed,0);
}

void Turn_Left(int speed)
{
  Motor_Right(speed,0);
  Motor_Left(speed,1);
}
