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

## How to run on linux

1. ./controller/server:
    ```
        controller/server $ go run main.go -public ..
    ```

    OR if you are on windows, open `cmd`
    ```
        C:\yadiyada\controller\server> desn1000-server.exe -public ..
    ```


2. socat

    ```
        controller/server $ socat /dev/ttyACM0,b9600,raw,echo=0,crnl TCP:localhost:8081
    ```
    On windows, you will need to [install
    socat](https://github.com/tech128/socat-1.7.3.0-windows) and replace
    `/dev/ttyACM0` with `COM4` or something like it.


3. Open `http://localhost:8080` in your web browser (not firefox)

You should see the screen.