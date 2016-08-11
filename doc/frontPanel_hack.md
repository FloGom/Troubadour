# Reverse Engineering the front panel

If you want to code new messages or new features for the front panel whithout knowing precisely its design, go to [How To Program the front panel](how_to_program.md)

The front panel can be detached from the case. Using a multimeter and datasheets 
I was able to reverse engineering the front panel :)

You can find the schematics and pinouts [here](../schematics). 
They are made with [Fritzing](http://fritzing.org/home/).


## 1. Wiring of the front panel

### a. The 4 digits, 7-segments display
With the schematics, we can think about the way it works. 
As you can see on the schematics, the polarity is reverse :
* the transistors used are PNP, I made a reminder of their funcioning in the lower left corner for myself. Here they are used as commutators : if the B pin is set LOW, the current pass through the transistor from E to C.
* the 7-segments display is a common anode display. To light up a segment you have to pull LOW the output pin (a, b, c...) and to pull HIGH the anode pin (here *dig1*):   
```
dig1 -------+----->|--- a   
            |
            +----->|--- b
       	    |
            +----->|--- c
            |
            .
            .
            .
            +----->|--- dot point
```    
With a PNP transistor ahead of the anode, the *dig1* must be pull LOW also to light up segments.


### b. 74HC164D chip
This chip is a shift register (serial-in, parallel-out). The bits (LOW or HIGH) sent to the serial pins (DSA and DSB) are shift to the right on each rising edge (LOW to HIGH) on the clock pulse pin.   
More details [here](../schematics/74hc164d.pdf).

The output pins are wired to the cathodes (a, b, c, ...) of the 7-segments display.

So to light up only the **a** segment of the **digit 1** we must do the following :   
* reset the 74hc164d
* set clock pulse pin to LOW
* set the serial pin to LOW
* set clock pulse pin to HIGH
* set the dig1 pin of the display to LOW

To display a "1" we send the following binary code, 0b0110 0000, to the shift register :   

1. We transform 0b0110 0000 into 0b1001 1111 with a XOR because the polarity is reversed : 0b0110 0000 ^ 0b1111 1111 = 0b1001 1111
2. the current bit to send is the LSB (**L**ess **S**ignificant **B**it) : 0b1001 111**1**
3. set clock pulse pin to LOW
4. set the serial pin to the value of the current bit (for the LSB it's HIGH)
5. set clock pulse pin to HIGH
6. we restart from step 2, but this time we take the second bit : 0b1001 11**1**1. And we go on until we reach the MSB (**M**ost **S**ignificant **B**it) 0b**1**001 1111.
7. set the dig1 pin of the display to LOW


### c. buttons
In order to lower the number of output pins for the front panel the buttons are wired to use the shift register.

The buttons are wired that way :
```
            ___        Switches
Serial in -| 7 |-Q0------__---+
  Clock p -| 4 |-Q1------__---|
           | H |-Q2------__---|
           | C |-Q3------__---+--- 7 (output pin)
           | 1 |-Q4------__---|
           | 6 |-Q5------__---|
           | 4 |-Q6------__---|
           |_D_|-Q7------__---+
                              |
                              +----VVVV--- +5V
```

The buttons are normally open and pulled HIGH.

To check if button n°1 *CH+* has been pressed, we use the shift register. The code 0b0111 1111 (reverse polarity : 0 means button pressed, 1 button released) is sent to the shift register, and the state of pin number 7 is checked. If it's LOW the button is pressed, otherwise the button is not pressed :

```
            ___ State  Switches
Serial in -| 7 |-0------------+
  Clock p -| 4 |-1-------__---|
           | H |-1-------__---|
           | C |-1-------__---+--- we read 0 (LOW) on pin 7
           | 1 |-1-------__---|
           | 6 |-1-------__---|
           | 4 |-1-------__---|
           |_D_|-1-------__---+
                              |
                              +----VVVV--- +5V
```

If we pressed another button, we read 1 (HIGH).

See [How To Program the front panel](how_to_program.md) and the source code [frontPanel.c](../frontPanelProg/frontPanel.c) for more details.