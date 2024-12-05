#include <Arduino.h>
#include <FastLED.h>

#define DATA_PIN 2   // pino do esp32

#define NUM_LEDS 24     // Quantas leds vao ser configuradas


int trocarCorLed = 0xe88717;

bool ativarFitaLed = true;

CRGB leds[NUM_LEDS];


void inicializa_fitaLed()
{
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}


void acaoJardim()
{
  if (ativarFitaLed == true)
  {
    for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = trocarCorLed; //
  }
  FastLED.show();
  }
  else 
  {
    for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = 0x000000; //
  }
  FastLED.show();
}
}