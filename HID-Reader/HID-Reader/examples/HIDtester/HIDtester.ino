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
HID hid;

void setup()
{
  isr(0);
  isr(1);
  hid.begin(); 
  
  Serial.begin(9600);
  Serial.println("Passe o cartao leitor:");
}

void loop() {
  long data = hid.readHID();
  if(data!=0)
  {
    Serial.println(data, HEX);
  }
}

void reader1One(void) {
  hid.reader1Count++;
  hid.reader1 = hid.reader1 << 1;
  hid.reader1 |= 1;
}

void reader1Zero(void) {
  hid.reader1Count++;
  hid.reader1 = hid.reader1 << 1;
}

void isr(int action) {
  if ( action == 1 ) { 
    // Start
      attachInterrupt(0, reader1Zero, FALLING);//DATA0 to pin 2
      attachInterrupt(1, reader1One, FALLING); //DATA1 to pin 3
      delay(10); 
  } else { 
    // Stop
      detachInterrupt(0);
      detachInterrupt(1);
  }     
}