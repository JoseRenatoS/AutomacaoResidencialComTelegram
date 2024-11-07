#include <WiFi.h>                                   // Usada para conectar o ESP32 à uma rede Wi-Fi
#include <WiFiClientSecure.h>                       // Estabelece uma conexão segura com o Wi-Fi
#include <UniversalTelegramBot.h>                   // Usada para interagir com a API do Telegram
#include <ArduinoJson.h>                            // Facilita o trabalho com JSON no Arduino, essencial para processar dados de APIs como a do Telegram.
#include <HardwareSerial.h>                         // permite o uso de portas seriais de hardware do ESP32, além da serial padrão.                      

// Inicializando a conexao Wi-Fi
char ssid[] = "NomeDoWifi";                             // nome da rede Wi-Fi
char password[] = "SenhaDoWifi";                    // senha da rede Wi-Fi

HardwareSerial SerialPort(2);                       // determina o pino 16 como RX (receber dados do Arduino) e o pino 17 como TX (enviar dados para o Arduino)             

//Inicializa o BOT Telegram
#define BOTtoken "chaveToken"          // define 'BOTtoken' com a chave Token do BOT

#define ID_CHAT "IDTelegramUsuario"                                               // define "ID_CHAT" com a ID do Telegram do usuario


#define sensorChama 34                              // pino 34 - sensor de chama
#define sensorGas 12                                // pino 25 - sensor de gás
#define sensorUmidadeSolo 26                        // pino 26 - sensor de umidade do solo
#define sensorMovimento 14                          // pino 14 - sensor de movimento
#define moduloLDR 27                                // pino 27 - modelo LDR (lógico) - interno
#define salaLuz 5                                   // pino  5 - luz da sala
#define cozinhaLuz 21                               // pino 21 - luz da cozinha
#define quartoLuz 22                                // pino 22 - luz do quarto
#define banheiroLuz 23                              // pino 23 - luz do banheiro
#define areaLuz 33                                  // pino 33 - luz da área de serviço
#define quintalLuz 32                               // pino 32 - luz do quintal

WiFiClientSecure client;                            // Cria um cliente seguro para conexões Wi-Fi
UniversalTelegramBot bot(BOTtoken, client);         // Inicializa o bot do Telegram com a chave de acesso definida e o cliente WiFi seguro

int Bot_mtbs = 1000;                                // define o tempo entre a leitura das mensagens do bot
long Bot_lasttime;                                  // Armazena o tempo da última mensagem recebida
//bool Start = false;                                 // Variável booleana usada para controlar o início
String texto = "";                                  // Variável para armazenar a mensagem recebida pelo SerialPort (Arduino)
String textoArduino = "";                           // Variável para armazenar a mensagem recebida pelo SerialPort (Arduino)
const int intervalo = 1000;                         // Defina a frequência de amostragem (em milissegundos)
volatile int count;                                 // Contador de pulsos



// VARIÁVEIS USADAS PARA ARMAZENAR O ESTADO DOS SENSORES
int state_Sensor_Movimento;                         // armazena o estado do sensor de movimento
int state_Sensor_Gas;                               // armazena o estado do sensor de gás
int state_Sensor_Chama;                             // armazena o estado do sensor de chama
int state_Sensor_Umidade_Solo;                      // armazena o estado do sensor de umidade do solo
int state_modulo_LDR;                               // armazena o estado do módulo LDR (digital)


// VARIÁVEIS USADAS PARA ARMAZENAR O ESTADO DA ILUMINAÇÃO DE CADA CÔMODO
bool state_Luz_Sala = false;                        // armazena o estado da iluminação da sala
bool state_Luz_Cozinha = false;                     // armazena o estado da iluminação da cozinha
bool state_Luz_Quarto = false;                      // armazena o estado da iluminação da quarto
bool state_Luz_Banheiro = false;                    // armazena o estado da iluminação da banheiro
bool state_Luz_Area = false;                        // armazena o estado da iluminação da area
bool state_Luz_Quintal = false;                     // armazena o estado da iluminação da quintal



// VARIÁVEIS USADAS PARA ARMAZENAR O ESTADO DE CADA SISTEMA:
// false - sistema desativado  (sensores funcionam, mas nenhuma tarefa é executada)
//  true - sistema ativado     (sensores funcionam e as tarefas são executadas)
bool state_Sistema_Alarme = false;                  // armazena o estado do Sistema de Alarme de Movimento 
bool state_Sistema_Gas = true;                      // armazena o estado do Sistema de Gas (ventilação da cozinha)
bool state_Sistema_Irrigacao = true;                // armazena o estado do Sistema de Irrigação
bool state_Sistema_Temp_Umid = true;                // armazena o estado do Sistema de Temperatura e Umidade (ventilação da sala)
bool state_Sistema_Luz_Externa = true;              // armazena o estado do Sistema de Iluminação Externa
bool state_Sistema_Luz_Interna = true;              // armazena o estado do Sistema de Iluminação Interna



