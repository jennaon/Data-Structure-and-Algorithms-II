#ifndef HEAP_H_INCLUDED
#define HEAP_H_INCLUDED

#include <vector>
#include <string>
#include "hash.h"

class heap{
public:
 //
  // heap - The constructor allocates space for the nodes of the heap
  // and the mapping (hash table) based on the specified capacity
  //
  heap(int capacity);

  //
  // insert - Inserts a new node into the binary heap
  //
  // Inserts a node with the specified id string, key,
  // and optionally a pointer. Then key is used to
  // determine the final position of the new node.
  //
  // Returns:
  //   0 on success
  //   1 if the heap is already filled to capacity
  //   2 if a node with the given id already exists (but the heap
  //     is not filled to capacity)
  //
  int insert(const std::string &id, int key, void *pv = NULL);

  //
  // setKey - set the key of the specified node to the specified value
  //
  // I have decided that the class should provide this member function
  // instead of two separate increaseKey and decreaseKey functions.
  //
  // Returns:
  //   0 on success
  //   1 if a node with the given id does not exist
  //   2 if the heap is empty
  int setKey(const std::string &id, int key);

  //
  // deleteMin - return the data associated with the smallest key
  //             and delete that node from the binary heap
  //
  // If pId is supplied (i.e., it is not NULL), write to that address
  // the id of the node being deleted. If pKey is supplied, write to
  // that address the key of the node being deleted. If ppData is
  // supplied, write to that address the associated void pointer.
  //
  // Returns:
  //   0 on success
  //   1 if the heap is empty
  //
  int deleteMin(std::string *pId = NULL, int *pKey = NULL, void *ppData = NULL);

  //
  // remove - delete the node with the specified id from the binary heap
  //
  // If pKey is supplied, write to that address the key of the node
  // being deleted. If ppData is supplied, write to that address the
  // associated void pointer.
  //
  // Returns:
  //   0 on success
  //   1 if a node with the given id does not exist
  //
  int remove(const std::string &id, int *pKey = NULL, void *ppData = NULL);

  /* printHeap - printout the entire heap, useful for debugging
   */
  void printHeap();
  void printHash();

private:
    class node { // An inner class within heap
    public:
        node(const std::string &id = "", int key = 0, void *pData = NULL);
        //node( const node &n );
        //~node();
        std::string id; // The id of this node
        int key; // The key of this node
        void *pData; // A pointer to the actual data
        friend heap;
  };
  std::vector<node> data; // The actual binary heap
  hashTable * mapping; // maps ids to node pointers
  int currentSize;  //indicates the size of current heap

  //Enforces heap properties by percolating up the item at posCur
  //Returns the final position of the item
  int percolateUp(int posCur);

  //Enforces heap properties by percolating down the item at posCur
  //Returns the final position of the item
  int percolateDown(int posCur);

  //Finds the parent of the node with index, or input posCur
  //Returns the index of parent on success
  //Returns -1 if there is no parent
  int getParent( int posCur );

  /* getSmallerChild - returns the heap address of the smaller child of
   * the node passed on from input parameter.
   */
  int getSmallerChild ( int posCur );

  /* getPos - Returns the position of the node within the heap as the address in array.
   * Found by simple pointer subtraction.
   */
  int getPos(node* pn);

  /* swap - swap all variables within two nodes passed on as input parameters
   */
  void swap ( node* pn1 , node* pn2 );
};

#endif // HEAP_H_INCLUDED
