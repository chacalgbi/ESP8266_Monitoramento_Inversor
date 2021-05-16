// Variáveis da configuração WIFI - SPIFFS
String REDE="";  char ssid[30];   int tamanho_rede = 0;
String SENHA=""; char pw[30];     int tamanho_senha = 0;
String NOME="";  char device[30]; int tamanho_nome = 0;

char auth[] = "wfX7sEbCeRYoMPX4wYG8oUUVrI9fM1WB";

float temp_dht = 0;
float umi_dht = 0; 
float bat1   = 0;

int tensao_minima = 0;

bool inverter = false;
bool serial_ativo  = false;
bool error_conect = false;
bool limite_AC1_trava = false;
bool limite_AC2_trava = false;
bool limite_AC3_trava = false;
bool limite_AC4_trava = false;
bool limite_AC_trava_mono = false;
bool rele1_estado = false;

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
