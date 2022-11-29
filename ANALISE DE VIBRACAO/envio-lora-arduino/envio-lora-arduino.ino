#include <Wire.h>
#include <LoRa.h>
#include <SPI.h>

#include "arduinoFFT.h"

/* VARIAVEIS PARA CONEXAO LORA */
#define ss 10
#define rst 0
#define dio0 4

/* VARIAVEIS PARA COLETA DA VIBRACAO */
// Endereco I2C do sensor MPU-6050
const int MPU = 0x68;

// Variaveis para armazenar valores do sensor
float AccX, AccY, AccZ, Temp;

//const int samples = 4096; // (2^12) tamanho máximo pela capacidade do arduino nano (armazenar dados)
// /*amostras de teste para poder comparar o valor do nano com o esp (e ver se tá passando certo).*/
const int samples = 128; // (2^7) tamanho máximo pela capacidade do arduino nano (armazenar dados), maximo para processamento da fft no nano
const int samplingFrequency = 10000;  
unsigned int sampling_period_us;
unsigned long microseconds;
  
arduinoFFT FFT = arduinoFFT();
double vImag[samples];

/* GUARDAR OS VALORES LIDOS PELO SENSOR e FAZER CONVERSOES */
double vReal[samples];
int aux_envio;

void setup() {
  Serial.begin(230400);

  // Inicializa o MPU-6050
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); //registrador 107, Power Management 1 (PWR_MGMT_1)
  Wire.write(0b00000000);
  Wire.endTransmission(true);

  // Configura Acelerometro
  Wire.beginTransmission(MPU);
  Wire.write(0x1C); //registrador (28) do acelerometro
  Wire.write(0b00011000); //sensibilidade
  Wire.endTransmission();

//  Serial.print("Dispositivo configurado para ENVIO de dados.");
  
  LoRa.setPins(ss, rst, dio0);
  while (!LoRa.begin(433E6))
  {
    Serial.println(".");
    delay(500);
  }

  LoRa.setSyncWord(0xA5);
//  Serial.println("LoRa conectado.");

  sampling_period_us = round(1000000 * (1.0 / samplingFrequency));
  while(!Serial);
}

void loop() {
  
  for (int i = 0; i < samples; i++) {

    microseconds = micros();

    Wire.beginTransmission(MPU);
    Wire.write(0x3B); //registrador 59, medicao da aceleracao com 2 bytes
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 14, true); // Solicita os dados ao sensor

    // Armazena o valor dos sensores nas variaveis correspondentes
    AccX = Wire.read() << 8 | Wire.read(); //0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
    AccY = Wire.read() << 8 | Wire.read(); //0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
    AccZ = Wire.read() << 8 | Wire.read(); //0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
    Temp = Wire.read() << 8 | Wire.read(); //0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  
//    vReal[i] = AccX / 2048 - 1; //1 do offset
//    vReal[i] = AccY / 2048; //1 do offset
    vReal[i] = AccZ / 2048-1.06; //+0.95 do offset para o sinal puro
    vImag[i] = 0;
        
    LoRa.beginPacket();
    LoRa.print(vReal[i]);
    LoRa.endPacket();

    Serial.println(vReal[i]);
    
    while (micros() - microseconds < sampling_period_us) {
      //empty loop
    }
    microseconds += sampling_period_us;
  }

  /* INDICAR FIM DA AMOSTRAGEM */
  LoRa.beginPacket();
  LoRa.print(999);
  LoRa.endPacket();

  /* TOMAR A FFT */
  FFT.Windowing(vReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImag, samples, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, samples);

  /* MENSURAR AS VARIACOES */
//  double x;
//  double v;
//  
//  FFT.MajorPeak(vReal, samples, samplingFrequency, &x, &v);
//  
//  Serial.print("\n Frequencia: ");
//  Serial.print(x, 6);
//  Serial.print(", ");
//  Serial.print("Amplitude: ");
//  Serial.println(v, 6);

    /* EXECUCAO UNICA */
//  while(1);

    /* EXECUCAO SEQUENCIAL */
  delay(5000);
}
