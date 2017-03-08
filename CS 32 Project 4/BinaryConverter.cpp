#include "provided.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;


//compute hash for string
unsigned int computeHash(string s)
{
    unsigned int hashValue = 0;
    
    //convert characters in string to int
    for (int i = 0; i < s.length(); i++)
        hashValue += (i + 1) * s[i];
    
    return hashValue;
}

//compute hash for unsigned short
unsigned int computeHash(unsigned short s)
{
    unsigned int hashValue;
    //mod of max capacity
    hashValue = s % 16384;
    return hashValue;
}



const size_t BITS_PER_UNSIGNED_SHORT = 16;

string convertNumberToBitString(unsigned short number);
bool convertBitStringToNumber(string bitString, unsigned short& number);

string BinaryConverter::encode(const vector<unsigned short>& numbers)
{
    string bitString = ""; //initalize output
    for(int i = 0; i < numbers.size(); i++)
    {
        //convert vector int to string using given function
        string result = convertNumberToBitString(numbers[i]);
        for(int k = 0; k < result.size(); k++)
        {
            //based on value of result, add correct character to bitstring
            if(result[k] == '1')
                bitString += '\t';
            if(result[k] == '0')
                bitString += ' ';
        }
    }
    
    return bitString;
}

bool BinaryConverter::decode(const string& bitString,
                             vector<unsigned short>& numbers)
{
    //clear numbers to start with
    numbers.erase(numbers.begin(), numbers.end());
    
    // if bitstring is not corect length, return false
    if((bitString.size() % 16) != 0 || bitString.size() == 0)
        return false;
    
    unsigned short reps = bitString.size()/16; //count multiples of 16 characters
    int count = 0;
    for(int i = 0; i < reps; i++)
    {
        string newBitString = "";
        //add next 16 characters from bitstring
        for(int i = 0; i < 16; i++)
        {
            newBitString += bitString[count + i];
        }
        //increment by 16 to go to next 16 for next rep of loop
        count += 16;
        
        //change spaces and tabs to 1's and 0's
        for(int k = 0; k < newBitString.size(); k++)
        {
            if(newBitString[k] == ' ')
                newBitString[k] = '0';
            if(newBitString[k] == '\t')
                newBitString[k] = '1';
        }
        
        unsigned short value = 0;
        //convert 1's and 0's using given function, and add to vector numbers
        if((convertBitStringToNumber(newBitString, value)))
        {
            numbers.push_back(value);
        }
        else
            return false;
        
    }
    
    return true;
}

string convertNumberToBitString(unsigned short number)
{
    string result(BITS_PER_UNSIGNED_SHORT, '0');
    for (size_t k = BITS_PER_UNSIGNED_SHORT; number != 0; number /= 2)
    {
        k--;
        if (number % 2 == 1)
            result[k] = '1';
    }
    return result;
}

bool convertBitStringToNumber(string bitString, unsigned short& number)
{
    if (bitString.size() != BITS_PER_UNSIGNED_SHORT)
        return false;
    number = 0;
    for (size_t k = 0; k < bitString.size(); k++)
    {
        number *= 2;
        if (bitString[k] == '1')
            number++;
        else if (bitString[k] != '0')
            return false;
    }
    return true;
}