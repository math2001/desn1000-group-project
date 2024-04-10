# Big picture view

```
WEB INTERFACE (in your browser)
^
|
| (web socket)
|
v
./controller/server (also acts as a HTTP server)
^
|
| (TCP/IP)
|
V
socat (linux program)
^
|
| Serial
|
v
./controller/client/client.ino (arduino board)
```

## How to run

1. ./controller/server:
    ```
        controller/server $ go run main.go -public ..
    ```

2. socat

    ```
        socat /dev/ttyACM0,b9600,raw,echo=0,crnl TCP:localhost:8081
    ```

3. Open `http://localhost:8080` in your web browser (not firefox)

You should see the screen.