void restart_(){
  yield();      //Alimenta o Watchdog de Software. 3 Segundos.
  ESP.wdtFeed();//Alimenta o Watchdog de Hardware. 8 Segundos.
}

void mostrar_terminal (String aviso){
  terminal.print(currentTime);
  terminal.print(" - ");
  terminal.println(aviso);
  terminal.flush();
}

void log_serial(String x){
  if(serial_ativo == true){
    Serial.println(x);
  }
}

void verifica_conexao(){ // Pisca o LED do App.
  int tempo = 40;
  if(error_conect == true){
    if(inverter){led_ativo.on();  digitalWrite(led, HIGH);}
    else{led_ativo.off(); digitalWrite(led, LOW);}
  }
  else{
    if (WiFi.status() == WL_CONNECTED){ // Se não tiver internet, mas tiver rede, pisca rapidamente 4 vezes a cada 1 segundo
      for( int i=0; i<5; i++){
        digitalWrite(led, !digitalRead(led));
        delay(tempo);
      }
      digitalWrite(led, LOW);
    }
    else{ // Se não tiver internet nem rede mas a energia estiver OK e o módulo funcionando, mantém o LED ligado
      digitalWrite(led, HIGH);
    }
  }
  inverter = !inverter;
}

void converter_strings(){
  tamanho_rede  = REDE.length();
  tamanho_senha = SENHA.length();
  tamanho_nome  = NOME.length();
  
  REDE.toCharArray (ssid,  tamanho_rede);
  SENHA.toCharArray(pw,    tamanho_senha);
  NOME.toCharArray(device, tamanho_nome);
  
}
 
void formatFS(void){
  LittleFS.format();
}

void indicador_inicilizacao(){
  for( int i=0; i<50; i++){
    digitalWrite(led, !digitalRead(led));
    delay(50);
  }
  digitalWrite(led, LOW);
}

void createFile(void){
  File wFile;
 
  //Cria o arquivo se ele não existir
  if(LittleFS.exists("/log.txt")){
    log_serial("Arquivo ja existe!");
  } else {
    log_serial("Criando o arquivo...");
    wFile = LittleFS.open("/log.txt","w+");
 
    //Verifica a criação do arquivo
    if(!wFile){
      log_serial("Erro ao criar arquivo!");
    } else {
      log_serial("Arquivo criado com sucesso!");
    }
  }
  wFile.close();
}
 
void deleteFile(void) {
  //Remove o arquivo
  if(!LittleFS.remove("/log.txt")){
    log_serial("Erro ao remover arquivo!");
  } else {
    log_serial("Arquivo removido com sucesso!");
  }
}
 
void writeFile(String msg) {
 
  //Abre o arquivo para adição (append)
  //Inclue sempre a escrita na ultima linha do arquivo
  File rFile = LittleFS.open("/log.txt","a+");
 
  if(!rFile){
    log_serial("Erro ao abrir arquivo!");
  } else {
    rFile.println(msg);
    log_serial(msg);
  }
  rFile.close();
}
 
void readFile(void) {
  byte cont = 1;
  File rFile = LittleFS.open("/log.txt","r");
  log_serial("Lendo Arquivo SPIFFS...");
  while(rFile.available()) {
    if(cont == 1){REDE  = rFile.readStringUntil('\n'); log_serial(REDE);  }
    if(cont == 2){SENHA = rFile.readStringUntil('\n'); log_serial(SENHA); }
    if(cont == 3){NOME  = rFile.readStringUntil('\n'); log_serial(NOME);  }
    cont++;
    delay(10);
  }
  rFile.close();
}
 
void closeFS(void){
  LittleFS.end();
}
 
void openFS(void){
  //Abre o sistema de arquivos
  if(!LittleFS.begin()){
    log_serial("Erro ao abrir o sistema de arquivos");
  } else {
    log_serial("Sistema de arquivos aberto com sucesso!");
  }
}

void handleRoot() {
  //Go to http://192.168.4.1 in a web browser
String html  = "<!DOCTYPE html><head><html lang='pt-br'><meta charset='UTF-8'><meta http-equiv='X-UA-Compatible' content='IE=edge'><meta name='viewport' content='width=device-width, initial-scale=1'><title>Configurar WIFI</title>";
       html += "</head><body onload=\"myFunction()\" align='center'>";
       html += "<h3>CONFIGURAÇÃO<br> WIFI</h3>";
       html += "<form method='POST' action='/form'>";
       html += "<input type=text placeholder='DIGITE O NOME DA REDE'  name=txt_ssid /><br><br>";
       html += "<input type=text placeholder='DIGITE A SENHA DA REDE' name=txt_pw   /><br><br>";
       html += "<input type=text placeholder='NOME DO LOCAL'          name=txt_nome /><br><br>";
       html += "<input type=submit name=botao value=Enviar />";
       html += "</form>";
       html += "</body></html>";
server.send(200, "text/html", html);
}

