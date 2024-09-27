#include <Arduino.h>
#include "EntradasIrrigacao.h"


#define pinBombaIrrigacao 19

bool EstadoBombaIrrigacao = false; 

void inicializa_saidasIrrigacao()
{
    pinMode(pinBombaIrrigacao, OUTPUT);
}

void atualiza_bombasIrrigacao()
{
    digitalWrite(pinBombaIrrigacao, EstadoBombaIrrigacao);
    
}

