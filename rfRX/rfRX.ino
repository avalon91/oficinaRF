#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

RF24 radio(9,10);      //Pinos CE e CSN

int vai[4] = {15,15,55,1010};
int veio[4];

const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };      //Endereços dos dois pipes de comunicação

void setup(){
  Serial.begin(9600);
  radio.begin();                          //Inicialização do RF
  radio.setRetries(15,15);             //Quantidade de tentativas: o delay entre elas e a quantidade
  radio.setPayloadSize(sizeof(vai));    //Tamanho do pacote de envio
  radio.openWritingPipe(pipes[1]);    //Abertura do pipe de escrita
  radio.openReadingPipe(1,pipes[0]);    //Abertura do pipe de leitura
  radio.startListening();            //Inicialização da interceptação de pacotes
  radio.setChannel(100);                //Canal para comunicação
  radio.setPALevel(RF24_PA_MAX);      //Amplificador de força no máximo
  radio.setDataRate(RF24_250KBPS);  //Taxa de comunicação
  radio.setCRCLength(RF24_CRC_8);    //Configuração do tamanho do CRC
}

void loop(){
  if(radio.available()){                        //Se houver tráfego no pipe de leitura
    bool done = false;
    while(!done)
      done = radio.read(&veio, sizeof(veio));    //Esperar até a função de leitura retornar TRUE - armazenamento do pacote na variável
    Serial.print("Recebido: ");
    for(int i = 0; i < 4; i++){
      Serial.print(veio[i]);            //Impressão dos dados recebidos
      Serial.print(" ");
    }
    Serial.println();
  }
  /*
  if(Serial.available() > 0){
    int i = Serial.read();
    if(i == '1'){
      radio.stopListening();            //Interrupção da interceptação de pacotes (necessário antes de realizar um write())
      Serial.println("Enviando...");
      bool ok = radio.write(&vai, sizeof(vai));    //Envio da variável
      if(ok)
        Serial.println("Envio OK!");
      else
        Serial.println("Falha no envio");
      radio.startListening();          //Reinicialização da interceptação de pacotes
    }
  }*/
}

