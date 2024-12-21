#!/bin/bash

current_file="${PWD}/${0%/*}"

# CD to Ruby main dir 
cd "${current_file}/.."

if [[ $# == 0 ]]
then
    PREMAKE_FILE=premake5.lua
elif [[ $# == 1 ]]
then
    PREMAKE_FILE=$1
else
    echo "Usage: gen_gmake.sh [premake file]"
    exit 1
fi
Dependencies/premake/Linux/premake5 gmake2 --file=$PREMAKE_FILE --cc=gcc
