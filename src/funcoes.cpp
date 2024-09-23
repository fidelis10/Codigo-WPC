#include <Arduino.h>
#include "funcoes.h"
#include "tempo.h"

int numero_randomico;

const unsigned long intervalo_normal = 30000;
const unsigned long intervalo_extra = 90000;

unsigned long tempo_anterior_token = 0;
unsigned long intervalo_token = 0;

void inicializa_random()
{
  randomSeed(timeStamp());
}

int gera_senha()
{
  reseta_usuario();
  unsigned long tempo_atual = millis();
  if (tempo_atual - tempo_anterior_token >= intervalo_token)
  {
    if (tempo_anterior_token != intervalo_normal)
      intervalo_token = intervalo_normal; // so pra nao comecar com o numero 0 :)
    tempo_anterior_token = tempo_atual;
    numero_randomico = random(1000, 9999);
    Serial.printf("Nova senha: %d\n", numero_randomico);
  }
  return numero_randomico;
}

void tempo_extra()
{
  if (intervalo_token != intervalo_extra)
  {
  tempo_anterior_token = millis();
  intervalo_token = intervalo_extra;
  Serial.println("Senha estendida por 90s");
  }
}