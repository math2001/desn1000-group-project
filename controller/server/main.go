package main

import (
	"bytes"
	"flag"
	"log"
	"net/http"
	"sync"
	"time"

	"github.com/gorilla/websocket"
)

var addr = flag.String("addr", ":8080", "http service address")
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
var termClients map[TermClientID]*websocket.Conn
var termClientsLock sync.Mutex

func main() {
	flag.Parse()

	webClients = make(map[WebClientID]*websocket.Conn)
	termClients = make(map[TermClientID]*websocket.Conn)

	http.Handle("/", http.FileServer(http.Dir(*public)))
	http.HandleFunc("/ws", func(w http.ResponseWriter, r *http.Request) {
		c, err := upgrader.Upgrade(w, r, nil)
		if err != nil {
			log.Print("upgrade:", err)
			return
		}
		defer c.Close()

		_, message, err := c.ReadMessage()
		if err != nil {
			log.Println("read:", err)
			return
		}
		if bytes.Equal(message, []byte("web")) {
			log.Println("new web client")
			handleWebClient(c)
		} else if bytes.Equal(message, []byte("terminal")) {
			log.Println("new terminal client")
			handleTermClient(c)
		} else {
			c.WriteMessage(websocket.TextMessage, []byte("unknown client state"))
			log.Println("invalid new client, closed connection")
			return
		}
	})

	server := &http.Server{
		Addr:              *addr,
		ReadHeaderTimeout: 3 * time.Second,
	}

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
		mt, msg, err := ws.ReadMessage()
		if err != nil {
			log.Printf("web client read error: %s", err)
			webClientsLock.Lock()
			webClients[id] = ws
			webClientsLock.Unlock()
			return
		}

		termClientsLock.Lock()
		for termClientId, termClient := range termClients {
			if err := termClient.WriteMessage(mt, msg); err != nil {
				log.Printf("term client write error: %s", err)
				log.Printf("  => closing %s", termClient.Close())
				delete(termClients, termClientId)
			}
		}
		termClientsLock.Unlock()
	}
}

func handleTermClient(ws *websocket.Conn) {
	termClientsLock.Lock()
	termClientsId++
	id := termClientsId // current client's ID
	termClients[termClientsId] = ws
	termClientsLock.Unlock()

	log.Printf("New term client %d", id)

	for {
		mt, msg, err := ws.ReadMessage()
		if err != nil {
			log.Printf("web client read error: %s", err)
			termClientsLock.Lock()
			termClients[id] = ws
			termClientsLock.Unlock()
			return
		}

		termClientsLock.Lock()
		for webClientId, webClient := range webClients {
			if err := webClient.WriteMessage(mt, msg); err != nil {
				log.Printf("term client write error: %s", err)
				log.Printf("  => closing %s", webClient.Close())
				delete(webClients, webClientId)
			}
		}
		termClientsLock.Unlock()
	}
}
