# Tutorial: Raspberry Pi controlando o Arduino
# sudo raspi-config - interface options - habilitar I2C
# sudo i2cdetect -y 1 - verifica o barramento I2C - ao realizar a conexão I2C física com
#Arduino o endereço ocupado deve aparecer
from smbus import SMBus # importa a classe SMBus
addr = 0x8 # bus address - define o endereço do dispositivo I2C como 0x8 (hexa)
bus = SMBus(1) # /dev/ic2-1 - inicializa o objeto, criando uma instância para
#se comunicar com barramento I2C n° 1
flag = True # variv. usada p/ controlar o loop
print ("Digite 1 para ON ou 0 para OFF. Digite r para ler 2 bytes")
while flag:
	leitura = input(">>>> ") # solicita entrada no terminal
	if leitura == "1":
		bus.write_byte(addr, 0x1) # se for 1, escreve 0x1 no endereço
	elif leitura == "0":
		bus.write_byte(addr, 0x0) # ou escreve 0x0, caso 0
	elif leitura == "r":
		data = bus.read_i2c_block_data(0x8, 0, 2)
		print("lido do arduino: {}".format(data))
		valor_lido = (data[0] << 8) + data[1]
		print("ou seja: valor = {}".format(valor_lido))
		print("tensao aproximada = {}".format(valor_lido * 5.0 / 1023))
	else:
		flag = False # encerra o loop caso digite algo diferente de 0 ou 1
	# Os valores de entrada acima, enviam 1 ou 0 para o barramento I2C - ou 0 em caso de
	# valor de entrada diferente de 0 ou 1.
	# Até aqui, o código acima, se testado, deve ser capaz de controlar o LED do Arduino,
	#apagando (quando enviado 0) ou ligando (quando enviado 1). Para que o Arduino recebe o
	#controle, deve ser feito o upload de um programa em C, via IDE. O tutorial encontra-se
	#abaixo.
	# A partir daqui, conforme solicitado no roteiro da prática, a linha abaixo deve ser
	#configurada para a leitura de dois bytes - referente aos valores analógicos que serão
#lidos do potenciômetro conectado ao Arduino - na resolução de 10 bits
# <Bus.read_i2c_block_data(endereço do arduino, registrador, conjunto debytes)>
# exemplo: data = bus.read_i2c_block_data(0x8, 0, 2)
# por fim, realizar a conversão da saída acima, de 0 e 1023 (10 bits) recebidos do Arduino, para imprimir no terminal da Rasp o valor entre 0 e 255 (8 bits)
#exemplo: value = data[0]*256+data[1]
