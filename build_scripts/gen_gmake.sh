#!/bin/bash

current_file="${PWD}/${0%/*}"

cd "${current_file}/.."

./third_party/premake/Linux/premake5 gmake2