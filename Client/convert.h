// convert.h

#ifndef CONVERT_H
#define CONVERT_H

#include <map>
#include <string>
#include <cmath>

using namespace std;

string hex2bin(string hex) {
    map<char, string> mp = {
        {'0', "0000"}, {'1', "0001"}, {'2', "0010"}, {'3', "0011"},
        {'4', "0100"}, {'5', "0101"}, {'6', "0110"}, {'7', "0111"},
        {'8', "1000"}, {'9', "1001"}, {'A', "1010"}, {'B', "1011"},
        {'C', "1100"}, {'D', "1101"}, {'E', "1110"}, {'F', "1111"}};
    string bin = "";
    for (int i = 0; i < hex.length(); i++) bin += mp[hex[i]];
    return bin;
}

string bin2hex(string bin) {
    map<string, char> mp = {
        {"0000", '0'}, {"0001", '1'}, {"0010", '2'}, {"0011", '3'},
        {"0100", '4'}, {"0101", '5'}, {"0110", '6'}, {"0111", '7'},
        {"1000", '8'}, {"1001", '9'}, {"1010", 'A'}, {"1011", 'B'},
        {"1100", 'C'}, {"1101", 'D'}, {"1110", 'E'}, {"1111", 'F'}};
    string hex = "", bin_temp = "";
    for (int i = 0; i < bin.length(); i += 4) {
        bin_temp = "";
        bin_temp += bin[i];
        bin_temp += bin[i + 1];
        bin_temp += bin[i + 2];
        bin_temp += bin[i + 3];
        hex += mp[bin_temp];
    }
    return hex;
}

string dec2bin(int decimal, int width = 4) {
    if (decimal == 0) return string(width, '0');

    string binary = "";
    while (decimal > 0) {
        binary = to_string(decimal % 2) + binary;
        decimal /= 2;
    }

    if (binary.length() % 4 != 0) {
        int div = binary.length() / 4;
        int counter = (4 * (div + 1)) - binary.length();
        while (counter--) binary = '0' + binary;
    }
    return binary;
}

int bin2dec(string binary) {
    int decimal = 0, iter = 0;
    for (int i = binary.length() - 1; i >= 0; i--)
        decimal += int(binary[i] - '0') * pow(2, iter++);
    return decimal;
}

string ascii2hex(string ascii) {
    string hex = "";
    for (int i = 0; i < ascii.length(); i++)
        hex += bin2hex(dec2bin(int(ascii[i])));
    return hex;
}

string hex2ascii(const string& hexString) {
    string asciiString = "";

    for (size_t i = 0; i < hexString.length(); i += 2) {
        // Extract 2 characters from the hexadecimal string
        string hexByte = hexString.substr(i, 2);

        // Convert the hexadecimal byte to an integer
        int decimalValue;
        sscanf(hexByte.c_str(), "%x", &decimalValue);

        // Convert the integer to a character and append it to the ASCII string
        asciiString += static_cast<char>(decimalValue);
    }
    return asciiString;
}

string bin2ascii(string bin){
    return hex2ascii(bin2hex(bin));
}

string ascii2bin(string ascii){
    return hex2bin(ascii2hex(ascii));
}

#endif
