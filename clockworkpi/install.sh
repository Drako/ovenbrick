#!/bin/bash

# figure out script position
SOURCE="${BASH_SOURCE[0]}"
while [[ -h "$SOURCE" ]]; do # resolve $SOURCE until the file is no longer a symlink
  DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null && pwd )"
  SOURCE="$(readlink "$SOURCE")"
  [[ ${SOURCE} != /* ]] && SOURCE="$DIR/$SOURCE" # if $SOURCE was a relative symlink, we need to resolve it relative to the path where the symlink file was located
done
DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null && pwd )"

# install SFML dependencies
sudo apt-get -qq update
sudo apt-get install -y libxrandr-dev libudev-dev libgl1-mesa-dev libopenal-dev libflac-dev libvorbis-dev

# checkout dependencies
cd ..
git submodule update --init

# build
cd "${DIR}"
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ../..
cmake --build . --target ovenbrick -- -j 4

# install
echo "${DIR}/build/ovenbrick" > /home/cpi/apps/launcher/Menu/GameShell/ovenbrick.sh
chmod +x /home/cpi/apps/launcher/Menu/GameShell/ovenbrick.sh
