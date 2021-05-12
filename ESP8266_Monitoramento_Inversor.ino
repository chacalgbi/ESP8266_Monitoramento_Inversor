#include "definicoes.h"
#include "configs.h"
#include "variaveis.h"
#include "Pinos.h"
#include "funcoes_gerais.h"
#include "Modulos.h"

BLYNK_READ(V4) {Blynk.virtualWrite(V4, dht_str + " - " + bat1_str);}

BLYNK_WRITE(V5) { // Relé 1
  int buttonState = param.asInt();
    if (buttonState == 1) { pino(rele1, 1); mostrar_terminal ("Relé1 ON");  }
    if (buttonState == 0) { pino(rele1, 0); mostrar_terminal ("Relé1 OFF"); }
}

void normal(){
  Testar_Reles();
  readFile();
  converter_strings();
  conecta_wifi_blynk(ssid, pw);
  iniciar_OTA();
}

void normal_loop(){
  restart_();
  ArduinoOTA.handle();
  if(error_conect == true){ Blynk.run(); };
  error_conect = Blynk.connected();
}

void setup() {
  iniciar_PINs();
  if(serial_ativo  == true){ Serial.begin(9600); log_serial("  "); log_serial("Iniciando..."); delay(100); }
  openFS(); delay(500);
  EEPROM.begin(2);
  tensao_minima = EEPROM.read(0);

  WiFi.disconnect(); 
  delay(500);
  
  if(!digitalRead(config_wifi)){modo_config = true;} // Iniciar normalmente ou modo Config
  if(modo_config == true){config_rede();} else {normal();}
}

void loop() {
  if(modo_config){ server.handleClient();} else {normal_loop();}
}
