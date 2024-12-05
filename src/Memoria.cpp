#include <Arduino.h>
#include <UrlEncode.h>
#include <HTTPClient.h>
#include "EntradasIrrigacao.h"
#include "EntradasControleDeAcesso.h"

int valores[10];  // Array para armazenar os últimos 10 valores de umidade
int indice = 0;   // Índice para gerenciar o buffer circular
float valoresMl[10]; 
int indiceMl = 0;
unsigned long intervalo = 5000;  // Intervalo de 5 segundos entre as leituras
unsigned long ultimoTempo = 0;   // Armazena o tempo da última leitura

void limparValores();
void limparValoresMl();
int media;
float mediaMl;

void iniciaMemoria() {
  limparValores();
  limparValoresMl();
}

void atualizaMemoria() {
  unsigned long tempoAtual = millis();  // Obtém o tempo atual em milissegundos

  // Verifica se já passaram 5 segundos (5000 ms) desde a última leitura
  if (tempoAtual - ultimoTempo >= intervalo) {
    ultimoTempo = tempoAtual;  // Atualiza o tempo da última execução

    // Exemplo de valores simulados (substitua pelos valores reais do sensor)

    // Atualizar o array com o novo valor de umidade
    valores[indice] = porcentagem;
    // Atualizar o array com o novo valor de volume
    valoresMl[indiceMl] = volume_ml;

    // Exibir o valor lido de umidade
    Serial.print("Umidade ");
    Serial.print(indice + 1);
    Serial.print(": ");
    Serial.println(valores[indice]);

    // Exibir o valor lido de volume
    Serial.print("Volume Ml ");
    Serial.print(indiceMl + 1);
    Serial.print(": ");
    Serial.println(valoresMl[indiceMl]);

    // Incrementar os índices
    indice++;
    indiceMl++;

    // Verificar se já lemos 10 valores de umidade
    if (indice >= 10) {
      // Calcular a média dos 10 valores de umidade
      int soma = 0;
      for (int i = 0; i < 10; i++) {
        soma += valores[i];
      }
      media = soma / 10;  // Média dos 10 valores

      // Exibir a média
      Serial.print("Média dos últimos 10 valores de umidade: ");
      Serial.println(media);

      // Após 10 valores, limpar os valores e reiniciar o ciclo
      limparValores();
      indice = 0;  // Reiniciar o índice para começar novamente
    }

    // Verificar se já lemos 10 valores de volume
    if (indiceMl >= 10) {
      // Calcular a média dos 10 valores de volume
      float somaMl = 0;
      for (int i = 0; i < 10; i++) {
        somaMl += valoresMl[i];
      }
      mediaMl = somaMl / 10;  // Média dos 10 valores de volume

      // Exibir a média
      Serial.print("Média dos últimos 10 valores de volume: ");
      Serial.println(mediaMl);

      // Após 10 valores, limpar os valores e reiniciar o ciclo
      limparValoresMl();
      indiceMl = 0;  // Reiniciar o índice para começar novamente
    }
  }
}

// Função para limpar o array de valores de umidade
void limparValores() {
  for (int i = 0; i < 10; i++) {
    valores[i] = 0;  // Limpar o array de valores
  }
  Serial.println("Valores de umidade reiniciados.");
}

// Função para limpar o array de valores de volume
void limparValoresMl() {
  for (int i = 0; i < 10; i++) {
    valoresMl[i] = 0;  // Limpar o array de valores
  }
  Serial.println("Valores de volume reiniciados.");
}
