#!/bin/bash

current_file="${PWD}/${0%/*}"

# CD to Ruby main dir 
cd "${current_file}/.."

Dependencies/premake/Linux/premake5 gmake2 --file=premake5.lua --cc=gcc
