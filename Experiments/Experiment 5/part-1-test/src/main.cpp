#include <Arduino.h>
int capacitiveValue;
int threshold = 20; //limiar para ajustar
void setup()
{
  Serial.begin(115200); // taxa de transferência serial (baud rate) = 115200 bits por segundo (bps)
  pinMode(2, OUTPUT); // pino correspondente ao LED onboard como saída
  digitalWrite(2, LOW); // inicialmente desligado (LED apagado)
  Serial.println("ESP32 Touch Sensor Test"); // mensagem inicial impressa no monitor serial
  // A função Serial.println imprime uma mensagem ou valor no monitor serial e após isso ocorre quebra de linha
  // na função Serial.print não ocorre quebra de linha
} //end setup
// --- Loop Infinito ---
void loop()
{
  capacitiveValue = touchRead(13); // armazena o valor lido do touch do pino13
  Serial.print("key:"); // imprimir a msg "key"...
  Serial.println(capacitiveValue); // e o valor lido no monitor serial
  delay(500); // delay de 500 ms para atualização
  if (touchRead(13) < threshold)
    { // lógica para acender o LED onboard quando o pino13 for tocado
    digitalWrite(2, HIGH); // se sim, acende o LED (pino 2)...
    Serial.println("touched"); // msg p/ sinalizar quando houve toque
    }
  else 
  {
    digitalWrite(2, LOW); // caso contrário, o LED permanece apagado
  }
}  //end loop