// VARIÁVEIS USADA PARA ARMAZENAR O ESTADO ANTERIOR DE CADA SENSOR (para não executar o buzzer e o envio da mensagem toda hora)
bool vazamento_Gas_Anterior = false;                // armazenar o estado anterior do vazamento de gás
bool chama_Anterior = false;                        // armazenar o estado anterior do incêndio
bool irrigacao_Anterior = false;                    // armazenar o estado anterior da irrigação do solo
bool alarme_Anterior = false;                       // armazenar o estado anterior do sensor movimento (movimento detectado ou não)



void handleNewMessages(int numNewMessages) {                     // Declaração de uma função chamada handleNewMessages que recebe o número de novas mensagens como argumento
  Serial.print("Mensagem recebida = ");                          // Imprime no console a mensagem "Mensagem recebida = "
  Serial.println(String(numNewMessages));                        // Imprime no console o número de novas mensagens recebidas


  for (int i = 0; i < numNewMessages; i++) {                     // Loop que percorre as novas mensagens recebidas
    String chat_id = String(bot.messages[i].chat_id);            // Obtém o ID da conversa da mensagem atual, em formato string

    if (chat_id != ID_CHAT) {                                    // Compara o ID obtido na linha anterior com o ID_CHAT definido na linha 16 
      bot.sendMessage(chat_id, "Usuário não autorizado", "");    // Caso os IDs sejam diferente, é exibido a mensagem "usuário não autorizado" no Telegram
      continue;                                                  // A iteração atual é interrompida e avança para a próxima iteração (não sendo possível executar nenhuma das tarefas da função)
    }
    
    String text = bot.messages[i].text;                          // Obtém o texto da mensagem atual, que é armazenado na String 'text' criada

    String from_name = bot.messages[i].from_name;                // Obtém o nome do remetente da mensagem atual, que é armazenada na String 'from_name' criada
    if (from_name == "") from_name = "Usuário desconhecido";     // Verifica se o nome do remetente está vazio na variável anterior e, se sim, define como "Usuário desconhecido"


    if (text == "/status") {                                                          // comando usado verificar a situação de casa sistema
      String mensagemArduino = "";                                                    // uma variável criada para armazenada a mensagem recebida do Arduino
      SerialPort.println("status");                                                   // envia a mensagem "status" para o SerialPort
      delay(1000);
    

      while(SerialPort.available()) {                                                 // Verifica se há dados disponíveis na porta serial (SerialPort)
        char inChar = SerialPort.read();                                              // Lê um caractere da porta serial e armazena na variável "inChar"
        textoArduino += inChar;                                                       // Adiciona o caractere lido ao final da variável string "textoArduino"
        

        // Verifica se a mensagem foi completada
        if (inChar == '\t') {                                                         // Se o caractere lido for uma tabulação ('\t'), significa que a mensagem terminou
          textoArduino.trim();                                                        // Remove espaços em branco ou quebras de linha extras no início ou final da variável "texto"
          Serial.println("Mensagem recebida: " + textoArduino);                       // Exibe a mensagem recebida no monitor serial
        }
      }


      String welcome = "SITUAÇÃO DOS SISTEMAS DA RESIDÊNCIA\n\n";                     // Adiciona mensagem na variável


      if (state_Sistema_Gas == 1) {                                                   // Verifica o estado da variável responsável por indicar se o Sistema de Vazamento de Gás está ligado
        welcome += "Sistema de Vazamento de Gás ligado\n";                            // Adiciona mensagem indicando que o Sistema de Vazamento de Gás está ligado                     
      } else {                                                                        // Caso contrário, se o Sistema de Vazamento de Gás estiver desligada
        welcome += "Sistema de Vazamento de Gás desligado\n";                         // Adiciona mensagem indicando que o Sistema de Vazamento de Gás está desligado                 
      }
      if (state_Sistema_Alarme == 1) {                                                // Verifica o estado da variável responsável por indicar se o Sistema de Alarme de Movimento está ligado
        welcome += "Sistema de Alarme de Movimento ligado\n";                         // Adiciona mensagem indicando que o Sistema de Alarme de Movimento está ligado                     
      } else {                                                                        // Caso contrário, se o Sistema de Alarme de Movimento estiver desligada
        welcome += "Sistema de Alarme de Movimento desligado\n";                      // Adiciona mensagem indicando que o Sistema de Alarme de Movimento está desligado                  
      }
      if (state_Sistema_Irrigacao == 1) {                                             // Verifica o estado da variável responsável por indicar se o Sistema de Irrigação do Jardim está ligado
        welcome += "Sistema de Irrigação do Jardim ligado\n";                         // Adiciona mensagem indicando que o Sistema de Irrigação do Jardim está ligado                     
      } else {                                                                        // Caso contrário, se o Sistema de Irrigação do Jardim estiver desligada
        welcome += "Sistema de Irrigação do Jardim desligado\n";                      // Adiciona mensagem indicando que o Sistema de Irrigação do Jardim está desligado                 
      }
      if (state_Sistema_Temp_Umid == 1) {                                             // Verifica o estado da variável responsável por indicar se o Sistema de Ventilação da Sala está ligado
        welcome += "Sistema de Ventilação da Sala ligado\n";                          // Adiciona mensagem indicando que o Sistema de Ventilação da Sala está ligado                    
      } else {                                                                        // Caso contrário, se o Sistema de Ventilação da Sala estiver desligada
        welcome += "Sistema de Ventilação da Sala desligado\n";                       // Adiciona mensagem indicando que o Sistema de Ventilação da Sala está desligado                  
      }
      if (state_Sistema_Luz_Externa == 1) {                                           // Verifica o estado da variável responsável por indicar se o Sistema de Iluminação Externa está ligado
        welcome += "Sistema de Iluminação Externa ligado\n\n";                        // Adiciona mensagem indicando que o Sistema de Iluminação Externa está ligado                    
      } else {                                                                        // Caso contrário, se o Sistema de Iluminação Externa estiver desligada
        welcome += "Sistema de Iluminação Externa desligado\n\n";                     // Adiciona mensagem indicando que o Sistema de Iluminação Externa está desligado           
      }




      if (state_Sensor_Gas == 1) {                                                    // Verifica se o sensor vazamento de gás na cozinha
        welcome += "ALERTA: Vazamento de Gás detectado\n";                            // Adiciona mensagem indicando que foi detectado vazamento de gás na cozinha                    
      } else {                                                                        // Caso contrário, se não foi detectado vazamento de gás na cozinha
        welcome += "Ambiente seguro: SEM vazamento de gás\n";                         // Adiciona mensagem indicando que não foi detectado vazamento de gás na cozinha                 
      }
      if (state_Sensor_Chama == 1) {                                                  // Verifica se o sensor detectou chama na cozinha
        welcome += "ALERTA: Chama detectada\n";                                       // Adiciona mensagem indicando que foi detectado chama na cozinha                                     
      } else {                                                                        // Caso contrário, se não foi detectado chama na cozinha
        welcome += "Ambiente seguro: SEM chama detectada\n";                          // Adiciona mensagem indicando que não foi detectado chama na cozinha                          
      }
      if (state_Sensor_Movimento == 1) {                                              // Verifica se o sensor detectou movimento no quintal
        welcome += "ALERTA: movimento detectado no quintal\n";                        // Adiciona mensagem indicando que foi detectado movimento no quintal
      } else {                                                                        // Caso contrário, se não foi detectado movimento
        welcome += "Área segura: nenhum movimento detectado\n";                       // Adiciona mensagem indicando que não foi detectado movimento no quintal              
      }
      if (state_Sensor_Umidade_Solo == 1) {                                           // Verifica o estado do SENSOR DE UMIDADE DO SOLO, indicando se o solo está seco
        welcome += "ALERTA: Solo seco\n\n";                                           // Adiciona mensagem indicando que a Iluminação da Sala está ligada                          
      } else {                                                                        // Caso contrário, se
        welcome += "Solo úmido\n\n";                                                  // Adiciona mensagem indicando que a Iluminação da Sala está desligada                
      }
      
      
      if (state_Luz_Sala == 1) {                                                      // Verifica o estado da variável responsável por indicar se a Iluminação da Sala está ligada
        welcome += "Iluminação da Sala ligada\n";                                     // Adiciona mensagem indicando que a Iluminação da Sala está ligada                            
      } else {                                                                        // Caso contrário, se a Iluminação do Quintal estiver desligada
        welcome += "Iluminação da Sala desligada\n";                                  // Adiciona mensagem indicando que a Iluminação da Sala está desligada              
      }
      if (state_Luz_Cozinha == 1) {                                                   // Verifica o estado da variável responsável por indicar se a Iluminação da Cozinha está ligada
        welcome += "Iluminação da Cozinha ligada\n";                                  // Adiciona mensagem indicando que a Iluminação da Cozinha está ligada                            
      } else {                                                                        // Caso contrário, se a Iluminação do Quintal estiver desligada
        welcome += "Iluminação da Cozinha desligada\n";                               // Adiciona mensagem indicando que a Iluminação da Cozinha está desligada              
      }
      if (state_Luz_Quarto == 1) {                                                    // Verifica o estado da variável responsável por indicar se a Iluminação do Quarto está ligada
        welcome += "Iluminação do Quarto ligada\n";                                   // Adiciona mensagem indicando que a Iluminação do Quarto está ligada                         
      } else {                                                                        // Caso contrário, se a Iluminação do Quintal estiver desligada
        welcome += "Iluminação do Quarto desligada\n";                                // Adiciona mensagem indicando que a Iluminação do Quarto está desligada               
      }
      if (state_Luz_Banheiro == 1) {                                                  // Verifica o estado da variável responsável por indicar se a Iluminação do Banheiro está ligada
        welcome += "Iluminação do Banheiro ligada\n";                                 // Adiciona mensagem indicando que a Iluminação do Banheiro está ligada                           
      } else {                                                                        // Caso contrário, se a Iluminação do Quintal estiver desligada
        welcome += "Iluminação do Banheiro desligada\n";                              // Adiciona mensagem indicando que a Iluminação do Banheiro está desligada                
      }
      if (state_Luz_Area == 1) {                                                      // Verifica o estado da variável responsável por indicar se a Iluminação da Área de Serviço está ligada
        welcome += "Iluminação da Área de Serviço ligada\n";                          // Adiciona mensagem indicando que a Iluminação da Área de Serviço está ligada    
      } else {                                                                        // Caso contrário, se a Iluminação do Quintal estiver desligada
        welcome += "Iluminação da Área de Serviço desligada\n";                       // Adiciona mensagem indicando que a Iluminação da Área de Serviço está desligada               
      }
      if (state_Luz_Quintal == 1) {                                                   // Verifica o estado da variável responsável por indicar se a Iluminação do Quintal está ligada
        welcome += "Iluminação do Quintal ligada\n\n";                                // Adiciona mensagem indicando que a Iluminação do Quintal está ligada                
      } else {                                                                        // Caso contrário, se a Iluminação do Quintal estiver desligada
        welcome += "Iluminação do Quintal desligada\n\n";                             // Adiciona mensagem indicando que a Iluminação do Quintal está desligada
      }
      bot.sendMessage(chat_id, welcome + textoArduino, "Markdown");                   // envia mensagem para o Telegram

      delay(500);

    } else if (text.startsWith("/sala_on")) {                                         // verifica se o texto inicia com "/sala_on"
      digitalWrite(salaLuz, HIGH);                                                    // liga a iluminação da sala
      bot.sendMessage(chat_id, "Iluminação da sala ativada", "");                     // envia mensagem para o Telegram

    } else if (text.startsWith("/sala_off")) {                                        // verifica se o texto inicia com "/sala_off"
      digitalWrite(salaLuz, LOW);                                                     // desliga a iluminação da sala
      bot.sendMessage(chat_id, "Iluminação da sala desativada", "");                  // envia mensagem para o Telegram
    
    } else if (text.startsWith("/cozinha_on")) {                                      // verifica se o texto inicia com "/cozinha_on"
      digitalWrite(cozinhaLuz, HIGH);                                                 // liga a iluminação da cozinha
      bot.sendMessage(chat_id, "Iluminação da cozinha ativada", "");                  // envia mensagem para o Telegram
    
    } else if (text.startsWith("/cozinha_off")) {                                     // verifica se o texto inicia com "/cozinha_off"
      digitalWrite(cozinhaLuz, LOW);                                                  // desliga a iluminação da cozinha
      bot.sendMessage(chat_id, "Iluminação da cozinha desativada", "");               // envia mensagem para o Telegram
    
    } else if (text.startsWith("/quarto_on")) {                                       // verifica se o texto inicia com "/quarto_on"
      digitalWrite(quartoLuz, HIGH);                                                  // liga a iluminação do quarto
      bot.sendMessage(chat_id, "Iluminação do quarto ativada", "");                   // envia mensagem para o Telegram
     
    } else if (text.startsWith("/quarto_off")) {                                      // verifica se o texto inicia com "/quarto_off"
      digitalWrite(quartoLuz, LOW);                                                   // liga a iluminação do quarto
      bot.sendMessage(chat_id, "Iluminação do quarto desativada", "");                // envia mensagem para o Telegram
    
    } else if (text.startsWith("/banheiro_on")) {                                     // verifica se o texto inicia com "/banheiro_on"
      digitalWrite(banheiroLuz, HIGH);                                                // liga a iluminação do banheiro
      bot.sendMessage(chat_id, "Iluminação do banheiro ativada", "");                 // envia mensagem para o Telegram
    
    } else if (text.startsWith("/banheiro_off")) {                                    // verifica se o texto inicia com "/banheiro_off"
      digitalWrite(banheiroLuz, LOW);                                                 // liga a iluminação do banheiro
      bot.sendMessage(chat_id, "Iluminação do banheiro desativada", "");              // envia mensagem para o Telegram
    
    } else if (text.startsWith("/area_on")) {                                         // verifica se o texto inicia com "/area_on"
      digitalWrite(areaLuz, HIGH);                                                    // liga a iluminação da área de serviço
      bot.sendMessage(chat_id, "Iluminação da área de serviço ativada", "");          // envia mensagem para o Telegram
    
    } else if (text.startsWith("/area_off")) {                                        // verifica se o texto inicia com "/area_of"
      digitalWrite(areaLuz, LOW);                                                     // desliga a iluminação da área de serviço
      bot.sendMessage(chat_id, "Iluminação da área de serviço desativada", "");       // envia mensagem para o Telegram
    
    } else if (text.startsWith("/quintal_on")) {                                      // verifica se o texto inicia com "/quintal_on"
      state_Sistema_Luz_Externa = true;                                               // Ativa o Sistema de Iluminação Externa
      bot.sendMessage(chat_id, "Sistema de Iluminação Externa ATIVADO", "");          // envia mensagem para o Telegram
    
    } else if (text.startsWith("/quintal_off")) {                                     // verifica se o texto inicia com "/quintal_off"
      state_Sistema_Luz_Externa = false;                                              // Desativa o Sistema de Iluminação Externa
      digitalWrite(quintalLuz, LOW);                                                  // Apaga a iluminação do quintal
      bot.sendMessage(chat_id, "Sistema de Iluminação Externa DESATIVADO", "");       // envia mensagem para o Telegram
    
    } else if (text.startsWith("/alarmePIR_on")) {                                    // verifica se o texto inicia com "/alarme_on"
      state_Sistema_Alarme = true;                                                    // Ativa o Sistema de Alarme de Movimento
      bot.sendMessage(chat_id, "Sistema de Alarme de Movimento ATIVADO", "");         // envia mensagem para o Telegram
    
    } else if (text.startsWith("/alarmePIR_off")) {                                   // verifica se o texto inicia com "/alarme_off"
      state_Sistema_Alarme = false;                                                   // Desativa o Sistema de Alarme de Movimento
      SerialPort.println("led_red_off");                                              // Envia o comando "led_red_off" para o monitor serial SerialPort (Arduino via UART2), para desligar o LED de emergência
      bot.sendMessage(chat_id, "Sistema de Alarme de Movimento DESATIVADO", "");      // envia mensagem para o Telegram
    
    } else if (text.startsWith("/ventila_sala_on")) {                                 // verifica se o texto inicia com "/ventila_sala_on"
      state_Sistema_Temp_Umid = true;                                                 // Ativa o sistema de temperatura e umidade (ventilação da sala)
      SerialPort.println("ventila_sala_on");                                          // Envia o comando "ventila_sala_on" para o monitor serial SerialPort (Arduino via UART2), para ativar o sistema de ventilação da sala no Arduíno
      bot.sendMessage(chat_id, "Sistema de Ventilação da sala ATIVADO", "");          // envia mensagem para o Telegram
    } else if (text.startsWith("/ventila_sala_off")) {                                // verifica se o texto inicia com "/ventila_sala_off"
      state_Sistema_Temp_Umid = false;                                                // Desativa o sistema de temperatura e umidade (ventilação da sala)
      SerialPort.println("ventila_sala_off");                                         // Envia o comando "ventila_sala_on" para o monitor serial SerialPort (Arduino via UART2), para desligar a ventilação da sala
      bot.sendMessage(chat_id, "Sistema de Ventilação da sala DESATIVADO", "");       // envia mensagem para o Telegram
    
    } else if (text.startsWith("/ventila_cozinha_on")) {                              // verifica se o texto inicia com "/ventila_cozinha_on"
      state_Sistema_Gas = true;                                                       // Ativa o sistema de vazamento de gás
      bot.sendMessage(chat_id, "Sistema de Ventilação da cozinha ATIVADO", "");       // envia mensagem para o Telegram
    
    } else if (text.startsWith("/ventila_cozinha_off")) {                             // verifica se o texto inicia com "/ventila_cozinha_off"
      SerialPort.println("ventila_cozinha_off");                                      // Envia o comando "ventila_cozinha_off" para o monitor serial SerialPort (Arduino via UART2), para desligar a ventilação da cozinha
      delay(250);                                                                     // atraso de 250 milissegundos
      state_Sistema_Gas = false;                                                      // Desativa o sistema de vazamento de gás
      vazamento_Gas_Anterior = false;                                                 // Altera o nível lógico da variável vazamento_Gas_Anterior
      SerialPort.println("led_red_off");                                              // Envia o comando "led_red_off" para o Arduino via UART2, para desligar o LED de emergência
      bot.sendMessage(chat_id, "Sistema de Ventilação da cozinha DESATIVADO", "");    // envia mensagem para o Telegram
    
    } else if (text.startsWith("/irrigador_on")) {                                    // verifica se o texto inicia com "/irrigador_on"
      state_Sistema_Irrigacao = true;                                                 // Ativa o sistema de Irrigação
      bot.sendMessage(chat_id, "Sistema de Irrigação do Jardim ATIVADO", "");         // envia mensagem para o Telegram
    
    } else if (text.startsWith("/irrigador_off")) {                                   // verifica se o texto inicia com "/irrigador_off"
      SerialPort.println("irrigador_off");                                            // Envia o comando "irrigador_off" para o Arduino via UART2
      state_Sistema_Irrigacao = false;                                                // Desativa o sistema de Irrigação
      irrigacao_Anterior = false;                                                     // Muda o nível lógico para quando o sistema for ativado e o sensor também já esteja ativo
      bot.sendMessage(chat_id, "Sistema de Irrigação do Jardim DESATIVADO", "");      // envia mensagem para o Telegram
    
    } else if (text.startsWith("/temp_umid")) {                                       // verifica se o texto inicia com "/temp_umid"
      SerialPort.println("temp_umid");                                                //  Envia o comando "temp_umid" para o Arduino via UART2
      delay(500);                                                                     // atraso de 500 milissegundos
    } else {                                                                          // exibe as opções de atividades
      String keyboardJson = "[[\"/sala_on\", \"/sala_off\", \"/cozinha_on\", \"/cozinha_off\"], [\"/quarto_on\", \"/quarto_off\", \"/banheiro_on\", \"/banheiro_off\"], [\"/area_on\", \"/area_off\", \"/quintal_on\", \"/quintal_off\"], [\"/ventila_sala_on\", \"/ventila_sala_off\", \"/ventila_cozinha_on\", \"/ventila_cozinha_off\"], [\"/alarmePIR_on\", \"/alarmePIR_off\", \"/irrigador_on\", \"/irrigador_off\"], [\"/temp_umid\", \"/status\"]]";
      bot.sendMessageWithReplyKeyboard(chat_id, "Bem-vindo ao Painel de Controle de sua Residencial.\nO que deseja fazer? Escolha uma das opções abaixo:", "", keyboardJson, true);
      // a primeira linha cria uma string no formato JSON, que representa um teclado virtual com botões. 
      // a segunda linha exibe este teclado virtual no Telegram e envia também uma mensagem acima
    }
  }
}




