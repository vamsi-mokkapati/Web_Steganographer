#include "provided.h"
#include <string>
#include <vector>
#include "HashTable.h"
#include <iostream>
using namespace std;

void Compressor::compress(const string& s, vector<unsigned short>& numbers)
{
    //initalize buckets and capacity
    unsigned int capacity = 0;
    if((s.size()/2 + 512) < 16384)
        capacity = static_cast<int>(s.size())/2 + 512;
    else
        capacity = 16384;
    unsigned int buckets = capacity*2;
    
    //initalize hash table using buckets and capacity
    HashTable<string,unsigned short> H(buckets,capacity);
    
    //add 256 associations using loop
    for(int j = 0; j < 256; j++)
    {
        string str = "";
        str += static_cast<char>(j);
        H.set(str, j, true);
    }
    
    //initalize variables
    int nextFreeID = 256;
    string runSoFar = "";
    
    //clear vector so no left over data
    numbers.clear();
    
    //make new vector to add stuff to
    vector<unsigned short> V;
    
    //for each character in string
    for(int i = 0; i < s.size(); i++)
    {
        //I followed pseudocode in spec
        string expandedRun = runSoFar + s[i];
        unsigned short value;
        
        if(H.get(expandedRun, value))
        {
            runSoFar = expandedRun;
            continue;
        }
        
        else
        {
            unsigned short x;
            H.get(runSoFar, x);
            V.push_back(x);
        }
        
        
        H.touch(runSoFar);
        runSoFar = "";
        
        unsigned short cv;
        string str = "";
        str += s[i];
        H.get(str, cv);
        V.push_back(cv);
        
        
        if(!H.isFull())
        {
            H.set(expandedRun, nextFreeID);
            nextFreeID++;
        }
        
        else
        {
            unsigned short a;
            string key;
            H.discard(key, a);
            H.set(expandedRun,a);
        }
        
    }
    
    if(!runSoFar.empty())
    {
        unsigned short x;
        H.get(runSoFar, x);
        V.push_back(x);
    }
    
    V.push_back(capacity);
    numbers = V;
}

bool Compressor::decompress(const vector<unsigned short>& numbers, string& s)
{
    //I followed psuedocode in spec
    unsigned int capacity = numbers[numbers.size()-1];
    unsigned int buckets = capacity*2;
    HashTable<unsigned short,string> H(buckets,capacity);
    
    for(int j = 0; j < 256; j++)
    {
        string str = "";
        str += static_cast<char>(j);
        H.set(j, str, true);
    }
    
    int nextFreeID = 256;
    string runSoFar = "";
    s = "";
    string output = "";
    
    for(int i = 0; i < numbers.size(); i++)
    {
        s = output;
        
        string expandedRun = "";
        
        unsigned short us = numbers[i];
        
        if(us <= 255)
        {
            string str = "";
            H.get(us,str);
            output += str;
            
            if(runSoFar.empty())
            {
                runSoFar += str;
                continue;
            }
            
            else
            {
                expandedRun = runSoFar + str;
            }
            
            if(!H.isFull())
            {
                H.set(nextFreeID,expandedRun);
                nextFreeID++;
            }
            
            else
            {
                string a;
                unsigned short key;
                H.discard(key, a);
                H.set(key, expandedRun);
            }
            
            runSoFar = "";
            continue;
            
        }
        
        else
        {
            string x;
            if(!H.get(us,x))
                return false;
            else
                H.touch(us);
            
            string str = "";
            H.get(us,str);
            output += str;
            runSoFar = str;
            
            continue;
            
        }
        
    }
    
    return true;
}