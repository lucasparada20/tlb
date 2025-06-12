#!/bin/sh

BUILD_TYPE="Release"
CXX_FLAGS=""

if [ "$1" = "debug" ]; then
    BUILD_TYPE="Debug"
    CXX_FLAGS="-ggdb3 -fsanitize=undefined -fsanitize=address"
elif [ "$1" = "valgrind" ]; then
    BUILD_TYPE="Debug"
    CXX_FLAGS="-ggdb3"
elif [ "$1" = "release" ] || [ -z "$1" ]; then
    BUILD_TYPE="Release"
    CXX_FLAGS="-O3 -march=native -mtune=native -flto=$(nproc) -mavx2 -mfma -ffast-math -s"
fi

echo "Building the generator with configuration: ${BUILD_TYPE} and CXX flags: ${CXX_FLAGS}"

# Check if matio is installed
if ! ldconfig -p | grep -q libmatio; then
    echo "Error: libmatio not found. Please install with: sudo apt install libmatio-dev"
    exit 1
fi

# Compile and link
g++ $CXX_FLAGS \
    src_generator_datner/main.cpp \
    src_generator_datner/Load.cpp \
    src_generator_datner/RandomNumbers.cpp \
    src_generator_datner/MakeScenario.cpp \
    src_generator_datner/PrintInstance.cpp \
    -lmatio \
    -o gen_datner

# Check if binary was successfully created
if [ -f ./gen_datner ]; then
    echo "Build successful: ./gen_datner created"
    exit 0
else
    echo "Build failed: gen_datner not found"
    exit 1
fi

