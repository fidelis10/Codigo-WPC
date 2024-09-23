#include <ESP32Servo.h>
#include "Atuadores.h"

#define pinServo 12
#define pinServo2 15

int angulo = 0;
int angulo_servo2 = 0;


Servo servoMotor;
Servo servoMotor2;

void inicializa_servo() {
    servoMotor.attach(pinServo, 500, 2500);
    servoMotor2.attach(pinServo2, 500, 2500);
}

void posiciona_servo(int posicao) 
{
    angulo = posicao;
    servoMotor.write(angulo);
}
void posiciona_servo2(int posicao2) 
{
    angulo_servo2 = posicao2;
    servoMotor2.write(angulo_servo2);
}