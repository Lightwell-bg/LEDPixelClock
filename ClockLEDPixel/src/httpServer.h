void notFound(AsyncWebServerRequest *request);
String processor(const String& var);

void init_HTTPServer(void) {
    // Route for root / web page
    HTTP.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/index.html", "text/html");
    });
      // send a file when /index is requested
    HTTP.on("/index.html", HTTP_ANY, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/index.html", "text/html");
    });
     // Route to load style.css file
    HTTP.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/style.css", "text/css");
    });  
    HTTP.on("/img/favicon.png", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/img/favicon.png", "image/x-icon");
    });      
    HTTP.on("/function.js", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/function.js", "application/javascript");
    }); 
    HTTP.on("/lang_EN.js", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/lang_EN.js", "application/javascript");
    });  
    HTTP.on("/lang_RU.js", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/lang_RU.js", "application/javascript");
    });    
    HTTP.on("/lang_BG.js", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/lang_BG.js", "application/javascript");
    });  
    HTTP.on("/wifi.html", HTTP_ANY, [](AsyncWebServerRequest *request){
        //request->send(SPIFFS, "/wifi.html", "text/html");
        request->send(SPIFFS, "/wifi.html", String(), false, processor);
    });
    HTTP.on("/time.html", HTTP_ANY, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/time.html", "text/html");
    });
    HTTP.on("/setup.html", HTTP_ANY, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/setup.html", "text/html");
    });
    HTTP.on("/help.html", HTTP_ANY, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/help.html", "text/html");
    }); 
    HTTP.on(filePath.c_str(), HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/myConfig.json", "application/json");
    });     
    HTTP.on("/restart", HTTP_GET, [](AsyncWebServerRequest *request) {   //Перезагрузка модуля по запросу вида /restart?device=ok      
        String restart = request->getParam("device")->value(); 
        if (restart == "ok") {                        
            request->send(200, "text/html", "Restart OK"); 
            ESP.restart();                              
        }
        else {                                        
            request->send(200, "text/html", "Restart NO");  
        }
    }); 
    HTTP.on("/resetConfig", HTTP_GET, [](AsyncWebServerRequest *request) {   //Reset og configuration file     
        String reset = request->getParam("device")->value(); 
        if (reset == "ok") {  
            //SPIFFS.format();
            SPIFFS.remove(filePath);
            Serial.println("ESP erase Config file");
            delay(3000);                                   
            request->send(200, "text/html", "Reset OK"); 
            delay(3000);
            ESP.restart();                                
        }
        else {                                        
            request->send(200, "text/html", "Reset NO");  
        }
    }); 
    HTTP.on("/configs.json", HTTP_GET, [](AsyncWebServerRequest *request) {
        struct tm tnow = myESPTime.getTimeStruct();
        String root = "{}";  
        DynamicJsonDocument jsonDoc(5096); //4096
        DeserializationError error =  deserializeJson(jsonDoc, root);
        if (error) {
            Serial.print(F("deserializeJson() failed with code "));
            Serial.println(error.c_str());
            return;
        }     
        JsonObject json = jsonDoc.as<JsonObject>();    
        json["ssid"] = myWIFI.getNameSSID();
        json["password"] = myWIFI.getPassSSID();
        json["SSDP"] = myWIFI.getNameSSDP();
        json["ssidAP"] = myWIFI.getNameAPSSID();
        json["passwordAP"] = myWIFI.getPassAPSSID();
        json["ip"] = myWIFI.getDevStatusIP();
        json["time"] = myESPTime.getTimeStr(true);
        json["date"] = myESPTime.getDateStr();
        json["time_h"] = String(tnow.tm_hour);
        json["time_m"] = String(tnow.tm_min);
        json["time_s"] = String(tnow.tm_sec); 
        json["date_day"] = tnow.tm_mday;
        json["date_month"] = tnow.tm_mon + 1;
        json["date_year"] = tnow.tm_year + 1900;
        json["timezone"] = timezone;
        json["isDayLightSaving"] = (isDayLightSaving?"checked":"");
        json["lang"] = (lang==0?"RU":lang==1?"BG":lang==2?"EN":"EN");
        json["ntpserver"] = sNtpServerName; 
        json["use_sync"] = (useRTC?"":"checked");
        json["brightd"] = brightd; json["brightn"] = brightn;
        json["dmodefrom"] = dmodefrom; json["dmodeto"] = dmodeto; 
        json["autoBright"] = (autoBright?"checked":"");
        json["global_start"] = global_start; json["global_stop"] = global_stop;
        json["isLedClock"]=(isLedClock?"checked":"");json["isLedDate"]=(isLedDate?"checked":"");
        json["isLedTemp"]=(isLedTemp?"checked":"");json["isLedHum"]=(isLedHum?"checked":"");
        json["isLedPres"]=(isLedPres?"checked":"");
        json["clockFrom"] = clockFrom; json["clockTo"] = clockTo;
        json["dateFrom"] = dateFrom; json["dateTo"] = dateTo;
        json["tempFrom"] = tempFrom; json["tempTo"] = tempTo;
        json["humFrom"] = humFrom; json["humTo"] = humTo;
        json["presFrom"] = presFrom; json["presTo"] = presTo;
        json["clockColor"] = clockColor; json["dateColor"] = dateColor; 
        json["tempColor"] = tempColor; json["humColor"] = humColor; json["presColor"] = presColor; 
        json["onlyClockNight"]=(onlyClockNight?"checked":"");
        json["mqttOn"]=(mqttOn?"checked":"");
        json["mqtt_server"] = mqtt_server;
        json["mqtt_port"] = mqtt_port;
        json["mqtt_user"] = mqtt_user;
        json["mqtt_pass"] = mqtt_pass;
        json["mqtt_name"] = mqtt_name;
        json["mqtt_Temp"] = mqtt_Temp;
        json["mqtt_Hum"] = mqtt_Hum;
        json["mqtt_Pres"] = mqtt_Pres;
    #if USE_DHT == true
        //json["temp_now"] = getTempDHT();
        //json["hum_now"] = getHumDHT() + "%";
        json["temp_now"] = "n/a";
        json["hum_now"] = "n/a";
    #endif
    #if USE_BME280 == true
        json["temp_now"] = "Temp " + getStrTempBME280();
        json["hum_now"] = " Hum " + getStrHumBME280() + "%  Press " + getStrPressBME280() + "mm";
    #endif  
        json["tspeakOn"] = (tspeakOn?"checked":"");
        json["tspeak_server"] = tspeak_server;
        json["tspeak_channal"] = tspeak_channal;
        json["tspeak_wapi"] = tspeak_wapi; 
        json["cVersion"] = cVersion;  
        root = "";
        serializeJson(json, root);
        request->send(200, "application/json", root);
        //root = String();
    });    
    HTTP.on("/lang", HTTP_GET, [](AsyncWebServerRequest *request){    
        if(request->getParam("lang")->value() == "RU" && lang !=0) lang = 0; 
        else if(request->getParam("lang")->value() == "BG" && lang !=1) lang = 1; 
        else if(request->getParam("lang")->value() == "EN" && lang !=2) lang = 2; 
        else {request->send(200, "text/html", "OK"); return;} 
        Serial.print("Set lang: ");    Serial.println(request->getParam("lang")->value());
        saveConfig();
        request->send(200, "text/html", "OK");
    });  

    HTTP.on("/ssid", HTTP_GET, [](AsyncWebServerRequest *request){   
        ssid = request->getParam("ssid")->value();   
        password = request->getParam("password")->value();  
        ssidAP = request->getParam("ssidAP")->value();
        passwordAP = request->getParam("passwordAP")->value();
        SSDP_Name = request->getParam("ssdp")->value();
        //request->getParam("ssid")->value().toCharArray(ssid, (request->getParam("ssid")->value().length() + 1));
        //saveConfig(); 
        myWIFI.setConfigWIFI(ssid.c_str(), password.c_str(), SSDP_Name.c_str(), ssidAP.c_str(), passwordAP.c_str()); 
        Serial.println("SSID: " + String(ssid) + ", password: " + String(password) + ", SSID AP: " + String(ssidAP) + ", AP password: " + String(passwordAP) + ", SSDP: " + String(SSDP_Name)); 
        request->send(200, "text/html", "OK");        
    }); 

    HTTP.on("/Time", HTTP_GET, [](AsyncWebServerRequest *request){    
        myESPTime.timeSynch();
        Serial.println(F("timeSynch()")); 
        request->send(200, "text/html", "OK");        
    }); 

    HTTP.on("/TimeZone", HTTP_GET, [](AsyncWebServerRequest *request){    
        timezone = request->getParam("timezone")->value().toInt(); 
        isDayLightSaving = request->getParam("isDayLightSaving")->value().toInt(); 
        myESPTime.setTimeParam(useRTC, timezone, isDayLightSaving, sNtpServerName);
        saveConfig();
        myESPTime.timeSynch();
        Serial.println("NTP Time Zone: " + String(timezone) + ",  isDayLightSaving: " + String(isDayLightSaving));
        request->send(200, "text/html", "OK");        
    }); 

    HTTP.on("/setntpserver", HTTP_GET, [](AsyncWebServerRequest *request){
        sNtpServerName = request->getParam("ntpserver")->value().c_str(); 
        request->getParam("use_sync")->value().toInt()==1?useRTC=false:useRTC=true;
        myESPTime.setTimeParam(useRTC, timezone, isDayLightSaving, sNtpServerName);
        saveConfig();
        myESPTime.timeSynch();
        Serial.println("sNtpServerName: " + sNtpServerName + ", useRTC: " + useRTC);
        request->send(200, "text/html", "OK"); 
    }); 

    HTTP.on("/TimeNew", HTTP_GET, [](AsyncWebServerRequest *request) {
        struct tm timeman;
        time_t epoch_time;
        uint8_t hour_s = request->getParam("time_h")->value().toInt(); 
        uint8_t min_s = request->getParam("time_m")->value().toInt();
        uint8_t sec_s = request->getParam("time_s")->value().toInt();
        uint8_t mday_s = request->getParam("date_day")->value().toInt();
        uint8_t mon_s = request->getParam("date_month")->value().toInt();
        uint16_t year_s = request->getParam("date_year")->value().toInt(); 
        timeman.tm_hour = hour_s; 
        timeman.tm_min = min_s;
        timeman.tm_sec = sec_s;
        timeman.tm_mday = mday_s;
        timeman.tm_mon = mon_s - 1;
        timeman.tm_year = year_s - 1900; 
        epoch_time = mktime(&timeman);
        printf("Setting time: %s", asctime(&timeman)); 
        Serial.print(F("Setting time UNIX: ")); Serial.println(epoch_time);   
        useRTC = true;
        myESPTime.setTimeParam(useRTC, timezone, isDayLightSaving, sNtpServerName);
        //myESPTime.setTimeRTC(year_s, mon_s, mday_s, hour_s, min_s, sec_s);
        myESPTime.setTimeRTC(epoch_time);
        saveConfig();
        myESPTime.timeSynch();
        request->send(200, "text/html", "OK"); 
    }); 

    HTTP.on("/ledsetup", HTTP_GET, [](AsyncWebServerRequest *request){
        brightd =request->getParam("brightd")->value().toInt();
        brightn = request->getParam("brightn")->value().toInt();
        dmodefrom = request->getParam("dmodefrom")->value().toInt();
        dmodeto = request->getParam("dmodeto")->value().toInt();
        request->getParam("autoBright")->value().toInt()==1?autoBright=true:autoBright=false;
        saveConfig();
        strip.setBrightness(brightd);
        Serial.print("brightd ");Serial.print(brightd);Serial.print(" brightn ");Serial.println(brightn);
        Serial.print("dmodefrom ");Serial.println(dmodefrom);Serial.print("dmodeto ");Serial.println(dmodeto);
        request->send(200, "text/html", "OK"); 
    });

    HTTP.on("/ledoption", HTTP_GET, [](AsyncWebServerRequest *request){
        request->getParam("isLedClock")->value().toInt()==1?isLedClock=true:isLedClock=false;
        request->getParam("isLedDate")->value().toInt()==1?isLedDate=true:isLedDate=false;
        request->getParam("isLedTemp")->value().toInt()==1?isLedTemp=true:isLedTemp=false; 
        request->getParam("isLedHum")->value().toInt()==1?isLedHum=true:isLedHum=false;
        request->getParam("isLedPres")->value().toInt()==1?isLedPres=true:isLedPres=false;      
        clockFrom = request->getParam("clockFrom")->value().toFloat(); clockTo = request->getParam("clockTo")->value().toFloat();
        dateFrom = request->getParam("dateFrom")->value().toFloat(); dateTo = request->getParam("dateTo")->value().toFloat();
        tempFrom = request->getParam("tempFrom")->value().toFloat(); tempTo = request->getParam("tempTo")->value().toFloat();
        humFrom = request->getParam("humFrom")->value().toFloat(); humTo = request->getParam("humTo")->value().toFloat();
        presFrom = request->getParam("presFrom")->value().toFloat(); presTo = request->getParam("presTo")->value().toFloat();
        clockColor = request->getParam("clockColor")->value().toInt(); dateColor = request->getParam("dateColor")->value().toInt();
        tempColor = request->getParam("tempColor")->value().toInt(); humColor = request->getParam("humColor")->value().toInt();
        presColor = request->getParam("presColor")->value().toInt();
        request->getParam("onlyClockNight")->value().toInt()==1?onlyClockNight=true:onlyClockNight=false;
        global_start = request->getParam("global_start")->value().toFloat();
        global_stop = request->getParam("global_stop")->value().toFloat();
        saveConfig(); 
        request->send(200, "text/html", "OK"); 
    });

    HTTP.on("/mqttSet", HTTP_GET, [](AsyncWebServerRequest *request){
        request->getParam("mqttOn")->value().toInt()==1?mqttOn=true:mqttOn=false;
        mqtt_server = request->getParam("mqtt_server")->value().c_str();
        mqtt_port = request->getParam("mqtt_port")->value().toInt();
        mqtt_user = request->getParam("mqtt_user")->value().c_str();
        mqtt_pass = request->getParam("mqtt_pass")->value().c_str();
        mqtt_name = request->getParam("mqtt_name")->value().c_str();
        mqtt_Temp = request->getParam("mqtt_Temp")->value().c_str();
        mqtt_Hum = request->getParam("mqtt_Hum")->value().c_str();
        mqtt_Pres = request->getParam("mqtt_Pres")->value().c_str();
        request->getParam("tspeakOn")->value().toInt()==1?tspeakOn=true:tspeakOn=false;
        tspeak_server = request->getParam("tspeak_server")->value().c_str();
        tspeak_channal = request->getParam("tspeak_channal")->value().toInt();
        tspeak_wapi = request->getParam("tspeak_wapi")->value().c_str();
        saveConfig(); 
        Serial.println("mqtt_server: " + mqtt_server + ", mqtt_user: " + mqtt_user + ", mqtt_name: " + mqtt_name);
        Serial.println("tspeak_server: " + tspeak_server + ", tspeak_channal: " + tspeak_channal + ", tspeak_wapi: " + tspeak_wapi);
        request->send(200, "text/html", "OK"); 
    });
    HTTP.on("/mqttOn", HTTP_GET, [](AsyncWebServerRequest *request){
        request->getParam("mqttOn")->value().toInt()==1?mqttOn=true:mqttOn=false;
        saveConfig();   
        Serial.println("mqttOn: " + String(mqttOn));
        request->send(200, "text/html", "OK"); 
    });    
    HTTP.on("/tspeakOn", HTTP_GET, [](AsyncWebServerRequest *request){
        request->getParam("tspeakOn")->value().toInt()==1?tspeakOn=true:tspeakOn=false;
        saveConfig();   
        Serial.println("tspeakOn: " + String(tspeakOn));
        request->send(200, "text/html", "OK"); 
    }); 

    HTTP.onNotFound(notFound);
    HTTP.begin();
}



