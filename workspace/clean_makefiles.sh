#!/bin/bash

cd ../build/
mkdir -p ../temp/resources
cp -r ./resources/* ../temp/resources
cp exec ../temp 
rm -r -f *
cp -r ../temp/* ./
rm -r -f ../temp
cd ../workspace
