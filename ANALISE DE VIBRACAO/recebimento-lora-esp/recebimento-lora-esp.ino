#include <LoRa.h>
#include <SPI.h>
#include "arduinoFFT.h"

#define ss 5
#define rst 14
#define dio0 2

arduinoFFT FFT = arduinoFFT();

//const int samples = 4096; // (2^12) tamanho máximo pela capacidade do arduino nano (armazenar dados)
/*2^7 = 128*/
const int samples = 128;
const int samplingFrequency = 10000;

double vReal[samples*3]; //dados recebidos do arduino
double vImag[samples*3];
double frequencia_caracteristica_equipamento = 0;

int i = 0;
int flag = 0;
int counter = 0;

void setup() {
  Serial.begin(230400);
  
  while (!Serial);
 
  LoRa.setPins(ss, rst, dio0);    //configuração do módulo transceptor LoRa
 
  while (!LoRa.begin(433E6))     //433E6 - Asia, 866E6 - Europe, 915E6 - North America
  {
    Serial.println(".");
    delay(500);
  }
  LoRa.setSyncWord(0xA5);

  frequencia_caracteristica_equipamento = 2.0;
}
 
void loop() {
  int packetSize = LoRa.parsePacket();
   
  if (packetSize) {
    while (LoRa.available()) {
      
      String LoRaData = LoRa.readString();
      
      if(LoRaData.toFloat() == 999) {
        i = 0;
        flag = 1;
      }
      else {
        vReal[i] = LoRaData.toFloat();
//        Serial.println("alguma coisa chegou");
        vImag[i] = 0;
        i++;
      }
      
      if(flag == 1) {//se chegou no EOF
      
        /* MOSTRAR DADOS NO TEMPO */
//        for(int j = 0; j < samples; j++){
//          Serial.println(vReal[j]);
//        }
        
        /* TOMAR A FFT */
        FFT.Windowing(vReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
        FFT.Compute(vReal, vImag, samples, FFT_FORWARD);
        FFT.ComplexToMagnitude(vReal, vImag, samples);

//        for(int j = 0; j < samples; j++){
//          Serial.println(vReal[j]);
//        }
        
        /* MENSURAR AS VARIACOES */
        double x;
        double v;
        
        FFT.MajorPeak(vReal, samples, samplingFrequency, &x, &v);
        Serial.println("\n\n\n\n");
        Serial.println("-------------------------------------------------");
        if(v < frequencia_caracteristica_equipamento) {
          Serial.print("| FUNCIONAMENTO ADEQUADO\t\t\t|\n");
          Serial.print("-------------------------------------------------\n");
          Serial.print("| FREQUENCIA: ");
          Serial.print(x);
          Serial.print(" Hz\t\t \t|");
          Serial.print("\n| AMPLITUDE: ");
          Serial.print(v);
          Serial.print(" db\t \t \t\t|");
          Serial.print("\n| % DE VIBRACAO (NOMINAL): ");
          Serial.print((v*100)/frequencia_caracteristica_equipamento);
          Serial.print(" %\t\t|\n");
        } else {
          Serial.print("| FUNCIONAMENTO INADEQUADO (MOTOR COM PROBLEMA) ");
          Serial.print("|\n");
          Serial.print("| INICIAR PROTOCOLOS DE MANUTENCAO \t\t|\n");
          Serial.print("-------------------------------------------------\n");
          Serial.print("| FREQUENCIA: ");
          Serial.print(x);
          Serial.print(" Hz \t \t\t|");
          Serial.print("\n| AMPLITUDE: ");
          Serial.print(v);
          Serial.print(" db\t \t \t \t|");
          Serial.print("\n| % DE VIBRACAO (NOMINAL): ");
          Serial.print((v*100)/frequencia_caracteristica_equipamento);
          Serial.print(" %\t\t|\n");
        }
        Serial.println("-------------------------------------------------");
        Serial.println("\n\n\n");

        /* ENVIAR PARA O BANCO DE DADOS */

          
  
        /* INDICAR O FIM DA AMOSTRA ATUAL */
        
        flag = 0;
      }
    }
  }
}
