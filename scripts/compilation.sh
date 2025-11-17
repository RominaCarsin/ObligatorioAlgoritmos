
#!/bin/bash

#set -x
BUILD_FOLDER=./build
FOLDER=./

mkdir -p "$BUILD_FOLDER" 

TEST_FOLDER=./tests/ejercicio8
CPPFILE=$FOLDER/ejercicio8.cpp


echo "Compilando..."
if [ -f "$CPPFILE" ]; then
    echo "Realizado en C++"
    if ! g++ $CPPFILE -o $BUILD_FOLDER/ejercicio8.out --std=c++11; then
    echo -e "\e[31mERROR en compilacion\e[0m"
    exit 1
    fi
fi
echo "Compilado terminado" 