// função usada para exibir a informação armazenada na variável "mensagem", será exibido na conversa do Telegram com o usuário do ID_CHAT
void envioMensagem(const String& mensagem) {                    // Define a função 'envioMensagem' que envia uma mensagem ao Telegram
  bot.sendMessage(ID_CHAT, mensagem, "");                       // Usa o método 'sendMessage' do objeto 'bot' para enviar a mensagem ao chat especificado
                                                                // 'ID_CHAT' é o ID do chat de destino, e a mensagem é o conteúdo enviado
}



// FUNÇÃO USADA PARA O SISTEMA DE VAZAMENTO DE GÁS
void vazamentoDeGas() {
  Serial.println("estado do Sensor de Gás: " + String(state_Sensor_Gas));   // exibe no monitor serial o estado atual do sensor de gás
  
  if(state_Sistema_Gas == 1) {
    if(state_Sensor_Gas == 1 && vazamento_Gas_Anterior == false) {
      SerialPort.println("led_red_on");                                     // Envia o comando "led_red_on" para o Arduino via UART2, para acionar o LED VERMELHO DE EMERGÊNCIA
      delay(250);                                                           // atraso de 250 milissegundos
      SerialPort.println("ventila_cozinha_on");                             // Envia o comando "ventila_cozinha_on" para o Arduino via UART2, para ligar a ventilação da cozinha
      delay(250);                                                           // atraso de 250 milissegundos
      SerialPort.println("buzzer");                                         // Envia o comando "buzzer" para o Arduino via UART2, para emitir um alarme sonoro
      envioMensagem("Vazamento de Gás Detectado!");                         // Envia a mensagem para o Telegram
      vazamento_Gas_Anterior = true;                                        // Atualiza a variavel "vazamento_Gas_Anterior"
    } else if (state_Sensor_Gas == 0 && vazamento_Gas_Anterior == true) {
      SerialPort.println("led_red_off");                                    // Envia o comando "led_red_off" para o Arduino via UART2, para desativar o LED VERMELHO DE EMERGÊNCIA
      delay(250);                                                           // atraso de 250 milissegundos
      SerialPort.println("ventila_cozinha_off");                            // Envia o comando "ventila_cozinha_off" para o Arduino via UART2, para desligar a ventilação da cozinha
      delay(250);                                                           // atraso de 250 milissegundos
      envioMensagem("Nenhum Vazamento de Gás detectado!");                  // Envia a mensagem para o Telegram
      vazamento_Gas_Anterior = false;                                       // Atualiza a variavel "vazamento_Gas_Anterior"
    }
    delay(200);
  }
}



