#!/bin/bash

START_DIR=$PWD
LIBDAISY_DIR=$PWD/libDaisy
DAISYSP_DIR=$PWD/DaisySP
CRLIB_DIR=$PWD/seed/CrLib

echo "building libDaisy . . ."
cd "$LIBDAISY_DIR" ; make -s clean ; make -j -s
if [ $? -ne 0 ]
then
    echo "Failed to compile libDaisy"
    exit 1
fi
echo "done."

echo "building DaisySP . . ."
cd "$DAISYSP_DIR" ; make -s clean ; make -j -s
if [ $? -ne 0 ]
then
    echo "Failed to compile DaisySP"
    exit 1
fi
echo "done."

echo "building CrLib . . ."
cd "$CRLIB_DIR" ; make -s clean ; make -j -s
if [ $? -ne 0 ]
then
    echo "Failed to compile CrLib"
    exit 1
fi
echo "done."

