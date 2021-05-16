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
  setSyncInterval(10 * 60);
  timer.setInterval(  1000L,  verifica_conexao);
  timer.setInterval(  2000L,  sensores_AC);
  timer.setInterval(  3000L,  ler_tensao);
  timer.setInterval(  8000L,  Formatar_data_hora);
  timer.setInterval( 10000L,  leituras_dht);
  timer.setInterval( 15000L,  chama_conecta_wifi_blynk);
}

void normal_loop(){
  restart_();
  ArduinoOTA.handle();
  timer.run();
  Blynk.run();
  if(error_conect == true){ Blynk.run(); };
  error_conect = Blynk.connected();
}

void sensores_AC(){
  if(digitalRead(AC_in)) {led_AC1.on();} else {led_AC1.off();}
  if(digitalRead(AC_out)){led_AC2.on();} else {led_AC2.off();}
}
  
void ler_tensao(){
  int x = 0;
  for( int i=0; i<10; i++){
    x+=analogRead(A0);
    delay(10);
  }
  x = x / 10;
  bat1 = (0.0009775 * x) * 22.69;
  bat1_str = String(bat1,1) + "V";
}

void setup() {
  iniciar_PINs();
  if(serial_ativo  == true){ Serial.begin(9600); log_serial("  "); log_serial("Iniciando..."); delay(100); }
  openFS(); delay(500);
  EEPROM.begin(2);
  tensao_minima = EEPROM.read(0);

  WiFi.disconnect(); 
  delay(500);
  
  if(digitalRead(config_wifi)){modo_config = true;} // Iniciar normalmente ou modo Config
  if(modo_config == true){config_rede();} else {normal();}
}

void loop() {
  if(modo_config){ server.handleClient();} else {normal_loop();}
}
