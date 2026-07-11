#!/bin/bash
tests=$(ls tests/*.c >&1) # Get the output of the command
tests=${tests//".c"/} # replacing the extension .c with empty space

for i in $tests; # get each of the executable
    do ./$i; # run the executables
done

