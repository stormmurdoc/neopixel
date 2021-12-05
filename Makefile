#SHELL := /bin/bash
#PATH := /usr/local/bin:$(PATH)


all:
	platformio -f -c vim run

serial:
	source ./bin/set_my_vars.sh;platformio -f -c vim run --target upload --target monitor --environment d1_serial

monitor:
	platformio -f -c vim device monitor

ota:
	source ./bin/set_my_vars.sh;platformio -f -c vim run --target upload --environment d1_ota

otaserial:
	source ./bin/set_my_vars.sh;platformio -f -c vim run --target upload --target monitor --environment d1_ota


clean:
	platformio -f -c vim run --target clean

program:
	platformio -f -c vim run --target program

uploadfs:
	platformio -f -c vim run --target uploadfs

update:
	platformio -f -c vim update

erase:
	platformio -f -c vim run --target erase

dump:
	platformio -f -c vim run --target envdump

fhem:
	./bin/fhem_device.sh
edit:
	vim -p src/* bin/* platformio.ini
