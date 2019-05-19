TARGET      := brightkb
CC          ?= cc
INSTALL     := install
INSTALL_DIR := /usr/local/bin/
BUILD_HOST  := build_host.h

BRIGHTNESS = $(shell find /sys/class/leds/*::kbd_backlight/brightness | head -1)
MAX_BRIGHTNESS = $(shell find /sys/class/leds/*::kbd_backlight/max_brightness | head -1)

all: release

release: $(BUILD_HOST)
	$(CC) -O2 -s -o $(TARGET) $(TARGET).c

debug: $(BUILD_HOST)
	$(CC) -g -ggdb -DDEBUG -Wpedantic -Wall -Wextra -Wconversion -o $(TARGET) $(TARGET).c

$(BUILD_HOST):
	@echo "#define BRIGHTNESS \"$(BRIGHTNESS)\"" > ./$(BUILD_HOST)
	@echo "#define MAX_BRIGHTNESS \"$(MAX_BRIGHTNESS)\"" >> ./$(BUILD_HOST)
	@echo "write ./$(BUILD_HOST)"

clean:
	-rm -f ./$(BUILD_HOST)
	-rm -f ./$(TARGET)

install: release
	$(INSTALL) ./$(TARGET) $(INSTALL_DIR)

uninstall: clean
	-rm -f $(INSTALL_DIR)$(TARGET)

.PHONY : all release debug $(BUILD_HOST) clean install uninstall
