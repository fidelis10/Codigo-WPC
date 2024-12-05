#include <Arduino.h>
#include <DHTesp.h>
#include "SaidasIrrigacao.h"
#include "EntradasIrrigacao.h"

#define pinSensorUmidade 34
#define DHTPin 32

DHTesp dht;

int SensorUmidade = 0;

bool automaticoUmidade = false;

int porcentagem = 0;

int estadoSolo;

float Temperatura = 0;

bool tratamento_bomba_irrigacao;

String temperaturaDuasCasas;

String estadoDoSolo;

void inicia_entradasIrrigacao()
{
    pinMode(pinSensorUmidade, INPUT);
    dht.setup(DHTPin, DHTesp::DHT22);
}

void atualiza_entradasIrrigacao()
{
    Temperatura = dht.getTemperature();
    temperaturaDuasCasas = String(Temperatura, 2);
    SensorUmidade = analogRead(pinSensorUmidade);
    porcentagem = map(SensorUmidade, 1700, 4095, 100, 0);
    
 
 }
void verificar_automatico()
{
   if (automaticoUmidade)
  {
  if (porcentagem < 10) {
    EstadoBombaIrrigacao = true;
    tratamento_bomba_irrigacao = true;
    estadoSolo = 1;
   }
 else if (porcentagem >= 10 && porcentagem <= 50)
  {
    EstadoBombaIrrigacao = true;
    tratamento_bomba_irrigacao = true;
    estadoSolo = 2;
  }
   else if (porcentagem > 50) {
    EstadoBombaIrrigacao = false;
    tratamento_bomba_irrigacao = false;
    estadoSolo = 3;
      }
  }
  else
  {
    if (porcentagem <= 10) {
    estadoSolo = 1;
   }
 else if (porcentagem > 10 && porcentagem <= 50)
  {
    estadoSolo = 2;
  }
   else if (porcentagem > 60) {
    estadoSolo = 3;
      }
  }
}
 void verificar_estadoSolo()
{
  switch (estadoSolo)
  {
  case 1:
    estadoSolo = 1;
    estadoDoSolo = "Solo Seco";
    break;
  case 2:
    estadoSolo = 2;
    estadoDoSolo = "Solo Umido";
    break;
  case 3:
    estadoSolo = 3;
    estadoDoSolo = "Solo Umido o Suficiente";
    break;
  default:
    estadoSolo = 0;
    break;
  }
}

