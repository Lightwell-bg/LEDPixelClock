void callback(char* topic, byte* payload, unsigned int length);

void init_mqtt() {
    mqttClient.setServer(mqtt_server.c_str(), mqtt_port);
    mqttClient.setCallback(callback);  
}

void callback(char* topic, byte* payload, unsigned int length) { // get data from server
    Serial.print("Message arrived ");
    payload[length] = '\0';
    String strTopic = String(topic);
    String strPayload = String((char*)payload);
    Serial.print(strTopic);  Serial.print(" => "); Serial.println(strPayload);
    if(strTopic == mqtt_Timezone) {
        timezone = strPayload.toInt();
        myESPTime.setTimeParam(useRTC, timezone, isDayLightSaving, sNtpServerName);
        saveConfig();
        myESPTime.timeSynch();
    }  
    //mqttNewData = true;                         
}

boolean mqttReconnect(uint16_t period) {
    static unsigned int lastReconnectAttempt = 0;
    if (!mqttClient.connected()) {
        if (millis() - lastReconnectAttempt > period) {
            //Serial.print("Attempting MQTT connection...");
            if (mqttClient.connect(mqtt_name.c_str(), mqtt_user.c_str(), mqtt_pass.c_str())) {
                //Serial.println("connected");
                // Once connected, publish an announcement...
                mqttClient.publish("HelloTopic", "Hello world");//*************************************************************
                // ... and resubscribe
                mqttClient.subscribe(mqtt_Timezone.c_str());
                lastReconnectAttempt = millis();
                return true;
            }
            else {
                Serial.print("failed, rc=");
                Serial.print(mqttClient.state());
                Serial.println(" try again in 5 seconds");
                lastReconnectAttempt = millis();
                return false;
            }
        }
    }
    else return true;
}

void sendTHP(uint32_t period) {
    static uint32_t countMQTT = 0;
    if (millis() - countMQTT < period) {
        return;
    }
#if USE_BME280 == true    
    String dataSensor = getStrTempBME280();
    dataSensor.toCharArray(mqttData, (dataSensor.length() + 1));
    mqttClient.publish(mqtt_Temp.c_str(), mqttData);
    dataSensor = getStrHumBME280();
    dataSensor.toCharArray(mqttData, (dataSensor.length() + 1));
    mqttClient.publish(mqtt_Hum.c_str(), mqttData);
    dataSensor = getStrPressBME280();
    dataSensor.toCharArray(mqttData, (dataSensor.length() + 1));
    mqttClient.publish(mqtt_Pres.c_str(), mqttData); 
#endif //BME280
#if USE_DHT == true  
    String dataSensor = getTempDHT();
    dataSensor.toCharArray(mqttData, (dataSensor.length() + 1));
    mqttClient.publish(mqtt_Temp.c_str(), mqttData);
    dataSensor = getHumDHT();
    dataSensor.toCharArray(mqttData, (dataSensor.length() + 1));
    mqttClient.publish(mqtt_Hum.c_str(), mqttData);     
#endif //DHT
    countMQTT = millis();
}