void printTime(struct tm *tnow);
void dataToArray(int data, uint32_t ledColor = offColor, edataArrType showType = STIME, uint8_t dot = 0);
void print2digits(uint8_t number);
uint32_t colorStripRGB(uint8_t* mcolor);

void init_LStrip() {
    strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
    //strip.show();            // Turn OFF all pixels ASAP
    strip.clear();
    strip.setBrightness(brightd); // Set BRIGHTNESS to about 1/5 (max = 255)
}

void showTime() {
    static uint8_t lastSec= 66;
    struct tm tnow = myESPTime.getTimeStruct();
    if (lastSec != tnow.tm_sec) {
      printTime(&tnow);
      int time1000 = tnow.tm_hour*100 + tnow.tm_min;
      uint8_t Dot;
      if (tnow.tm_sec % 2 == 0) Dot = 0; else Dot = 2; 
      dataToArray(time1000, colorStripRGB(&clockColor), STIME, Dot);
      strip.show(); 
      lastSec = tnow.tm_sec;
    }
}

void showDate() {
    struct tm tnow = myESPTime.getTimeStruct(); 
    int date1000 = tnow.tm_mday*100 + (tnow.tm_mon+1); 
    dataToArray(date1000, colorStripRGB(&dateColor), SDATE, 1);
    Serial.print(F("Date: ")); Serial.println(date1000);
    //Serial.print(F("dateColor: ")); Serial.println(dateColor);
    strip.show(); 
}

void showTemp() {
    int temp1000 = getTempBME280()*10;
    dataToArray(temp1000, colorStripRGB(&tempColor), STEMP, 1);
    strip.show(); 
}

void showHum() {
    int hum1000 = getHumBME280()*100; 
    dataToArray(hum1000, colorStripRGB(&humColor), SHUM, 0);
    strip.show(); 
}

void showPres() {
    int pres1000 = getPressBME280()*10; 
    dataToArray(pres1000, colorStripRGB(&presColor), SPRES, 0);
    strip.show(); 
}

void dataToArray(int data, uint32_t ledColor, edataArrType showType, uint8_t dot) {
    uint8_t cursor = LED_COUNT; //7*4+2
    switch (dot) {
      case 0:
        strip.setPixelColor(14, offColor);
        strip.setPixelColor(15, offColor);
        break; 
      case 1:
        strip.setPixelColor(14, offColor);
        strip.setPixelColor(15, ledColor);
        break; 
      case 2:
        strip.setPixelColor(14, ledColor);
        strip.setPixelColor(15, ledColor);
        break; 
      default:
        strip.setPixelColor(14, offColor);
        strip.setPixelColor(15, offColor);
    } 
    for(uint8_t i=1; i<=4;i++) {
      int digit = data % 10; // get last digit in time
      if (i==1) {
        //Serial.print("Digit 4 is : "); Serial.print(digit); Serial.print(" ");
        cursor =23;
        if (showType == STEMP) {digit = 10;} //grad
        if (showType == SHUM) {digit = 12;}
        if (showType == SPRES) {digit = 13;}
        for(uint8_t k=0; k<=6; k++) { 
          //Serial.print(digits[digit][k]);
          if (digits[digit][k]== 1) {strip.setPixelColor(cursor, ledColor);}
          else if (digits[digit][k]==0) {strip.setPixelColor(cursor, offColor);};
          cursor ++;
        };
        //Serial.println();
      }
      else if (i==2) {
        //Serial.print("Digit 3 is : ");Serial.print(digit);Serial.print(" ");
        cursor -=14;
        //if (showType == STEMP || showType == SHUM) {digit = 10;}
        if (showType == SHUM) {digit = 10;}
        for(uint8_t k=0; k<=6; k++){ 
          //Serial.print(digits[digit][k]);
          if (digits[digit][k]== 1) {strip.setPixelColor(cursor, ledColor);}
          else if (digits[digit][k]==0) {strip.setPixelColor(cursor, offColor);};
          cursor ++;
        };
        //Serial.println();
      }
      else if (i==3) {
        //Serial.print("Digit 2 is : ");Serial.print(digit);Serial.print(" ");
        cursor =7;
        for(uint8_t k=0; k<=6; k++) { 
          //Serial.print(digits[digit][k]);
          if(digits[digit][k]== 1) {strip.setPixelColor(cursor, ledColor);}
          else if (digits[digit][k]==0){strip.setPixelColor(cursor, offColor);};
          cursor ++;
        };
        //Serial.println();
      }
      else if (i==4){
        //Serial.print("Digit1 is : ");Serial.print(digit);Serial.print(" ");
        cursor =0;
        for(uint8_t k=0; k<=6; k++) { 
          //Serial.print(digits[digit][k]);
          if (digits[digit][k]== 1){strip.setPixelColor(cursor, ledColor);}
          else if (digits[digit][k]==0){strip.setPixelColor(cursor, offColor);};
          cursor ++;
        };
        //Serial.println();
      }
      data /= 10;
    }; 
}

void printTime(struct tm *tnow) {
      Serial.print("Ok, Time = "); print2digits(tnow->tm_hour);
      Serial.write(':'); print2digits(tnow->tm_min);
      Serial.write(':');    print2digits(tnow->tm_sec);
      Serial.print(", Date (D/M/Y) = ");    Serial.print(tnow->tm_mday);
      Serial.write('/');    Serial.print(tnow->tm_mon + 1);
      Serial.write('/');    Serial.print(tnow->tm_year + 1900);
      Serial.println();
}

void print2digits(uint8_t number) {
  if (number >= 0 && number < 10) {
    Serial.write('0');
  }
  Serial.print(number);
}

uint32_t colorStripRGB(uint8_t* mcolor) {
  //Serial.print("link*mcolor:"); Serial.println(*mcolor);
  switch((int)*mcolor) {           // Start the new animation...
    case 0:
      return strip.Color(0, 0, 0);    // Black/off
      break;
    case 1:
      return strip.Color(255, 0, 0);    // Red
      break;
    case 2:
      return strip.Color(255, 127, 0);    // Orange
      break;
    case 3:
      return strip.Color(255, 255, 0);    // Yellow
      break;
    case 4:
      return strip.Color(0, 255, 0);    // Green
      break;
    case 5:
      return strip.Color(0, 255, 127);    // Gyan
      break;
    case 6:
      return strip.Color(0, 0, 255);    // Blue
      break;
    case 7:
      return strip.Color(127, 0, 255);    // Purple
      break;
    case 8:
      return strip.Color(255, 255, 255); // White
      break;
    default:
      return strip.Color(0, 0, 0);    // Black/off
  }
}