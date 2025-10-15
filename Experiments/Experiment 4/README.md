# Atividade Prática - Sistemas Embarcados: Experimento 4

## Descrição Geral
Esta pasta contém os códigos desenvolvidos para o Experimento 4 da disciplina de Projetos em Sistemas Embarcados. O objetivo deste experimento era explorar a automação de tarefas com scripts, controle de GPIO via shell e Python, além de realizar a integração com systemd para automatizar serviços embarcados.

## Estrutura dos Arquivos e Descrição

- **blink.sh**: Script Bash para piscar um LED conectado ao GPIO 21.

  - Faz um loop infinito de atribuições para a saída do pino GPIO 21 com 0 ou 1 (0 = Apagado, 1 = Aceso) em intervalos de 0.2 segundos.

- **blink_sh.service**: Unidade systemd para automatizar o script blink.sh como serviço.

- **output_systemctl_status_blink_sh.txt**: Saída do comando `systemctl status blink_sh.service` mostrando o status do serviço.

- **spi.py**: Código em Python com funções para uso de um leitor RFID usando protocolo SPI. (mesmo código da prática 3, parte 2).

  - Ao encostar um cartão cadastrado no leitor RFID um LED verde se acende (enquanto o LED vermelho fica apagado).
  - Ao encostar um cartão não cadastrado no leitor RFID um LED vermelho se acende (enquanto o LED verde fica apagado).
  
- **turn_off_led.sh**: Script Bash para desligar LEDs conectados aos GPIO 2 e 3.

- **card_reader.service**: Unidade systemd para automatizar leitura de cartão RFID e controle de LEDs.
    - Para a inicialização de execução (ExecStart) carrega o Python (essencial para rodar qualquer código Python antes da inicialização completa sistema operacional) e o arquivo `spi.py`, que já foi descrito acima. Para o interrompimento de execução (ExecStop) carrega o script Bash `turn_off_led.sh`, que também já foi descrito acima. 



- Seguem abaixo duas fotos do circuito com o programa `spi.py` ativo, sendo executado por meio do card_reader.service:
![Leitor Ativo e com entrada correta](https://github.com/user-attachments/assets/44e8e49f-018d-4b9f-bc8c-512a3d08ea9c)
![Leitor Ativo e com entrada incorreta](https://github.com/user-attachments/assets/7505cf8b-6a0c-4884-a111-439ad5d8ac2e)

- Segue abaixo a foto do circuito, quando o comando: `sudo systemctl stop card_reader` foi executado, assim assinando a condição de parada de execução que ativava o script Bash `turn_off_led.sh`.

![ExecStop do card_reader.service](https://github.com/user-attachments/assets/09953dfe-61a2-4ca1-bccc-a0be183ceb8f)







## Observações
- Os códigos foram desenvolvidos e testados em ambiente Linux, com hardware Raspberry Pi 3b+.
- Consulte o arquivo PDF "SEL0630_Roteiro_Pratica_4" para mais detalhes do experimento.

---

## Autores

| Nome                        | NUSP     |
|-----------------------------|----------|
| João Pedro Gomes            | 13839069 |
| Giulliano Olivato da Silva  | 9944204  |


