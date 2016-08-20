import RPi.GPIO as GPIO
import os
from time import sleep


debug = 0

# === Set Up === #

# disable the warnings for testing purpose
GPIO.setwarnings(False)

# on utilise la numerotation de la puce BCM
# on retrouve ce numero sur le pinOut du Pi
GPIO.setmode(GPIO.BCM)

# set pins for green LED, Serial Out, clock pulse and buttons
greenLED = 2
serial = 3
cp = 4
buttons = 17

GPIO.setup(greenLED, GPIO.OUT, initial = GPIO.LOW)
GPIO.setup(serial, GPIO.OUT, initial = GPIO.LOW)
GPIO.setup(cp, GPIO.OUT, initial = GPIO.LOW)
GPIO.setup(buttons, GPIO.IN)

# set pins for the 4 *  7 segments display
# LOW = enabled, HIGH = disabled
dig1 = 22

for i in range(dig1, dig1 + 4):
    GPIO.setup(i, GPIO.OUT, initial = GPIO.HIGH)

if debug:
    print("Setup done")

# === Global Variables === #
# Q0 Q1 Q2 Q3 Q4 Q5 Q6 Q7
#  a  b  c  d  e  f  g  colon

listCarac =    [0b11111100, # 0
		0b01100000, # 1
		0b11011010, # 2
		0b11110010, # 3
		0b01100110, # 4
		0b10110110, # 5
		0b10111110, # 6
		0b11100000, # 7
		0b11111110, # 8
		0b11110110, # 9
		0b11101110, # A
		0b00111110, # b
		0b10011100, # C
		0b01111010, # d
		0b10011110, # E
		0b10001110, # F
		0b10111100, # G
		0b00101110, # h
		0b00001000, # i
		0b01111000, # J
		0b00101110, # k=h
		0b00001100, # l = 1
		0b11101100, # M = PI
		0b00101010, # n
		0b00111010, # o
		0b11001110, # P
		0b11100110, # q
		0b00001010, # r
		0b10110110, # S = 5
		0b00011110, # t
		0b00111000, # u
		0b00111000, # v = u
		0b01111100, # W = U
		0b01101110, # X = H
		0b01110110, # Y
		0b11011010  # Z = 2
]


# === Function === #

def zpe2write(octet):
    "send a word e. g. 0b01001000 to the front panel using serial pin and cp pin"
    word = octet
    for i in range(8):
	# print(word)
        GPIO.output(cp, GPIO.LOW)
        lastBit = (word ^ 0b11111111) & 0b00000001
	# print(lastBit)
        GPIO.output(serial, lastBit)
        word = word >> 1
        # the bit is shift to the right when there is a rising edge on cp pin
        GPIO.output(cp, GPIO.HIGH)


def zpe2print(digit):
    "print a digit i. e. number between 0 and 9"
    zpe2write(listCarac[digit])

def zpe2printLine(word):
    "Print a full line of text during a very small time < 0.005, need to be repeated in a loop"
    for i in range(4):
        ordCarac = ord(word[i]) - ord('a') + 10
        if ordCarac < 0:
        # if the character to be displayed is a number
            ordCarac = ordCarac + 39 # that way '0' = 0
        # send the character to the front panel
        zpe2print(ordCarac)
        # select the digit to light up
        GPIO.output(dig1 + i, 0)
        sleep(0.001) # a small pause in order to see it
        GPIO.output(dig1 + i, 1)

def zpe2button():
    buttonPressed = 0
    # button order
    listButtons = [1, 2, 3, 4, 6, 5, 7]
    # we put 0 in Q0
    # because buttons are normally HIGH, due to a pull up resistor of 4,7kOhm between Vcc(1) and buttons(7)
    zpe2write(0b10000000) # don't forget the polarity is reverse
    # after the zpe2write function the buffer looks like this :
    # Q0 Q1 Q2 Q3 Q4 Q5 Q6 Q7
    #  0  1  1  1  1  1  1  1
    GPIO.output(serial, 1) # that way 1 appear when the  shift to the right is done
    for i in range(7):
        GPIO.output(cp, 0)
        # when the button is pressed the pin is LOW, see schematics for more details
        if not(GPIO.input(buttons)):
            buttonPressed = listButtons[i]
        GPIO.output(cp, 1)
    return buttonPressed

# === Main Loop === #

# test code : it displays all the alphanumeric character
#GPIO.output(dig1, GPIO.LOW)
#zpe2write(listCarac[ord('c')-ord('a')+10])
#for i in range(26):
#    zpe2write(listCarac[i + 10])
#    sleep(2)

#for i in range(4):
#    word = "lipo"
#    GPIO.output(dig1 + i, 0)
#    zpe2write(listCarac[ord(word[i])-ord('a')+10])
#    sleep(1)
#    GPIO.output(dig1 + i, 1)


if __name__ == '__main__':

    mot = 'game'
    buttonCheck = 0

    while True:
        # display the 'mot'
        zpe2printLine(mot)
        # check the buttons
        buttonCheck = zpe2button()
        if buttonCheck == 0:
            mot = 'game'
        elif buttonCheck == 1:
            os.system('vol -')
        elif buttonCheck == 2:
            os.system('vol +')
        elif buttonCheck == 7:
            os.system('sudo halt')
        else:
            mot = str(buttonCheck)*4
