package main

import (
	"log"

	"github.com/tarm/serial"
)

func main() {
	c := &serial.Config{Name: "COM7", Baud: 9600}
	s, err := serial.OpenPort(c)
	if err != nil {
		log.Fatal(err)
	}

	_, err = s.Write([]byte("test"))
	if err != nil {
		log.Fatal(err)
	}

	buf := make([]byte, 128)
	n, err := s.Read(buf)
	if err != nil {
		log.Fatal(err)
	}
	log.Printf("%q", buf[:n])
}
