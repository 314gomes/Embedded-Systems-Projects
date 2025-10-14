#importa a função sleep da biblioteca nativa time, é crucial para poder realizar as funções de cronometragem do timer.
from time import sleep 

"""
Importa a função system da biblioteca nativa OS, nesse código é útil por conta da opção de usar o comando clear(windows) ou clear(unix), 
que permite limpar o terminal, assim deixando o timer sempre atualizado e sem suas entradas passadas aparecendo.
"""

from os import system 
 

import RPi.GPIO as GPIO

pino_led = 21

GPIO.setmode(GPIO.BCM)
GPIO.setup(pino_led, GPIO.OUT)

'''
A função anti_troll tem 2 objetivos principais:
[1]: Verificar se o usuário inseriu um n° de segundos e forma de inteiro
[2]: Deixar um pequeno easter-egg (um segredo), caso ele erre propositalmente mais do que 3 vezes seguidas. 

'''
def anti_troll():
    n_mancadas = 0 #inicia o easter-egg
    while True:
            try: #verifica se o usuário vai colocar um inteiro positivo maior do que zero, daqui até a linha 31 está a parte crucial do código para verificar se o número de segundo é mesmo inteiro.
                  max = int(input('Insira o número inteiro positivo maior do que zero de segundos que você deseja para a contagem regressiva:\n> '))
                  if max <= 0:
                    print('O n° de segundos inserido deve ser um número inteiro positivo, talvez você tenha digitado errado?')
                    sleep(1) #pausa o terminal por 1 segundos
                    system('clear') #limpa o terminal
                    print('tente novamente agora')
                    sleep(1)
                    system('clear')
                    continue
                  
                  return max #Se o número é inteiro ele retorna para a função main o valor de segundos
            
            except ValueError: #Caso o usuário erre a entrada ele desencadeia esse loop que só termina quando ele colocar um inteiro
                  if n_mancadas ==  0:
                    print('O n° de segundos inserido deve ser um número inteiro, talvez você tenha digitado errado?')
                    sleep(1) 
                    system('clear') 
                    print('tente novamente agora')
                    sleep(1)
                    system('clear')
                    n_mancadas = n_mancadas + 1
                  elif n_mancadas <= 3:
                        print('oops, parece que você cometeu outro erro, lembre-se, o valor é  inteiro')
                        sleep(1)
                        system('clear')
                        print('tente novamente agora')
                        sleep(1)
                        system('clear')
                        n_mancadas = n_mancadas + 1
                  else: #Aqui é o momento que ativa o easter-egg
                       ban = n_mancadas*10 #tempo que o usuário vai precisar para fazer uma nova tentativa
                       print(f'Você já errou esse input...{n_mancadas} vezes, eu estou cansado chefe...dessa vez, você vai ter que esperar {ban} segundos para tentar de novo, ok?')
                       sleep(3)
                       for i in range(ban):#cria um mini-contador, é o mesmo princípio que vamos usar para fazer a parte central do código
                            print(f'faltam {ban - i} segundos para você tentar de novo') #atualiza o tempo correspondente
                            sleep(1)
                            system('clear')
                            i = i+1
                       n_mancadas = n_mancadas + 1
                       
def tic_tac(x): #A função que determina, a partir do retorno da função antitroll o contador em si.
    for i in range(x): #x = n° de segundos
        a = x-i #impede que o número x seja alterado e se torne um loop infinito
        num_mins = (a//60)#calcula o número de minutos pelo numerador, a cada 60 segundos = 1 minuto 
        num_secs = (a%60) #calcula o n° de segundos que não foram convertido em minutos, o limite é de 59 segundos
        num_hrs = 0 #deixa no display 00 para o n° de horas caso n seja necessário calcular ela
        num_dias = 0 #a mesma coisa da linha de cima só que para dias.

        if num_mins >= 60: #calcula o numero de horas caso ultrapasse 60 segundos
            num_hrs = num_mins//60
            num_mins = num_mins%60 #verifica se sobrou alguns minutos, o limite é de 59 segundos

            if num_hrs >= 24: #a mesma coisa do if de cima, só que para dias, caso o número de horas exceda 24
                 num_dias = num_hrs//24
                 num_hrs = num_hrs%24

        #mostra o número de dias, horas, minutos e segundos com duas casas de unidade (ex: 00, 01, 02,...,09)
        print(f'[Faltam: [{num_dias:02d}] Dias [{num_hrs:02d}] Horas [{num_mins:02d}] Minutos e [{num_secs:02d}] Segundos]')
        sleep(1)
        system('clear')
    print("A contagem acabou, hora de acender o LED!")
    sleep(1)
    system('clear')


def main():
     max = anti_troll()
     tic_tac(max)
     GPIO.output(pino_led,True)
     sleep(10)
     system('clear')
     print('O fim')
     sleep(2)
     GPIO.cleanup()
     system('clear') #limpa o terminal ao encerrar o programa



main() #Executa o código de cima para baixo depois que ele já foi lido uma vez (garante que não haja problemas de funções recursivas implícitas)
