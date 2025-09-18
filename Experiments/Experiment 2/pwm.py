# Importa as bibliotecas necessárias para o código funcionar
import RPi.GPIO as GPIO
from time import sleep

#Aqui criamos uma variável para facilmente alterar o pino da GPIO caso fosse necessário.
led_pin = 18

#A função setmode define se vamos usar o padrão de numeração dos pinos da broadcom ou da placa em si. Abaixo selecionamos o padrão da broadcom
GPIO.setmode(GPIO.BCM)

#definimos o pino escolhido como saída.
GPIO.setup(led_pin, GPIO.OUT)

#Na funçaõ PWM abaixo definimos o Pino do LED e a frequência que o PWM irá acionar
pwm_led = GPIO.PWM(led_pin , 100)

#Abaixo ligamos o LED
pwm_led.start(0)

#No loop abaixo iteramos o ciclo de trabalho de forma que gradativamente o LED fique mais aceso, o sleep garante uma transição suave para o processo.
for i in range(100):
	pwm_led.ChangeDutyCycle(i)
	sleep(0.1)
	print("Current duty cycle: {}\r".format(i))
