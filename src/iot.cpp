#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h> // Baixar a biblioteca PubSubClient
#include <TimeLib.h>
#include "iot.h"
#include "senhas.h"
#include "Saidas.h"
#include "Atuadores.h"
#include "Entradas.h"
#include "funcoes.h"
#include <U8g2lib.h>
#include <ArduinoJson.h>

#define BOT_TOKEN "7019499301:AAH0ZMY1iIDW0YftWxebAJdRwR5sLT_gcuw"

// Use @myidbot (IDBot) para saber qual o seu ID
#define CHAT_ID "5976715759"

bool porta;

String usuario_autorizado = "()*&#$$@!@#$%^()";


// Definição dos tópicos de inscrição
#define mqtt_topic1 "projeto/WPC"
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
  espClient.setCACert(AWS_CERT_CA);
  espClient.setCertificate(AWS_CERT_CRT);
  espClient.setPrivateKey(AWS_CERT_PRIVATE);
}

// Inicia a conexão MQTT


// Atualiza a conexão MQTT
void atualiza_mqtt()
{
  client.loop();
  SensorDeChuva();
  if (!client.connected())
  {
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
    Serial.print("Tentando se conectar ao Broker MQTT: ");
    Serial.println(AWS_IOT_ENDPOINT);
    if (client.connect(THINGNAME))
    {
      Serial.println("Conectado ao Broker MQTT");
      inscricao_topicos();
    }
    else
    {
      Serial.println("Falha ao conectar ao Broker."); 
      Serial.println("Havera nova tentativa de conexao em 2 segundos");
      delay(2000);
    }
  }
}

// Publica uma mensagem no tópico MQTT
void publica_mqtt(String topico, String msg)
{
  client.publish(topico.c_str(), msg.c_str());
}

// Inscreve nos tópicos MQTT
void inscricao_topicos()
{
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
      Telhado == doc["Telhado"];
      if (Telhado)
      {
        Acionar_Telhado = true;
      }
      else 
      {
        Acionar_Telhado = false;
      }
    }
    if (doc.containsKey("Porta"))
    {
      porta == doc["Porta"];
      if (porta)
      {
        Acionar_teclado = true;
      }
      else {
         Acionar_teclado = false;
      }
      }
    if (doc.containsKey("Bomba1"))
    {
      bomba1 == doc["Bomba1"];
      if (bomba1)
      {
        EstadoBomba1 = !EstadoBomba1;
        
      }
      else 
      {
        EstadoBomba1 = !EstadoBomba1;
      }
    }
    if (doc.containsKey("Bomba2"))
    {
     bomba2 == doc["Bomba2"];
     if (bomba2)
      {
        EstadoBomba2 = !EstadoBomba2;
        
      }
      else 
      {
        EstadoBomba2 = !EstadoBomba2;
    }
    if (doc.containsKey("Bomba3"))
    {
      bomba3 == doc["Bomba3"];
      if (bomba3)
      {
        EstadoBomba3 = !EstadoBomba3;
      }
      else 
      {
        EstadoBomba3 = !EstadoBomba3;
      }
    }
  }
}
  else if (strcmp(topic, mqtt_topic3) == 0)
  {
    int senha = gera_senha();
    JsonDocument doc;
    deserializeJson(doc, msg);

    if (doc.containsKey("Token"))
    {
      if (doc["Token"] == senha)
      {
        if (doc.containsKey("User"))
        {
          String user = doc["User"].as<String>();
          if (usuario_autorizado == "()*&#$$@!@#$%^()") // se é a primeira conecxao
            usuario_autorizado = user;

          if (usuario_autorizado == user)
          {
            tempo_extra();
          if (doc.containsKey("Telhado"))
    {
      Telhado == doc["Telhado"];
      if (Telhado)
      {
        Acionar_Telhado = true;
      }
      else 
      {
        Acionar_Telhado = false;
      }
    }
    if (doc.containsKey("Porta"))
    {
      porta == doc["Porta"];
      if (porta)
      {
        Acionar_teclado = true;
      }
      else {
         Acionar_teclado = false;
      }
      }
    if (doc.containsKey("Bomba1"))
    {
      bomba1 == doc["Bomba1"];
      if (bomba1)
      {
        EstadoBomba1 = !EstadoBomba1;
        
      }
      else 
      {
        EstadoBomba1 = !EstadoBomba1;
      }
    }
    if (doc.containsKey("Bomba2"))
    {
     bomba2 == doc["Bomba2"];
     if (bomba2)
      {
        EstadoBomba2 = !EstadoBomba2;
        
      }
      else 
      {
        EstadoBomba2 = !EstadoBomba2;
    }
    if (doc.containsKey("Bomba3"))
    {
      bomba3 == doc["Bomba3"];
      if (bomba3)
      {
        EstadoBomba3 = !EstadoBomba3;
      }
      else 
      {
        EstadoBomba3 = !EstadoBomba3;
      }
    }
  }
          }
        }
      }
    }
  }
}
void reseta_usuario()
{
  usuario_autorizado = "()*&#$$@!@#$%^()";
}