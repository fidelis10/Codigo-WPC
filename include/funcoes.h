#ifndef FUNCOES_H
#define FUNCOES_H

extern int numero_randomico;
extern const unsigned long intervalo_normal;
extern unsigned long tempo_anterior_token;
extern unsigned long intervalo_token;
extern const unsigned long intervalo_extra;
// extern const unsigned long tempo_extra;

void inicializa_random();
int gera_senha();
void tempo_extra();
void reseta_usuario();

#endif