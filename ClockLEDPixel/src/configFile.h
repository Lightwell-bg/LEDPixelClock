bool saveConfig();

bool loadConfig() {
    if(!SPIFFS.exists(filePath)) {
        Serial.println(F("Failed to open config file"));
        saveConfig();   
        return false;
    }
    File configFile = SPIFFS.open(filePath, "r");   
    size_t size = configFile.size();   
    if (size > 2048) {  
        Serial.print(F("Config file size is too large: ")); Serial.println(size);
        configFile.close();
        return false;
    }
    jsonConfig = configFile.readString(); 
    configFile.close();
    DynamicJsonDocument jsonDoc(5096); //4096
    DeserializationError error = deserializeJson(jsonDoc, jsonConfig);
    if (error) {
      Serial.print(F("loadConfig() deserializeJson() failed with code "));
      Serial.println(error.c_str());
      return false;
    }   
    JsonObject root = jsonDoc.as<JsonObject>();    
    timezone = root["timezone"].as<int>();               
    isDayLightSaving = root["isDayLightSaving"];
    sNtpServerName = root["ntp_server"].as<String>(); 
    useRTC = root["useRTC"];   
    lang = root["lang"].as<int>(); 
    brightd = root["brightd"]; brightn = root["brightn"];
    dmodefrom = root["dmodefrom"]; dmodeto = root["dmodeto"];
    autoBright = root["autoBright"];
    global_start = root["global_start"]; global_stop = root["global_stop"];
    isLedClock = root["isLedClock"]; isLedDate = root["isLedDate"];
    isLedTemp = root["isLedTemp"]; isLedHum = root["isLedHum"];
    isLedPres = root["isLedPres"];
    clockFrom = root["clockFrom"]; clockTo = root["clockTo"];
    dateFrom = root["dateFrom"]; dateTo = root["dateTo"];
    tempFrom = root["tempFrom"]; tempTo = root["tempTo"];
    humFrom = root["humFrom"]; humTo = root["humTo"];
    presFrom = root["presFrom"]; presTo = root["presTo"];
    clockColor = root["clockColor"]; dateColor = root["dateColor"]; 
    tempColor = root["tempColor"]; humColor = root["humColor"]; presColor = root["presColor"]; 
    onlyClockNight = root["onlyClockNight"];
    mqttOn = root["mqttOn"];
    mqtt_server = root["mqtt_server"].as<String>();
    mqtt_port = root["mqtt_port"].as<int>();
    mqtt_user = root["mqtt_user"].as<String>();
    mqtt_pass = root["mqtt_pass"].as<String>();
    mqtt_name = root["mqtt_name"].as<String>();
    mqtt_Temp = root["mqtt_Temp"].as<String>();
    mqtt_Hum = root["mqtt_Hum"].as<String>();
    mqtt_Pres = root["mqtt_Pres"].as<String>();
    tspeakOn = root["tspeakOn"];
    tspeak_server = root["tspeak_server"].as<String>();
    tspeak_channal = root["tspeak_channal"].as<int>();
    tspeak_wapi = root["tspeak_wapi"].as<String>();
    return true;
}

// Запись данных в файл config.json
bool saveConfig() {
   DynamicJsonDocument jsonDoc(5096); //4096
   DeserializationError error = deserializeJson(jsonDoc, jsonConfig);
   if (error) {
      Serial.print(F("saveConfig() deserializeJson() failed with code "));
      Serial.println(error.c_str());
      return false;
   }     
   JsonObject json = jsonDoc.as<JsonObject>();  
    json["timezone"] = timezone;
    json["isDayLightSaving"] = isDayLightSaving;
    json["ntp_server"] = sNtpServerName;
    json["useRTC"] = useRTC;
    json["lang"] = lang;
    json["brightd"] = brightd; json["brightn"] = brightn;
    json["dmodefrom"] = dmodefrom; json["dmodeto"] = dmodeto;
    json["autoBright"] = autoBright;
    json["global_start"] = global_start; json["global_stop"] = global_stop;
    json["isLedClock"] = isLedClock; json["isLedDate"] = isLedDate;
    json["isLedTemp"] = isLedTemp; json["isLedHum"] = isLedHum;
    json["isLedPres"] = isLedPres;
    json["clockFrom"] = clockFrom; json["clockTo"] = clockTo;
    json["dateFrom"] = dateFrom; json["dateTo"] = dateTo;
    json["tempFrom"] = tempFrom; json["tempTo"] = tempTo;
    json["humFrom"] = humFrom; json["humTo"] = humTo;
    json["presFrom"] = presFrom; json["presTo"] = presTo;
    json["clockColor"] = clockColor; json["dateColor"] = dateColor; 
    json["tempColor"] = tempColor; json["humColor"] = humColor; json["presColor"] = presColor; 
    json["onlyClockNight"] = onlyClockNight;
    json["mqttOn"] = mqttOn;
    json["mqtt_server"] = mqtt_server;
    json["mqtt_port"] = mqtt_port;
    json["mqtt_user"] = mqtt_user;
    json["mqtt_pass"] = mqtt_pass;
    json["mqtt_name"] = mqtt_name;
    json["mqtt_Temp"] = mqtt_Temp;
    json["mqtt_Hum"] = mqtt_Hum;
    json["mqtt_Pres"] = mqtt_Pres;
    json["tspeakOn"] = tspeakOn;
    json["tspeak_server"] = tspeak_server;
    json["tspeak_channal"] = tspeak_channal;
    json["tspeak_wapi"] = tspeak_wapi;
    json["cVersion"] = cVersion;
    serializeJson(json, jsonConfig);
    File configFile = SPIFFS.open(filePath, "w");
    if (!configFile) {
        Serial.println(F("Failed to open config file for writing"));
        configFile.close();
        return false;
    }
    serializeJson(json, configFile);
    configFile.close();
    return true;
}