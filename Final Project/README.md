# Relatório — Atualização OTA via Web Updater em ESP32

## Resumo:

Este relatório descreve um projeto de atualização remota de firmware (OTA) para ESP32 usando um web updater hospedado no próprio dispositivo. Foram empregados conceitos de sistemas operacionais de tempo real (RTOS) para gerenciar tarefas concorrentes. O foco é demonstrar a arquitetura, a implementação e os resultados experimentais, com ênfase na prática de atualização de firmware por navegador — útil em aplicações de IoT e Indústria 4.0.

## Objetivos

- Implementar um servidor web no ESP32 que permita upload de binário para atualização OTA.
- Demonstrar o fluxo de autenticação simples (login/senha) e o processo de gravação do firmware recebido.
- Comparar dois firmwares: antes e depois da atualização (identificação via mensagens no Serial).
- Integrar uma tarefa de blink com FreeRTOS (versão com blink) para separar responsabilidades, demonstrando a aplicação de RTOS.

## Materiais

- Placa de desenvolvimento com ESP32 (ex.: ESP32 Dev Kit).
- Cabo micro-USB para programação e alimentação.
- Rede Wi‑Fi disponível para conectar o ESP32 e a máquina que acessará o web updater.
- IDE Platformio.

## Metodologia

1. Preparar dois firmwares:
   - [Firmware A (antes da atualização)](<No%20Blink/src/main.cpp>): implementa o servidor web para OTA e gerencia as requisições na função `loop` principal.
   - [Firmware B (depois da atualização)](<With%20Blink/src/main.cpp>): conteúdo similar, mas introduz uma tarefa FreeRTOS para fazer o LED onboard piscar e outra para lidar com o servidor web, servindo como comprovação visual e funcional da atualização.
2. Rodar o Firmware A no ESP32; acessar o endereço IP exibido no Serial Monitor.
3. Autenticar-se na página (credenciais padrão: admin / admin).
4. Fazer upload do binário compilado do Firmware B via formulário HTML e aguardar o processo de gravação.
5. Após gravação, o ESP32 reinicia e passa a executar o Firmware B.

## Sumário da Implementação

- Servidor web baseado em WebServer (porta 80), com páginas HTML embutidas para login e upload.
- Endpoint POST `/update` que recebe o arquivo multipart, grava com a API Update e reinicia o dispositivo se bem sucedido.
- Uso de mDNS (host: `esp32`) para facilitar acesso via `http://esp32.local` quando disponível.
- Versão com blink: a tarefa de piscar o LED foi separada em uma tarefa FreeRTOS, enquanto outra tarefa processa o servidor, garantindo responsividade e demonstrando os benefícios do uso de RTOS.

## Localização dos códigos-fonte

- Firmware com blink (FreeRTOS tasks): file:///home/gomes/Documents/embarcados/Embedded-Systems-Projects/Final Project/With Blink/src/main.cpp
- Firmware sem blink: file:///home/gomes/Documents/embarcados/Embedded-Systems-Projects/Final Project/No blink/src/main.cpp

## Instruções de uso resumidas

1. Atualize as credenciais Wi‑Fi (`ssid` e `password`) nos arquivos indicados.
2. Abra o projeto na Arduino IDE e selecione a placa ESP32 correta.
3. Para gerar o binário a ser enviado via web updater:
   - Menu Sketch → Export compiled Binary.
   - O arquivo `.bin` gerado estará na mesma pasta do sketch.
4. No dispositivo rodando o firmware inicial, abra o Serial Monitor (115200 baud) para obter o IP do ESP32.
5. No navegador, acesse `http://<IP_DO_ESP32>` e efetue o login (admin/admin por padrão).
6. Na página de upload, selecione o `.bin` gerado e clique em Update. Aguarde o progresso até 100% e o reinício do ESP32.

## Boas práticas e observações

- Testar localmente em rede confiável antes de implantar em campo.
- Considerar autenticação mais robusta e HTTPS para cenários de produção.
- Verificar limites de tamanho do firmware e memória disponível; usar Update.begin com tamanho apropriado se conhecido.
- Em aplicações críticas, manter fallback (partição dual) ou mecanismo de rollback.

## Resultados Esperados

- Após o upload do Firmware B ser bem-sucedido, o ESP32 reinicia e o LED onboard começará a piscar, confirmando a atualização OTA e a execução do novo firmware com tarefas FreeRTOS.
- O Serial Monitor exibirá mensagens de inicialização das tarefas LedBlinkTask e WebServerTask.

## Conclusão
O web updater é uma solução prática para atualizações OTA em redes locais. A integração de conceitos de RTOS** permitiu a criação de um sistema com bom gerenciamento de tarefas concorrentes - como o web server e o blink do LED. Para produção, recomenda-se fortalecer segurança e prever mecanismos de recuperação. O projeto apresentado é adequado como exercício acadêmico e protótipo funcional para entender o fluxo de OTA em ESP32.

## Referências

- Códigos do projeto: ver caminhos indicados na seção "Localização dos códigos-fonte".
- Documentação ESP32 Arduino: https://docs.espressif.com/projects/arduino-esp32/en/latest/
