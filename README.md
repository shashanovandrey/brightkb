# brightkb
Simple program adjust backlight brightness of keyboard (only with 2.6+ kernels)

Inspirations: xbright. Author: Alex Kozadaev https://github.com/snobb

## Usage:
Rule to allow users in the "video" group to set the keyboard backlight.

To do so, place a file in `/etc/udev/rules.d/90-backlight.rules` containing:
```
KERNEL=="*::kbd_backlight", SUBSYSTEM=="leds", ACTION=="add", \
  RUN+="/bin/chgrp video %S%p/brightness", \
  RUN+="/bin/chmod g+w %S%p/brightness"
```
```
$ brightkb [-u|-d|-s <digit>]
  -u <digit>  up
  -d <digit>  down
  -s <digit>  set
```

## Installation:
The program is auto-configured during make
```
$ make && sudo make install
```

## Uninstallation:
```
$ sudo make uninstall
```
