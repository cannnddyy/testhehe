#!/bin/bash
set -e

# optional: install dependencies
sudo apt update
sudo apt install -y build-essential cmake qt6-base-dev qt6-webengine-dev qt6-webchannel-dev libgl1-mesa-dev

# remove old build
rm -rf build
mkdir build
cd build

# run cmake inside cannnddyy folder
cmake ../cannnddyy
make -j$(nproc)

# run the browser
./cannnddyy
