#!/bin/bash

if [[ $UID != 0 ]]; then
    echo "Please run this script with sudo:"
    echo "sudo $0 $*"
    exit 1
fi

cp -r ./src/irrKlang-64bit-1.6.0 /usr/local/lib
echo "/usr/local/lib/irrKlang-64bit-1.6.0/bin/linux-gcc-64/" > /etc/ld.so.conf.d/irrKlang-64bit-1.6.0.conf

ldconfig

apt install gcc
apt install g++
apt install libsdl2-dev
apt install libopengl-dev
apt install libglx-dev
apt install libglm-dev
apt install libsfml-dev
apt install libglew-dev

cd ./src

make cosmo

cp ./cosmo ./../

make clean

cd ..
