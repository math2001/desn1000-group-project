DEVICE=/dev/ttyACM0
SKETCH_NAME=$(shell basename $(shell pwd))

upload:
	arduino-cli compile --fqbn arduino:avr:uno $(SKETCH_NAME).ino
	arduino-cli upload -p $(DEVICE) --fqbn arduino:avr:uno $(SKETCH_NAME).ino

connect:
	socat $(DEVICE),b9600,raw,echo=0,crnl -