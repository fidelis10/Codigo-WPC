#include <Arduino.h>
#include "SaidasTelhado.h"
#include "EntradasTelhado.h"

#define pinSensorDeChuva 35

bool Telhado;

float SensordeChuva = 0;

bool estadoAnterior = false;
bool estadoAtual;

bool Chuva;

void inicializa_entradas_Telhado()
{
    pinMode(pinSensorDeChuva, INPUT);
}

void atualiza_entradas_Telhado()
{
    SensordeChuva = analogRead(pinSensorDeChuva);
}

void SensorDeChuva()
{
  // Atualiza o estado atual com base na leitura do sensor
  if (SensordeChuva > 2000)
  {
    estadoAtual = false;
  }
  else
  {
    estadoAtual = true;
  }
  //
  if (estadoAnterior != estadoAtual)
  {
    if (estadoAtual == false)
    {
      Acionar_Telhado = false;
      Chuva = false;
      Telhado = false;
    }
    else
    {
      Acionar_Telhado = true;
      Chuva = true;
      Telhado = true;
    }

    // Atualiza o estadoAnterior para o próximo ciclo
    estadoAnterior = estadoAtual;
  }
}