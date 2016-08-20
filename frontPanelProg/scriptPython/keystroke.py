import uinput
from time import sleep

device = uinput.Device([uinput.KEY_ESC])
sleep(1)
device.emit_click(uinput.KEY_ESC)
