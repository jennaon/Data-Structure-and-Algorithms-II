#include <iostream>
#include <fstream>
#include <string>
#include "heap.h"
#include "hash.h"

heap::heap(int capacity){
    currentSize = 0;
    data.resize( capacity + 1 );
    int hashSize;
    if ( capacity *2 < 3145739 ) hashSize = 3145739;
    else hashSize = capacity * 2;
    mapping = new hashTable( hashSize );
}

int heap::insert( const std::string &id, int key, void *pv ){
    int newIndex;
    if( currentSize == data.size()-1 ) return 1;
    if ( mapping -> contains( id ) ) return 2; //check whether this string already exists
    else {
        currentSize ++;
        //Append the new item at the end
        data.at( currentSize )= *( new node( id, key, pv ) );
        //Heapify and find the position for the new item
        newIndex = percolateUp( currentSize );
        //Map the new position in the hash table
        mapping->insert(id, & (data[newIndex]) );
        return 0;
    }
}

int heap::setKey( const std::string &id, int key ) {
    bool itemExist; //later used as bool pointer to know whether the item exists
    if ( data.size() == 0 ) return 2;
    else {
        //get the item from the hash table
        node * pNode =  static_cast< node* > ( mapping->getPointer( id, &itemExist ) );
        if(! itemExist ) return 1;
        int addr = getPos( pNode );
        //change the key in heap
        pNode->key = key;
        //percolate up or down
        percolateUp( addr );
        percolateDown( addr );
        //update the item in hash table
        mapping->setPointer( data[addr].id, &data[ addr ] );
        return 0;
    }
}

int heap::deleteMin( std::string *pId, int *pKey, void *ppData ){
    if ( currentSize == 0 ) return 1;
    else {
        //Assign return values
        if( pId ) *pId = ( data[1].id );
        //Remove first item of the heap
        remove( data[1].id, pKey, ppData );
        return 0;
    }
}

int heap::remove( const std::string &id, int *pKey, void *ppData ) {
    bool itemExist;
    node * pNode =  static_cast< node* > ( mapping->getPointer( id, &itemExist ) );
    int addr = getPos( pNode );

    if( currentSize == 0 ) return 1;
    if( !itemExist ) return 1;
    else{
        //Update input pointer parameters
        if( pKey ) * pKey = ( pNode->key );
        if( ppData ) *(static_cast<void **> (ppData) ) = pNode->pData;

        //Remove the ID from hashTable. Check whether ID is valid at the same time.
        //Swap the last element with the element in the designated address.
        swap( & ( data[currentSize] ), pNode ) ;
        //Delete the last element afterwards, both from the heap and hashTable
        mapping->remove( data[currentSize].id );    //from hash
        data[ currentSize ] = *( new node() );      //reset heap
        currentSize --;     //update size'
        //Enforce heap properties
        percolateDown( addr );
        percolateUp( addr );
        return 0;
    }
}

/** Initialization of private methods and variables */
heap::node::node(const std::string &id, int key, void *pData): id(id), key(key), pData(pData) {}

int heap::percolateUp( int posCur ) {
    int newKey = data.at(posCur).key;
    int parent = getParent(posCur);

    //Repeat as long as there exists parent of this node
    while ( parent != -1 ){
        if( newKey < data.at(parent).key ){
            //swap data in the node
            swap ( &(data.at(posCur)), &(data.at(parent)) );
            //move index forward
            posCur = parent;
            parent = getParent( parent );
        }
        else break;     //Seems unnecessary, but I'll just leave it.
    }
    return posCur;
}

int heap::percolateDown ( int posCur ) {
    int newKey = data.at(posCur).key;
    int child = getSmallerChild(posCur);

    //Repeat as long as there exist children of the node
    while ( child != -1 ){
        if( data.at(child).key < newKey ){
            //swap data stored within the node
            swap( &(data.at(posCur) ), &(data.at(child)) );
            //move index downward
            posCur = child;
            child = getSmallerChild( child );
        }
        else break;
    }
    return posCur;
}

int heap::getPos( node *pn ) {
    int pos = pn - &data[0];
    return pos;
}

int heap::getParent ( int posCur ) {
    int val = ( posCur ) /2;
    if( val > 0) return val;
    else return -1;
}

int heap::getSmallerChild ( int posCur ){
    //Return smaller child of two
    int left = ( posCur * 2 );      //left child
    int right = ( posCur * 2) +1;   //right child
    //If left child is larger than the size of heap
    //there's no children for this node
    if ( left > currentSize  ) return -1;
    //if left child address is smaller than the size of heap,
    //but right child's address is larger than the size of heap,
    //there's only one child (left) for this node
    else if ( right > currentSize ) return left;
    //Otherwise, this node has both children
    else if( data.at(left).key > data.at(right).key ) return right;
    else return left;
}

void heap::swap ( node *pn1 , node *pn2 ){
    //Swap pointers on the hashTable
    node * temp = pn1;
    mapping->setPointer( pn1->id, pn2 );
    mapping->setPointer( pn2->id, temp );
    //Swap ID
    pn1->id.swap( pn2->id );
    //Swap Keys
    int keyTemp = pn1->key;
    pn1->key = pn2->key;
    pn2->key = keyTemp;
    //Swap (optional for project #2) data
    void* pTemp = pn1->pData;
    pn1->pData = pn2->pData;
    pn2->pData = pTemp;
}

void heap::printHeap(){
    if ( data.size() == 0 ) std::cout<<"Heap is empty" << std::endl;
    else {
        std::cout<<"printing your heap...(key-ID [location])"<<std::endl;
        for(int i = 0; i<data.size(); i++){
            std::cout<<data[i].key <<"-"<<data[i].id<<", [" <<i<<"]   ";
        }
        std::cout<<std::endl;
    }
    std::cout<<"-------------------------------"<<std::endl;
}

void heap::printHash(){
    mapping->printTable();
}
