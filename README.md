# LineFollowerArduino 
English Below
1. Bước calib trước khi chạy là quan trọng, do đó cần phải calib cảm biến trước. Trước tiên cho manualCalibration = 0 và bắt đầu trượt cảm biến qua các vạch đen trong vòng 10s khi đèn LED-BUILTIN của arduino bắt đầu sáng. Mở Serial lên và xem các giá trị max và min tương ứng.

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

Gán các giá trị calib vào dòng lệnh :
        uint16_t minVal[5] = {444,420,416,412,352};
        uint16_t maxVal[5] = {2500,2500,2500,2500,2500};
và cho manualCalibration = 1
2. Tuỳ theo cấu hình robot (Tốc độ quay của động cơ, đường kính bánh xe, khoảng cách lái, ...) mà có cách chọn thông số PID cho phù hợp. Do mô hình đã có khâu tích phna6 lý tưởng nên robot chỉ cần bộ PD là điều khiển được.  Bộ điều khiển PID đơn giản được trình bày như sau:
  uint16_t position = qtr.readLineBlack(sensorValues);
  int error = position - 2000;
  PID_out = 0.1 * error + 0.001 * (error - lastError)/0.01;
  lastError = error;
  int Speed0 = 250;
  int Left_Out = max(min(Speed0 + PID_out,255),-255);
  int Right_Out = max(min(Speed0 - PID_out,255),-255);
  
 trong đó out = max(min(x,255),255) là khâu bão hoà ngỏ ra:
 {
 out =  255, nếu x>= 255; 
 out = -255, nếu x<=-255;
 out =    x, các trường hợp còn lại   
 }