// FUNÇÃO USADA PARA O SISTEMA DE INCÊNDIO
void alarmeDeIncendio() {
  Serial.println("estado do sensor de chama: " + String(state_Sensor_Chama)); // exibe no monitor serial o estado atual do sensor de chama
  
  if(state_Sensor_Chama == 0 && chama_Anterior == false) {
    SerialPort.println("led_red_on");                                       // Envia o comando "led_red_on" para o Arduino via UART2, para acionar o LED VERMELHO DE EMERGÊNCIA
    delay(250);                                                             // atraso de 250 milissegundos
    SerialPort.println("buzzer");                                           // Envia o comando "buzzer" para o Arduino via UART2, para emitir um alarme sonoro
    delay(250);                                                             // atraso de 250 milissegundos
    envioMensagem("Chama detectada!");                                      // Envia a mensagem para o Telegram
    chama_Anterior = true;                                                  // Atualiza o estado da variavel "chama_Anterior"
  } else if (state_Sensor_Chama == 1 && chama_Anterior == true) {
    SerialPort.println("led_red_off");                                      // Envia o comando "led_red_off" para o Arduino via UART2, para desativar o LED VERMELHO DE EMERGÊNCIA
    delay(250);                                                             // atraso de 250 milissegundos
    envioMensagem("Nenhuma chama detectada!");                              // Envia a mensagem para o Telegram
    chama_Anterior = false;                                                 // Atualiza o estado da variavel "chama_Anterior"
  }
  delay(200);                                                            // atraso de 200 milissegundos
}



