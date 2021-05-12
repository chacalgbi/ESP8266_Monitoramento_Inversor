void iniciar_PINs(){
  pinMode(led,   OUTPUT); digitalWrite(led,   LOW);
  pinMode(rele1, OUTPUT); digitalWrite(rele1, LOW);
  
  pinMode(config_wifi, INPUT);
  pinMode(AC_in, INPUT);
  pinMode(AC_out, INPUT);
}

void pino(byte pin, byte nivel){
  if(nivel == 0){digitalWrite(pin, LOW);}
  if(nivel == 1){digitalWrite(pin, HIGH);}
}
