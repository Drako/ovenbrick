# OvenBrick

[![License](https://img.shields.io/badge/license-MIT-green.svg)](https://github.com/Drako/ovenbrick/blob/master/COPYING)
[![Build Status](https://travis-ci.org/Drako/ovenbrick.svg?branch=master)](https://travis-ci.org/Drako/ovenbrick)

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
