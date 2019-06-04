/*
Simple program adjust backlight brightness of keyboard.
Thanks to everyone for their help and code examples.
Andrey Shashanov (2019)

Inspirations: xbright, author: Alex Kozadaev, https://github.com/snobb

Build:      make
Install:    sudo make install
Uninstall:  sudo make uninstall

Rule to allow users in the "video" group to set the keyboard backlight.
To do so, place a file in "/etc/udev/rules.d/90-backlight.rules" containing:

# Allow video group to control leds
KERNEL=="*::kbd_backlight", SUBSYSTEM=="leds", ACTION=="add" \
  RUN+="/bin/chgrp video %S%p/brightness", \
  RUN+="/bin/chmod g+w %S%p/brightness"

*/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "build_host.h"

#define HELP_STR "Current keyboard backlight brightness: %d\n" \
                 "Usage:\t%s [-u|-d|-s 0...%d]\n"              \
                 "\t-u <digit>\tup\n"                          \
                 "\t-d <digit>\tdown\n"                        \
                 "\t-s <digit>\tset\n"

int main(int argc, char *argv[])
{
    FILE *fp;
    int cur_value, max_value;
    char fp_buf[32];

    if ((fp = fopen(BRIGHTNESS, "rb")) == NULL)
    {
        fprintf(stderr, "%s: %s " BRIGHTNESS "\n", argv[0],
                strerror(errno));
        return EXIT_FAILURE;
    }
    setvbuf(fp, fp_buf, _IOFBF, sizeof fp_buf);
    fscanf(fp, "%d", &cur_value);
    fclose(fp);

    if ((fp = fopen(MAX_BRIGHTNESS, "rb")) == NULL)
    {
        fprintf(stderr, "%s: %s " MAX_BRIGHTNESS "\n", argv[0],
                strerror(errno));
        return EXIT_FAILURE;
    }
    setvbuf(fp, fp_buf, _IOFBF, sizeof fp_buf);
    fscanf(fp, "%d", &max_value);
    fclose(fp);

    if (argc == 3)
    {
        int offset_value, new_value;

        new_value = cur_value;

        switch (*(++argv[1]))
        {
        case 'u':
            offset_value = atoi(argv[2]);
            new_value = cur_value + offset_value;
            if (new_value > max_value)
                new_value = max_value;
            break;
        case 'd':
            offset_value = atoi(argv[2]);
            new_value = cur_value - offset_value;
            if (new_value < 0)
                new_value = 0;
            break;
        case 's':
            new_value = atoi(argv[2]);
            if (new_value < 0)
                new_value = 0;
            else if (new_value > max_value)
                new_value = max_value;
            break;
        default:
            goto help_and_exit_success;
        }

        if (new_value != cur_value)
        {
            if ((fp = fopen(BRIGHTNESS, "wb")) == NULL)
            {
                fprintf(stderr, "%s: %s " BRIGHTNESS "\n", argv[0],
                        strerror(errno));
                return EXIT_FAILURE;
            }
            setvbuf(fp, fp_buf, _IOFBF, sizeof fp_buf);
            fprintf(fp, "%d", new_value);
            fclose(fp);
        }
        return EXIT_SUCCESS;
    }

help_and_exit_success:
    printf(HELP_STR, cur_value, argv[0], max_value);

    return EXIT_SUCCESS;
}
