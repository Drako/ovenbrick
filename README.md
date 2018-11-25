# OvenBrick

![GitHub](https://img.shields.io/github/license/Drako/ovenbrick.svg)

OvenBrick is a trading card game loosely based on another popular game with a similar name.
The plan is to run it on [this thing](https://www.clockworkpi.com/), which supports a resolution of 320x240.

## Building

```bash
git clone https://github.com/Drako/ovenbrick
git submodule init
cd ovenbrick
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

The CMake command might complain about a lot of missing dependencies,
mostly things that are required for building SFML.
