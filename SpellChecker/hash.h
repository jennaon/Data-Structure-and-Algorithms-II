#include <iostream>
#include <string>
#include "hash.h"

hashTable::hashTable( int size ){
    capacity = getPrime( size );
    filled = 0;
    data.assign( capacity , hashItem() );
}

int hashTable::insert( const std::string &key, void *pv ){

    /** Rehash the entire table if filled more than half */
    if( filled > capacity / 2 ) {
        bool rehashResult = rehash ();
        if( ! rehashResult ) {
            std::cout << " Fail to rehash, dictionary source is too large" <<std::endl;
            return 2;
        }
    }
    hashItem * newItem = new hashItem(key, true, false, NULL);
    bool isDuplicate = false;
    int position = hash( key );

    /** Single-chaining; Proceed until finding the empty slot
      * If duplicate is found, terminate */
    do{
        if( position == data.size() ) position = position % data.size();
        if( data.at( position ).key == key ) {
            isDuplicate = true;
            break;
        }
        position++;

    /** Search until find the empty slot */
    }while ( data.at( position -1 ).isOccupied == true);

    /** Insert the new Item when the item isn't a duplicate */
    if( ! isDuplicate ) {
        data.at( position -1 ) = *newItem;
        filled ++;
        return 0;
    }else return 1;
}

bool hashTable::contains(const std::string &key){
    if( findPos( key ) == -1 ) return false;
    else return true;
}

//not due for Project #1
void * hashTable::getPointer(const std::string &key, bool *b){
    return NULL;
}

//not due for Project #1
int hashTable::setPointer(const std::string &key, void *pv){
    return 1;
}

//not due for Project #1
bool hashTable::remove(const std::string &key){
    return false;
}

/** Initialization of private methods and variables */
hashTable::hashItem::hashItem(const std::string &key, \
                              bool isOccupied, bool isDeleted, void *pv): \
                              key(key), isOccupied(isOccupied), \
                              isDeleted(isDeleted), pv(pv){}

int hashTable::hash(const std::string &key ){
    int hashIndex = 0;

    for(int i = 0; i < key.length (); i++ )  hashIndex = 11 * hashIndex + key[i];

    hashIndex %= capacity;
    if( hashIndex < 0 ) hashIndex += capacity;

    return hashIndex;
}

int hashTable::findPos( const std::string &key ){
    if ( filled == 0 ) return -1;
    int position = hash ( key );
    while( data.at( position ).isOccupied == true ) {
        if( data.at ( position ).key . compare( key ) == 0 ) return position; //string compare
        position ++;
    }
    return -1;
}

bool hashTable::rehash(){
    capacity = getPrime( capacity + 1 );

    if( capacity == 0 ){
        capacity = data.size();
        return false;
    }else{
        std::vector<hashItem> copyData = data;
        data.assign( capacity ,  hashItem() );

        int rehashResult = 0;
        for( std::vector <hashItem> ::iterator itr = copyData.begin(); itr != copyData.end(); ++itr ){
            if ( itr -> isOccupied ) rehashResult += insert ( itr -> key ) ;
        }
        if( rehashResult != 0 ) return false;
        else return true;
    }
}

unsigned int hashTable::getPrime(int size){
    static unsigned int primeList [] = { 49157, 98317, 196613, 393241, 786433, 1572869, 3145739, 6291469 };
    int i = 0;
    while( primeList[i] < size ) {
        i ++ ;
        //Error if the new input size is too large
        if( i == sizeof(primeList) / sizeof(*primeList) ) {
            return 0;
            break;
        }
    }
    return primeList[i];
}
