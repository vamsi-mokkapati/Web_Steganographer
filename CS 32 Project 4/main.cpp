#include "HashTable.h"
#include "provided.h"
#include "http.h"
#include <iostream>
using namespace std;

int main()
{
    string originalPageText =
    "<html>\n"
    "<head>\n"
    " <title>My Pretend Web Page</title>\n"
    "</head>\n"
    "<body>\n"
    "<h2>My Pretend Web Page<h1>\n"
    "<p>\n"
    "I wish I were creative enough to have something interesting \n"
    "to say here.  I'm not, though, so this is a boring page.\n"
    "</p>\n"
    "<p>\n"
    "Oh, well.\n"
    "</p>\n"
    "</body>\n"
    "</html>\n"
    ;
    HTTP().set("http://boring.com", originalPageText);
    string plan =
    "Lefty and Mugsy enter the bank by the back door.\n"
    "Shorty waits out front in the getaway car.\n"
    "Don't let the teller trip the silent alarm.\n"
    ;
    string newPageText;
    if ( ! WebSteg::hideMessageInPage("http://boring.com", plan, newPageText))
    {
        cout << "Error hiding!" << endl;
        return 1;
    }
    
    HTTP().set("http://boring.com", newPageText);
    string msg;
    if ( ! WebSteg::revealMessageInPage("http://boring.com", msg))
    {
        cout << "Error revealing!" << endl;
        return 1;
    }
    if (msg != plan)
    {
        cout << "Message not recovered properly:\n" << msg << endl << plan;
        return 1;
    }
    cout << "Recovered text is what was hidden:\n" << msg;
    
    
    HashTable<string, int> nameToAge(100, 200);
    nameToAge.set("Carey", 43);
    nameToAge.set("David", 97);
    nameToAge.set("Timothy", 43, true);
    nameToAge.set("Ivan", 28);
    nameToAge.set("Sally", 22);
    nameToAge.set("David", 55);
    nameToAge.touch("Carey");
    // let's discard the two least recently written items
    for (int k = 0; k < 2; k++)
    {
        string discardedName;
        int discardedAge;
        if (nameToAge.discard(discardedName,discardedAge))
            cout << "Discarded " << discardedName
            << " who was " << discardedAge
            << " years old.\n";
        else
            cout << "There are no items to discard!\n";
    }
}

