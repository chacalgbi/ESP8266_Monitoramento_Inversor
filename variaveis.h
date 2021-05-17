// Variáveis da configuração WIFI - SPIFFS
String REDE="";  char ssid[30];   int tamanho_rede = 0;
String SENHA=""; char pw[30];     int tamanho_senha = 0;
String NOME="";  char device[30]; int tamanho_nome = 0;

char auth[] = "wfX7sEbCeRYoMPX4wYG8oUUVrI9fM1WB"; // placa 1
//char auth[] = "jlewVyJVaMD9Atoy0_98AHOB2ypS7t8G"; // placa 2
//char auth[] = "MU31BfSk4nBca-TtVupZQYNRv6eXiiGw"; // placa 3

float temp_dht = 0;
float umi_dht = 0; 
float bat1   = 0;
float bat_minima = 0;
float bat_normal = 0;
float xx = 0;
float bat_const = 22.69; // placa 1
//float bat_const = 22.89; // placa 2
//float bat_const = 22.78; // placa 3

byte tensoes = 0;

byte tensao_minima = 0;
byte tensao_normal = 0;

bool inverter = false;
bool serial_ativo  = false;
bool error_conect = false;
bool trava_bateria = false;
bool trava_AC_IN = false;
bool trava_AC_OUT = false;
bool Sem_AC_IN = false;
bool Sem_AC_OUT = false;
bool iniciar = false;
bool modo_config = false;

String temp = "";
String device_str = "";
String pw_str = "";
String ssid_str = "";
String bat1_str   = "";
String dht_str    = "";
String IP = "";
String sinal_wifi = "";
String currentTime = "";
String hora = "";
String data_ = "";
String dia_semana = "";
