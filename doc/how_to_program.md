# How to program the front panel

The program used for the front panel is written in C and the library [wiringPi](http://wiringpi.com/) is used.   

I tried to comment a lot my program in order to help anyone who wants to modify it :)

Do not pay attention to the french comments. It's "personal" comments that help me to learn C.

## How to add messages
Look for the following line, near #90 :
```c
char messages[NUMITEMS][5] =
```
Add your message at the end, no more than 4 characters and no space for the moment.   
Don't forget to change the number of items in the constant `NUMITEMS` just above.

## How to add features when a button is pressed
Button `OK` (5) and `MENU` (6) are not used, so you can add features.

To do this edit the portion of code below the second `switch(myButton){`, add `case 5:` and `case 6:`.

### Mini case-study : a mini-game
If you want for example add a mini game on the 4 digit display when you press the 'MENU' button you can do this as follow :
* When button #6 is pressed put 2 in evenement variable.
* In the first switch create the `case 2:` and put your code here for the mini game, don't forget that the loop can not be stoped during your mini-game. 
Your game can use any of the button while you are in this evenment, even those previously used.
* Don't forget to use one of them to quit the game.

## Wiring of buttons
The integers returned by the function zpe2buttons are like below :
```
                               buttons number
        ^       MENU    |            1       6 
 o    <   >             |     7    3   4  
        v       OK      |            2       5 
```

