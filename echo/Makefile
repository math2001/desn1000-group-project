DEVICE=/dev/ttyACM0
SKETCH_NAME=$(shell basename $(shell pwd))

run:
	arduino-cli compile --fqbn arduino:avr:uno $(SKETCH_NAME).ino
	arduino-cli upload -p $(DEVICE) --fqbn arduino:avr:uno $(SKETCH_NAME).ino
	sleep 1
	socat $(DEVICE),b9600,raw,echo=0,crnl -