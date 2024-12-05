#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "DisplayLCD.h"
#include "funcoes.h"
#include "iot.h"

LiquidCrystal_I2C lcd(0x27, 20, 4);

int contador = 30;

unsigned long tempoAnteriorLCD = 0;

unsigned long tempoAnteriorLCD2 = 0;

bool trocaSenha = false;

int intervaloAtual;

bool mudaDisplay = true;

byte vintePorCento[] =
    {
        B10000,
        B10000,
        B10000,
        B10000,
        B10000,
        B10000,
        B10000,
        B10000};

byte quarentaPorCento[] =
    {
        B11000,
        B11000,
        B11000,
        B11000,
        B11000,
        B11000,
        B11000,
        B11000};

byte SessentaPorCento[] =
    {
        B11100,
        B11100,
        B11100,
        B11100,
        B11100,
        B11100,
        B11100,
        B11100};

byte OitentaPorCento[] =
    {
        B11110,
        B11110,
        B11110,
        B11110,
        B11110,
        B11110,
        B11110,
        B11110};

void inicializaLCD()
{
    lcd.init();
    lcd.backlight();
    lcd.createChar(0, vintePorCento);
    lcd.createChar(1, quarentaPorCento);
    lcd.createChar(2, SessentaPorCento);
    lcd.createChar(3, OitentaPorCento);
}

void temporizador(byte linha, int tempo, int tempoTotal)
{
    lcd.setCursor(0, linha);
    byte blocosPreenchidos = tempo * 20 / tempoTotal;
    byte RestoDivisao = (tempo * 20) % tempoTotal;
    byte blocoParcial = map(RestoDivisao, 0, tempoTotal, 0, 4); // vai de 0 ate o tempo total e faz ele ficar entre 0 e 4
    
    for (int i = 0; i < blocosPreenchidos; i++)
    {
        lcd.write(byte(255));
    }
    if (blocoParcial > 0)
    {
        lcd.write(byte(blocoParcial));
    }
    for (int i = blocosPreenchidos + (blocoParcial > 0 ? 1 : 0); i < 20; i++)
    {
        lcd.print(" ");
    }
}

void atualizaTemporizador()
{
    if (mudaDisplay == true)
    {

    intervaloAtual = trocaSenha ? 90 : 30;
    if (millis() - tempoAnteriorLCD >= 1000)
    {
        tempoAnteriorLCD = millis();
        if (contador > 0)
    {
        // Decrementa o contador
        contador--;

        // Reseta o contador quando chega a zero
        if (contador <= 0)
        {
            contador = intervaloAtual;
        }
    }
    }
    // Atualiza a barra de progresso no display de acordo com o intervalo
    temporizador(3, contador, intervaloAtual);
}
}

void atualizaDisplay()
{
    lcd.setCursor(0, 0);
    lcd.print("Senha:         ");
    lcd.setCursor(7, 0);
    lcd.print(senha);
    lcd.setCursor(0, 1);
    lcd.print("Usuario: ");
}

void atualizaNome()
{
    lcd.setCursor(9, 1);
    lcd.print(usuarioAutorizado);
}

void limpaNome()
{
    lcd.setCursor(9, 1);
    lcd.print("             ");
}

void limpaSegundaLinha()
{
    lcd.setCursor(0, 2);
    lcd.print("                ");
}

