DEVICE=/dev/ttyACM0
SKETCH_NAME=$(shell basename $(shell pwd))
BOARD=arduino:avr:mega

upload:
	arduino-cli compile --fqbn $(BOARD) $(SKETCH_NAME).ino
	arduino-cli upload -p $(DEVICE) --fqbn $(BOARD) $(SKETCH_NAME).ino

connect:
	socat $(DEVICE),b9600,raw,echo=0,crnl TCP:localhost:8081