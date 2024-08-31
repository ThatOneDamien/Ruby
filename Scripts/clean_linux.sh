#!/bin/bash

current_file="${PWD}/${0%/*}"

cd "${current_file}/.."

find . -type f -name 'Makefile' -print -delete
rm -rf bin

