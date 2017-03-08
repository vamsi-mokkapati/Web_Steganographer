#include "provided.h"
#include <string>
#include <iostream>
using namespace std;

bool Steg::hide(const string& hostIn, const string& msg, string& hostOut)
{
    //if hostin is empty, dont do anything
    if(hostIn.empty())
        return false;
    
    //count number of lines in hostin
    int lines = 0;
    for(int i = 0; i < hostIn.size(); i++)
    {
        if(hostIn[i] == '\n')
            lines++;
    }
    
    //compress and encode message into new string
    vector<unsigned short> v;
    Compressor::compress(msg,v);
    string str = BinaryConverter::encode(v);
    
    //count the number of characters to be in each line
    int characters = (static_cast<int>(str.length())/lines) + 1;
    
    //initialize position of the encoded string above (str)
    int pos = 0;
    
    //keep track of substrings
    int substrings = 0;
    
    //make substrings that will have (L/N) + 1 characters
    int max = str.length()%lines;
    
    
    //for each character in hostin
    for(int i = 0; i < hostIn.size(); i++)
    {
        string s = "";
        
        //get characters until \r or \n
        while(hostIn[i] != '\n' && hostIn[i] != '\r' && i < hostIn.size())
        {
            s += hostIn[i];
            i++;
        }
        
        
        //strip tabs and spaces
        if(s.length() != 0)
        {
            for(size_t j = s.length() - 1; s[j] == '\t' || s[j] == ' '; j--)
            {
                s.erase(j);
            }
        }
        
        
        //add correct number of character based on number of substring from the encoded string str
        if(substrings < max)
        {
            for(int k = 0; k < characters && pos < str.size(); k++)
            {
                s += str[pos];
                pos++;
            }
        }
        
        else
        {
            for(int k = 0; k < (characters - 1) && pos < str.size(); k++)
            {
                s += str[pos];
                pos++;
            }
        }
        
        //increment number of substrings that we have added from hostin
        substrings++;
        
        //add each substring to hostout
        hostOut += s + '\n';
        
    }
    
    return true;
}

bool Steg::reveal(const string& hostIn, string& msg)
{
    //if hostin is empty, dont do anyting
    if(hostIn.length() == 0)
        return false;
    
    string s2 = "";
    
    //go through characters in hostin
    for(int i = 0; i < hostIn.size(); i++)
    {
        string s1 = "";
        //get characters until \r or \n
        while(hostIn[i] != '\n' && hostIn[i] != '\r' && i < hostIn.size())
        {
            s1 += hostIn[i];
            i++;
        }
        
        //go to position where we want, going past all the whitespace
        size_t j;
        for( j = s1.size() - 1; s1[j] == '\t' || s1[j] == ' '; j--)
        {
        }
        
        //add to new string s2 all the characters other than whitespace at the end
        for(size_t k = j + 1; k < s1.size(); k++)
            s2 += s1[k];
        
    }
    
    //decode and decompresss into msg
    vector<unsigned short> v;
    if(!BinaryConverter::decode(s2,v))
        return false;
    Compressor::decompress(v,msg);
    
    
    
    return true;
    
}
