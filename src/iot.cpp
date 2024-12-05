#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h> // Baixar a biblioteca PubSubClient
#include <TimeLib.h>
#include "iot.h"
#include "senhas.h"
#include "SaidasControleDeAcesso.h"
#include "SaidasIrrigacao.h"
#include "SaidasTelhado.h"
#include "Saidas.h"
#include "Atuadores.h"
#include "EntradasControleDeAcesso.h"
#include "funcoes.h"
#include "EntradasIrrigacao.h"
#include "EntradasTelhado.h"
#include "Preferencias.h"
#include "DisplayLCD.h"
#include "FitaLed.h"
// #include <U8g2lib.h>
#include <ArduinoJson.h>


// Use @myidbot (IDBot) para saber qual o seu ID
#define CHAT_ID "5976715759"

bool porta;


String user;

#define USUARIO_PADRAO "!@#$%^&*()xyz"

String usuarioAutorizado = USUARIO_PADRAO;







// Definição dos tópicos de inscrição
#define mqtt_topic1 "projeto/dados"
#define mqtt_topic2 "projeto/node-red" // Topico do node-red:(que ira receber informacoes)
#define mqtt_topic3 "projeto/site" // Topico do site e o do node-red
#define mqtt_topic4 "projeto/appInventor" // Topico do aplicativo

// Definição do ID do cliente MQTT randomico
const String cliente_id = "ESP32Client" + String(random(0xffff), HEX);

unsigned long tempoMensagemMQTT = 0;      // Tempo em que a última mensagem MQTT foi recebida
const unsigned long duracaoAberto = 5000; // Duração em milissegundos (5 segundos)

// Definição dos dados de conexão


// Protótipos das funções
void tratar_msg(char *topic, String msg);
void callback(char *topic, byte *payload, unsigned int length);
void reconecta_mqtt();
void inscricao_topicos();

WiFiClientSecure espClient; 
PubSubClient client(AWS_IOT_ENDPOINT, mqtt_port, callback, espClient);

// Inicia a conexão WiFi
void setup_wifi()
{
  
  Serial.println();
  Serial.print("Conectando-se a Rede WiFi ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Conectado ao WiFi com sucesso com IP: ");
  Serial.println(WiFi.localIP());
  espClient.setCACert(AWS_CERT_CA);
  espClient.setCertificate(AWS_CERT_CRT);
  espClient.setPrivateKey(AWS_CERT_PRIVATE);
  client.setBufferSize(512);
}

// Inicia a conexão MQTT


// Atualiza a conexão MQTT
void atualiza_mqtt()
{
  // Serial.print("Passo 6");
  client.loop();
  if (!client.connected())
  {
    // Serial.print("Passo 7");
    reconecta_mqtt();
  }
}

// Função de callback chamada quando uma mensagem é recebida
void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.printf("Mensagem recebida [ %s ] \n\r", topic);
  String msg = "";
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
    msg += (char)payload[i];
  }
  Serial.println();
  tratar_msg(topic, msg);
}

// Função de reconexão ao Broker MQTT
void reconecta_mqtt()
{
  while (!client.connected())
  {
    Serial.print("Tentando se conectar ao Broker AWS: ");
    Serial.println(AWS_IOT_ENDPOINT);
    if (client.connect(THINGNAME))
    {
      Serial.println("Conectado a AWS");
      inscricao_topicos();
    }
    else
    {
      Serial.println("Falha ao conectar a AWS."); 
    
      Serial.println("Havera nova tentativa de conexao em 2 segundos");
      delay(2000);
    }
  }
}

// Publica uma mensagem no tópico MQTT
bool publica_mqtt(String topico, char * msg)
{
  // Serial.println("Publicando mensagem MQTT no topico: " + topico + " - " + msg);
  return client.publish(topico.c_str(), msg);
}

// Inscreve nos tópicos MQTT
void inscricao_topicos()
{
  // client.subscribe(mqtt_topic1);
  client.subscribe(mqtt_topic2);
  client.subscribe(mqtt_topic3);
  client.subscribe(mqtt_topic4);
}

