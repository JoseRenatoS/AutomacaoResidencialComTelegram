#include <SoftwareSerial.h>             // usada para a comunicação serial em pinos específicos
#include <Wire.h>                       // usada para a comunicação I2C, em uma interface de dois fios (SDA e SCL)
#include <LiquidCrystal_I2C.h>          // usada para facilitar o controle de displays LCD com controlador I2C,
#include "DHT.h"                        // usada para o controle de sensores (DHT11/DHT22) de temperatura e umidade


#define sala_Ventilacao 6               // pino 6 - aciona ventilação da sala
#define cozinha_Ventilacao 5            // pino 5 - aciona ventilação da cozinha
#define irrigador 4                     // pino 4 - aciona o irrigador do solo (bomba d'água)
#define LedRed 3                        // pino 3 - aciona LED de Emergência Vermelho ao lado do display LCD
#define buzzer 2                        // pino 2 - aciona o buzzer
#define DHTPIN A1                       // pino A1 - receber dados do Sensor de Umidade-Temperatura
#define DHTTYPE DHT11                   // Define o tipo de sensor utilizado (DHT11)
DHT dht(DHTPIN, DHTTYPE);               // Cria uma instância do sensor DHT no pino A1 e especifica o tipo (DHT11)


SoftwareSerial mySerial(A3, A2);        // Inicializa uma comunicação serial em software com os pinos A3 como RX (recebe) e A2 como TX (transmissão)
LiquidCrystal_I2C lcd(0x27, 16, 2);     // Inicializa o display no endereco 0x27, e usa os pinos A5 como SCL e A4 como SDA


String mensagem = "";                   // Variável usada armazenar a mensagem recebida pela mySerial (ESP32)
bool stateVentilaCozinha = false;       // Variável usada armazenar o estado (ligado/desligado) do ventilador da cozinha
bool stateVentilaSala = false;          // Variável usada armazenar o estado (ligado/desligado) do ventilador da sala
bool stateIrrigador = false;            // Variável usada armazenar o estado (ligado/desligado) do irrigador
bool stateLED = false;                  // Variável usada armazenar o estado (ligado/desligado) do led
int i;                                  // contador usado na função que aciona o buzzer
String mensagemTempUmid = "";           // Variável usada armazenar uma mensagem e os valores de temperatura e umidade, em formato de texto
String valorTemp = "";                  // Variável usada armazenar o valor da Temperatura, em formato de texto (após conversão de float para texto)
String valorUmid = "";                  // Variável usada armazenar o valor da Umidade, em formato de texto (após conversão de float para texto)
String mensagemStatus = "";             // Variável usada armazenar uma mensagem que informa a situação de todas as tarefas (se estão ativadas ou desativadas)