// FUNÇÃO USADA PARA O SISTEMA DE IRRIGAÇÃO DO JARDIM
void irrigacao() {
  Serial.println("estado do sensor de Umidade do Solo: " + String(state_Sensor_Umidade_Solo));
  
  if(state_Sistema_Irrigacao == 1) {
    if(state_Sensor_Umidade_Solo == 1 && irrigacao_Anterior == false) {
      SerialPort.println("irrigador_on");                                  // Envia o comando "irrigador_on" para o Arduino via UART2, para acionar o irrigador
      delay(250);                                                          // atraso de 250 milissegundos
      envioMensagem("Solo seco - Irrigador ativado.");                     // Envia a mensagem para o Telegram 
      irrigacao_Anterior = true;                                           // Atualiza o estado da variavel "irrigacao_Anterior"
    } else if (state_Sensor_Umidade_Solo == 0 && irrigacao_Anterior == true) {
      SerialPort.println("irrigador_off");                                 // Envia o comando "irrigador_off" para o Arduino via UART2, para acionar o irrigador
      delay(250);                                                          // atraso de 250 milissegundos
      envioMensagem("Solo úmido - Irrigador desativado.");                 // Envia a mensagem para o Telegram
      irrigacao_Anterior = false;                                          // Atualiza o estado da variavel "irrigacao_Anterior"
    }
    delay(200);                                                            // atraso de 200 milissegundos
  }
}



