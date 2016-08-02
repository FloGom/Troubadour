# Troubadour
Troubadour is a RetroPie system installed in an old french TV decoder, a [Metronic Zapbox Easy 2](http://www.fnac.com/Metronic-Zapbox-Easy-2/a1663879/w-4) called MZE2 below.

**Fr**   
Troubadour est une console de jeux R�tro utilisant la distribution RetroPie, et install�e dans un ancien d�codeur TNT [Metronic Zapbox Easy 2](http://www.fnac.com/Metronic-Zapbox-Easy-2/a1663879/w-4) appel� MZE2 par la suite.
Pour plus d'infos voir [LISEZMOI.MD](LISEZMOI.MD)

![troubadour](doc/media/troubadour.png)

## Updates
* **2016/08/02** - Create this repository.

## Features
* Powered with a Raspberry Pi B.
* Power supply from the TV decoder.
* Original front panel used : volume control, display infos, shutdown button. See [doc/usage.md](doc/usage.md) for more details.
* Most of the original connectors were used.
* HDMI, Composite compatible : usable on most TV.

Rear panel of Troubadour :
![rear panel](doc/media/rearPanel.jpg)

## Internal Wiring
See [doc/wiring.md](doc/wiring.md)

## Volume control script
See [doc/vol_install.md](doc/vol_install.md)

## Build the front panel program
The program is written in C and launch on startup.   
See [doc/build.md](doc/build.md) for further details.

You can add more features by editing the frontPanel.c program, see [doc/frontPanel_hack.md](doc/frontPanel_hack.md) to learn how.

## License
[GNU GPL v3](LICENSE)