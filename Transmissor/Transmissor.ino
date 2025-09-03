//Bibliotecas necessárias
#include <WiFi.h>
#include <esp_now.h>

//variável para o endereço MAC
uint8_t receiverMac[] = {0x88, 0x13, 0xBF, 0x69, 0x99, 0xFC};

typedef struct dados_carro{
  /*
  int temp_motor;
  int press_oleo;
  int marcha;
  int cons_insta;
  int press_pneus[4];
  */
  int16_t velocidade;
  int16_t RPM;
} dados_carro;

dados_carro dados;

//função para verificar se foi enviado
void callBackEnviar(const wifi_tx_info_t *info, esp_now_send_status_t status){
  Serial.print("Status do envio: ");
  if (status == ESP_NOW_SEND_SUCCESS) {
    Serial.println("Sucesso");
  } else {
    Serial.println("Falhou");
  }
}

void setup(){
  Serial.begin(115200); //inicializa serial
  WiFi.mode(WIFI_STA); //ativa modo station

  Serial.println("MAC do Transmissor: ");
  Serial.println(WiFi.macAddress());

  //inicia esp now
  if (esp_now_init() != ESP_OK){
    Serial.println("Erro ao inicializar ESP-NOW");
    return;
  }

  //chama a função a cada envio
  esp_now_register_send_cb(callBackEnviar);

  esp_now_peer_info_t peerInfo = {};

  memcpy(peerInfo.peer_addr, receiverMac, 6); //define o MAC do receptor 
  peerInfo.channel = 0; //define o canal usado para transmissão
  peerInfo.encrypt = false; //define sem criptografia

  //verifica se o canal foi acionado
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {  
    Serial.println("Falha ao adicionar o peer");
    return;
  }



  dados.velocidade = 130;
  dados.RPM = 7000;
}

void loop(){
  dados.velocidade++;
  dados.RPM++;

  //envia os dados para o receptor
  esp_err_t result = esp_now_send(receiverMac, (uint8_t *) &dados, sizeof(dados));

  if (result == ESP_OK) {
    Serial.println("Pacote enviado com sucesso");
  } else {
    Serial.println("Erro ao enviar pacote");
  }
}