void formularioEnviado() {
String html =  "<!DOCTYPE html><head><html lang='pt-br'><meta charset='UTF-8'><meta http-equiv='X-UA-Compatible' content='IE=edge'><meta name='viewport' content='width=device-width, initial-scale=1'><title>WIFI Configurado</title></head><body align='center'><h3>Seu dispositivo foi<br> configurado</h3>";
       html += "<h1><font color='red'>SSID: ";
       html += server.arg("txt_ssid");
       html += "</h1></font><br><h1><font color='green'>SENHA: ";
       html += server.arg("txt_pw");
       html += "</font></h1></body></html>";
       server.send(200, "text/html", html);

  REDE  = server.arg("txt_ssid"); REDE.trim();  writeFile(REDE);
  SENHA = server.arg("txt_pw");   SENHA.trim(); writeFile(SENHA);
  NOME  = server.arg("txt_nome"); NOME.trim();  writeFile(NOME);

}

void config_rede(){
  WiFi.softAP("Monitoramento");
  server.on("/", handleRoot);
  server.on("/form", HTTP_POST, formularioEnviado);
  server.begin();
  deleteFile();
  delay(100);
  createFile();
  delay(100);
}

void Formatar_data_hora(){ // A cada 8 segundos atualiza a Hora, Apita de Hora em Hora, Avisa se Iniciou e Envia os LOGs OffLine
       if(weekday() == 1){dia_semana = "Dom";}
  else if(weekday() == 2){dia_semana = "Seg";}
  else if(weekday() == 3){dia_semana = "Ter";}
  else if(weekday() == 4){dia_semana = "Qua";}
  else if(weekday() == 5){dia_semana = "Qui";}
  else if(weekday() == 6){dia_semana = "Sex";}
  else if(weekday() == 7){dia_semana = "Sab";}

  String hora1 = hour()   < 10 ? "0" + String(hour())   : String(hour());
  String minu1 = minute() < 10 ? "0" + String(minute()) : String(minute());
  String segu1 = second() < 10 ? "0" + String(second()) : String(second());
  String dia1  = day()    < 10 ? "0" + String(day())    : String(day());
  String mes1  = month()  < 10 ? "0" + String(month())  : String(month());
  String ano1  = year()   < 10 ? "0" + String(year())   : String(year());

  IP = WiFi.localIP().toString().c_str();
  sinal_wifi = String(WiFi.RSSI()) + "dB";
  
  hora  = hora1 + ":" + minu1 + ":" + segu1;
  data_ = dia1  + "/" + mes1  + "/" + ano1;
  
  currentTime = hora + " " + data_;

  if(error_conect == true && iniciar == false){  // apenas quando ligar
    iniciar = true;
    mostrar_terminal("Iniciado");
    Blynk.virtualWrite(V5, LOW);
  }
}

void iniciar_OTA(){
  ArduinoOTA.setHostname("Monitoramento");
  ArduinoOTA.begin();
  ArduinoOTA.onStart([]() { LittleFS.end(); log_serial("Atualizando..."); });
  ArduinoOTA.onEnd([]()   { log_serial("Fim da Atualizacao"); });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total){
    unsigned int progress1;
    if(progress1 != progress){ progress1 = progress;}
    log_serial("Atualizando"); 
    });
  ArduinoOTA.onError([](ota_error_t error) { log_serial("Erro ao Atualizar"); });
}

void conecta_wifi_blynk(char* ssid_, char* pw_){

  if(WiFi.status() != WL_CONNECTED){
    int contador = 0;
    WiFi.disconnect(); 
    delay(500);
    WiFi.mode(WIFI_STA);
    delay(100);
    WiFi.begin(ssid_, pw_);
    delay(100);
    while ( contador <= 20) {
      restart_();
      if (WiFi.status() == WL_CONNECTED){ contador = 50; break; }
      delay(1000);
    }
    if (WiFi.status() == WL_CONNECTED){
      log_serial("WIFI OK");
    }
    else{
      log_serial("WIFI ERRO");
    }
  }
  
  if(!Blynk.connected()) {
      if (WiFi.status() == WL_CONNECTED){ // Só entra se já tiver conectado no WIFI
        Blynk.disconnect();
        Blynk.config(auth);
        delay(500);
        Blynk.connect(9000);
        error_conect = Blynk.connected();
        if (!error_conect) {
          log_serial("Blynk Erro");
        }
        else{
          Blynk.run();
          log_serial("Blynk OK");
        }
        error_conect = Blynk.connected();
    }
  }

}

void chama_conecta_wifi_blynk(){
  conecta_wifi_blynk(ssid, pw);
}
