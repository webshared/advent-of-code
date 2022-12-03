#!/bin/sh

set -e
g++ -O3 -o ./build/$1 ./src/$1.cpp -pthread -std=c++17 -static-libstdc++ -lboost_regex
./build/$1
