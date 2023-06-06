#ifndef HID_H
#define HID_H
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#define CARD_LEN 26

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

class HID {
 private:
   unsigned long cardValue;
   int incomingByte; 
   int sInputCount;
 public:
  HID();
  volatile unsigned long reader1;
  volatile int reader1Count;
  void begin(void);
  //void reader1One(void);
  //void reader1Zero(void);
  //void isr(int action);
  void resetState();
  void writeCardData(volatile unsigned long sendValue);
  long readCardData(unsigned long data);
  long readHID();
  //void process(void);
};
#endif
