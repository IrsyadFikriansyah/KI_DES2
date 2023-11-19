#include <algorithm>
#include <bitset>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "convert.h"
#include "des_table.h"

using namespace std;

string permute(string s, vector<int> table, int size) {
    string permutation = "";
    for (int i = 0; i < size; i++) permutation += s[table[i] - 1];
    return permutation;
}

string circularLeftShift(string value, int shiftAmount) {
    string result = "";
    for (int i = 0; i < shiftAmount; i++) {
        for (int j = 1; j < value.length(); j++) result += value[j];
        result += value[0];
        value = result;
        result = "";
    }
    return value;
}

vector<string> createRoundKey(string key) {
    // getting 56 bit key from 64 bit using the parity bits
    key = permute(key, keyp, 56);

    vector<string> roundKeys;
    string left = key.substr(0, 28);
    string right = key.substr(28);
    for (int i = 0; i < 16; i++) {
        // shifting key
        left = circularLeftShift(left, shift_table[i]);
        right = circularLeftShift(right, shift_table[i]);
        string combineString = left + right;

        // getting 56 bit key from 64 bit using the parity bits
        string roundKey = permute(combineString, key_comp, 48);
        roundKeys.push_back(roundKey);
    }
    return roundKeys;
}

string exclusivelyOR(string a, string b) {
    // if the length are different
    string ans = "";
    int maxLen = max(a.length(), b.length());
    for (int i = a.length(); i < maxLen; i++) a = "0" + a;
    for (int i = b.length(); i < maxLen; i++) b = "0" + b;

    // xor
    for (int i = 0; i < maxLen; i++) {
        if (a[i] != b[i])
            ans += "1";
        else
            ans += "0";
    }
    return ans;
}

string encrypt(string pt, string key) {
    // turning plain text and key into binaries
    pt = hex2bin(ascii2hex(pt));
    key = hex2bin(key);

    if (pt.length() < 64) {
        cout << "Input is less than 64 bits!" << endl;
        exit(1);
    } else if (pt.length() > 64) {
        cout << "Input is more than 64 bits!" << endl;
        exit(1);
    }

    // initial permutation
    pt = permute(pt, initial_perm, 64);

    // creating keys for each round (16 rounds)
    vector<string> roundKey = createRoundKey(key);

    // spliting plain text
    string left = pt.substr(0, 32);
    string right = pt.substr(32);

    // round
    for (int i = 0; i < 16; i++) {
        // expanding right from 32 into 48 bits
        string rightExpanded = permute(right, exp_d, 48);

        // XOR with key
        string xorResult = exclusivelyOR(rightExpanded, roundKey[i]);

        // Keyed Substitution (8 S-Boxes)
        string sBoxResult = "";
        for (int j = 0; j < 8; j++) {
            string row = "", col = "";

            row += xorResult[j * 6];
            row += xorResult[j * 6 + 5];

            col += xorResult[j * 6 + 1];
            col += xorResult[j * 6 + 2];
            col += xorResult[j * 6 + 3];
            col += xorResult[j * 6 + 4];

            int val = sbox[j][bin2dec(row)][bin2dec(col)];
            sBoxResult += dec2bin(val);
        }

        // Straight Permutation
        sBoxResult = permute(sBoxResult, per, 32);

        // XOR left and modified right (sBoxResult)
        string modifiedRight = exclusivelyOR(left, sBoxResult);
        left = modifiedRight;

        // Swapper
        if (i != 15) swap(left, right);
    }
    // Combination
    string combine = left + right;

    // Final permutation: final rearranging of bits to get cipher text
    string cipher_text = permute(combine, final_perm, 64);
    return cipher_text;
}

int main() {
    string key = "123456789ABCDEFF";
    string pt;

    cin >> pt;

    cout << "pt : " << pt << endl;
    cout << "len : " << pt.size() << endl;
    string cipher_text = encrypt(pt, key);
    cout << "Cipher text (bin)\t: " << cipher_text << endl;

    FILE *fptr = fopen("/root/DES/cipher_text.txt", "w"); 
    fprintf(fptr,"%s", cipher_text.c_str()); 
    fclose(fptr);
}