// Trata as mensagens recebidas
void tratar_msg(char *topic, String msg)
{
  if (strcmp(topic, mqtt_topic2) == 0)
  {
    JsonDocument doc;
    deserializeJson(doc, msg);
    if (doc.containsKey("Telhado"))
    {
      TelhadoAutomatico = false;
      Telhado = doc["Telhado"];
      if (Telhado)
      {
        Acionar_Telhado = true;
        // ativarFitaLed = true;
        // salvarEstadoTelhado(Acionar_Telhado, SensordeChuva);
      }
      else 
      {
        Acionar_Telhado = false;
        // ativarFitaLed = false;
        // salvarEstadoTelhado(Acionar_Telhado, SensordeChuva);
      }
    }
    if (doc.containsKey("Porta"))
    {
      porta = doc["Porta"];
      if (porta)
      {
        Acionar_teclado = true;
      }
      else {
         Acionar_teclado = false;
      }
      }
    if (doc.containsKey("BombaIrrigacao"))
    {
      automaticoUmidade = false;
      EstadoBombaIrrigacao = doc["BombaIrrigacao"];
    }
    if (doc.containsKey("BombaCaixaDeAgua"))
    {
      automatico_laser = false;
     EstadoBombaCaixaDeAgua = doc["BombaCaixaDeAgua"];
  }
  if (doc.containsKey("Automatico_Umidade"))
  {
    automaticoUmidade = doc["Automatico_Umidade"];
  }
  if (doc.containsKey("Automatico_Laser"))
  {
    automatico_laser = doc["Automatico_Laser"];
  }
  if (doc.containsKey("Automatico_Telhado"))
  {
    TelhadoAutomatico = true;
    TelhadoAutomatico = doc["Automatico_Telhado"];
  }
  if (doc.containsKey("ColorPicker"))
  {
    String hexString = doc["ColorPicker"];
    // Serial.println(hexString);
    trocarCorLed = strtol(hexString.c_str(), NULL, 16);  // Converte para int
    Serial.println(trocarCorLed, HEX);
}
  if (doc.containsKey("LigarLed"))
  {
    ativarFitaLed = doc["LigarLed"];
  }
  else if (strcmp(topic, mqtt_topic3) == 0)
  {
    int senha = randomiza_senha();

    JsonDocument doc;
    deserializeJson(doc, msg);
    if (doc.containsKey("token")) //tem o campo token?
    {
      if (doc["token"] == senha) //o token é igual ao gerado?
      {
        if (doc.containsKey("user")) //tem o campo user?
        {
          String user = doc["user"]; //pega o valor do campo user

          if (usuarioAutorizado == USUARIO_PADRAO) // se o usuario autorizado for o padrao
            usuarioAutorizado = user; //atualiza o usuario autorizado

          if (usuarioAutorizado == user) //se o usuario autorizado for igual ao usuario que enviou a mensagem
          {
            tempoSenhaEstendido(); //estende o tempo da senha, ao espirar o usuario autorizado volta a ser o padrao

            
            

          if (doc.containsKey("Telhado"))
          
    {
      TelhadoAutomatico = false;
      Telhado = doc["Telhado"];
      if (Telhado)
      {
        Acionar_Telhado = true;
        // ativarFitaLed = true;
        // salvarEstadoTelhado(Acionar_Telhado, SensordeChuva);
      }
      else 
      {
        Acionar_Telhado = false;
        // ativarFitaLed = false;
        // salvarEstadoTelhado(Acionar_Telhado, SensordeChuva);
      }
    }
    if (doc.containsKey("Porta"))
    {
      porta = doc["Porta"];
      if (porta)
      {
        Acionar_teclado = true;
      }
      else {
         Acionar_teclado = false;
      }
      }
    if (doc.containsKey("BombaIrrigacao"))
    {
      automaticoUmidade = false;
     EstadoBombaIrrigacao = doc["BombaIrrigacao"];
    }
    if (doc.containsKey("BombaCaixaDeAgua"))
    {
      automatico_laser = false;
      EstadoBombaCaixaDeAgua = doc["BombaCaixaDeAgua"];
       }
  if (doc.containsKey("Automatico_Umidade"))
  {
    automaticoUmidade = doc["Automatico_Umidade"];
  }
  if (doc.containsKey("Automatico_Laser"))
  {
    automatico_laser = doc["Automatico_Laser"];
  }
  if (doc.containsKey("Led"))
  {
    EstadoLed = doc["Led"];
          }
          if (doc.containsKey("Automatico_Telhado"))
  {
    TelhadoAutomatico = true;
    TelhadoAutomatico = doc["Automatico_Telhado"];
  }
  if (doc.containsKey("FitaLed"))
  {
    // ativarFitaLed = doc["FitaLed"];
  }
        }
      }
    }
  }
  }
  else if (strcmp(topic, mqtt_topic4) == 0)
  {
    JsonDocument doc;
    deserializeJson(doc, msg);
    if(doc.containsKey("Telhado"))
    {
      TelhadoAutomatico = false;
      Telhado = doc["Telhado"];
      if (Telhado)
      {
        Acionar_Telhado = true;
        // ativarFitaLed = true;
        // salvarEstadoTelhado(Acionar_Telhado, SensordeChuva);
      }
      else 
      {
        Acionar_Telhado = false;
        // ativarFitaLed = false;
        // salvarEstadoTelhado(Acionar_Telhado, SensordeChuva);
      }
    }
    if (doc.containsKey("Porta"))
    {
      porta = doc["Porta"];
      if (porta)
      {
        Acionar_teclado = true;
      }
      else {
         Acionar_teclado = false;
      }
      }
    if (doc.containsKey("BombaIrrigacao"))
    {
      automaticoUmidade = false;
      EstadoBombaIrrigacao = doc["BombaIrrigacao"];
    }
    if (doc.containsKey("BombaCaixaDeAgua"))
    {
      automatico_laser = false;
     EstadoBombaCaixaDeAgua = doc["BombaCaixaDeAgua"];
  }
  if (doc.containsKey("Automatico_Umidade"))
  {
    automaticoUmidade = doc["Automatico_Umidade"];
  }
  if (doc.containsKey("Automatico_Laser"))
  {
    automatico_laser = doc["Automatico_Laser"];
  }
  if (doc.containsKey("Led"))
  {
    EstadoLed = doc["Led"];
  }
  if (doc.containsKey("Automatico_Telhado"))
  {
    TelhadoAutomatico = doc["Automatico_Telhado"];
  }
  if (doc.containsKey("ParaTempo"))
  {
    //  ParaSenha = false;
    //  mudaDisplay = false;
  }
  if (doc.containsKey("VoltaTempo"))
  {
    //  ParaSenha = true;
    //  mudaDisplay = true;
  }
  if (doc.containsKey("FitaLed"))
  {
    // ativarFitaLed = doc["FitaLed"];
  }
  if (doc.containsKey("ColorPicker"))
  {
   trocarCorLed = doc["ColorPicker"];
}
}
}
}
void resetaUsuario()
{
  usuarioAutorizado = USUARIO_PADRAO;
}