// FUNÇÃO USADA PARA REALIZAR A LEITURA DO SENSOR DHT11 E LIGAR/DESLIGAR A VENTILAÇÃO DA SALA SE CUMPRIR OS REQUISITOS
void TemperaturaUmidade() {
  
  float h = dht.le_umidade();                                // Lê o valor de umidade do sensor DHT e armazena na variável "h"
  float t = dht.ler_Temperatura();                           // Lê o valor de temperatura do sensor DHT e armazena na variável "t"

  valorUmid = String(h);                                     // Converte o valor flutuante da variável "h" em string, e armazena na variavel "valorUmid"
  valorTemp = String(t);                                     // Converte o valor flutuante da variável "t" em string, e armazena na variavel "valorTemp"


  if (isnan(t) || isnan(h)) {                                // Verifica se os valores lidos são 'NaN' (não numéricos), indicando falha de leitura do sensor
    Serial.println("Falha ao ler o sensor DHT");             // Caso falhe, é exibe a mensagem de erro na serial
  }

  lcd.clear();                                               // limpa o display
  lcd.setCursor(0, 0);                                       // posiciona o cursor na coluna 0, linha 0
  lcd.print("Temp.:");                                       // exibe o texto no display, iniciando na posição estabelecida acima
  lcd.setCursor(7, 0);                                       // posiciona o cursor na coluna 7, linha 0
  lcd.print(t);                                              // exibe o texto no display, iniciando na posição estabelecida acima
  lcd.setCursor(13, 0);                                      // posiciona o cursor na coluna 13, linha 0
  lcd.write(223);                                            // exibe o texto no display, iniciando na posição estabelecida acima
  lcd.setCursor(14, 0);                                      // posiciona o cursor na coluna 13, linha 0
  lcd.print("C");                                            // exibe o texto no display, iniciando na posição estabelecida acima
  lcd.setCursor(0, 1);                                       // posiciona o cursor na coluna 0, linha 1
  lcd.print("Umid.:");                                       // exibe o texto no display, iniciando na posição estabelecida acima
  lcd.setCursor(7, 1);                                       // posiciona o cursor na coluna 7, linha 1
  lcd.print(h);                                              // exibe o texto no display, iniciando na posição estabelecida acima
  lcd.setCursor(13, 1);                                      // posiciona o cursor na coluna 13, linha 1
  lcd.print("%");                                            // exibe o texto no display, iniciando na posição estabelecida acima


  mensagemTempUmid = "Temperatura: " + valorTemp + "ºC\nUmidade: " + valorUmid + "%\t";                    // armazena os valores na variável "mensagemTempUmid" com uma mensagem
   

  // se a TEMPERATURA estiver acima de 25ºC, a UMIDADE abaixo de 80% (e o estado da ventilação desligado), a ventilação será acionada
  // se a TEMPERATURA estiver abaixo ou igual a 25ºC, a UMIDADE acima de 80% (e o estado da ventilação ligado), a ventilação será desligada
  // qualquer outra condição, deixará o sistema do jeito que se encontra



  if(h < 80.00 && t >= 25.00 && stateVentilaSala == false) {
    digitalWrite(sala_Ventilacao, HIGH);                                                                     // ativa a ventilação da sala
    Serial.println("ventilação sala ativada");                                                               // envia a mensagem para o monitor serial
    mySerial.println(mensagemTempUmid);                                                                      // envia uma mensagem da variável para a serial secundária: mySerial (ESP32)
    delay(200);                                                                                              // atraso de 200 milissegundos
    mySerial.println("Ventilação da sala ativada. \nUmidade baixa e temperatura alta. Se hidrate!!!\t");     // envia uma mensagem para a serial secundária: mySerial (ESP32)
    delay(200);                                                                                              // atraso de 200 milissegundos
    stateVentilaSala = true;                                                                                 // Altera o estado da ventilação da sala para indicar que está ligada
  } else if(h >= 80.00 && t <= 25.00 && stateVentilaSala == true) {
    digitalWrite(sala_Ventilacao, LOW);                                                                      // desliga a ventilação da sala
    Serial.println("ventilação sala desativada");                                                            // envia a mensagem para o monitor serial
    mySerial.println(mensagemTempUmid);                                                                      // envia uma mensagem da variável para a serial secundária: mySerial (ESP32)
    delay(200);                                                                                              // atraso de 200 milissegundos
    mySerial.println("A ventilação na sala foi desligada.");                                                 // envia uma mensagem para a serial secundária: mySerial (ESP32)
    delay(200);                                                                                              // atraso de 200 milissegundos
    stateVentilaSala = false;                                                                                // Altera o estado da ventilação da sala para indicar que está desligada
  }
  delay(300);                                                                                                // atraso de 300 milissegundos
}



// FUNÇÃO USADA PARA ACIONAR O BUZZER
void acionarBuzzer() {
  for (i = 0; i < 3; i++) {               // Repete 3 vezes
    digitalWrite(buzzer, HIGH);           // Liga o buzzer
    delay(500);                           // Mantém o som por 500 milissegundos (0,5 segundos)
    digitalWrite(buzzer, LOW);            // Desliga o buzzer
    delay(500);                           // Pausa de 500 milissegundos entre os bipes
  }
}



