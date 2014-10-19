#include <iostream>
#include <fstream>
#include "spellcheck.h"
#include "hash.h"

spellcheck::spellcheck (const std::string &dictionary){
    std::ifstream dic( dictionary.data() );
    std::string temp_str;

    int c;
    bool valid;

    /** Read dictionary file, filter unnecessary words */
    if( dic.is_open() ){      //If the file is open
        while( ! dic.eof() ){   //until reaching the end of the file
            getline( dic, temp_str );   //read each line; ASSUMPTION: dictionary contains only one word per line
            valid = true;

            if( temp_str.length() > 20 ) valid = false;      //Longer than 20? Not valid, skip
            else{
                for( int i = 0; i < ( int ) temp_str.length(); i++ ){   //Check validity of each entry
                    c = int( temp_str.at(i) );

                    /** Un-capitalize everything */
                    if( c >= 'A' && c <= 'Z' ) {
                        c += 32 ;
                        temp_str.replace( i, 1, 1, ( char ) c );    //Convert to lowercase
                    } else if( ! isValid( c ) ) {
                        valid = false;
                        break;
                    }
                }
            }
            if( valid ) {
                int result;
                result = h -> insert ( temp_str );
                 //if program fails to rehash, stop taking more entries
                if( result == 2 ) break;
            }
        }
        dic.close();
    }
    else std::cout<<"Error loading dictionary";
}

bool spellcheck::isValid(const int c){
    if( ( c >= '0' && c <= '9' ) || ( c >= 'a' && c <= 'z' ) || ( c >= 'A' && c <= 'Z' ) ) return true;
    else if( c == '-' || c == '\'' ) return true;
    else return false;
}

void spellcheck::spellchecker(const std::string & input, const std::string & output){

    std::ifstream inputStream ( input.data() );
    std::ofstream outputStream( output.data() );

    std::string temp_str, headword = "";
    int c, lineNumber = 1;
    bool tooLong = false, completeWord = true;

    if( inputStream.is_open() && outputStream.is_open() ){
        while( ! inputStream.eof() ){
            c = inputStream.get();

            /** Reading in valid characters */
            if( isValid ( c ) ){

                /** Convert to lowercase */
                if( c >= 'A' && c <= 'Z' )  c += 32 ;

                temp_str += c;
                completeWord = false;

                /** Stop reading the word if word is too long */
                if ( temp_str.length() > 20 ) {
                    //For C++11, replace this line with
                    //temp_str.pop_back();
                    temp_str.erase( temp_str.length() -1 , 1 );

                    //Proceed till the end of the word
                    while( isValid ( c ) ) c = inputStream.get();
                    completeWord = true;
                    tooLong = true;
                }
                /** Completely ignore the words that contain digits */
                else if ( c >= '0'  && c <= '9' ){
                    //Proceed till the end of the word
                    while( isValid ( c ) ) c = inputStream.get();
                    //this word will be ignored
                    temp_str = "";
                    completeWord = false;
                }

            }else if ( temp_str.size() > 0 ) completeWord = true;
            else completeWord = false;

            /** Actions for each word entry */
            if( completeWord ){
                if( tooLong )  headword = "Long";
                else if( ! h -> contains ( temp_str ) )   headword = "Unknown";

                if( headword != "" ){
                    outputStream << headword << " word at line " << lineNumber;

                    if( tooLong ) {
                        outputStream << ", starts";
                        tooLong = false;
                    }
                    outputStream << ": " << temp_str << std::endl;
                    headword = "";
                }
                temp_str = "";
            }

            /** Actions for newline ("\n") character */
            if( c == 10 )   lineNumber ++;
        }
        inputStream.close();
        outputStream.close();
    } else std::cout<< " Cannot find input and/or output file " <<std::endl;
}
