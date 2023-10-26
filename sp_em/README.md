# lobbyq

Tests AppleWin emulator with SmartPort / Fujinet bridge

## Building

Running make will build the application for all targets by default.

### Creating APPLE ProDos Bitsy DISK image of application

```shell
make TARGETS=apple2 clean all dist po
```

The generated file is at `dist/sp_em.po`
