#!/bin/bash

# Compile the C++ program
g++ -std=c++17 DES/des.cpp -o DES/des

# Check if compilation was successful
if [ $? -eq 0 ]; then
    # Run the C++ program with input provided as a command-line argument
    echo $1 | ./DES/des
else
    echo "Compilation failed."
fi

# Copy the output file
scp DES/cipher_text.txt root@192.168.1.3:~/
