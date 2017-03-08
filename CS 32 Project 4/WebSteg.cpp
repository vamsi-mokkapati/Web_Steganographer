#include "provided.h"
#include <string>
#include <iostream>
#include "http.h"
using namespace std;

bool WebSteg::hideMessageInPage(const string& url, const string& msg, string& host)
{
    //retrieve url
    string text = "";
    if (!HTTP().get(url, text))
        return false;
    
    //use hide to compress, encode
    string content;
    Steg::hide(text, msg, content);
    
    //put result into host
    host = content;
    
    return true;
}

bool WebSteg::revealMessageInPage(const string& url, string& msg)
{
    //retrive url
    string text = "";
    if (!HTTP().get(url, text))
        return false;
    
    //use reveal to decode and decompress
    Steg::reveal(text,msg);
    
    return true;
}
