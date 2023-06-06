/*
* HID ProxPoint 
* Autor: Ricardo Sampaio, Sorocaba SP
* Email: jricksam@gmail.com
* Fone: (15)996455614
* 
* Este codigo faz a leitura dos cartoes HID mas deve-se atentar para a pinagem correta.
* Pinos do Arduino
*   2   - ProxPoint Green (DATA 0)
*   3   - ProxPoint White (DATA 1)
*   5V  - To ProxPoint Red
*   GND - To ProxPoint Black
*
* O Leitor entrega os dados no formato 26-Bit Wiegant: 
* Bit 1 = Even Parity over Bits 2 - 13
* Bits 2 - 9 = Facility Code (0 to 255) Bit 2 is MSB
* Bits 10 - 25 = ID Number (0 to 65,535) Bit 10 is MSB
* Bits 26 Odd parity over bits 14 to 25
*
* Infelismente este exemplo somente funciona com os data 0 e 1 ligados a porta 2 e 3 do arduino UNO
* Segundo este link http://arduino.cc/en/Reference/attachInterrupt nao tem como ligar em outra porta devido o uso da funcao attachInterrupt()
*/

#include "HID.h"

HID::HID() {
  cardValue = 0;
  incomingByte = 0; 
  sInputCount = 0;
  reader1 = 0;
  reader1Count = 0;
}

void HID::begin(void) {
  //isr(0);
  //isr(1);

  for(int i = 2; i<4; i++){
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH); // enable internal pull up causing a one
    digitalWrite(i, LOW); // disable internal pull up causing zero and thus an interrupt
    pinMode(i, INPUT);
    digitalWrite(i, HIGH); // enable internal pull up
  }
  delay(10);
  reader1 = 0;
  reader1Count = 0;
  sInputCount = 0;
}

/*void HID::reader1One(void) {
  reader1Count++;
  reader1 = reader1 << 1;
  reader1 |= 1;
}

void HID::reader1Zero(void) {
  reader1Count++;
  reader1 = reader1 << 1;
}*/



void HID::resetState() {
  delay(10);
  Serial.flush();
  sInputCount=0;
  reader1 = 0;
  reader1Count=0;
}

void HID::writeCardData(volatile unsigned long sendValue) {
    for (short x=CARD_LEN; x>=0; x--) {
      if ( bitRead(sendValue,x) == 1 ) {
        //digitalWrite(VX_D1, LOW);
        delayMicroseconds(34);
        //digitalWrite(VX_D1, HIGH);
      } else if ( bitRead(sendValue,x) == 0 ) {
        //digitalWrite(VX_D0, LOW);
        delayMicroseconds(34);
        //digitalWrite(VX_D0, HIGH);
      }
      delay(2);
    }
}

long HID::readCardData(unsigned long data) {
  //digitalWrite(LED_LEITOR,HIGH);
  int serialNumber = (data >> 1) & 0x3fff;
  int siteCode = (data >> 17) & 0x3ff;
  if(serialNumber != 0x3fff)
  {
    return data;
    //Serial.print("\tH: ");
    //Serial.print(data,HEX);
    //Serial.print(" SC: ");
    //Serial.print(siteCode);
    //Serial.print(" C: ");
    //Serial.println(serialNumber);
    //Serial.print("\tB: ");
    //Serial.println(data, BIN);
  }
  
  return 0;
}

long HID::readHID()
{
  long r = 0;
  if(reader1Count >= 26)
  {
    r = readCardData(reader1);
    writeCardData(reader1);
    resetState();
    //digitalWrite(LED_LEITOR,LOW);
  }
  return r;
}