PATH="~/.platformio/penv/bin/:~/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin"

default: help

help:
	@echo "Usage:"
	@echo "     make [command]"
	@echo "Available commands:"
	@grep -v '^_' Makefile | grep '^[^#[:space:]].*:' | grep -v '=' | grep -v '^default' | sed 's/:\(.*\)//' | xargs -n 1 echo ' -'

build:
	PATH=${PATH} platformio run

clean:
	PATH=${PATH} platformio run --target clean

upload:
	PATH=${PATH} platformio run --target upload --environment esp8266
