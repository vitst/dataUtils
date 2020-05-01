#!/bin/bash python3

# control script
# using checkMesh to get a total volume

import subprocess

bashCommand = "checkMesh -constant -time 'constant' | grep 'bounding box'"
print("bash: {}".format(bashCommand), flush=True)
process = subprocess.call(bashCommand, shell=True)

print(process)

