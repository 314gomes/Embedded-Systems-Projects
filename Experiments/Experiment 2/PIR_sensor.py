# Importa apenas as bibliotecas necessárias para o código funcionar
from gpiozero import MotionSensor,LED
from signal import pause

#definimos variáveis para alterar os pinos do LED e do PIR, caso fosse necessário.
pir = MotionSensor(18)
led = LED(15)

#se detectar movimento: Acende o LED
def motion_function():
    print("Motion Detected")
    led.on()

#se não detectar movimento: apaga o LED, ou, se for a primeira vez, não acende o LED.
def no_motion_function():
    print("Motion stopped")
    led.off()

#Usa a função when_motion/when_no_motion para definir o que o código deve executar caso haja movimento ou caso não haja movimento.
pir.when_motion = motion_function
pir.when_no_motion = no_motion_function

#permite que o código não seja encerrado depois da primeira vez iniciado, isto é: ele sempre aguarda que um dos dois estados aconteça para poder começar a funcionar.
pause()
