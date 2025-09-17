# importa as bibliotecas necessárias para threads, tempo e gpio
import threading
import time

import RPi.GPIO as GPIO

# código de exemplo de multithreading
# uma thread controla um led que pisca
# a outra thread lê de um botão e mostra o resultado no led

GPIO.setmode(GPIO.BCM)

LED1_PIN = 21
LED2_PIN = 20
BUTTON_PIN = 16
# configura os pinos dos leds como saída e do botão como entrada
GPIO.setup(LED1_PIN, GPIO.OUT)
GPIO.setup(LED2_PIN, GPIO.OUT)
GPIO.setup(BUTTON_PIN, GPIO.IN, pull_up_down=GPIO.PUD_UP)

# função da thread 1: pisca o led1 enquanto o evento estiver ativo
def tarefa1(running: threading.Event):
	while running.is_set():
		GPIO.output(LED1_PIN, GPIO.HIGH)
		time.sleep(0.5)
		print("LED1 ON")
		GPIO.output(LED1_PIN, GPIO.LOW)
		time.sleep(0.5)
		print("LED1 OFF")
# função da thread 2: acende o led2 quando o botão é pressionado
def tarefa2(running: threading.Event):
	while running.is_set():
		if GPIO.input(BUTTON_PIN) == GPIO.LOW: # se o pino está em low, o botão foi pressionado
			GPIO.output(LED2_PIN, GPIO.HIGH) # acende o led
			# print("botão pressionado -> led aceso")
		else:
			GPIO.output(LED2_PIN, GPIO.LOW) # apaga o led
			# print("botão solto -> led apagado")

if __name__ == '__main__':
	try:
        # inicia o evento de controle das threads
		running = threading.Event()
		running.set()
		# cria as threads para as duas tarefas
		thread1 = threading.Thread(target=tarefa1, args=(running, ))
		thread2 = threading.Thread(target=tarefa2, args=(running, ))
		# inicia as threads
		thread2.start()
		thread1.start()
		# espera as threads terminarem
		thread1.join()
		thread2.join()

	except KeyboardInterrupt:
		print("programa interrompido pelo usuário.")
		running.clear()
		GPIO.cleanup()
		print("programa encerrado")
		exit(0)