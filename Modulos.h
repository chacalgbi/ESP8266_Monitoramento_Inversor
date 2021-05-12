void iniciar_modulos(){
  dht.begin(); delay(10);
}

void leituras_dht(){
  umi_dht  = dht.readHumidity();
  temp_dht = dht.readTemperature();
  
  if (isnan(umi_dht) || isnan(temp_dht)){
    dht_str = "Erro Temp";
    return;
  }
  else{
    dht_str = String(temp_dht,1) + "C, " + String(umi_dht,0) + "%";
  }
}

void Testar_Reles(){
  pino(rele1, 1);
  delay(1000);
  pino(rele1, 0);
  delay(1000);
}
