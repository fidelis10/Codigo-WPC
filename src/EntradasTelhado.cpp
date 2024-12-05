#include <Arduino.h>
#include "SaidasTelhado.h"
#include "EntradasTelhado.h"
#include "FitaLed.h"

#define pinSensorDeChuva 35

bool Telhado;

int SensordeChuva = 0;

bool estadoAnterior = false;
bool estadoAtual;

bool Chuva;

bool TelhadoAutomatico;

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
  if (TelhadoAutomatico)
  {
  // Atualiza o estado atual com base na leitura do sensor
  if (SensordeChuva > 2000)
  {
    Chuva = false;
    Telhado = false;
    Acionar_Telhado = false;
    // ativarFitaLed = false;
  }
  else
  {
     Chuva = true;
     Telhado = true;
     Acionar_Telhado = true;
    //  ativarFitaLed = true;
  }
  }
  else {
    if (SensordeChuva > 2000)
  {
    Chuva = false;
    Telhado = false;
  }
  else
  {
     Chuva = true;
     Telhado = true;
  }
  }
}

