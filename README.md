# Sistema de Automação Residencial controlado via Telegram

<p align="center">
  <img src="http://img.shields.io/static/v1?label=STATUS&message=ANDAMENTO&color=GREEN&style=for-the-badge"/>
</p>

<p align="center">
  <img src="https://github.com/user-attachments/assets/9d1d2e78-9b45-4ee9-9de9-100d98b397a6" width="650" height="500"/>
</p>
<p align="center">Figura 1: Protótipo do Sistema de Automação Residencial</p>



## ℹ Índice 
* [:house: Descrição do Projeto](#house-descrição-do-projeto)
* [:toolbox: Materiais](#toolbox-materiais)
* [:hammer: Funcionalidades do projeto](#hammer-funcionalidades-do-projeto)
* [:computer: Pré-Requisitos](#computer-pré-requisitos)
* [:memo: Licença](#memo-licença)
* [:bookmark_tabs: Referências](#bookmark_tabs-referencia)
<br><br><br><br><br>





## 🏠 Descrição do Projeto
Trata-se do Trabalho de Conclusão de Curso, do Curso de Engenharia Elétrica. Foi desenvolvimento o protótipo de um sistema de automação residencial que pode ser controlado pelo aplicativo de comunicação Telegram.
<br><br><br><br><br>



## 🧰 Materiais
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



## 💻 Pré-requisitos

Para utilizar o projeto, alguns requisitos são necessário:

- Você instalou a versão mais recente de `<linguagem / dependência / requeridos>`
- Você tem uma máquina `<Windows / Linux / Mac>`. Indique qual sistema operacional é compatível / não compatível.
- Você leu `<guia / link / documentação_relacionada_ao_projeto>`.
<br><br><br><br><br>



## 📝 Licença



## 📑 Referências
