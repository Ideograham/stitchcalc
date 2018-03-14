#/bin/bash

pushd ../../build

#cl %CL_FLAGS% ..\stitchcalc\code\stitchcalc.cpp
clang++ -o ./stitchcalc ../stitchcalc/code/stitchcalc.cpp
popd
