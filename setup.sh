#!/usr/bin/env bash

# NOTE - 
# Running this script spawns a child process (meaning that we won't end up with idf capabilities after the process dies)
# so use the 'source' keyword instead of './' to get setup

cd esp-idf
. ./export.sh
cd ..
