#include <Arduino.h>
#include "EntradasControleDeAcesso.h"
#include "saidas.h"
#include "Atuadores.h"
#include "iot.h"
#include "SaidasControleDeAcesso.h"
#include <Keypad.h>

#include <Wire.h>
#include <Adafruit_VL53L0X.h>



// Definição dos pinos dos botões




int posicao = 0;






// Dimensões da garrafa cilíndrica
const float raio_cm = 5.0;        // Raio da base da garrafa em cm
const float altura_max_cm = 18.0; // Altura total da garrafa em cm

float volume_ml = 0;

// Criar o objeto VL53L0X
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

VL53L0X_RangingMeasurementData_t measure;

// Variáveis para controle de tempo
unsigned long intervaloLeitura = 1000; // Intervalo de 1 segundo
unsigned long tempoAnteriorLaser = 0;  // Armazena o tempo da última leitura







const byte rowsCount = 4;
const byte columsCount = 3;

char keys[rowsCount][columsCount] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}};

byte columnPins[columsCount] = {27, 13, 4}; // pino1, pino2, pino3
byte rowPins[rowsCount] = {33, 25, 26, 14}; // pino4, pino5, pino6, pino7
// Criacao de objetos
Keypad keypad = Keypad(makeKeymap(keys), rowPins, columnPins, rowsCount, columsCount);

const char password[] = "40028922";
/* Quantidade de caracteres que a senha possui */
const int caracteres = 8;

void sendMessage(String message);

// Inicializa as entradas


void atualiza_entradas()
{
  Wire.begin();
}




void teclado()
{
  char key = keypad.getKey();

  /* Se uma tecla for pressionada */
  if (key)
  {
    Serial.println(key);

    /* Se a tecla pressionada for “*” ou “#”, tranca a fechadura */
    if (key == '*' || key == '#')
    {
      Acionar_teclado = false;
      posicao = 0;
    }
    else if (key == password[posicao])
    {
      posicao++;
    }
    else
    {
      posicao = 0; // Reinicia a posição se a tecla não coincidir
    }
    if (posicao == caracteres)
    {
      Acionar_teclado = true;
    }
  }
}

void laser()
{
  unsigned long tempoAtual = millis();

  // Verificar se já se passou 1 segundo desde a última leitura
  if (tempoAtual - tempoAnteriorLaser >= intervaloLeitura)
  {
    // Atualizar o tempo da última leitura
    tempoAnteriorLaser = tempoAtual;

    // Fazer uma leitura de distância
    lox.rangingTest(&measure, false);

    // Verificar se a medição é válida
    if (measure.RangeStatus != 4)
    { // Se não houver erro de medição
      // Converter a distância de mm para cm
      float distancia_cm = measure.RangeMilliMeter / 10.0;

      // Calcular a altura da água na garrafa
      float altura_agua_cm = distancia_cm;

      // Se a altura da água for válida
      if (altura_agua_cm > 0 && altura_agua_cm <= altura_max_cm)
      {
        // Calcular o volume da água na garrafa cilíndrica
        float volume_cm3 = 3.1416 * raio_cm * raio_cm * altura_agua_cm;

        // Converter volume de cm³ para ml (1 cm³ = 1 ml)
        volume_ml = volume_cm3;

        // Exibir a distância medida e o volume calculado
        Serial.print("Distância medida: ");
        Serial.print(distancia_cm);
        Serial.println(" cm");

        Serial.print("Volume de água: ");
        Serial.print(volume_ml);
        Serial.println(" ml");
      }
      else
      {
        Serial.println("Altura da água fora do intervalo válido.");
      }
    }
    else
    {
      Serial.println("Falha na medição de distância.");
    }
  }
}


