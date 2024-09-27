#include <Arduino.h>
#include <U8g2lib.h>
#include "EntradasControleDeAcesso.h"
#include "Saidas.h"
#include "Atuadores.h"
#include "EntradasIrrigacao.h"

#include "EntradasTelhado.h"

unsigned long tempo_anterior = 0;
const unsigned long intervalo = 5000;

#define pinLed 2


#define pinBombaCisterna 5

bool EstadoLed = false;

bool EstadoBombaCisterna = false;

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

void inicializa_saidas()
{

  pinMode(pinLed, OUTPUT);
  pinMode(pinBombaCisterna, OUTPUT);
  u8g2.begin();
}

void display()
{
  digitalWrite(pinLed, EstadoLed);
  if (millis() - tempo_anterior >= intervalo)
  {
    tempo_anterior = millis();
    u8g2.clearBuffer();
    u8g2.drawRFrame(2, 3, 123, 60, 4);  // retangulo com as bordas arredondadas
    u8g2.setFont(u8g2_font_lubBI10_te); // muda para a fonte de texto
    u8g2.drawStr(10, 23, "Umi.Solo:");
    u8g2.drawStr(80, 23, String(porcentagem).c_str()); // escreve a umidade do solo
    u8g2.drawStr(100, 23, "%");
    u8g2.drawStr(10, 52, "Temp:");
    u8g2.drawStr(60, 52, String(temperaturaDuasCasas).c_str()); // escreve a temperatura do ambiente
    u8g2.drawStr(80, 52, "*C");
    u8g2.sendBuffer();
  }
}

void atualiza_bombas()
{
 
  digitalWrite(pinBombaCisterna, EstadoBombaCisterna);
}
