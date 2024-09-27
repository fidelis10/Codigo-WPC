#include <Arduino.h>
#include "Atuadores.h"

bool Acionar_Telhado = false;

void funcao_telhado()
{
  if (Acionar_Telhado == false)
  {
    angulo = 0;
    // if (millis() - tempoAnterior4 >= espera3)
    // {
    //   tempoAnterior4 = millis();
    //   sendMessage("Telhado Aberto");
    // }
  }
  else if (Acionar_Telhado == true)
  {
    angulo = 180;
    // if (millis() - tempoAnterior5 >= espera3)
    // {
    //   tempoAnterior5 = millis();
    //   sendMessage("Telhado Aberto");
    // }
  }
  posiciona_servo(angulo);
}