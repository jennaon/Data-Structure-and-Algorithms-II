#ifndef SPELLCHECK_H_INCLUDED
#define SPELLCHECK_H_INCLUDED
#include "hash.h"

class spellcheck{
    public:
    spellcheck (const std::string &dictionary = "");
    void spellchecker(const std::string & input , const std:: string & output );

    private:
    hashTable *h = new hashTable();

    /** Check whether the input character/integer is valid or not.
     * Return true if valid, false if not.
     * Valid letters: A-Z, a-z, 0-9, - and '. */
    bool isValid( const int c );
};

/*
To do:
    - Loading the dictionary should've happened outside of the constructor
    - Separate functions for isDigit(); isCapital() to simplify the code

*/

#endif // SPELLCHECK_H_INCLUDED
