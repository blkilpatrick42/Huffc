#include "gPriorityQueue.h"

#define N_EXPAND 5
#define NULL 0

template <class t>
int gPriorityQueueNode<t>::getPriority(){
   return nPriority;
}

template<class t>
void gPriorityQueueNode<t>::setPriority(int nInPriority){
   nPriority = nInPriority;
}

template <class t>
t gPriorityQueueNode<t>::getData(){
   return data;
}

template <class t>
void gPriorityQueueNode<t>::setData(t inData){
   data = inData;
}

template <class t>
gPriorityQueueNode<t>::gPriorityQueueNode(){};


template <class t>
gPriorityQueueNode<t>::gPriorityQueueNode(t inData, int nInPriority){
   setData(inData);
   setPriority(nInPriority);
}

template <class t>
gPriorityQueue<t>::gPriorityQueue(){
   aData = new gPriorityQueueNode<t>[N_EXPAND];
   nSize = 0;
   nMaxSize = N_EXPAND;
}

template <class t>
gPriorityQueue<t>::~gPriorityQueue(){
  delete[] aData;
}

template <class t>
void gPriorityQueue<t>::insertWithPriority(t insertData, int nPriority){
   gPriorityQueueNode<t> insertNode(insertData,nPriority); 
   insertWithPriority(insertNode,insertNode.getPriority());
}

/*inserts a node into the queue by priority using a linear O(n) search.
 *if there are multiple nodes with the same priority, they are sorted
 *such that they are dequeued in FIFO order
 */
template <class t>
void gPriorityQueue<t>::insertWithPriority(gPriorityQueueNode<t> insertNode, int nPriority){
  int nIndex = 0;
  
  insertNode.setPriority(nPriority);
  
  for(int i = 0; i < nSize; i++){
     if(aData[i].getPriority() < nPriority){
        nIndex++;
     }
     else{
        break;
     }
  }
  
  //if the data array is not large enough to take the new node
  //increase the array size by N_EXPAND
  if(nSize+1 > nMaxSize){
      growArray(N_EXPAND);
  } 
  nSize = nSize + 1;
  shiftArray(nIndex);
  aData[nIndex] = insertNode;
}

/*Grows the size of the allocated heap array*/
template <class t>
void gPriorityQueue<t>::growArray(int nGrowSize){
   gPriorityQueueNode<t> *aNewData = new gPriorityQueueNode<t>[nMaxSize + nGrowSize];
   for(int i = 0; i < nSize; i ++){
      aNewData[i] = aData[i];
   }
   delete[] aData;
   aData = aNewData;
   nMaxSize = nMaxSize + nGrowSize;  
}

/*Shrinks the size of the allocated heap array*/
template <class t>
void gPriorityQueue<t>::shrinkArray(int nGrowSize){
   gPriorityQueueNode<t> *aNewData = new gPriorityQueueNode<t>[nMaxSize - nGrowSize];
   for(int i = 0; i < nSize; i ++){
      aNewData[i] = aData[i];
   }
   delete[] aData;
   aData = aNewData;
   nMaxSize = nMaxSize - nGrowSize;  
}

/*
 * Shifts all elements of the array down by 1 element,
 * starting at the index nIndex. After this function,
 * nIndex will still hold its previous value
 */
template <class t>
void gPriorityQueue<t>::shiftArray(int nIndex){ 
  for(int i = nSize-1; i > nIndex; i--){
     if(i > 0){
        aData[i] = aData[i-1];
     }
  }
}

/*Shifts all elements of the array up by 1 element,
 * starting at the index nIndex. After this function,
 * nIndex will still hold its previous value
 */
template <class t>
void gPriorityQueue<t>::shiftArrayLeft(int nIndex){ 
  for(int i = nIndex; i < nSize-1; i++){
     aData[i] = aData[i+1];
  }
}

/*dequeues the node with the highest prioirty*/
template <class t>
t  gPriorityQueue<t>::dequeue(){
   t ret;
   if(nSize > 0){
      ret = aData[nSize-1].getData();
      nSize = nSize - 1;
   }
   return ret; 
}

/*dequeues the node with the lowest priority*/
template <class t>
t  gPriorityQueue<t>::lowDequeue(){
   t ret;
   if(nSize > 0){
      ret = aData[0].getData();
      shiftArrayLeft(0);
      nSize = nSize - 1;
   }
   return ret; 
}

template<class t>
int gPriorityQueue<t>::getSize(){
   return nSize;
}

template<class t>
int gPriorityQueue<t>::getMaxSize(){
   return nMaxSize;
}

/*returns the data of the node with the highest priority*/
template<class t>
t gPriorityQueue<t>::peek(){
    t ret;
    if(nSize > 0){
       ret = aData[nSize-1].getData();
    }
    else{
       ret = aData[0].getData();
    }
    return ret;
}

#undef N_EXPAND
#undef NULL



