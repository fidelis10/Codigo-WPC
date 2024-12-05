#include <ESP32Servo.h>
#include "Atuadores.h"
#include "Preferencias.h"

#define pinServo 15
#define pinServo2 12

int angulo = 0;
int angulo_servo2 = 0;

Servo servoMotor;
Servo servoMotor2;

unsigned long tempoAnterior7 = 0;
unsigned long tempoAnterior8 = 0;
unsigned long espera7 = 1000;
unsigned long espera8 = 1000;

void inicializa_servo()
{
    servoMotor.attach(pinServo, 500, 2500);
    servoMotor2.attach(pinServo2, 500, 2500);
}

void posiciona_servo(int posicao)
{

    angulo = posicao;
    servoMotor.write(angulo);
    if (millis() - tempoAnterior7 >= espera7)
    {
        tempoAnterior7 = millis();
        Serial.println(angulo);
    }
}
void posiciona_servo2(int posicao2)
{
    
    angulo_servo2 = posicao2;
    servoMotor2.write(angulo_servo2);
    if (millis() - tempoAnterior8 >= espera8)
    {
        tempoAnterior8 = millis();
        Serial.println(angulo_servo2);
    }
}