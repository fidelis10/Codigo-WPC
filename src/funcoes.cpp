#include <Arduino.h>
#include "funcoes.h"
#include "tempo.h"
#include "DisplayLCD.h"
#include "iot.h"

int senha;
const unsigned long intervaloNormal = 30000;
const unsigned long intervaloEstendido = 90000;
unsigned long tempoInicialResetSenha = 0;
unsigned long intervaloResetSenha = 0;

int randomiza_senha()
{
  unsigned long tempoAtual = millis();

  if (tempoAtual - tempoInicialResetSenha >= intervaloResetSenha)
  {
    resetaUsuario();
    if (intervaloResetSenha != intervaloNormal)
      intervaloResetSenha = intervaloNormal;
    tempoInicialResetSenha = tempoAtual;
    senha = random(1000, 9999);
    Serial.printf("Nova Senha: %d\n", senha);
  }
  return senha;
}

void tempoSenhaEstendido()
{
  if (intervaloResetSenha != intervaloEstendido)
  {
    unsigned long tempoAtual = millis();
    tempoInicialResetSenha = tempoAtual;
    intervaloResetSenha = intervaloEstendido;
    Serial.println("Senha estendida por 90 segundos");
  }
}