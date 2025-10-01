# Para descobrir qual a codificação da Tag com texto gravado anteriormente
from mfrc522 import SimpleMFRC522
from time import sleep
import RPi.GPIO as GPIO
#desabilitar os avisos
GPIO.setwarnings(False)
#cria o objeto "leitor" para a instância "SimpleMFRC522" da biblioteca
leitor = SimpleMFRC522()

#GPIO.setmode(GPIO.BCM)

red_led_pin = 3
green_led_pin = 5

GPIO.setup(red_led_pin, GPIO.OUT)
GPIO.setup(green_led_pin, GPIO.OUT)


print("Aproxime a tag do leitor para leitura.")
while True: #loop
#cria as variáveis "id" e "texto", e as atribui as leituras da id e do texto coletado da tag pelo leitor, respectivamente
	id,texto = leitor.read()
	print("ID: {}\nTexto: {}".format(id, texto)) #exibe as informações coletadas
	# Testar se é o ID do cartão com acesso
	if id == 712936067078:
		#caso seja, ligar o LED verde e desligar o vermelho
		GPIO.output(green_led_pin, GPIO.HIGH)
		GPIO.output(red_led_pin, GPIO.LOW)
	else:
		#caso seja, ligar o LED vermelho e desligar o verde
		GPIO.output(green_led_pin, GPIO.LOW)
		GPIO.output(red_led_pin, GPIO.HIGH)

	sleep(3) #aguarda 3 segundos para nova leitura