// FUNÇÃO USADA PARA O SISTEMA DE ALARME DE MOVIMENTO
void alarmeMovimento() {
  Serial.println("estado do Alarme: " + String(state_Sistema_Alarme));
  Serial.println("estado do Sensor de Movimento: " + String(state_Sensor_Movimento));

  if(state_Sistema_Alarme == 1) {
    if(state_Sensor_Movimento == 1 && alarme_Anterior == false) {
      envioMensagem("Detectado movimento no quintal");                           // Envia a mensagem para o Telegram
      delay(250);                                                                // atraso de 250 milissegundos
      SerialPort.println("buzzer");
      alarme_Anterior = true;                                                    // Atualiza o estado da variável "alarme_Anterior"
    } else if (state_Sensor_Movimento == 0 && alarme_Anterior == true) {
      envioMensagem("Nenhum movimento foi detectado no quintal");                // Envia a mensagem para o Telegram
      alarme_Anterior = false;                                                   // Atualiza o estado da variável "alarme_Anterior"
      delay(250);
    }
  }
  delay(200);                                                            // atraso de 200 milissegundos
}



// FUNÇÃO USADA PARA O SISTEMA DE ILUMINAÇÃO EXTERNA
void iluminacaoExterna() {
  Serial.println("estado do modulo LDR: " + String(state_modulo_LDR));                                    // exibe o nível lógico da variável do sensor: 1 (escuro) 0 (claro)
  Serial.println("estado do Sistema de Iluminação Externa: " + String(state_Sistema_Luz_Externa));        // exibe o nível lógico da variável do sistema de iluminação externa
                               

  if(state_Sistema_Luz_Externa == 1) {
    if(state_Sensor_Movimento == 1  &&  state_modulo_LDR == 1  &&  state_Luz_Quintal == false) {
      digitalWrite(quintalLuz, HIGH);
      delay(200);
      envioMensagem("Iluminação do Quintal ativada");
      state_Luz_Quintal = true;
    } else if (state_Sensor_Movimento == 1  &&  state_modulo_LDR == 0  &&  state_Luz_Quintal == true) {
      digitalWrite(quintalLuz, LOW);
      delay(200);
      envioMensagem("Iluminação do Quintal desativada");
      state_Luz_Quintal = false;
    } else if (state_Sensor_Movimento == 0  &&  state_modulo_LDR == 1  &&  state_Luz_Quintal == true) {
      digitalWrite(quintalLuz, LOW);
      delay(200);
      envioMensagem("Iluminação do Quintal desativada");
      state_Luz_Quintal = false;
    } else if (state_Sensor_Movimento == 0  &&  state_modulo_LDR == 0  &&  state_Luz_Quintal == true) {
      digitalWrite(quintalLuz, LOW);
      delay(200);
      envioMensagem("Iluminação do Quintal desativada");
      state_Luz_Quintal = false;
    }
    delay(200);                                                                                           // atraso de 200 milissegundos
  }
}




