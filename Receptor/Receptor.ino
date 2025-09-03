//Bibliotecas necessárias
#include <esp_now.h>
#include "esp_wifi.h"
#include <WiFi.h>

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

dados_carro dadosRecebidos;

//função para avisar quando chegou um dado e printa-lo
void CallBackDadosRecebidos(const esp_now_recv_info_t *esp_now_info, const uint8_t *data, int data_len){
  //copia os dados recebidos para a struct
  memcpy(&dadosRecebidos, data, sizeof(dadosRecebidos));

  //Apresenta os dados
  Serial.print("Velocidade: ");
  Serial.println(dadosRecebidos.velocidade);

  Serial.print("RPM: ");
  Serial.println(dadosRecebidos.RPM);
}


void setup(){
  Serial.begin(115200); //ativa serial
  WiFi.mode(WIFI_STA); //ativa modo station
  Serial.println("MAC do Receptor: ");
  Serial.println(WiFi.macAddress());

  //inicializa esp now
  if (esp_now_init() != ESP_OK) {
    Serial.println("Erro ao inicializar ESP-NOW");
    return;
  }

  //usa a função sempre que receber os dados
  esp_now_register_recv_cb(CallBackDadosRecebidos);
}

void loop(){

}



