#include <Arduino.h>
#include "iot.h"
#include "saidas.h"
#include "entradas.h"
#include "tempo.h"
#include "Saidas.h"
#include "funcoes.h"

#include "Atuadores.h"
#include <ArduinoJson.h>
#include <Wire.h>
#include <WebServer.h>
#include <UrlEncode.h>
#include <HTTPClient.h>


unsigned long tempoAnterior = 0;
unsigned long tempoAnterior2 = 0;
unsigned long tempoAnterior3 = 0;
const unsigned long espera = 5000;
const unsigned long espera2 = 15000;

WebServer server(80);

String phoneNumber = "5511966143935"; // Seu número de telefone com código internacional
String apiKey = "6415795";            // Sua API Key



// Definicao dos topicos de publicacao
#define mqtt_publish_topic1 "projeto/WPC"
#define mqtt_publish_topic2 "projeto/dados"

void sendMessage(String message)
{
  String encodedMessage = urlEncode(message);
  String url = "https://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" + encodedMessage;
  HTTPClient http;
  http.begin(url); // Inicializa a conexão HTTP com a URL completa

  int httpResponseCode = http.GET(); // Muda para GET, já que estamos chamando uma URL diretamente

  if (httpResponseCode == 200 || httpResponseCode == 201)
  {
    Serial.println("Mensagem enviada com sucesso");
  }
  else
  {
    Serial.println("Erro no envio da mensagem");
    Serial.print("Código de resposta HTTP: ");
    Serial.println(httpResponseCode);
  }

  http.end(); 
}
void setup()
{
  Wire.begin();
  Serial.begin(115200);
  setup_wifi();
  setup_time();
  inicializa_entradas();
  inicializa_saidas();
  inicializa_servo();
  inicializa_random();
  server.on("/ligar_led", []()
            {
    EstadoLed = !EstadoLed;
    if (millis() - tempoAnterior2 >= espera2) {
      tempoAnterior2 = millis();
      sendMessage("Led ligado");
    }
    server.send(200, "text/plain","Ligar LED"); });
  server.on("/desligar_led", []()
            {
    EstadoLed = !EstadoLed;
    if (millis() - tempoAnterior3 >= espera2) {
      tempoAnterior3 = millis();
      sendMessage("Led desligado");
    }
    server.send(200, "text/plain","Desligar LED"); });
  server.on("/abrir_telhado", []()
            {
    Acionar_Telhado = false;
    server.send(200, "text/plain","Abrir Telhado"); });
  server.on("/fechar_telhado", []()
            {
    Acionar_Telhado = true;
    // if (millis() - tempoAnterior3 >= espera2) {
    //   tempoAnterior2 = millis();
    //   sendMessage("Telhado Fechado");
    // }
    server.send(200, "text/plain","Fechar Telhado"); });
  server.on("/abrir_porta", []()
            {
    Acionar_teclado = true;
    // if (millis() - tempoAnterior3 >= espera2) {
    //   tempoAnterior2 = millis();
    //   sendMessage("Led desligado");
    // }
    server.send(200, "text/plain","Abrir Porta"); });
  server.on("/fechar_porta", []()
            {
    Acionar_teclado = false;
    // if (millis() - tempoAnterior3 >= espera2) {
    //   tempoAnterior2 = millis();
    //   sendMessage("Led desligado");
    // }
    server.send(200, "text/plain","Fechar Porta"); });
  server.begin();
}
void loop()
{
  
  atualiza_time();
  atualiza_entradas();
  atualiza_mqtt();
  SensorDeChuva();
  display();
  temperatura();
  teclado();
  funcao_telhado();
  funcao_teclado();
  atualiza_bombas();
  gera_senha();
  server.handleClient();
  // trancada();
  // destrancada();

  String Json;
  JsonDocument doc;
  String temperaturaDuasCasas = String(Temperatura, 2);
  if (millis() - tempoAnterior >= espera)
  {
    tempoAnterior = millis();
    doc["Led"] = EstadoLed;
    doc["Umidade"] = porcentagem;
    doc["Chuva"] = Chuva;
    doc["Telhado"] = Telhado;
    doc["Temperatura"] = temperaturaDuasCasas;
    doc["Porta"] = porta;
    doc["Bomba1"] = bomba1;
    doc["Bomba2"] = bomba2;
    doc["Bomba3"] = bomba3;
    doc["Capacidade_da_caixa_dagua"] = volume_ml;
    serializeJson(doc, Json);
    publica_mqtt(mqtt_publish_topic1, Json);
  }
}



