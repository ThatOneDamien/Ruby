#!/bin/bash

current_file="${PWD}/${0%/*}"

cd "${current_file}/.."

./Dependencies/premake/Linux/premake5 gmake2 --file=premake5.lua --cc=gcc
