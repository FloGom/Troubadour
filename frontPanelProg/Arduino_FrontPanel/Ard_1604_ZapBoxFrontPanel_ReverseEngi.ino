#include <IRremote.h>
#include <IRremoteInt.h>
// https://github.com/z3t0/Arduino-IRremote
// http://www.sbprojects.com/knowledge/ir/index.php
/* ZapBox Easy 2 - front panel - reverse engineering

    Florentin SALOMEZ - 13 Juillet 2016
*/
// IR Libraries

// on envoie le LSB d'abord
//            x Q0 Q1 Q2 Q3 Q4 Q5 Q6 Q7
byte nombre =  B00110000;  // écrire un 1
byte masque =  B00000001;
byte nbWrite = B00000000;
byte listNombres [10]= {
  B1111110,
  B0110000,
  B1101101,
  B1111001,
  B0110011,
  B1011011,
  B1011111,
  B1110000,
  B1111111,
  B1111101
};

byte button = B00000000;

void setup() {

  // pin for each digit
  // set HIGH in order to disabled each one
  for(byte i = 8; i<12; i++){
   pinMode(i, OUTPUT);
   digitalWrite(i, HIGH); 
  }

  pinMode(3, INPUT);  // boutons
  
  pinMode(4, OUTPUT); // LED verte
  digitalWrite(4, HIGH);
  pinMode(5, OUTPUT); // serial in
  pinMode(6, OUTPUT); // clock signal
  digitalWrite(6, LOW);

  digitalWrite(8, LOW);

  for (byte i = 0; i < 7; i++) {
    digitalWrite(6, LOW);
    //delay(500);
    nbWrite = (nombre ^ B11111111) & masque;
    digitalWrite(5, nbWrite);
    nombre = nombre >> 1;
    digitalWrite(6, HIGH);

  }

  digitalWrite(4,LOW); //ligth up the green led
  
  delay(1000);
  nombre = B1101101;
  for (byte i = 0; i < 7; i++) {
    digitalWrite(6, LOW);
    //delay(500);
    nbWrite = (nombre ^ B11111111) & masque;
    digitalWrite(5, nbWrite);
    nombre = nombre >> 1;
    digitalWrite(6, HIGH);
  }

  delay(1000);
  zpe2PrintDigit(3);

  digitalWrite(8, HIGH);
}

byte compteur = 0;


void loop() {
  // we send the bits
  zpe2PrintDigit(button);
  // we activate one digit
  digitalWrite(8 + compteur%4, LOW);
  //make a pause of some ms in order to see the light of the LED
  delay(1);
  // we deactivate the digit
  digitalWrite(8 + compteur%4, HIGH);
  compteur++;

  //test for button pressed
  button = zpe2Button();
}


void zpe2PrintDigit(byte nb){
  zpe2Write(listNombres[nb]);
}

void zpe2Write(byte _word){
  byte nombre = _word;
  for (byte i = 0; i < 7; i++) {
    digitalWrite(6, LOW);
    //delay(500);
    // with ^ (XOR) we change 0 to 1 and 1 to 0
    // with a mask B0000 0001 we keep the last bit
    nbWrite = (nombre ^ B11111111) & masque;
    digitalWrite(5, nbWrite);
    nombre = nombre >> 1;
    digitalWrite(6, HIGH);
  }
}

byte zpe2Button(){
  byte buttonPressed = 0;
  // buttons order
  byte buttons[7]= {1, 2, 3, 4, 6, 5, 7};
  // on met un 1 dans Q0
  zpe2Write(B01111111);
  digitalWrite(5, HIGH);
  // on mesure la broche 7 
  for(byte i = 0; i<7; i++){
    // set the clock pin to LOW
    digitalWrite(6, LOW);
    // if a button is pressed
    if(!digitalRead(3)){
      buttonPressed = buttons[i];
    }
    // set the clock pin to HIGH, doing so make the 1 to shift right
    digitalWrite(6, HIGH);
  }
  return buttonPressed;
}

