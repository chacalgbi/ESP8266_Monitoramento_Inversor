// Sempre lembrar de definir o tamanho pra 4MB (FS:1Mb OTA:~1019)
#include "FS.h"
#include <ArduinoOTA.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <LittleFS.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <EEPROM.h>
#include <WidgetRTC.h>
#include <TimeLib.h>
#include "DHT.h"

#define DHTTYPE DHT22
#define dhtpin       4 // DHT22
#define config_wifi 16 // Input
#define led         14 // LED da Placa
#define rele1        5 // rele1
#define AC_in       12 // Entrada AC
#define AC_out      13 // Saída AC
