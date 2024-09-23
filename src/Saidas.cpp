#include <Arduino.h>
#include <U8g2lib.h>
#include "Entradas.h"
#include "Saidas.h"
#include "Atuadores.h"

unsigned long tempo_anterior = 0;
const unsigned long intervalo = 5000;

#define pinLed 2
#define pinBomba1 18
#define pinBomba2 5
#define pinBomba3 19


bool EstadoLed = false;
bool EstadoBomba1 = false; 
bool EstadoBomba2 = false; 
bool EstadoBomba3 = false; 

bool Acionar_Telhado = false;

bool Acionar_teclado = false;

bool bomba1;
bool bomba2;
bool bomba3;







U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

void inicializa_saidas()
{

  pinMode(pinLed, OUTPUT);
  pinMode(pinBomba1, OUTPUT);
  pinMode(pinBomba2, OUTPUT);
  pinMode(pinBomba3, OUTPUT);
  u8g2.begin();
}

void display()
{
   digitalWrite(pinLed, EstadoLed);
   if (millis() - tempo_anterior >= intervalo)
   {
    tempo_anterior = millis();
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_profont11_tf);
    u8g2.setCursor(0, 10);
    u8g2.print("Umidade: ");
    u8g2.print(SensorUmidade);
    u8g2.print(" %");
    u8g2.setCursor(0, 20);
    u8g2.print("Chuva: ");
    u8g2.print(Chuva);
    u8g2.print(" %");
    u8g2.sendBuffer();
   }
}

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

void atualiza_bombas()
{
    
    digitalWrite(pinBomba1, EstadoBomba1);
    digitalWrite(pinBomba2, EstadoBomba2); 
    digitalWrite(pinBomba3, EstadoBomba3);  
}
