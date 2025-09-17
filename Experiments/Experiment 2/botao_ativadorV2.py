# Importa apenas as bibliotecas necessárias para o código funcionar
from RPi.GPIO import input, output, setup, add_event_detect, cleanup
from RPi.GPIO import IN as dentro, OUT as fora, HIGH as alto, LOW as baixo, BOTH as ambos, PUD_UP as pudup
from time import sleep

# Define os números dos pinos GPIO que serão utilizados
LED_PIN = 14
BUTTON_PIN = 23

def GPIO_setup(): #Configura os pinos da GPIO.
    
   
    setup(LED_PIN, fora) #Configura o pino do LED como saída
    
    setup(BUTTON_PIN, dentro, pull_up_down=pudup) #Quando o botão não está pressionado, o pino estará em nível lógico ALTO (HIGH).

    print("Programa iniciado. Pressione o botão para acender o LED.")
    print("Pressione CTRL+C para sair.")

def button_callback(): #Essa função Verifica o estado atual do pino do botão
    if  input(BUTTON_PIN) == baixo: # Se o pino está em LOW, o botão foi pressionado
        output(LED_PIN, alto) # Acende o LED
        print("Botão pressionado -> LED aceso")
    else:
        output(LED_PIN, baixo) # Apaga o LED
        print("Botão solto -> LED apagado")

def main():
    try:
        GPIO_setup()

        """
        # Adiciona a detecção de eventos no pino do botão.
        # GPIO.BOTH detecta tanto a borda de subida (soltar o botão) quanto a de descida (pressionar o botão).
        # O bouncetime evita múltiplas detecções devido a ruídos elétricos do botão.
        """ 
        add_event_detect(BUTTON_PIN, ambos, callback=button_callback, bouncetime=50)
        
        while True:
            sleep(1) # Mantém o script em execução

    except KeyboardInterrupt: # Interrompe o programa quando o usuário pressiona Ctrl + C
        
        print("\nPrograma interrompido pelo usuário.")
    


main()

cleanup() #Limpa o Pino do LED
print("Programa encerrado")