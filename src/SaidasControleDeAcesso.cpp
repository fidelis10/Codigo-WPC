#include <Arduino.h>
#include "Atuadores.h"

bool Acionar_teclado = false;

bool EstadoBombaCaixaDeAgua = false;

#define pinBombaCaixaDeAgua 18

void inicia_bombaCaixaDeAgua()
{
    pinMode(pinBombaCaixaDeAgua, OUTPUT);
}

void atualiza_bombaCaixaDeAgua()
{
    digitalWrite(pinBombaCaixaDeAgua, EstadoBombaCaixaDeAgua);
}
void funcao_teclado()
{
    if (Acionar_teclado == false)
    {
        angulo_servo2 = 90;
        // if (millis() - tempoAnterior6 >= espera3)
        // {
        //   tempoAnterior6 = millis();
        //   sendMessage("Porta Fechada");
        // }
    }
    else if (Acionar_teclado == true)
    {
        angulo_servo2 = 180;

        // if (millis() - tempoAnterior7 >= espera3)
        // {
        //   tempoAnterior7 = millis();
        //   sendMessage("Porta Aberta");
        // }
    }
    posiciona_servo2(angulo_servo2);
}