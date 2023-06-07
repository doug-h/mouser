# mouser

Super basic SDL2 app that sends keyboard+mouse input between devices.

** ALL DATA IS SENT UNENCRYPTED OVER UDP **

## Building
Only dependency should be SDL2.

### Linux
```
mkdir build
make
```
### Windows
```
mkdir build
build_x86.bat
```

## Use
On one device run `server`, on another run `client <ip of server>`.
May need to allow app through firewall (port 34197).

## TODO
* Implement switching between multiple clients
* Smarter network layer, right now we just spam out packets at a fixed rate.
* Holding down keys should repeat (works on Linux, but not Windows)
