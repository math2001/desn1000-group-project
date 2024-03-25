package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"strconv"
	"time"

	"tinygo.org/x/bluetooth"
)

var adapter = bluetooth.DefaultAdapter

func must(msg string, err error) {
	if err != nil {
		panic(msg)
	}
}

func selectDevice() bluetooth.ScanResult {
	stdin := bufio.NewReader(os.Stdin)
	for {
		log.Printf("Scanning bluetooth devices for 1 seconds...")
		var results = make([]bluetooth.ScanResult, 0)
		var start = time.Now()
		err := adapter.Scan(func(a *bluetooth.Adapter, device bluetooth.ScanResult) {
			// assume this function doesn't get call concurrently
			if time.Since(start) > 1*time.Second {
				a.StopScan()
				return
			}
			results = append(results, device)
		})
		must("starting a scan", err)

		for i, device := range results {
			fmt.Printf("index=%d addr='%s' rssi='%d' name='%s'\n", i+1, device.Address.String(), device.RSSI, device.LocalName())
		}

		fmt.Print("Type the device index number to connect to it, or 'r' to rescan: ")
		text, err := stdin.ReadString('\n')
		must("reading stdin", err)
		if text != "r\n" {
			n, err := strconv.ParseInt(text[:len(text)-1], 10, 32)
			if err != nil {
				fmt.Printf("couldn't parse index '%s'\n", err)
			}
			if 0 <= n-1 && n-1 < int64(len(results)) {
				return results[n-1]
			}
			fmt.Println("invalid index")
		}

	}
}

func main() {
	must("Enable bluetooth low energy (BLE) stack", adapter.Enable())
	match := selectDevice()
	fmt.Println(match)
	device, err := adapter.Connect(match.Address, bluetooth.ConnectionParams{})
	must("failed to connect", err)

}
