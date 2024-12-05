#include <Arduino.h>
#include "iot.h"
#include "saidas.h"
#include "EntradasControleDeAcesso.h"
#include "tempo.h"
#include "Saidas.h"
#include "funcoes.h"
#include "EntradasIrrigacao.h"
#include "EntradasTelhado.h"

#include "SaidasControleDeAcesso.h"

#include "SaidasIrrigacao.h"

#include "SaidasTelhado.h"

#include "Saidas.h"

#include "Preferencias.h"

#include "Atuadores.h"
#include "DisplayLCD.h"
#include "FitaLed.h"
#include <Memoria.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <WebServer.h>
#include <UrlEncode.h>
#include <HTTPClient.h>
// #include <Preferences.h>

unsigned long tempoAnterior = 0;
unsigned long tempoAnterior2 = 0;
unsigned long tempoAnterior3 = 0;
const unsigned long espera = 5000;
const unsigned long espera2 = 50000;

WebServer server(80);

String phoneNumber = "5511966143935"; // Seu número de telefone com código internacional
String apiKey = "6415795";            // Sua API Key

// Definicao dos topicos de publicacao
#define mqtt_publish_topic1 "projeto/WPC"
#define mqtt_publish_topic2 "projeto/dados"

void MandaMensagem();

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
  // setup_time();
  // atualiza_entradas();
  inicia_entradasIrrigacao();
  // Serial.println("Passo 3");
  inicia_bombaCaixaDeAgua();
  // Serial.println("Passo 4");
  inicializa_entradas_Telhado();
  // Serial.println("Passo 5");
  inicializa_saidasIrrigacao();
  // Serial.println("Passo 6");
  // iniciaMemoria();
  inicializa_saidas();
  Serial.println("Passo 7");
  inicializa_servo();
  Serial.println("Passo 8");
  // carregarEstadoTelhado(Acionar_Telhado, SensordeChuva);
  inicializaLCD();
  Serial.println("Passo 9");
  // inicializa_fitaLed();
  Serial.println("Passo 10");
  inicializa_fitaLed();
  Serial.println("Passo 11");
  inicializa_random();
  Serial.println("Passo 12");
  server.on("/ligar_led", []()
            {
    EstadoLed = true;
    Serial.println("Ligado");
    if (millis() - tempoAnterior2 >= espera2) {
      tempoAnterior2 = millis();
      sendMessage("Led ligado");
    }
    server.send(200, "text/plain","Ligar LED"); });
  server.on("/desligar_led", []()
            {
    EstadoLed = false;
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
  server.on("/ligar_bomba1", []()
            {
        automatico_laser = false;
        EstadoBombaCaixaDeAgua = true;
        server.send(200, "text/plain","Ligar Bomba 1"); });
  server.on("/desligar_bomba1", []()
            {
        automatico_laser = false;
        EstadoBombaCaixaDeAgua = false;
        server.send(200, "text/plain","Desligar Bomba 1"); });
  server.on("/ligar_bomba2", []()
            {
        automaticoUmidade = false;
        EstadoBombaIrrigacao = true;
        server.send(200, "text/plain","Ligar Bomba 2"); });
  server.on("/desligar_bomba2", []()
            {
      automaticoUmidade = false;
      EstadoBombaIrrigacao = false;
      server.send(200, "text/plain","Desligar Bomba 2"); });
  server.on("/ligar_bomba3", []()
            {
        EstadoBombaCisterna = true;
        server.send(200, "text/plain","Ligar Bomba 3"); });
  server.on("/desligar_bomba3", []()
            {
        EstadoBombaCisterna = false;
        server.send(200, "text/plain","Desligar Bomba 3"); });
  server.on("/Ativar_automatico_umidade", []()
            {
        automaticoUmidade = true;
        server.send(200, "text/plain","Ativar Automatico Umidade"); });
  server.on("/Ativar_automatico_laser", []()
            {
        automatico_laser = true;
        server.send(200, "text/plain","Ativar Automatico Laser"); });
  server.begin();
  Serial.print("Passo 13");
}
void loop()
{
  // atualiza_time();
  atualiza_mqtt();
 // Serial.println("Passo 14");
  SensorDeChuva();
 // Serial.println("Passo 15");
  display();
  atualiza_entradasIrrigacao();
 // Serial.println("Passo 16");
  atualiza_entradas_Telhado();
  // Serial.println("Passo 17");
  atualiza_bombaCaixaDeAgua();
  // Serial.println("Passo 18");
  atualiza_bombasIrrigacao();
  //  Serial.println("Passo 19");
  teclado();
  // Serial.println("Passo 20");
  funcao_telhado();
  // Serial.println("Passo 21");
  funcao_teclado();
  // atualiza_bombas();
  verificar_automatico();
  // Serial.println("Passo 22");
  verificar_estadoSolo();
  // Serial.println("Passo 23");
  verifica_CaixaDeAgua();
  // Serial.println("Passo 24");
  // laser();
  atualizaMemoria();
  // Serial.println("Passo 25");
  MandaMensagem();
  // Serial.println("Passo 26");
  atualizaTemporizador(); 
  atualizaDisplay();
  randomiza_senha();
  acaoJardim();
  // Serial.println("Passo 27");
  // Serial.println("Passo 28");
  server.handleClient();
  // trancada();
  // destrancada();

 char Json[512];
  JsonDocument doc;

  if (millis() - tempoAnterior >= espera)
  {
    tempoAnterior = millis();
    doc["Led"] = EstadoLed;
    doc["Umidade"] = porcentagem;
    doc["Chuva"] = Chuva;
    doc["Telhado"] = Telhado;
    doc["Temperatura"] = temperaturaDuasCasas;
    doc["Porta"] = porta;
    doc["BombaIrrigacao"] = EstadoBombaIrrigacao;
    doc["BombaCaixaDeAgua"] = EstadoBombaCaixaDeAgua;
    doc["BombaCisterna"] = EstadoBombaCisterna;
    doc["Capacidade_da_caixa_dagua"] = volume_ml;
    doc["EstadoSolo"] = estadoDoSolo;
    doc["Automatico_Umidade"] = automaticoUmidade;
    doc["Automatico_Laser"] = automatico_laser;
    doc["Senha"] = senha;
    serializeJson(doc, Json, sizeof(Json));
    if(!publica_mqtt(mqtt_publish_topic2, Json))
    {
      Serial.println("Erro ao publicar");
    }
  }
}

void MandaMensagem()
{
  if (millis() - tempoAnterior2 >= espera2)
  {
    tempoAnterior2 = millis();
    // sendMessage("A media dos 10 ultimos valores do sensor de umidade é: " + String(media) + "%");
    // sendMessage("A media dos 10 ultimos valores em ML da Caixa D'Agua é: " + String(mediaMl));
  }
}
