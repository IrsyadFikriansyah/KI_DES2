#include <algorithm>
#include <bitset>
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

string decrypt(string ct, string key) {
    // turning cipher text and key into binaries
    key = hex2bin(key);

    if (ct.length() < 64) {
        cout << "Input is less than 64 bits!" << endl;
        cout << ct << endl;
        exit(1);
    } else if (ct.length() > 64) {
        cout << "Input is more than 64 bits!" << endl;
        cout << ct << endl;
        exit(1);
    }

    // initial permutation
    ct = permute(ct, initial_perm, 64);

    // creating keys for each round (16 rounds)
    vector<string> roundKey = createRoundKey(key);
    reverse(roundKey.begin(), roundKey.end());

    // spliting cipher text
    string left = ct.substr(0, 32);
    string right = ct.substr(32);

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

        // printf("Round %3d %s %s %s\n", i + 1, bin2hex(left).c_str(),
        //    bin2hex(right).c_str(), bin2hex(roundKey[i]).c_str());
    }
    // Combination
    string combine = left + right;

    // Final permutation: final rearranging of bits to get cipher text
    string cipher_text = permute(combine, final_perm, 64);
    return cipher_text;
}

int main() {
    string key = "123456789ABCDEFF";
    string cipher_text;
    cin >> cipher_text;

    cout << endl << "Decryption" << endl;
    string plain_text = decrypt(cipher_text, key);
    cout << "Cipher text (hex)\t: " << bin2hex(cipher_text) << endl;
    cout << "plain text (bin)\t: " << plain_text << endl;
    cout << "plain text (hex)\t: " << bin2hex(plain_text) << endl;
    cout << "plain text (ascii)\t: " << bin2ascii(plain_text) << endl;
}