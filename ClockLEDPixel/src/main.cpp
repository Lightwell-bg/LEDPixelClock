#include <Arduino.h>
#include "config.h" 
#include "configFile.h"
#include "sensor.h"
#include "httpServer.h"
#include "mqtt.h"
#include "thingspeak.h"
#include "display.h"
#include "fwareupd.h"

void setup() {
    Serial.begin(115200);
    char host[16];
    snprintf(host, 12, "ESP%08X", ESP.getChipId());    
    if(!SPIFFS.begin()) { // Initialize SPIFFS
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    } 
    loadConfig(); Serial.println(F("FileConfig init"));
    myWIFI.begin(ssid.c_str(), password.c_str(), SSDP_Name.c_str(), ssidAP.c_str(), passwordAP.c_str(), apIP.c_str()); Serial.println(F("Wi-Fi init"));
    Serial.printf("Ready! Open http://%s.local in your browser\n", host); 
    myESPTime.begin(timezone, isDayLightSaving, sNtpServerName, sNtpServerName2, sNtpServerName3, useRTC, updateRTCfromNTP); Serial.println(F("Start Time"));  
    init_firmWareUpdate(); Serial.println(F("Start init FirmWare update"));
    SSDP_Name = myWIFI.getNameSSDP(); Serial.println(SSDP_Name);
    mySSDP.begin(SSDP_Name.c_str(), "000000001", modelName.c_str(), nVersion.c_str(), "LIGHTWELL", "https://diy.led-lightwell.eu"); Serial.println(F("Start init SSDP"));
    init_HTTPServer(); Serial.println(F("HTTPServer init"));
    Serial.println(myWIFI.getDevStatusIP()); 
    init_sensor(); Serial.println(F("Sensor init"));
    init_mqtt();
    init_LStrip();  Serial.println(F("LED strip init"));
}

void loop() {
  struct tm tnow = myESPTime.getTimeStruct();
  static uint8_t sMode=0, oldsMode = 1;
  static uint32_t tCount = 0;
  static bool chMode = true;
  if (myESPTime.compTimeInt(global_start, global_stop, &tnow)) {
    if (sMode != oldsMode) {Serial.print(F("Mode ")); Serial.println(sMode); oldsMode = sMode;}
    if (myESPTime.compTimeInt(dmodefrom, dmodeto, &tnow))
        strip.setBrightness(map(brightd, 0, 15, 100, 255));
        else strip.setBrightness(map(brightn, 0, 15, 100, 255));
    if (millis()-tCount > intShowData) {sMode++; chMode = true; tCount = millis();}
    if (onlyClockNight && !myESPTime.compTimeInt(dmodefrom, dmodeto, &tnow)) sMode = 1; //Show only Clock Night
      if (sMode > maxMode) {sMode = 1;}
      if (sMode==1) {
        if (isLedClock && myESPTime.compTimeInt(clockFrom, clockTo, &tnow)) {
          showTime();
        }
        else sMode++;
      }
      if (sMode==2 && chMode) {
        if (isLedDate && myESPTime.compTimeInt(dateFrom, dateTo, &tnow)) {
          showDate(); chMode = false;
        }
        else sMode++;
      }
      if (sMode==3 && chMode) {
        if (isLedTemp && myESPTime.compTimeInt(tempFrom, tempTo, &tnow)) {        
          showTemp(); chMode = false;
        }
        else sMode++;
      }
      if (sMode==4 && chMode) {
        if (isLedHum && myESPTime.compTimeInt(humFrom, humTo, &tnow)) {        
          showHum(); chMode = false;
        }
        else sMode++;
      }
      if (sMode==5 && chMode) {
        if (isLedPres && myESPTime.compTimeInt(presFrom, presTo, &tnow)) {        
          showPres(); chMode = false;
        }
        else sMode++;
      }
  }
  if (tspeakOn) {
      sendTspeak(TSPEAK_SEND_INT);
  }
  if (mqttOn) {
      mqttReconnect(60000);
      sendTHP(MQTT_SEND_INT);
  }
  delay(1); //Вроде нужна для восстановления стека
  yield();
}