#!/bin/bash
shopt -s nullglob
TIMEFORMAT=%R
set -o xtrace
cd ./src/build
time ./lemondb --listen test/test.query --thread 0
