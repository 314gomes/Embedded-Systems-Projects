# Atividade Prática - Sistemas Embarcados: Experimento 4

## Descrição Geral
Esta pasta contém os códigos desenvolvidos para o Experimento 4 da disciplina de Sistemas Embarcados. O objetivo é explorar automação de tarefas com scripts, controle de GPIO via shell e Python, e integração com systemd para automação de serviços embarcados.

## Estrutura dos Arquivos e Descrição

- **blink.sh**: Script Bash para piscar um LED conectado ao GPIO 21.
- **contador_regressivo2.py**: Código Python que implementa um contador regressivo e acende o LED ao final da contagem.
- **turn_off_led.sh**: Script Bash para desligar LEDs conectados aos GPIO 2 e 3.
- **blink_sh.service**: Unidade systemd para automatizar o script blink.sh como serviço.
- **card_reader.service**: Unidade systemd para automatizar leitura de cartão RFID e controle de LEDs.
- **output_systemctl_status_blink_sh.txt**: Saída do comando `systemctl status blink_sh.service` mostrando o status do serviço.
  
## Observações
- Os códigos foram desenvolvidos e testados em ambiente Linux, com hardware Raspberry Pi 3b+.
- Consulte o arquivo PDF "SEL0630_Roteiro_Pratica_4" para mais detalhes do experimento.

---

## Autores

| Nome                        | NUSP     |
|-----------------------------|----------|
| João Pedro Gomes            | 13839069 |
| Giulliano Olivato da Silva  | 9944204  |


