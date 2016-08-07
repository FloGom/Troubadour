# Reverse Engineering the front panel

If you want to code new messages  new features for the front panel whithout knowing precisely its design, go to [How To Program the front panel](how_to_program.md)

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
This chip is a shift register (serial-in, parallel-out). The bits (LOW or HIGH) sent to the serial pins (DSA and DSB) are shift to the right on each rising edge on the clock pulse pin.   
More details [here](../schematics/74hc164d.pdf).

The output pins are wired to the cathodes (a, b, c, ...) of the 7-segments display.

So to light up only the **a** segment of the **digit 1** we must do the following :
* reset the 74hc164d
* set clock pusle pin to LOW
* set the serial pin to LOW
* set clock pulse pin to HIGH
* set the dig1 pin of the display to LOW

To display a "1" we send the following binary code, 0b0110 0000, to the shift register :
1. the current bit to send is the LSB (**L**ess **S**ignificant **B**it) : 0b0110 000**0**
2. set clock pusle pin to LOW
3. set the serial pin to the value of the current bit
4. set clock pulse pin to HIGH
5. we restart from step 1, but this time we take the second bit : 0b0110 00**0**0. And we go on that way until the MSB (**M**ost **S**ignificant **B**it) 0b**0**110 0000.
6. set the dig1 pin of the display to LOW


### c. buttons
In order to lower the number of output pins for the front panel the buttons are wired to use the available components.

To check if button n°1 *CH+* has been pressed, we use the shift register. The code 0b0111 1111 (reverse polarity) is sent to the shift register, and the state of pin number 7 is checked. If it's LOW the button is pressed, otherwise the button is not pressed.