// FUNÇÃO USADA PARA ARMZENAR NA VARIÁVEL "mensagemStatus", A SITUAÇÃO (LIGADOS/DESLIGADOS) DOS DISPOSITIVOS CONECTADOS NO ARDUINO
void situacao() {
  if (stateVentilaCozinha == 1) {                                                          // verifica o estado da variável "stateVentilaSala" e exibe uma mensagem de acordo com o níve lógico dela
    mensagemStatus = "Ventilação da Cozinha ligada.\n";                                    // mensagem armazenada caso o nível lógico seja ALTO (ligado)
  } else {
    mensagemStatus = "Ventilação da Cozinha desligada.\n";                                 // mensagem armazenada caso o nível lógico seja BAIXO (desligado)
  }
  
  if (stateVentilaSala == 1) {                                                             // verifica o estado da variável "stateVentilaSala" e exibe uma mensagem de acordo com o níve lógico dela
    mensagemStatus += "Ventilação da Sala ligada.\n";                                      // mensagem armazenada caso o nível lógico seja ALTO (ligado)                                
  } else {
    mensagemStatus += "Ventilação da Sala desligada.\n";                                   // mensagem armazenada caso o nível lógico seja BAIXO (desligado)                           
  }
  
  if (stateIrrigador == 1) {                                                               // verifica o estado da variável "stateIrrigador" e exibe uma mensagem de acordo com o níve lógico dela
    mensagemStatus += "Irrigador do Jardim ligado.\n";                                     // mensagem armazenada caso o nível lógico seja ALTO (ligado)                     
  } else {
    mensagemStatus += "Irrigador do Jardim desligado.\n";                                  // mensagem armazenada caso o nível lógico seja BAIXO (desligado)                 
  
  } if (stateLED == 1) {                                                                   // verifica o estado da variável "stateIrrigador" e exibe uma mensagem de acordo com o níve lógico dela
    mensagemStatus += "LED de Emergência aceso.\n\n";                                      // mensagem armazenada caso o nível lógico seja ALTO (ligado)                 
  } else {
    mensagemStatus += "LED de Emergência apagado.\n\n";                                    // mensagem armazenada caso o nível lógico seja BAIXO (desligado)               
  }
  mensagemStatus += "Temperatura: " + valorTemp + "ºC\nUmidade: " + valorUmid + "%\t";     // Armazena os valores de temperatura e umidade na variável "mensagemStatus"
  return mensagemStatus;
}



void setup() {
  Serial.begin(9600);                       // Inicializa a comunicação serial com o computador a uma taxa de transmissão de 9600 bps
  mySerial.begin(9600);                     // Inicializa uma segunda comunicação serial (SoftwareSerial) com a mesma taxa de 9600

  lcd.init();                               // Inicializa o display LCD
  lcd.setBacklight(HIGH);                   // Liga a luz de fundo do LCD
  lcd.clear();                              // Limpa qualquer informação anterior exibida no LCD

  dht.comeco();                             // inicializa o Sensor de Umidade e de Temperatura (DHT11)

  pinMode(sala_Ventilacao, OUTPUT);         // pino 6 - saída
  pinMode(cozinha_Ventilacao, OUTPUT);      // pino 5 - saída
  pinMode(irrigador, OUTPUT);               // pino 4 - saída
  pinMode(LedRed, OUTPUT);                  // pino 3 - saída
  pinMode(buzzer, OUTPUT);                  // pino 2 - saída
  digitalWrite(sala_Ventilacao, LOW);       // pino 6 - baixo
  digitalWrite(cozinha_Ventilacao, LOW);    // pino 5 - baixo
  digitalWrite(irrigador, LOW);             // pino 4 - baixo
  digitalWrite(LedRed, LOW);                // pino 3 - baixo
  digitalWrite(buzzer, LOW);                // pino 2 - baixo
  delay(3000);                              // Atraso de 3 segundos
}



