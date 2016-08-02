# Build the front panel program
The program is build directly onto the RPi. 
This guide assumes you have a RPi install with the [RetroPie](https://retropie.org.uk/) distribution.

## Download and install

### 1. Install tools   

* The programm uses [wiringPi](http://wiringpi.com).   
[Download an install](http://wiringpi.com/download-and-install/) it from sources.


### 2. Download the sources

* If you are familiar with git tools you are recommended to use it :   
`git clone https://github.com/FloGom/Troubadour.git`

* But you can also download zip archive from :   
https://github.com/FloGom/Troubadour/archive/master.zip


## Build Program

1. open terminal window.
2. move to project directory :   
`cd Troubadour/frontPanelProg`

3. compile the program :   
`gcc frontPanel.c -o frontPanel -l wiringPi`   
If the command returns nothing it's Ok. If the command returns error(s), 
search on Internet by typing it in google or better [stackoverflow](http://stackoverflow.com/).

4. test the program, launch it with :   
`sudo ./frontPanel`    
(assuming you're always in frontPanelProg directory).   
Normally you will see something like this :   
![If you are able to read GAME everything is good!](media/frontPanelOk.jpg)   
5. to launch the programm on startup before any users have logged in, you need to edit `/etc/rc.local` :   
`sudo nano /etc/rc.local`   
add the following line after the commented lines (the lines beginning with #) :   
`sudo /home/pi/Troubadour/frontPanelProg/frontPanel &`   
*The `&` means that the program will run in background.*
6. Restart your RPi, you should now see *GAME* appears on the front panel after the RetroPie splashscreen.

## Usage of the front panel

See [usage.md](usage.md) to know more about the use of the front panel.