void setup() {
  Serial.begin(115200);                          // inicializa a comunicação serial com uma taxa de transmissão de 115200 bps por segundo
  SerialPort.begin(9600);


  WiFi.mode(WIFI_STA);                           // Configura o Wi-Fi do ESP32 para modo de estação (cliente), permitindo que ele se conecte a uma rede Wi-Fi.
  delay(100);                                    // atraso de 100 milissegundos
                
  Serial.print("Conectando Wifi: ");             // Exibe a mensagem "Conectando Wifi: " no monitor serial
  Serial.println(ssid);                          // Exibe o SSID (nome da rede Wi-Fi) que o ESP32 tentará se conectar
  WiFi.begin(ssid, password);                    // Inicia a conexão Wi-Fi usando o SSID e a senha definidos
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);   // Define o certificado raiz necessário para autenticar a conexão segura com a API do Telegram.

  while (WiFi.status() != WL_CONNECTED) {        // Aguarda até que o status do Wi-Fi seja conectado (WL_CONNECTED)
    Serial.print(".");                           // Exibe um ponto (.) no monitor serial para indicar o progresso da conexão
    delay(500);                                  // atraso de 0,5 segundos para evitar sobrecarregar o loop
  }
  Serial.println("");                            // Pula uma linha no monitor serial para fins de organização
  Serial.println("WiFi conectado");              // Exibe a mensagem "WiFi conectado" no monitor serial quando a conexão for estabelecida
  Serial.print("IP address: ");                  // Exibe "IP address: " no monitor serial.
  Serial.println(WiFi.localIP());                // Exibe o endereço IP local obtido após a conexão à rede

  SerialPort.begin(9600, SERIAL_8N1, 16, 17);    // Inicializa a comunicação serial em uma taxa de 9600 bps usando os pinos 16 (RX) e 17 (TX) do ESP32


  pinMode(sensorChama, INPUT);                   // pino 34 - entrada lógica
  pinMode(sensorGas, INPUT);                     // pino 35 - entrada lógica
  pinMode(sensorUmidadeSolo, INPUT);             // pino 39 - entrada lógica
  pinMode(sensorMovimento, INPUT);               // pino 36 - entrada lógica
  pinMode(moduloLDR, INPUT);                     // pino 27 - entrada lógica
  pinMode(salaLuz, OUTPUT);                      // pino  5 - saída
  pinMode(cozinhaLuz, OUTPUT);                   // pino 21 - saída
  pinMode(quartoLuz, OUTPUT);                    // pino 22 - saída
  pinMode(banheiroLuz, OUTPUT);                  // pino 23 - saída
  pinMode(areaLuz, OUTPUT);                      // pino 33 - saída
  pinMode(quintalLuz, OUTPUT);                   // pino 32 - saída

  

  delay(500);
}