void loop() {
  TemperaturaUmidade();                                                  // chama a função responsável pela leitura da Temperatura e da Umidade           
  situacao();                                                            // chama a função que armazena em uma mensagem a situação dos dispositivos conectados ao Arduino
  

  while(mySerial.available()) {                                          // Verifica se há dados disponíveis para ler na porta serial secundária (mySerial, dados vindos do ESP32) e executa este loop na quantidade de dados disponíveis
    char inChar = mySerial.read();                                       // Lê um caractere da porta serial (mySerial/ESP32) e armazena na variável "inChar"
    mensagem += inChar;                                                  // Adiciona o caractere lido ao final da variável string "mensagem"
    
    // Verifica se a mensagem foi completada
    if (inChar == '\n') {                                                // Se o caractere lido for uma nova linha ('\n'), significa que a mensagem terminou
      mensagem.trim();                                                   // Remove espaços em branco ou quebras de linha extras no início ou final da variável "mensagem"
      Serial.println("Mensagem recebida: " + mensagem);                  // Exibe a mensagem recebida no monitor serial
      
      if(mensagem.startsWith("led_red_on")) {                            // Verifica se a mensagem começa com "led_red_on"
        digitalWrite(LedRed, HIGH);                                      // Liga o LED
        stateLED = true;                                                 // Altera o estado do LED para indicar que está ligado

      } else if (mensagem.startsWith("led_red_off")) {                   // Verifica se a mensagem começa com "led_red_off"
        digitalWrite(LedRed, LOW);                                       // Desliga o LED
        stateLED = false;                                                // Altera o estado do LED para indicar que está desligado

      } else if (mensagem.startsWith("ventila_cozinha_on")) {            // Verifica se a mensagem começa com "ventila_cozinha_on"
        digitalWrite(cozinha_Ventilacao, HIGH);                          // Liga a ventilação da cozinha
        stateVentilaCozinha = true;                                      // Altera o estado da ventilação da cozinha para indicar que está ligado

      } else if (mensagem.startsWith("ventila_cozinha_off")) {           // Verifica se a mensagem começa com "ventila_cozinha_off"
        digitalWrite(cozinha_Ventilacao, LOW);                           // Desliga a ventilação da cozinha
        stateVentilaCozinha = false;                                     // Altera o estado da ventilação da cozinha para indicar que está desligado

      } else if (mensagem.startsWith("ventila_sala_on")) {               // Verifica se a mensagem começa com "ventila_sala_on"
        stateVentilaSala = false;                                        // Altera o estado da ventilação da sala para indicar que foi permitido a ativação da ventilação

      } else if (mensagem.startsWith("ventila_sala_off")) {              // Verifica se a mensagem começa com "ventila_sala_off"
        digitalWrite(sala_Ventilacao, LOW);                              // Desliga a ventilação da sala

      } else if (mensagem.startsWith("buzzer")) {                        // Verifica se a mensagem começa com "buzzer"
        acionarBuzzer();                                                 // chama a Função responsável por acionar o Buzzer

      } else if (mensagem.startsWith("irrigador_on")) {                  // Verifica se a mensagem começa com "irrigador_on"
        digitalWrite(irrigador, HIGH);                                   // Aciona o irrigador/bomba
        stateIrrigador = true;                                           // Altera o estado do irrigador para indicar que foi ligado

      } else if (mensagem.startsWith("irrigador_off")) {                 // Verifica se a mensagem começa com "irrigador_off"
        digitalWrite(irrigador, LOW);                                    // Desliga o irrigador/bomba
        stateIrrigador = false;                                          // Altera o estado do irrigador para indicar que foi desligado

      } else if (mensagem.startsWith("temp_umid")) {                     // Verifica se a mensagem começa com "temp_umid"
        mySerial.println(mensagemTempUmid);                              // Envia a mensagem do "mensagemTempUmid" pela porta serial secundária: mySerial (ESP32)
        delay(250);                                                      // atraso de 250 milissegundos

      } else if (mensagem.startsWith("status")) {                        // Verifica se a mensagem começa com "status"
        mySerial.println(mensagemStatus);                                // Envia a mensagem do "mensagemStatus" pela porta serial secundária: mySerial (ESP32)
        delay(250);                                                      // atraso de 250 milissegundos

      } else {                                                                            // Caso a mensagem não corresponda a nenhum comando, exibe as informações abaixo
        Serial.println("Erro de leitura / comando desconhecido. Tente novamente.");       // Envia a mensagem de erro no monitor serial
        mySerial.println("Erro de leitura / comando desconhecido. Tente novamente.");     // Envia a mensagem pela porta serial secundária: mySerial (ESP32)
      }

      mensagem = "";                                                     // Limpa a variável para a próxima mensagem
      mensagemStatus = "";                                               // Limpa a variável para a próxima mensagem
    }
  }
  
  mensagemTempUmid = "";                                                 // Limpa a variável para a próxima mensagem
  delay(100);                                                            // atraso de 100 milissegundos para evitar sobrecarga do loop
}
