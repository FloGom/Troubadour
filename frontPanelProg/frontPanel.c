#include <stdio.h>
#include <string.h>
#include <time.h>
#include <wiringPi.h>

// Pin number declaration using Broadcom chip pin numbers
const int greenLED = 2;
const int serial = 3;	// data pin
const int cp = 4;	// clock pulse, chip on the front panel shifts right bits on rising edge
const int buttons = 17;

// the first pin for the digits, the 3 others wil be dig1 + 1, dig + 2 and dig + 3
const int dig1 = 22;

// list of character
unsigned char listChar [] = {
0b11111100, // 0
0b01100000, // 1
0b11011010, // 2
0b11110010, // 3
0b01100110, // 4
0b10110110, // 5
0b10111110, // 6
0b11100000, // 7
0b11111110, // 8
0b11110110, // 9
0b11101110, // A
0b00111110, // b
0b10011100, // C
0b01111010, // d
0b10011110, // E
0b10001110, // F
0b10111100, // G
0b00101110, // h
0b00001000, // i
0b01111000, // J
0b00101110, // k=h
0b00001100, // l = 1
0b11101100, // M = PI
0b00101010, // n
0b00111010, // o
0b11001110, // P
0b11100110, // q
0b00001010, // r
0b10110110, // S = 5
0b00011110, // t
0b00111000, // u
0b00111000, // v = u
0b01111100, // W = U
0b01101110, // X = H
0b01110110, // Y
0b11011010  // Z = 2
};

// declaration of functions
void zpe2write(unsigned char octet);
void zpe2println(char *word, char start);
void zpe2println_colon(char *word, char start, int _colon);
void zpe2reset(void);
int zpe2buttons(void);

int minutes(double _startTime, double _endTime);
int hours(int minutes);

int main(void){
    // ===== SET UP ===== //
    // set up broadcom pin numbering system
    wiringPiSetupGpio();

    pinMode(greenLED, OUTPUT);
    pinMode(serial, OUTPUT);
    pinMode(cp, OUTPUT);
    pinMode(buttons, INPUT);
    // to avoid the error 'for' loop initial declarations are only allowed in mode C11 and C99
    // char before the loop
    int i;
    for(i=0; i<4; i++){
        pinMode(dig1 + i, OUTPUT);
        // by default digit LEDs are turned off
        digitalWrite(dig1 + i, 1);
    }

    time_t startTime, nowTime;
    startTime = time(NULL);

    /*
    // test
    digitalWrite(dig1, 0);
    zpe2write(listChar[0]);
    */

    // ===== LOOP =====
    char mot[] = "game";
    const int msgLines = 3;
    // constante pour éviter des problèmes d'initialisation
    #define NUMITEMS 3
    //                      0       1       2     3
    char messages[NUMITEMS][5] = {"game", "1984", "time"};
    int currentMsg = 0;

    char command[50];

    int myButton = 0;
    int oldButton = 0;
    int evenement = 0;

    int minutesPlayed;
    int hoursPlayed;

    while(1){
        nowTime = time(NULL);

        // playing time calculation
        minutesPlayed = minutes(startTime, nowTime);
        hoursPlayed = hours(minutesPlayed);
        minutesPlayed = minutesPlayed - hoursPlayed*60;
        // fait planter le programme si on déclare char *messages[3] = { ... } en ligne 95
        sprintf(messages[2], "%02d%02d", hoursPlayed, minutesPlayed);
        if(currentMsg==2){
            zpe2println_colon(mot, 0);
        }else{
            zpe2println(mot, 0);
        }

        myButton = zpe2buttons();
        switch(evenement){
            // no button has been pressed before
            case 0: switch(myButton){
                        case 0: strcpy(mot, messages[currentMsg]);
                                break;
                        case 1: strcpy(command, "vol +");
                                system(command);
                                break;
                        case 2: strcpy(command, "vol -");
                                system(command);
                                break;
                        case 3: if(myButton != oldButton){
                                    currentMsg--;
                                }
                                if (currentMsg < 0){
                                    currentMsg = NUMITEMS - 1;
                                }
                                break;
                        case 4: if(myButton != oldButton){
                                    currentMsg++;
                                }
                                if(currentMsg > NUMITEMS - 1){
                                    currentMsg = 0;
                                }
                                break;
                        case 7: strcpy(mot, "quit");
                                // go back home to avoid colon on quit when playing time is selected before
                                currentMsg = 0;
                                evenement = 1;
                                break;
                        default: //memset(mot, myButton + '0', 4); // show button number
                                //mot[0] = mot[1] = mot[2] = mot[3]; // char considérés pas modifiables
                                break;
                        }

                    oldButton = myButton;
                   
                    break;
            // button power has been pressed
            case 1: if(myButton == 5){ // confirm button 'ok'
                        strcpy(command, "sudo halt");
                        system(command);
                    }else if(myButton == 6){ // cancel the action
                        evenement = 0;
                    }
                    break;
            default: 
                    break;
        }
    }
    return 0;
}


