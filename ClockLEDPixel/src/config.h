#ifndef GLOBAL_H
#define GLOBAL_H
const String modelName = "LEDPixelClock";
const String nVersion = "v1.0";
#define USE_DHT false //USE OR NO
#define USE_BME280 true //USE OR NO

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include "FS.h"
#include <Updater.h>//for update firmware
#include <ESP8266SSDP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <Wire.h> // must be included here so that Arduino library object file references work
#include <PubSubClient.h> //"https://github.com/knolleary/pubsubclient.git"
#include <Adafruit_NeoPixel.h>
#include "NetCrtESP.h"
#include "ESPTimeFunc.h"
#include "ssdpAWS.h"

bool useRTC = true; //get time RTC

AsyncWebServer HTTP(80);
WiFiClient ESPclient;
PubSubClient mqttClient(ESPclient);
NetCrtESP myWIFI;
ESPTimeFunc myESPTime(useRTC);
ssdpAWS mySSDP(&HTTP);

String cVersion = modelName + nVersion;

#if USE_DHT == true
  #include <Adafruit_Sensor.h> // https://github.com/adafruit/Adafruit_Sensor
  #include "DHT.h" //https://github.com/adafruit/DHT-sensor-library
  //#define DHTTYPE DHT21   // DHT 21 (AM2301)
  #define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
  #define TEMPHUM_PIN 4    // D4 выберите входной контакт,к которому подключен датчик
  DHT dht(TEMPHUM_PIN, DHTTYPE);
  //cVersion += " DHT22 ";
#endif //DHT
#if USE_BME280 == true // I2C D1, D2
  #include "BlueDot_BME280.h"
  BlueDot_BME280 bme280 = BlueDot_BME280(); 
  //cVersion += String(" BME280"); 
#endif //BME280
  
#define LED_PIN 14 // D5
#define LED_COUNT 30 //7*4+2
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_RGB + NEO_KHZ800);

String filePath = "/myConfig.json"; //File for config
String jsonConfig = "{}";

// wifi
String ssid = "";
String password = "";
String ssidAP = "LED2811";   // SSID AP точки доступа
String passwordAP = ""; // пароль точки доступа
String SSDP_Name = "LED2811"; // Имя SSDP
//IPAddress apIP(192, 168, 4, 1);
String apIP = "192.168.4.1";

uint8_t lang = 2; //0-RU, 1-BG, 2 -EN, 3-UA
//Time
//const unsigned long TIME_UPDATE = 1000*60*10; //Интервал обновления времени
int8_t timezone = 2;               // часовой пояс GTM
String sNtpServerName = "us.pool.ntp.org";
const char sNtpServerName2[] = "pool.ntp.org";
const char sNtpServerName3[] = "time.nist.gov";
bool isDayLightSaving = false; //Summer time use
bool updateRTCfromNTP = true; //


//LED Strip
uint8_t maxMode = 5;
uint32_t intShowData = 3000;
uint8_t clockColor = 2;
uint8_t dateColor = 1;
uint8_t tempColor = 3;
uint8_t humColor = 4;
uint8_t presColor = 5;
const uint32_t offColor = strip.Color(0, 0, 0);
uint8_t digits[14][7] = {{0,1,1,1,1,1,1},  // Digit 0
                        {0,1,0,0,0,0,1},   // Digit 1
                        {1,1,1,0,1,1,0},   // Digit 2
                        {1,1,1,0,0,1,1},   // Digit 3
                        {1,1,0,1,0,0,1},   // Digit 4
                        {1,0,1,1,0,1,1},   // Digit 5
                        {1,0,1,1,1,1,1},   // Digit 6
                        {0,1,1,0,0,0,1},   // Digit 7
                        {1,1,1,1,1,1,1},   // Digit 8
                        {1,1,1,1,0,1,1},  // Digit 9 | 2D Array for numbers on 7 segment
                        {1,1,1,1,0,0,0},   // ° char
                        {0,0,1,1,1,1,0},   // C char
                        {1,0,0,0,1,1,1},  // % char
                        {1,1,1,1,1,0,0}};  // P char
uint8_t brightd = 15, brightn = 2; //brightness day and night
uint8_t dmodefrom = 8, dmodeto = 20; //DAY MODE
bool autoBright = false; //reserv
//show
bool onlyClockNight = false;
bool isLedClock = true, isLedDate = true, isLedTemp = true, isLedHum = false, isLedPres = false;
float clockFrom=0, dateFrom=0, tempFrom=0, humFrom=0, presFrom=0;
float clockTo=24, dateTo=24, tempTo=24, humTo=24, presTo=24; 
float global_start = 0, global_stop = 24; //Working time

enum edataArrType {
  STIME,
  SDATE,
  STEMP,
  SHUM,
  SPRES
};
//edataArrType dataArrType;
//MQTT
char mqttData[80]; //Данные для передачи на MQTT
bool mqttOn = false;
String mqtt_server = "hairdresser.cloudmqtt.com"; // Имя сервера MQTT
int mqtt_port = 18695; // Порт для подключения к серверу MQTT
String mqtt_user = ""; // Логи от сервер
String mqtt_pass = ""; // Пароль от сервера QvKr_jbEsHYb
String mqtt_name = "LED2811"; 
String mqtt_Temp = mqtt_name + "/temp";
String mqtt_Hum = mqtt_name + "/hum";
String mqtt_Pres = mqtt_name + "/pres";
String mqtt_Timezone = "timezone";
const unsigned long MQTT_SEND_INT = 1000*60*11; //Интервал отправки на MQTT
//thingspeak.com
bool tspeakOn = false;
String tspeak_server = "api.thingspeak.com";
unsigned long tspeak_channal = 9633;
String tspeak_wapi = "";  
const unsigned long TSPEAK_SEND_INT = 1000*60*10;

#endif