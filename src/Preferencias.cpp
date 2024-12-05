#include <Preferences.h>

Preferences preferences;

// Função para salvar o estado atual do telhado e o valor do sensor de chuva
void salvarEstadoTelhado(bool estadoTelhado, int valorSensorChuva)
{
    // preferences.begin("telhado", false);  // Namespace 'telhado'
    // preferences.putBool("estadoTelhado", estadoTelhado);
    // preferences.putInt("sensorChuva", valorSensorChuva);  // Nome mais curto
    // preferences.end();
}

// Função para carregar o estado salvo do telhado e o valor do sensor de chuva
void carregarEstadoTelhado(bool &estadoTelhado, int &valorSensorChuva)
{
    // preferences.begin("telhado", true);  // Abrir no modo somente leitura
    // estadoTelhado = preferences.getBool("estadoTelhado", false);  // Valor padrão: falso
    // valorSensorChuva = preferences.getInt("sensorChuva", 0);  // Valor padrão: 0
    // preferences.end();
}

