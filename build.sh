#!/bin/sh

# Instal all the dependencies
sudo apt install build-essential cmake libgtest-dev

# Set-up Google Test framework
mkdir .gtest
cd .gtest
cmake /usr/src/gtest
make
sudo cp *.a /usr/lib
cd ..

# Build XeEngine
cmake .
make all

# Run tests
ctest

