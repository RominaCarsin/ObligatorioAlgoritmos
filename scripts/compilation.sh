
#!/bin/bash

#set -x
BUILD_FOLDER=./build
FOLDER=./

mkdir -p "$BUILD_FOLDER" 

TEST_FOLDER=./tests/ejercicio5
CPPFILE=$FOLDER/ejercicio5.cpp


echo "Compilando..."
if [ -f "$CPPFILE" ]; then
    echo "Realizado en C++"
    if ! g++ $CPPFILE -o $BUILD_FOLDER/ejercicio5.out --std=c++11; then
    echo -e "\e[31mERROR en compilacion\e[0m"
    exit 1
    fi
fi
echo "Compilado terminado" 