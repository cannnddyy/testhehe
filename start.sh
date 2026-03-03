#!/bin/bash
set -e

# 1. install dependencies if not already
sudo apt update
sudo apt install -y build-essential cmake qt6-base-dev qt6-webengine-dev qt6-webchannel-dev libgl1-mesa-dev

# 2. remove old build
rm -rf build

# 3. make build folder
mkdir build
cd build

# 4. generate makefiles and build
cmake ..
make -j$(nproc)

# 5. run the browser
./cannnddyy
