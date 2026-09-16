

/*-------------------------------------------------------------------------
 * Título: Programa Robô Sumô ESP32 Etec Armando Pannunzio
 * Data: 31.10.2024
 * Arquivo: Robo_ESP32__Rev1.ino
 * Autor: Rodrigo Horikawa Watanabe
 * Rev: 1
 * 
 * Hardware: Esp32 30 pinos
 * Descrição:
 * Programa para rodar o Robô Sumo com ESP32 
 * - Controle do Driver RELE
 * - Controle de Bateria 
 * - Controle dos Leds através dos ULN2003
 * 
%------------------------------------------------------------------------*/

#include <BluetoothSerial.h>

//-------- Inicialização do Bluetooth -------------------------------------------------------------
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
 
#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif
//---- Configuração dos pinos do Driver

#define RL1 32 
#define RL2 33

#define RL3 25
#define RL4 26

//----- Configurando as saídas transistorizadas

#define J1 12  //Led - J1 - 12V
#define J2 14  //Led - J2 - 12V
#define J3 27  //Led - J3 - 5V

//----- Configurando os leds de controle

#define ledLow  2   //Led bateria LOW
#define ledHigh 4   //Led bateria HIGH
#define ledBlue 15  //Led Bluetooth Conectado

//----- Configurando o Medidor de Bateria

#define AN_Bat 34

//Conversor AD conectado no pino 7 - GPIO32 -  ADC4
//Conversor AD tipo SAR 12bits
//ADC1
//ADC - RTC
//ADC - DMA
//ADC Attenuation

//--------------------------------------

BluetoothSerial SerialBT;


char val; // variable to receive data from the serial port

int AN_Bat_Result = 0;  //Inicializando valor de Leitura
float Voltage = 0.0;

// BT: Bt_Status callback function
void Bt_Status(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) 
{
  if (event == ESP_SPP_OPEN_EVT) 
  {                                                        // BT: Checks if the SPP connection is open, the event comes// event == Client connected
    digitalWrite (ledBlue, HIGH);                                                     // BT: Turn ON the BLUE bluetooth indication LED (solid light)
  }
  else if (event == ESP_SPP_CLOSE_EVT)
  {                                                  // BT: event == Client disconnected
    digitalWrite(ledBlue, LOW);                                                       // BT: Turn OFF the BLUE bluetooth indication LED
  }
}


void setup() {

  Serial.begin(115200);
  SerialBT.begin("Marcal"); 
//  Serial.println("O dispositivo já pode ser pareado!");

  //adcAttachPin(AN_Bat);
  //analogSetClockDiv(255);
  //analogSetWidth(12);
  //analogSetAttenuation(ADC_11db);
  analogReadResolution(12);
  
  pinMode(ledLow, OUTPUT);  
  pinMode(ledHigh,OUTPUT);
  pinMode(ledBlue,OUTPUT); 
  
  pinMode(RL1, OUTPUT);  
  pinMode(RL2, OUTPUT);  
  pinMode(RL3, OUTPUT);  
  pinMode(RL4, OUTPUT);  
  
  pinMode(J1, OUTPUT);  
  pinMode(J2,OUTPUT);
  pinMode(J3,OUTPUT); 

//---- Pisca 3x o Led do Esp32

  digitalWrite(ledHigh, HIGH);  
  delay(200);
  digitalWrite(ledHigh, LOW);  
  delay(200);
  digitalWrite(ledLow, HIGH);  
  delay(200);
  digitalWrite(ledLow, LOW);  
  delay(200);
  digitalWrite(ledBlue, HIGH);  
  delay(200);
  digitalWrite(ledBlue, LOW);  
//--------------------------------------

//SerialBT.register_callback(Bt_Status);  // BT: Define o tratamento de evento Bt_Status


}

