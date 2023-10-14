# ipify

Test JSON and TEXT reading functions by using ipify API to return user's router's IP address.

## Building

Running make will build the application for all targets by default.

### Creating Atari ATR DISK image of application

```shell
# Atari, build and create ATR
$ make TARGETS=atari clean all dist atr
...
creating standard SD/90k image
Added file "dist/atr\ipify.com"
created image "dist/ipify.atr"
```

The generated file is at `dist/ipify.atr`

### Creating APPLE ProDos Bitsy DISK image of application

```shell
make TARGETS=apple2 clean all dist po
```

The generated file is at `dist/ipify.po`
