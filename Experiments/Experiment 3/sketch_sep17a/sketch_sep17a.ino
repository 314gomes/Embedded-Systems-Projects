// código em C a ser compilado na IDE do Arduino e feito upload na placa
// biblioteca Wire para I2C

#include <Wire.h>


// Controlar o LED da própria placa Arduino - LED_BUILTIN é uma constante que se refere ao pino do LED embutido na placa

const int ledPin = LED_BUILTIN;

void requestEvent () {
  Serial.print("Requisitado!!");
  int n = analogRead(A0);
  Wire.write(highByte(n));
  Wire.write(lowByte(n));
  Serial.print(n);
  
}

void setup() 
{// adicionando endereço no barramento I2C com dispositivo controlado e inicia a comunicação
  Wire.begin(0x8);
  Serial.begin(9600);
  Wire.onRequest(requestEvent);
  //Reportar “receiveEvent” quando receber dados
  Wire.onReceive(receiveEvent);
  // Define o pino do LED como saída e o deixa desligado inicialmente
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}



// Definição da Função receiveEvent executada sempre que dados são recebidos do controlador (Raspberry Pi)
void receiveEvent(int howMany) 
{
  while (Wire.available()) 
  { // loop enquanto houver dados no barramento
    char c = Wire.read(); // recebe o byte como char
    digitalWrite(ledPin, c); // define o estado do LED com base no valor de “c”
  }
}
void loop() 
{ // aguarda 100 ms antes de continuar a execução em loop
delay(100);
}

// void setup () {
// o parâmetro <Serial.begin(9600)> define a taxa de comunicação serial - 9600 bits por segundo, no caso
//a função < Wire.onRequest(requestEvents)> é chamada quando o Arduino receber solicitações da Rasp
// }

// void requestEvent () {
// a função: <analogRead(analog_pin)> realiza leitura do pino analogico que se deseja receber dados
// <Wire.write(highByte(n)); > e < Wire.write(lowByte(n));> são funções usadas na chamada da função de envio de bytes pelo I2C por highByte e lowByte
// }
// void receiveEvent(int number) {
// sem alterações
//}
// Lembrar sempre de, ao longo dos blocos, imprimir na tela os valores com < Serial.println>