void loop() {

  AN_Bat_Result = analogRead(AN_Bat);
/*
  Voltage = AN_Bat_Result;
  Serial.print("AD: ");
  Serial.println(AN_Bat_Result);
  Serial.print("Voltage: ");
  Serial.println(Voltage/1000);
*/
  if(AN_Bat_Result<3000)
  {
    digitalWrite(ledLow, HIGH);   
    digitalWrite(ledHigh, LOW);   
  }
  else
  {
    digitalWrite(ledLow, LOW);   
    digitalWrite(ledHigh, HIGH);   
  }

  if (SerialBT.available()) {
  val =(char)SerialBT.read();
  digitalWrite (ledBlue, HIGH);
  }
  
  if( val == 'U' )             // Aciona o Transistor 2
  {
    digitalWrite(J1, HIGH); 
  } 
  if(val == 'u')
  {
    digitalWrite(J1, LOW);   
  }
  if( val == 'W' )             // Aciona o Transistor 1
  {
    digitalWrite(J2, HIGH);  
  } 
  if(val == 'w')
  {
    digitalWrite(J2, LOW);   
  }
  if( val == 'V' )             // Aciona o Transistor 1
  {
    digitalWrite(J3, HIGH);  
  } 
  if(val == 'v')
  {
    digitalWrite(J3, LOW);   
  }
  
  if( val == 'F' )           // Frente
  {
    digitalWrite(RL1, HIGH);   // otherwise turn it OFF
    digitalWrite(RL2, LOW);   // otherwise turn it OFF
    digitalWrite(RL3, HIGH);   // otherwise turn it OFF
    digitalWrite(RL4, LOW);   // otherwise turn it OFF
  } 
  if( val == 'B' )            // Tras
  {
    digitalWrite(RL1, LOW);   // otherwise turn it OFF
    digitalWrite(RL2, HIGH);   // otherwise turn it OFF
    digitalWrite(RL3, LOW);   // otherwise turn it OFF
    digitalWrite(RL4, HIGH);   // otherwise turn it OFF
  } 
  if(val == 'L')              //Esquerda
  {
    digitalWrite(RL1, HIGH);   // otherwise turn it OFF
    digitalWrite(RL2, LOW);   // otherwise turn it OFF
    digitalWrite(RL3, LOW);   // otherwise turn it OFF
    digitalWrite(RL4, HIGH);   // otherwise turn it OFF
  }

  if(val == 'R')              //Direita
  {
    digitalWrite(RL1, LOW);   // otherwise turn it OFF
    digitalWrite(RL2, HIGH);   // otherwise turn it OFF
    digitalWrite(RL3, HIGH);   // otherwise turn it OFF
    digitalWrite(RL4, LOW);   // otherwise turn it OFF
  }
  if(val == 'S')              //Stop
  {
    digitalWrite(RL1,LOW);
    digitalWrite(RL2, LOW);   // otherwise turn it OFF
    digitalWrite(RL3, LOW);   // otherwise turn it OFF
    digitalWrite(RL4,LOW);
  }
  if(val == 'G')              //Frente Esquerda
  {
    digitalWrite(RL1, HIGH);   // otherwise turn it OFF
    digitalWrite(RL2, LOW);   // otherwise turn it OFF
    digitalWrite(RL3, LOW);   // otherwise turn it OFF
    digitalWrite(RL4, LOW);   // otherwise turn it OFF
  }
  if(val == 'I')              //Frente Direita
  {
    digitalWrite(RL1,LOW);
    digitalWrite(RL2, LOW);   // otherwise turn it OFF
    digitalWrite(RL3, HIGH);   // otherwise turn it OFF
    digitalWrite(RL4,LOW);
  }
  if(val == 'H')              //Tras Esquerda
  {
    digitalWrite(RL1, LOW);   // otherwise turn it OFF
    digitalWrite(RL2, HIGH);   // otherwise turn it OFF
    digitalWrite(RL3, LOW);   // otherwise turn it OFF
    digitalWrite(RL4, LOW);   // otherwise turn it OFF
  }
  if(val == 'J')              //Tras Direita
  {
    digitalWrite(RL1,LOW);
    digitalWrite(RL2, LOW);   // otherwise turn it OFF
    digitalWrite(RL3, LOW);   // otherwise turn it OFF
    digitalWrite(RL4,HIGH);
  }

  delay(10);                    // wait 100ms for next reading
  digitalWrite (ledBlue, LOW);
}
