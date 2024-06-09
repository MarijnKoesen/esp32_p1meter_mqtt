PIO_PATH="${PATH}:~/.platformio/penv/bin/:~/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin"

default: help

help:
	@echo "Usage:"
	@echo "     make [command]"
	@echo "Available commands:"
	@grep -v '^_' Makefile | grep '^[^#[:space:]].*:' | grep -v '=' | grep -v '^default' | sed 's/:\(.*\)//' | xargs -n 1 echo ' -'

build:
	PATH=${PIO_PATH} platformio run

clean:
	PATH=${PIO_PATH} platformio run --target clean

upload:
	PATH=${PIO_PATH} platformio run --target upload --environment esp32

serial-monitor:
	PATH=${PIO_PATH} platformio device monitor
