WiFiClient client;
ESP8266WebServer server(80);
//DHT dht(DHTPIN, DHTTYPE);
DHT_Unified dht(DHTPIN, DHTTYPE);

WidgetRTC rtc;
BlynkTimer timer;
BLYNK_CONNECTED() {rtc.begin();}
WidgetLED led_ativo(V0);
WidgetLED led_AC_in(V1);
WidgetLED led_AC_out(V2);
WidgetTerminal terminal(V3);
