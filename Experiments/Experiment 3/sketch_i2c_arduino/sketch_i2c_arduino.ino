// código em C a ser compilado na IDE do Arduino e feito upload na placa
// biblioteca Wire para I2C

#include <Wire.h>


// Controlar o LED da própria placa Arduino - LED_BUILTIN é uma constante que se refere ao pino do LED embutido na placa

const int ledPin = LED_BUILTIN;

// evento para quando leitura for requisitada pelo barramento I2C
void requestEvent () {
  Serial.print("Requisitado!!");
  // ler do pino analógico e escrever no serial UART e barramento I2C
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
