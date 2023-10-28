#include <Arduino.h>
#include <LiquidCrystal.h>
#include "ESP8266.h"
#include "SoftwareSerial.h"
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <MySQL_Driver.h>

SoftwareSerial ConsoleOut(8, 9);

#define SSID "MySSID"
#define PWD "MyPassword"

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int pinoSensor = A0;
const int pinoValvula = 10;
const int limiarSeco = 60;
const int tempoRega = 5;
int umidadeSolo = 0;
int SensorTempPino = 1;
int AlertaTempBaixa = 7;
int AlertaTempAlta = 6;
int TempBaixa = 23;
int TempAlta = 32;

// Configurações do banco de dados
char dbServer[] = "endereco_do_servidor";
char dbUser[] = "seu_usuario";
char dbPassword[] = "sua_senha";
char dbName[] = "seu_banco_de_dados";
char dbTable[] = "sua_tabela";

// Declaração de objetos MySQL
WiFiClient client;
MySQL_Connection conn((Client *)&client);
MySQL_Cursor cursor = conn.newCursor();

void setup() {
  pinMode(pinoValvula, OUTPUT);
  digitalWrite(pinoValvula, HIGH);
  pinMode(AlertaTempBaixa, OUTPUT);
  pinMode(AlertaTempAlta, OUTPUT);
  lcd.begin(16, 2);
  lcd.print("    BEM-VINDO ");
  Serial.begin(9600);
  delay(4000);
  lcd.clear();

  WiFi.reset(WIFI_RESET_HARD);
  WiFi.begin(9600);

  if (WiFi.join(SSID, PWD) == WIFI_ERR_OK) {
    char *ipAddress = WiFi.ip(WIFI_MODE_STA);
    ConsoleOut.print(F("\n\rIP address:"));
    ConsoleOut.print(ipAddress);
    ConsoleOut.print(':');
    char ap[31];
    if (WiFi.isConnect(ap))
      ConsoleOut.println(ap);
    else
      ConsoleOut.println(F("NAO ENCONTRADO"));
  } else
    while (1);
}

void loop() {
  int SensorTempTensao = analogRead(SensorTempPino);
  float Tensao = SensorTempTensao * 5;
  Tensao /= 1024;
  float TemperaturaC = (Tensao - 0.5) * 100;
  float TemperaturaF = (TemperaturaC * 9 / 5) + 32;

  lcd.setCursor(0, 1);
  lcd.print(TemperaturaC);

  lcd.setCursor(9, 1);
  lcd.print(TemperaturaF);

  if (TemperaturaC >= TempAlta) {
    digitalWrite(AlertaTempBaixa, LOW);
    digitalWrite(AlertaTempAlta, HIGH);
  } else if (TemperaturaC <= TempBaixa) {
    digitalWrite(AlertaTempBaixa, HIGH);
    digitalWrite(AlertaTempAlta, LOW);
  } else {
    digitalWrite(AlertaTempBaixa, LOW);
    digitalWrite(AlertaTempAlta, LOW);
  }

  for (int i = 0; i < 5; i++) {
    lcd.setCursor(0, 1);
    umidadeSolo = analogRead(pinoSensor);
    umidadeSolo = map(umidadeSolo, 1023, 0, 0, 100);
    lcd.print(umidadeSolo);
    lcd.print("%");
    delay(1000);
  }

  if (umidadeSolo <= limiarSeco) {
    lcd.setCursor(0, 1);
    lcd.print("    REGANDO     ");
    digitalWrite(pinoValvula, LOW);
    delay(tempoRega * 1000);
    digitalWrite(pinoValvula, HIGH);
  } else if (umidadeSolo > 75) {
    lcd.clear();
    lcd.print("SOLO ENCHARCADO");
    delay(2000);
    lcd.clear();
    lcd.print("REALIZE UMA");
    lcd.setCursor(0, 1);
    lcd.print("DRENAGEM!");
    delay(2000);
    lcd.clear();
    delay(1500);
  } else {
    lcd.setCursor(0, 1);
    lcd.clear();
    lcd.print("SOLO SACIADO!");
    delay(3000);
    lcd.clear();
  }

  // Conexão ao banco de dados
  if (conn.connect(dbServer, dbUser, dbPassword, dbName)) {
    // Inserção de dados no banco de dados
    char INSERT_SQL[128];
    sprintf(INSERT_SQL, "INSERT INTO %s (temperatura, umidade, timestamp) VALUES (%.2f, %d, NOW())", dbTable, TemperaturaC, umidadeSolo);

    cursor.execute(INSERT_SQL);

    // Desconexão do banco de dados
    conn.close();
  } else {
    // Falha na conexão com o banco de dados
  }

  // Aguarde um tempo antes de repetir a leitura da umidade e temperatura
  delay(60000);  // Aguarde 60 segundos
}

