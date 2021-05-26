#include "definicoes.h"
#include "configs.h"
#include "variaveis.h"
#include "Pinos.h"
#include "funcoes_gerais.h"
#include "Modulos.h"

BLYNK_WRITE(V3){ // Comandos terminal
  String log_terminal = param.asStr();
       if(log_terminal.substring(0,4)  == "Info")        {
        terminal.clear();
        terminal.println("Placa: " + NOME);
        terminal.println("IP: " + IP);
        terminal.println("Rede: " + String(ssid));
        terminal.flush();
        terminal.println("Limite Bateria: " + String(bat_minima,1) + "V");
        terminal.println("Bateria Normal: " + String(bat_normal,1) + "V");
        terminal.flush();
  }
  else if(log_terminal.substring(0,3)  == "Cmd")         {
        terminal.clear();
        terminal.println(F("Limpar Limpar o terminal"));
        terminal.println(F("Cmd Lista todos os comandos"));
        terminal.println(F("Temp:35 Seta a Temperatura maxima pra 35 graus"));
        terminal.println(F("Info Mostra as configurações salvas "));
        terminal.println(F("Reset Da um reboot no sistema"));
        terminal.println(F("Bateria-10.8 Liga Relé em 10.8V"));
        terminal.println(F("Bateria+12.6 Desliga Relé em 12.6V"));

        terminal.flush();
  }
  else if(log_terminal.substring(0,6)  == "Limpar")      {
    terminal.clear();
  }
  else if(log_terminal.substring(0,5)  == "Reset")       {
  terminal.clear();
  delay(500);
  mostrar_terminal ("Resetando...");
  delay(2000);
  ESP.restart();
  }
  else if(log_terminal.substring(0,8)  == "Bateria-")    {
    String local = log_terminal.substring(8);
    bat_minima = local.toFloat();
    if(bat_minima == 0){
      terminal.println("Digite um número válido!"); terminal.flush();
    }
    else{
      terminal.println("Limite Bateria: " + String(bat_minima) + " Volts"); terminal.flush();
      tensao_minima = bat_minima * 10;
      EEPROM.write(0, tensao_minima);
      EEPROM.commit();
      delay(50);
    }
  }
  else if(log_terminal.substring(0,8)  == "Bateria+")    {
    String local = log_terminal.substring(8);
    bat_normal = local.toFloat();
    if(bat_normal == 0){
      terminal.println("Digite um número válido!"); terminal.flush();
    }
    else{
      terminal.println("Bateria normaliza em: " + String(bat_normal) + " Volts"); terminal.flush();
      tensao_normal = bat_normal * 10;
      EEPROM.write(1, tensao_normal);
      EEPROM.commit();
      delay(50);
    }
  }
  else if(log_terminal.substring(0,5)  == "Temp:")       {
    String local = log_terminal.substring(5);
    limite_temp = local.toInt();
    if(limite_temp == 0){
      terminal.println("Digite um número válido!"); terminal.flush();
    }
    else{
      terminal.println("Limite de Temperatura: " + String(limite_temp) + " Graus"); terminal.flush();
      EEPROM.write(2, limite_temp);
      EEPROM.commit();
      delay(50);
    }
  }
  else{
    terminal.println("Comando Inválido!");        terminal.flush();  
  }
}

BLYNK_READ(V4) {Blynk.virtualWrite(V4, dht_str + "  " + bat1_str + "  " + sinal_wifi);}

BLYNK_WRITE(V5) { // Relé 1
  int buttonState = param.asInt();
    if (buttonState == 1) { pino(rele1, 1); mostrar_terminal ("Relé ON");  }
    if (buttonState == 0) { pino(rele1, 0); mostrar_terminal ("Relé OFF"); }
}

void normal(){
  //Testar_Reles();
  readFile();
  converter_strings();
  conecta_wifi_blynk(ssid, pw);

  //Se não conseguir conectar a rede ao iniciar, reseta o ESP8266
  if (WiFi.status() != WL_CONNECTED){ ESP.restart(); }
  
  iniciar_OTA();
  setSyncInterval(10 * 60);
  timer.setInterval(  1000L,  verifica_conexao);
  timer.setInterval(  2000L,  AC_Bateria);
  timer.setInterval(  3000L,  ler_tensao);
  timer.setInterval(  8000L,  Formatar_data_hora);
  timer.setInterval( 25000L,  leituras_dht);
  timer.setInterval( 26000L,  verifica_temperatura);
  timer.setInterval( 30000L,  chama_conecta_wifi_blynk);

  ler_tensao();
  
}

