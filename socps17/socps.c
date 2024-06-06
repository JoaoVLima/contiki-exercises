#include "contiki.h"
#include <stdio.h>
#include <random.h>

#define MIN_HEART_RATE 20
#define MAX_HEART_RATE 140
#define LOW_HEART_RATE 50
#define HIGH_HEART_RATE 90

#define MIN_OXYGEN_SATURATION 80
#define MAX_OXYGEN_SATURATION 100
#define LOW_OXYGEN_SATURATION 90

#define MIN_TEMPERATURE 34
#define MAX_TEMPERATURE 41
#define LOW_TEMPERATURE 35
#define HIGH_TEMPERATURE 37

PROCESS(heart_rate_process, "Monitoramento de Batimentos Cardíacos");
PROCESS(oxygen_saturation_process, "Monitoramento de Saturação de Oxigênio");
PROCESS(temperature_process, "Monitoramento de Temperatura");
PROCESS(alert_process, "Processo de Alerta");

AUTOSTART_PROCESSES(&heart_rate_process, &oxygen_saturation_process, &temperature_process, &alert_process);

// Variáveis globais para armazenar os dados vitais
static int heart_rate;
static int oxygen_saturation;
static int temperature;

// Processo para monitorar batimentos cardíacos
PROCESS_THREAD(heart_rate_process, ev, data)
{
  static struct etimer timer;
  
  PROCESS_BEGIN();

  while(true) {
    // Define o temporizador para 3 segundos
    etimer_set(&timer, CLOCK_SECOND * 3);

    // Espera até que o temporizador expire
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Gera um valor randômico para os batimentos cardíacos
    heart_rate = (random_rand() % (MAX_HEART_RATE - MIN_HEART_RATE + 1)) + MIN_HEART_RATE;
    printf("Batimentos Cardíacos: %d bpm\n", heart_rate);

    // Verifica se os batimentos estão fora do intervalo normal
    if(heart_rate < LOW_HEART_RATE) {
      printf("Alerta: Batimento Cardíaco Baixo!\n");
      process_post(&alert_process, PROCESS_EVENT_MSG, "Batimento Cardíaco Baixo");
    } else if(heart_rate > HIGH_HEART_RATE) {
      printf("Alerta: Batimento Cardíaco Alto!\n");
      process_post(&alert_process, PROCESS_EVENT_MSG, "Batimento Cardíaco Alto");
    }
  }

  PROCESS_END();
}

// Processo para monitorar saturação de oxigênio
PROCESS_THREAD(oxygen_saturation_process, ev, data)
{
  static struct etimer timer;
  
  PROCESS_BEGIN();

  while(true) {
    // Define o temporizador para 3 segundos
    etimer_set(&timer, CLOCK_SECOND * 3);

    // Espera até que o temporizador expire
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Gera um valor randômico para a saturação de oxigênio
    oxygen_saturation = (random_rand() % (MAX_OXYGEN_SATURATION - MIN_OXYGEN_SATURATION + 1)) + MIN_OXYGEN_SATURATION;
    printf("Saturação de Oxigênio: %d%%\n", oxygen_saturation);

    // Verifica se a saturação está fora do intervalo normal
    if(oxygen_saturation < LOW_OXYGEN_SATURATION) {
      printf("Alerta: Saturação Baixa!\n");
      process_post(&alert_process, PROCESS_EVENT_MSG, "Saturação Baixa");
    }
  }

  PROCESS_END();
}

// Processo para monitorar temperatura
PROCESS_THREAD(temperature_process, ev, data)
{
  static struct etimer timer;
  
  PROCESS_BEGIN();

  while(true) {
    // Define o temporizador para 3 segundos
    etimer_set(&timer, CLOCK_SECOND * 3);

    // Espera até que o temporizador expire
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Gera um valor randômico para a temperatura
    temperature = (random_rand() % (MAX_TEMPERATURE - MIN_TEMPERATURE + 1)) + MIN_TEMPERATURE;
    printf("Temperatura: %d°C\n", temperature);

    // Verifica se a temperatura está fora do intervalo normal
    if(temperature < LOW_TEMPERATURE) {
      printf("Alerta: Hipotermia!\n");
      process_post(&alert_process, PROCESS_EVENT_MSG, "Hipotermia");
    } else if(temperature > HIGH_TEMPERATURE) {
      printf("Alerta: Febre!\n");
      process_post(&alert_process, PROCESS_EVENT_MSG, "Febre");
    }
  }

  PROCESS_END();
}

// Processo para lidar com alertas
PROCESS_THREAD(alert_process, ev, data)
{
  PROCESS_BEGIN();

  while(true) {
    // Espera por um evento de alerta
    PROCESS_WAIT_EVENT();

    if(ev == PROCESS_EVENT_MSG) {
      printf("Alerta Recebido: %s\n", (char *)data);
    }
  }

  PROCESS_END();
}
