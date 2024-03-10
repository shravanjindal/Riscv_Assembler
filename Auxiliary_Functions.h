#ifndef AUXILIARY_FUNCTIONS_H
#define AUXILIARY_FUNCTIONS_H

#include <iostream>
#include <string>
#include <bitset>
#include <algorithm>

using namespace std;

// Function to remove leading and trailing spaces from a string
const string trim(const string &str)
{
    // Find the first non-space character
    auto start = find_if_not(str.begin(), str.end(), [](unsigned char c)
                             { return isspace(c); });

    // Find the last non-space character
    auto end = find_if_not(str.rbegin(), str.rend(), [](unsigned char c)
                           { return isspace(c); })
                   .base();

    // If start and end are the same, the string is either all spaces or empty
    if (start == end)
        return "";

    // Return the substring without leading and trailing spaces
    return string(start, end);
}

// Check if string is integer
const bool isNumber(string s)
{
    for (int i = 0; i < s.length(); i++)
        if (isdigit(s[i]) == false)
            return false;
 
    return true;
}

string convertTo12BitString(int num) {
    // Determine the sign bit
    char signBit = (num < 0) ? '1' : '0';
    
    // Convert the absolute value of the number to binary
    string binaryString = bitset<11>(abs(num)).to_string();

    // Combine sign bit and binary representation
    string result = signBit + binaryString;
    
    return result;
}
#endif