void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

String translateEncryptionType(uint8_t encryptionType) {
   switch (encryptionType) {
    case (7):
      return "Open";
    case (5):
      return "WEP";
    case (2):
      return "WPA_PSK";
    case (4):
      return "WPA2_PSK";
    case (8):
      return "WPA_WPA2_PSK";
    default:
      return "UNKNOWN"; 
  }
}

String processor(const String& var) { //For only LIST of Wi-Fi
    String listWIFI="";
    //Serial.println(var);
    if(var == "LISTWIFI") {
        if (WiFi.status() != WL_CONNECTED) {
            listWIFI="<strong>LIST of Wi-Fi networks:</strong></br>";
            int n = WiFi.scanComplete();
            if(n == -2) {
                WiFi.scanNetworks(true);
            }
            else
                if(n) {
                    for (int i = 0; i < n; ++i) {
                        listWIFI += "ssid: <strong>"+WiFi.SSID(i)+"</strong> ";
                        listWIFI += "rssi: <strong>"+String(WiFi.RSSI(i))+"</strong> ";
                        //listWIFI += ",\"bssid\":\""+WiFi.BSSIDstr(i)+"\"";
                        //listWIFI += ",\"channel\":"+String(WiFi.channel(i));
                        listWIFI += "secure: "+String(translateEncryptionType(WiFi.encryptionType(i)));
                        listWIFI += "</br>";
                    }
                WiFi.scanDelete();
                if(WiFi.scanComplete() == -2) {
                    WiFi.scanNetworks(true);
                }
            }    
    }
    return listWIFI;
  }
  return String();
}