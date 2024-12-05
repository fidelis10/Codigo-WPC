#include <Arduino.h>
void inicializaLCD();
void temporizador(byte linha, int tempo, int tempoTotal);
void atualizaTemporizador();
void atualizaDisplay();
void atualizaNome();
void limpaNome();

extern bool tempoAumenta;

extern int contador;

extern bool trocaSenha;

extern int intervaloAtual;

extern bool mudaDisplay;