#!/bin/bash

build_type=Debug

usage() {
    echo "Usage: $0 [-r -h]"
    echo
    echo "  -r : Build release"
    echo "  -h : Display this message"
}

while getopts 'rh' flag; do
    case "${flag}" in
        r) build_type=Release ;;
        h) usage
        exit 1 ;;
    esac
done

rm -rf build/*
cmake -S . -B build -DCMAKE_BUILD_TYPE=$build_type
cmake --build build
