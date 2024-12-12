# Sistema de Automação Residencial Controlado e Monitorado via Telegram

<p align="center">
  <img src="http://img.shields.io/static/v1?label=STATUS&message=CONCLUIDO&color=GREEN&style=for-the-badge"/>
</p>

<p align="center">
  <img src="https://github.com/user-attachments/assets/2aef1242-0a4b-43fe-b11e-9ba5767dd0ac" width="650" height="500"/>
</p>
<p align="center">Figura 1: Protótipo do Sistema de Automação Residencial</p>
<br><br>


## :round_pushpin: Índice 
* [:house: Descrição do Projeto](#house-descrição-do-projeto)
* [:hammer: Funcionalidades do projeto](#hammer-funcionalidades-do-projeto)
* [:computer: Pré-Requisitos](#computer-pré-requisitos)
* [:toolbox: Materiais & Custos](#toolbox-materiais--custos)
* [:memo: Licença](#memo-licença)
* [:bookmark_tabs: Referências](#bookmark_tabs-referências)
<br><br><br><br><br>





## :house: Descrição do Projeto
Trata-se do Trabalho de Conclusão de Curso, do Curso de Engenharia Elétrica. Foi desenvolvimento um protótipo de um sistema de automação residencial controlado e monitorado pelo aplicativo de comunicação Telegram.
- *arquivo "TCCArduinoESP32.ino" é o código usado no Arduino*
- *arquivo "TCCESP32Arduino.ino" é o código usado no ESP32*
<br><br><br><br><br>



## :hammer: Funcionalidades do projeto

- `Sistema de Iluminação Interna`: responsável pela iluminação interna da residência, localizado nos cômodos: sala, cozinha, banheiro, quarto, área de serviço. A iluminação de cada ambiente é ligada e desligada, apenas, pelo Telegram.
- `Sistema de Iluminação Externa`: responsável pela iluminação externa da residência, localizado apenas no quintal. A iluminação é ligada apenas quando três condições são cumpridas: o sistema está ativado, o ambiente está escuro e é detectado movimento no quintal, fora isso, a iluminação permanece desligada. Este sistema é ligado e desligado pelo Telegram apenas.
- `Sistema de Alarme de Movimento`: responsável por detectar qualquer movimento no quintal, informando o usuário pelo Telegram sobre uma posição invasão por exemplo. Este sistema é ligado e desligado  pelo Telegram apenas.
- `Sistema de Alarme de Incêndio`: responsável por detectar chama na cozinha, informando o usuário pelo Telegram sobre a situação (se há algum incêndio ou não). Este sistema é ligado e desligado  pelo Telegram apenas.
- `Sistema de Alarme de Vazamento de Gás`: responsável por detectar vazamento de gás na cozinha, informando o usuário pelo Telegram sobre a situação (há algum vazamento de gás ou não). Caso seja detectado, uma ventoinha instalada na cozinha é acionada para ajudar a dissipar o gás. Este sistema é ligado e desligado  pelo Telegram apenas.
- `Sistema de Ventilação da Sala`: responsável por monitorar a temperatura e a umidade da sala, quando a temperatura ultrapassa um determinado valor e a umidade está abaixo de um valor especifico, é enviado uma mensagem para o Telegram informando a situação e a ventoinha, instalada na sala, é ativada para arejar o cômodo. Este sistema é ligado e desligado  pelo Telegram apenas, além disso, é possível solicitar (através do comando ``/temp_umid``) as informações de temperatura e umidade da sala através do app.
- `Sistema de Irrigação do Jardim`: responsável por monitorar a umidade do solo (jardim ou vaso de planta). Caso seja detectado que o solo está seco (baixa umidade), o irrigador é acionado, e é desligado apenas quando for detectado que o solo está úmido (alta umidade). Este sistema é ligado e desligado  pelo Telegram apenas.
- `Sistema Integrado`: é a junção de todos os sistemas (visto na Figura 1). É possível monitorar a situação de todas os sistemas, sensores e tarefas (através do comando ``/status``) através do Telegram, além de ser possível controlar (ligar/desligar) cada sistema individualmente.
<br><br><br><br><br>



## :computer: Pré-requisitos

Para utilizar o projeto, alguns requisitos são necessário:

- Ter um computador para realizar qualquer alteração necessária no código-fonte, e para transferir os códigos para os microcontroladores correspondentes.
- Ter uma conta no Telegram, e o aplicativo instalado.
- Criar um Bot no Telegram e pegar a chave Token do Bot criado (use o [BotFather](https://t.me/BotFather) para realizar este processo).
- Identificar o seu ID de usuário no Telegram (use o [IDBot](https://t.me/myidbot) para realizar essa etapa).

> [!NOTE]
> *Caso não saiba criar o Bot no Telegram e identificar o ID do Usuário. Siga o passo a passo em [Random Nerd Tutorials](https://randomnerdtutorials.com/telegram-control-esp32-esp8266-nodemcu-outputs/).*

<br><br><br><br><br>



## :toolbox: Materiais & Custos
| Componentes | Preço Unitário (R$) | Quantidade | Preço (R$) |
|----------|:----------:|:----------:|:----------:|
| ESP32 Doit DevKitC V4 WROOM-32D  | 62,04 | 1 | 62,04 |
| Arduino Uno R3  | 46,90 | 1 | 46,90 |
| Sensor de Movimento Infravermelho  (PIR)  | 7,90 | 1 | 7,90 |
| Sensor de Chama  | 4,90 | 1 | 4,90 |
| Sensor de Gás de Cozinha e Gás Natural (MQ-6)  | 13,90 | 1 | 13,90 |
| Sensor de Umidade e de Temperatura (DTH11)  | 8,90 | 1 | 8,90 |
| Módulo Sensor de Umidade do Solo  | 10,90 | 1 | 10,90 |
| Módulo LDR | 6,72 | 1 | 6,72 |
| Display LCD 16x2 | 15,00 | 1 | 15,00 |
| Módulo Serial I2C | 6,90 | 1 | 6,90 |
| Mini Cooler 5V 25x25mm  | 17,61 | 2 | 35,22 |
| Mini Bomba de Água Submersa 3V-5V | 14,10 | 1 | 14,10 |
| Buzzer ativo | 2,51 | 1 | 2,51 |
| LED | 0,25 | 7 | 1,75 |
| Relé 4 Canais | 20,65 | 1 | 20,65 |
| Resistores de 10kΩ | 0,15 | 6 | 0,90 |
| Resistores de 2kΩ | 0,29 | 1 | 0,29 |
| Protoboard 830 pinos | 12,90 | 3 | 38,70 |
| Protoboard 170 pinos | 3,90 | 2 | 7,80 |
| Fonte Bivolt 9V/1A | 11,52 | 2 | 23,04 |
| Fonte Ajustável para Protoboard | 8,12 | 2 | 16,34 |
| **TOTAL**              ||| **R$ 340,02** |
<br>

> [!NOTE]
> *Os valores estão sujeitos à alterações, a depender de diversos fatores.*

> [!NOTE]
> *Não foi considerado o gasto com a fiação do projeto.*

<br><br><br><br><br>



## :memo: Licença
Este projeto está licenciado nos termos da [MIT License](https://github.com/JoseRenatoS/AutomacaoResidencialComTelegram/blob/main/LICENSE).
<br><br><br><br><br>



## :bookmark_tabs: Referências

* Repositório da biblioteca [Universal Telegram Bot Library](https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot?tab=readme-ov-file) utilizada para fazer a conexão entre o ESP32 e o API do Telegram Bot.
* A ideia de utilizar controle do sistema via Telegram, veio da inspiração do projeto [Telegram: Control ESP32/ESP8266 Outputs (Arduino IDE)](https://randomnerdtutorials.com/telegram-control-esp32-esp8266-nodemcu-outputs/) da Random Nerd Tutorials

