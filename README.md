# Sistema de Automação Residencial controlado via Telegram

<p align="center">
  <img src="http://img.shields.io/static/v1?label=STATUS&message=ANDAMENTO&color=GREEN&style=for-the-badge"/>
</p>

<p align="center">
  <img src="https://github.com/user-attachments/assets/9d1d2e78-9b45-4ee9-9de9-100d98b397a6" width="650" height="500"/>
</p>
<p align="center">Figura 1: Protótipo do Sistema de Automação Residencial</p>
<br><br><br><br><br>


## :round_pushpin: Índice 
* [:house: Descrição do Projeto](#house-descrição-do-projeto)
* [:hammer: Funcionalidades do projeto](#hammer-funcionalidades-do-projeto)
* [:computer: Pré-Requisitos](#computer-pré-requisitos)
* [:toolbox: Materiais & Custos](#toolbox-materiais--custos)
* [:memo: Licença](#memo-licença)
* [:bookmark_tabs: Referências](#bookmark_tabs-referências)
<br><br><br><br><br>





## :house: Descrição do Projeto
Trata-se do Trabalho de Conclusão de Curso, do Curso de Engenharia Elétrica. Foi desenvolvimento o protótipo de um sistema de automação residencial que pode ser controlado pelo aplicativo de comunicação Telegram.
- *arquivo "TCCArduinoESP32.ino" é o código usado no Arduino*
- *arquivo "TCCESP32Arduino.ino" é o código usado no ESP32*
<br><br><br><br><br>



## :hammer: Funcionalidades do projeto

- `Sistema de Iluminação Interna`: responsável pela iluminação interna da residência, localizado nos cômodos: sala, cozinha, banheiro, quarto, área de serviço.
- `Sistema de Iluminação Externa`: responsável pela iluminação externa da residência, localizado no quintal. A iluminação é ligada quando é detectado movimento no quintal e quando o ambiente está escuro, fora isso, a iluminação permanece desligada. O Telegram é responsável por ligar ou desligar esse sistema.
- `Sistema de Alarme de Movimento`: responsável por detectar qualquer movimento no quintal. Caso seja detectado, um alerta é enviado para o Telegram.
- `Sistema de Alarme de Incêndio`: responsável por detectar chama na cozinha. Caso seja detectado, é enviado uma mensagem de alerta para o Telegram.
- `Sistema de Alarme de Vazamento de Gás`: responsável por detectar vazamento de gás na cozinha. Caso seja detectado algum vazamento, uma ventilação instalada na cozinha é acionada para dissipar o gás, além de ser enviado um alerta para o Telegram. 
- `Sistema de Ventilação da Sala`: responsável por monitorar a temperatura e a umidade da sala, quando a temperatura superar um determinado valor e a umidade cai abaixo de um valor especifico, é enviado um alerta para o Telegram e a ventilação na sala é ativada para refrescar o ambiente.
- `Sistema de Irrigação do Jardim`: responsável por monitorar a umidade do solo (jardim). Caso seja detectado que o solo está seco (baixa umidade), o irrigador é acionado.
- `Sistema Integrado`: é o sistema final, envolvendo a união de todos os sistemas acima. Além disso, todos os sistemas podem ser desligado ou ligados pelo Telegram (bloqueando o funcionando deles).
<br><br><br><br><br>



## :computer: Pré-requisitos

Para utilizar o projeto, alguns requisitos são necessário:

- Você instalou a versão mais recente de `<linguagem / dependência / requeridos>`
- Você tem uma máquina `<Windows / Linux / Mac>`. Indique qual sistema operacional é compatível / não compatível.
- Você leu `<guia / link / documentação_relacionada_ao_projeto>`.
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
| Transistores NPN BC547 | 0,35 | 4 | 1,40 |
| Resistores de 10kΩ | 0,15 | 6 | 0,90 |
| Resistores de 2kΩ | 0,29 | 1 | 0,29 |
| Protoboard 830 pinos | 12,90 | 3 | 38,70 |
| Protoboard 170 pinos | 3,90 | 2 | 7,80 |
| Fonte Bivolt 9V/1A | 11,52 | 2 | 23,04 |
| Fonte Ajustável para Protoboard | 8,12 | 2 | 16,34 |
| **TOTAL**              ||| **R$ 326,11** |
<br>

:exclamation: **OBS:** *os valores podem ser diferentes a depender de diversos fatores (como as lojas, época, inflação, etc.).*
<br><br><br><br><br>



## :memo: Licença
<br><br><br><br><br>



## :bookmark_tabs: Referências
