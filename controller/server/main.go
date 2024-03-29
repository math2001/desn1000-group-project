package main

import (
	"bufio"
	"flag"
	"fmt"
	"log"
	"net"
	"net/http"
	"sync"
	"time"

	"github.com/gorilla/websocket"
)

var www = flag.String("www", ":8080", "http service address")
var out = flag.String("out", ":8081", "other end (TCP socket)")
var public = flag.String("public", ".", "public HTML and JS")
var upgrader = websocket.Upgrader{} // use default options

type ClientState string

const (
	ClientState_WEB      ClientState = "web"
	ClientState_TERMINAL ClientState = "term"
)

/* design

When a web client connects
	- add myself to web client list

	- start an infinite goroutine which
		- read message
		- acquire list of term clients
		- send message to them all
		- release list of term clients

	- if error
		- acquire list of web clients
		- remove myself
		- release list of web clients

When a term client connects
	- add myself to the term clients list

	- start an infinite goroutine which
		- read message
			- acquire list of web clients
			- send message to them all
			- release list of web clients
		- if error
			- acquire list of term clients
			- remove myself
			- release list of term clients
*/

type WebClientID int
type TermClientID int

var webClientsId WebClientID = 0
var termClientsId TermClientID = 0

var webClients map[WebClientID]*websocket.Conn
var webClientsLock sync.Mutex
var termClients map[TermClientID]net.Conn
var termClientsLock sync.Mutex

func main() {
	flag.Parse()

	webClients = make(map[WebClientID]*websocket.Conn)
	termClients = make(map[TermClientID]net.Conn)

	http.Handle("/", http.FileServer(http.Dir(*public)))
	http.HandleFunc("/ws", func(w http.ResponseWriter, r *http.Request) {
		log.Printf("new WebSocket connection")
		c, err := upgrader.Upgrade(w, r, nil)
		if err != nil {
			log.Print("upgrade:", err)
			return
		}
		defer c.Close()

		handleWebClient(c)
	})

	server := &http.Server{
		Addr:              *www,
		ReadHeaderTimeout: 3 * time.Second,
	}

	go handleTermClients()
	err := server.ListenAndServe()
	if err != nil {
		log.Fatal("ListenAndServe: ", err)
	}
}

func handleWebClient(ws *websocket.Conn) {
	webClientsLock.Lock()
	webClientsId++
	id := webClientsId // current client's ID
	webClients[webClientsId] = ws
	webClientsLock.Unlock()

	log.Printf("new web client %d", id)

	for {
		_, msg, err := ws.ReadMessage()
		fmt.Println("got message")
		if err != nil {
			log.Printf("web client read error: %s", err)
			webClientsLock.Lock()
			delete(webClients, id)
			webClientsLock.Unlock()
			return
		}

		termClientsLock.Lock()
		for termClientId, termClient := range termClients {
			w := bufio.NewWriter(termClient)

			if _, err := w.Write(msg); err != nil {
				log.Printf("term client write error: %s", err)
				log.Printf("  => closing %s", termClient.Close())
				delete(termClients, termClientId)
			}
			if err := w.Flush(); err != nil {
				log.Printf("term client write error: %s", err)
				log.Printf("  => closing %s", termClient.Close())
				delete(termClients, termClientId)
			}
		}
		termClientsLock.Unlock()
	}
}

func handleTermClients() {
	l, err := net.Listen("tcp", *out)
	if err != nil {
		log.Fatal(err)
	}
	for {
		conn, err := l.Accept()
		if err != nil {
			log.Printf("accept: %s", err)
		}
		go handleTermClient(conn)
	}
}

func handleTermClient(conn net.Conn) {
	termClientsLock.Lock()
	termClientsId++
	id := termClientsId // current client's ID
	termClients[termClientsId] = conn
	termClientsLock.Unlock()

	log.Printf("New term client %d", id)

	r := bufio.NewReader(conn)

	for {
		msg, err := r.ReadBytes('\n')
		if err != nil {
			log.Printf("web client read error: %s", err)
			termClientsLock.Lock()
			delete(termClients, id)
			termClientsLock.Unlock()
			return
		}

		termClientsLock.Lock()
		for webClientId, webClient := range webClients {
			if err := webClient.WriteMessage(websocket.TextMessage, msg); err != nil {
				log.Printf("term client write error: %s", err)
				log.Printf("  => closing %s", webClient.Close())
				delete(webClients, webClientId)
			}
		}
		termClientsLock.Unlock()
	}
}