void zpe2write(unsigned char octet){
    //write a byte to the front panel chip
    unsigned char word = octet;
    int i;
    for(i = 0; i<8; i++){
        digitalWrite(cp, 0);
	// polarity is reverse, a XOR mask is used to revrsed it
        unsigned char lsb = (word ^ 0b11111111) & 0b00000001;
        digitalWrite(serial, lsb);
        word = word >> 1;
	// clock pulse to shift right the bit
        digitalWrite(cp, 1);
    }
}

void zpe2println(char *word, char start){
    // print a full line of text during very small time
    int i, ordChar;
    // si char ordChar on a en sortie 218, 226, ... pas du tout ce qu'on attend, vérif' codage des nombres
    // '1' = 49
    // - 'a' = -97
    // 49 - 97 = -38 => -38 +256 = 218 => bouclé

    for(i = 0; i < 4; i++){
        ordChar = word[i] - 'a' + 10;
        //printf("%i\n",ordChar);
        //delay(2000);
        if(ordChar < 0){
            ordChar = ordChar + 39;
            //printf("%i\n",ordChar);
            //delay(2000);
        }
    zpe2write(listChar[ordChar]);
    // light up the digit (reverse polarity)
    digitalWrite(dig1 + i, 0);
    delay(2);
    digitalWrite(dig1 + i, 1);
    }
}

void zpe2println_colon(char *word, char start){
    // print a full line of text during very small time
    int i, ordChar;
    // si char ordChar on a en sortie 218, 226, ... pas du tout ce qu'on attend, vérif' codage des nombres
    // '1' = 49
    // - 'a' = -97
    // 49 - 97 = -38 => -38 +256 = 218 => bouclé
    for(i = 0; i < 4; i++){
        ordChar = word[i] - 'a' + 10;
        //printf("%i\n",ordChar);
        //delay(2000);
        if(ordChar < 0){
            ordChar = ordChar + 39;
            //printf("%i\n",ordChar);
            //delay(2000);
        }
        if( i == 2){
            zpe2write(listChar[ordChar] + 1);
            //last bit of 3rd digit (number 2) is for the colon
        }else{
            zpe2write(listChar[ordChar]);
        }
        // light up the digit (reverse polarity)
        digitalWrite(dig1 + i, 0);
        delay(2);
        digitalWrite(dig1 + i, 1);
    }
}

// to reset a digit we can also turn off the digit...
void zpe2resetDigit(void){
    // reset all digit
    digitalWrite(serial, 1);
    int i;
    for(i = 0; i<9; i++){
        digitalWrite(cp, 0);
        digitalWrite(cp, 1);
    }
}

int zpe2buttons(void){
    // return the number of the pressed button, see schematic for more details
    int i;
    int buttonPressed = 0;
    // buttons order
    int listButtons [7] = {1, 2, 3, 4, 6, 5, 7};
    // we put 0 in Q0 because the buttons are normally HIGH
    // the polarity is reversed so we send :
    zpe2write(0b10000000);
    // after the zpe2write function the buffer looks like this :
    // Q0 Q1 Q2 Q3 Q4 Q5 Q6 Q7
    //  0  1  1  1  1  1  1  1
    digitalWrite(serial, 1);
    for( i=0; i<7; i++){
        digitalWrite(cp, 0);
        // when the button is pressed the pin is LOW, see schematics for more details
        if(!digitalRead(buttons)){
            buttonPressed = listButtons[i];
        }
        digitalWrite(cp, 1);
    }
    return buttonPressed;
}
//scrolling text
//int compteur = zpe2println(char *word, int compteur)
// pas malin pour gérer le temps (variable globale)
//on utilise zpe2println()
// on joue avec start
//récup la longueur du texte =>calcul
// => liste avec les longueurs (pas automatique)

int minutes(double _startTime, double _endTime){
    return((int)(_endTime - _startTime)/60);
}

int hours(int minutes){
    return((int)(minutes/60));
}
