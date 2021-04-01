#!/bin/bash

./clean.sh
cmake -B ../build -S ..
make -C ../build
