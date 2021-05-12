WiFiClient client;
ESP8266WebServer server(80);
DHT dht(dhtpin, DHTTYPE);

WidgetRTC rtc;
BlynkTimer timer;
WidgetLED led_ativo(V0);
WidgetLED led_AC1(V1);
WidgetLED led_AC2(V2);
WidgetTerminal terminal(V3);
BLYNK_CONNECTED() {rtc.begin();}