void loop() {
  Serial.println("\n\n\nINÍCIO DA ITERAÇÃO");                     // exibe no monitor serial "INICIO DA ITERACAO"
  state_Sensor_Gas = digitalRead(sensorGas);                      // Lê o estado do sensor de gás e armazena na variável
  state_Sensor_Chama = digitalRead(sensorChama);                  // Lê o estado do sensor de chama e armazena na variável
  state_Sensor_Umidade_Solo = digitalRead(sensorUmidadeSolo);     // Lê o estado do sensor de umidade do solo e armazena na variável
  state_Sensor_Movimento = digitalRead(sensorMovimento);          // Lê o estado do sensor de movimento e armazena na variável
  state_modulo_LDR = digitalRead(moduloLDR);                      // 1 (escuro) 0 (claro)
  

  // FUNÇÕES CHAMADAS
  vazamentoDeGas();
  alarmeDeIncendio();
  irrigacao();
  alarmeMovimento();
  iluminacaoExterna();



  // Controle de mensagens do bot Telegram, verifica o intervalo de tempo para evitar checagem constante
  if (millis() > Bot_lasttime + Bot_mtbs) {                                  // Verifica se o tempo atual "millis()" é maior que o último envio de mensagem mais o intervalo definido
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);      // Obtém o número de novas mensagens a partir da última recebida pelo bot
 
    while (numNewMessages) {                                                 // Executa o loop enquanto houver novas mensagens para processar
      Serial.println("Resposta recebida do Telegram");                       // Exibe mensagem no monitor serial
      handleNewMessages(numNewMessages);                                     // Chama a função "handleNewMessages()" e processa as novas mensagens recebidas pelo Telegram
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);        // Atualiza o número de mensagens novas
    }

    Bot_lasttime = millis();                                                 // Atualiza a variável "Bot_lasttime" da última mensagem para o tempo atual
  }



  // Checa por dados recebidos na comunicação serial
  while(SerialPort.available()) {                                            // Verifica se há dados disponíveis na porta serial (SerialPort)
    char inChar = SerialPort.read();                                         // Lê um caractere da porta serial e armazena na variável "inChar"
    texto += inChar;                                                         // Adiciona o caractere lido ao final da variável string "texto"
    

    // Verifica se a mensagem foi completada
    if (inChar == '\t') {                                                    // Se o caractere lido for uma nova linha ('\n'), significa que a mensagem terminou
      texto.trim();                                                          // Remove espaços em branco ou quebras de linha extras no início ou final da variável "texto"
      Serial.println("Mensagem recebida: " + texto);                         // Exibe a mensagem recebida no monitor serial
  
      envioMensagem(texto);                                                  // chama a função envioMensagem(), que envia para o Telegram a mensagem armazenada na variável "texto"

  
      texto = "";                                                            // Limpa a variável "texto" para receber a próxima mensagem
    }
  }

  delay(100);                                                                // Pequeno delay para evitar sobrecarga do loop
}