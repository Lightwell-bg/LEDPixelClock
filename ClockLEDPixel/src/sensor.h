void init_sensor() {
#if USE_DHT == true
  dht.begin(); 
#endif //DHT
#if USE_BME280 == true // I2C D1, D2
  bme280.parameter.communication = 0;                  //Choose communication protocol
  bme280.parameter.I2CAddress = 0x76;                  //Choose I2C Address 0x77
  //0b00:     In sleep mode no measurements are performed, but power consumption is at a minimum
  //0b01:     In forced mode a single measured is performed and the device returns automatically to sleep mode
  //0b11:     In normal mode the sensor measures continually (default value)
  bme280.parameter.sensorMode = 0b11; 
  bme280.parameter.IIRfilter = 0b100;                    //Setup for IIR Filter
  bme280.parameter.humidOversampling = 0b101;            //Setup Humidity Oversampling
  bme280.parameter.tempOversampling = 0b101;             //Setup Temperature Ovesampling
  bme280.parameter.pressOversampling = 0b101;            //Setup Pressure Oversampling 
  bme280.parameter.pressureSeaLevel = 1013.25;           //default value of 1013.25 hPa
  bme280.parameter.tempOutsideCelsius = 15;              //default value of 15°C
  uint8_t tries = 10;
  while (--tries && bme280.init() != 0x60) {
    Serial.println(F("Ops! BME280 could not be found! Please check your connections."));
    delay(500);
  }
  if (tries > 0) {
    Serial.println(F("BME280 detected!"));
  }
#endif //BME280 
}

#if USE_DHT == true
String getTempDHT() {
  float h = dht.readHumidity(); 
  float t = dht.readTemperature(); 
  if (isnan(t) || isnan(h))  {
    Serial.println("Failed to read Temp from DHT");
    return "";
  }
  else {
    // Compute heat index in Celsius (isFahreheit = false)
    float hic = dht.computeHeatIndex(t, h, false);
    String tempDHT = String(hic, 1); 
    Serial.print("Temperature: "); Serial.print(tempDHT); Serial.println("°C");     
    return tempDHT;      
  }
}

String getHumDHT() {
  float h = dht.readHumidity(); 
  if (isnan(h))  {
    Serial.println("Failed to read Hum from DHT");
    return "";
  }
  else {
    // Compute heat index in Celsius (isFahreheit = false)
    String humDHT = String(h, 0); 
    Serial.print("Humidity: "); Serial.print(humDHT); Serial.println("%");     
    return humDHT;      
  }
}
#endif //DHT

#if USE_BME280 == true // I2C D1, D2
int getTempBME280() {
  float tempBME = bme280.readTempC()*10;
  Serial.print("Temperature: "); Serial.print(tempBME/10); Serial.println("°C"); 
  return (int)round(tempBME);
}

String getStrTempBME280() {
  float tempBME = bme280.readTempC();
  Serial.print("Temperature: "); Serial.print(tempBME); Serial.println("°C"); 
  return String(tempBME, 1);
}

int getHumBME280() {
  float humBME = bme280.readHumidity();
  Serial.print("Humidity: "); Serial.print(humBME); Serial.println("%"); 
  return (int)round(humBME);  
}

String getStrHumBME280() {
  float humBME = bme280.readHumidity();
  Serial.print("Humidity: "); Serial.print(humBME); Serial.println("%"); 
  return String(humBME, 0);  
}

int getPressBME280() {
  float pressBME = bme280.readPressure();
  Serial.print(F("Pressure "));Serial.print(pressBME);Serial.println(" hPa");
  Serial.print(F("Pressure "));Serial.print(pressBME*0.750062);Serial.println(" mm");
  Serial.print(F("Altitude in Meters:\t\t")); Serial.println(bme280.readAltitudeMeter());
  return (int)round(pressBME*0.750062);  
}

String getStrPressBME280() {
  float pressBME = bme280.readPressure();
  Serial.print(F("Pressure "));Serial.print(pressBME);Serial.println(" hPa");
  Serial.print(F("Pressure "));Serial.print(pressBME*0.750062);Serial.println(" mm");
  Serial.print(F("Altitude in Meters:\t\t")); Serial.println(bme280.readAltitudeMeter());
  return String(pressBME*0.750062,0);  
}
#endif //BME280 