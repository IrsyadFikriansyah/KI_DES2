#!/bin/bash

g++ -std=c++17 DES/des.cpp -o DES/des
cat /root/cipher_text.txt | DES/des