void normal_loop(){
  restart_();
  ArduinoOTA.handle();
  timer.run();
  if(error_conect == true){ Blynk.run(); };
  error_conect = Blynk.connected();
}

void verifica_temperatura(){
  if (trava_temp == false && temp_dht > limite_temp){
    trava_temp = true;
    mostrar_terminal("Temperatura acima do limte estabelecido");
    Blynk.notify(NOME + ": Temperatura acima do limte estabelecido. " + String(temp_dht,1) + "C");
  }

  if (trava_temp == true && temp_dht < limite_temp-3){
    trava_temp = false;
    mostrar_terminal("Temperatura Normalizada");
    Blynk.notify(NOME + ": Temperatura Normalizada. " + String(temp_dht,1) + "C");
  }
}

void AC_Bateria(){
  if(digitalRead(AC_in)) { led_AC_in.on();  Sem_AC_IN=false;  } else { led_AC_in.off();  Sem_AC_IN=true;  }
  
    //vai ligar o led falha quando tiver sem energia na saída/out
  if(digitalRead(AC_out)){ led_AC_out.off(); Sem_AC_OUT=false; } else { led_AC_out.on(); Sem_AC_OUT=true; }

  if(iniciar == true){
    if(Sem_AC_IN  == false && trava_AC_IN  == false){ mostrar_terminal("Entrada AC Normalizada");  trava_AC_IN  = true; }
    if(Sem_AC_OUT == false && trava_AC_OUT == false){ mostrar_terminal("Inversor Normalizado"); trava_AC_OUT = true; }

  
    if(Sem_AC_IN  == true && trava_AC_IN  == true){
      mostrar_terminal("Entrada AC sem Tensão"); 
      trava_AC_IN  = false;
      Blynk.notify(NOME + ": Tensão da Entrada AC está ausente");
      }
      
    // Quando não tiver tensao AC na saida/out envia essas msgs
    if(Sem_AC_OUT == true && trava_AC_OUT == true){
      mostrar_terminal("Falha no Inversor. Inversor no ByPass.");
      trava_AC_OUT = false;
      Blynk.notify(NOME + ": Falha no Inversor. Inversor no ByPass.");
      }
  }

  if( trava_bateria == false && bat1 < bat_minima ){
    trava_bateria = true;
    mostrar_terminal("Bateria Baixa!");
    Blynk.notify(NOME + ": Tensão da Bateria Baixa!");
    pino(rele1, 1);
    mostrar_terminal ("Relé ON");
    Blynk.virtualWrite(V5, HIGH);
    }
    
  if( trava_bateria == true  && bat1 > bat_normal ){
    trava_bateria = false;
    mostrar_terminal("Bateria OK!");
    Blynk.notify(NOME + ": Tensão da Bateria Normalizada!");
    pino(rele1, 0);
    mostrar_terminal ("Relé OFF");
    Blynk.virtualWrite(V5, LOW);
    }
}
  
void ler_tensao(){
  int x = 0;
  for( int i=0; i<10; i++){
    x+=analogRead(A0);
    delay(10);
  }
  x = x / 10;
  bat1 = (0.0009775 * x) * bat_const;
  bat1_str = String(bat1,1) + "V";
}

void setup() {
  iniciar_PINs();
  indicador_inicilizacao();
  if(serial_ativo  == true){ Serial.begin(9600); log_serial("  "); log_serial("Iniciando..."); delay(100); }
  openFS(); delay(500);

  WiFi.disconnect(); 
  delay(500);

  EEPROM.begin(10);
  tensao_minima = EEPROM.read(0);
  tensao_normal = EEPROM.read(1);
  limite_temp   = EEPROM.read(2);
  bat_minima = tensao_minima / 10.0;
  bat_normal = tensao_normal / 10.0;
  
  if(digitalRead(config_wifi)){modo_config = true;} // Iniciar normalmente ou modo Config
  if(modo_config == true){config_rede();} else {normal();}
}

void loop() {
  if(modo_config){
    server.handleClient();
    digitalWrite(led, !digitalRead(led));
    delay(100);
  }
  else {
    normal_loop();
  }
}
