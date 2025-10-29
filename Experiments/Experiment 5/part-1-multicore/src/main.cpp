/***************************************************************/
/* Biblioteca do Arduino */
#include <Arduino.h> // Inclui a biblioteca padrão do Arduino para utilizar este framework
/* Bibliotecas FreeRTOS */
#include "freertos/FreeRTOS.h" // Inclui a biblioteca do FreeRTOS para a criação e manipulação de tasks
#include "freertos/task.h" // Inclui funções específicas para manipulação de tasks no FreeRTOS
/* Mapeamento de pinos */
#define LED 2 // Define o pino 2 como o pino ao qual o LED está conectado
/* Variáveis para armazenamento do handle das tasks */
TaskHandle_t taks1Handle = NULL; // Handle para a task 1, usado para controle da task (interromper, reiniciar)
TaskHandle_t taks2Handle = NULL; // Handle para a task 2, usado para controle da task
/* Protótipos das Tasks */
void vTask1(void *pvParameters); // Protótipo da função da task 1
void vTask2(void *pvParameters); // Protótipo da função da task 2
/* Função setup */
void setup() {
  Serial.begin(9600); // Configura a comunicação serial com uma taxa de 9600 bits por segundo
  /* Criação das tasks */
  xTaskCreatePinnedToCore(
    vTask1, // Ponteiro para a função que define o comportamento da task 1
    "TASK1", // Dar um nome da task (apenas para referência)
    configMINIMAL_STACK_SIZE, // Função que define tamanho mínimo de pilha necessário para a task
    NULL, // Parâmetro passado para a task (neste caso, nenhum)
    1, // Prioridade da task 1 (1 é uma prioridade baixa)
    &taks1Handle, // Ponteiro para armazenar o handle da task 1
    0
  );
  xTaskCreatePinnedToCore(
    vTask2, // Ponteiro para a função que define o comportamento da task 2
    "TASK2", // Nome da task 2
    configMINIMAL_STACK_SIZE + 1024, // Tamanho da pilha aumentado para a task 2, pois ela envolve a comunicação serial
    NULL, // Parâmetro passado para a task (nenhum neste exemplo)
    2, // Prioridade da task 2 (prioridade maior que a task 1)
    &taks2Handle, // Ponteiro para armazenar o handle da task 2
    1
  );
}
/* Função loop */
void loop() {
  vTaskDelay(3000); // Libera a CPU por 3 segundos; esta função apenas gera atraso enquanto as tasks executam
}
/*
 vTask1
 Função que inverte o estado do LED em intervalos de 200 ms
*/
void vTask1(void *pvParameters) {
  pinMode(LED, OUTPUT); // Configura o pino LED como saída
  while (1) { // Loop infinito para que a task continue executando
    digitalWrite(LED, !digitalRead(LED)); // Inverte o estado atual do LED
    vTaskDelay(pdMS_TO_TICKS(200)); // Aguarda 200 ms antes de inverter novamente (usando o FreeRTOS para atraso)
  }
}
/*
 vTask2
 Função que imprime o valor de uma contagem a cada 1 segundo
*/
void vTask2(void *pvParameters) {
  int cont = 0; // Inicializa uma variável para contagem
  while (1) { // Loop infinito para a task continuar executando
    Serial.println("Task 2: " + String(cont++)); // Imprime o valor atual da contagem e incrementa em seguida
    vTaskDelay(pdMS_TO_TICKS(1000)); // Aguarda 1 segundo antes de imprimir novamente
  }
}