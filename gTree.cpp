#include "gTree.h"
#define NULL 0

template <class t>
void gTreeNode<t>::setData(t inData){
   data = inData;
}

template <class t>
void gTreeNode<t>::setLeftChild(gTreeNode<t> *inNode){
   leftChild = inNode;
}

template <class t>
void gTreeNode<t>::setRightChild(gTreeNode<t> *inNode){
   rightChild = inNode;
}

template <class t>
void gTreeNode<t>::setLeftChild(t inData){
   gTreeNode<t> *newNode = new gTreeNode<t>(inData);
   setLeftChild(newNode);
}

template <class t>
void gTreeNode<t>::setRightChild(t inData){
   gTreeNode<t> *newNode = new gTreeNode<t>(inData);
   setRightChild(newNode);
}

template <class t>
gTreeNode<t>* gTreeNode<t>::getLeftChild(){
   return leftChild;
}

template <class t>
gTreeNode<t>* gTreeNode<t>::getRightChild(){
   return rightChild;
}

template <class t>
gTreeNode<t>::gTreeNode(){
   leftChild = NULL;
   rightChild = NULL;
}

template <class t>
gTreeNode<t>::gTreeNode(t inData){ 
   setData(inData);
   leftChild = NULL;
   rightChild = NULL;
}

template <class t>
gTreeNode<t>::gTreeNode(t inData, gTreeNode<t> *inLeftChild, gTreeNode<t> *inRightChild){
   setData(inData);
   setLeftChild(inLeftChild);
   setRightChild(inRightChild);
}

template <class t>
gTreeNode<t>::gTreeNode(t inData, t inLeftData, t inRightData){
   setData(inData);
   gTreeNode<t> *setLeftNew = new gTreeNode<t>(inLeftData);
   gTreeNode<t> *setRightNew= new gTreeNode<t>(inRightData);
   setLeftChild(setLeftNew);
   setRightChild(setRightNew);
}

template <class t>
gTreeNode<t>::~gTreeNode(){
   if(leftChild != NULL){
      delete leftChild;  
   }
   if(rightChild != NULL){
      delete rightChild;
   }
}

template <class t>
void gTree<t>::setHead(gTreeNode<t> *inNode){
   treeHead = inNode;
}

template<class t>
gTree<t>::gTree(){
   setHead(NULL);
}

template <class t>
gTree<t>::gTree(gTreeNode<t> *headNode){
   setHead(headNode);
}

template <class t>
gTree<t>::gTree(t inData){
   gTreeNode<t> *newHead = new gTreeNode<t>(inData);
   setHead(newHead);
}

template <class t>
gTree<t>::~gTree(){
   delete treeHead;
}

template <class t>
gTreeNode<t>* gTree<t>::getHead(){
   return treeHead;
}

template <class t>
t gTreeNode<t>::getData(){
  return data;
}

template <class t>
int gTreeNode<t>::getHeight(){
    int leftHeight = 1;
    int rightHeight = 1;
    if(getLeftChild() != NULL){
       leftHeight+=getLeftChild()->getHeight();
    }
    if(getRightChild() != NULL){
       rightHeight+=getRightChild()->getHeight();
    }
    int returnHeight;
    if(leftHeight > rightHeight){
       returnHeight = leftHeight;
    }
    else{
       returnHeight = rightHeight;
    }
    return returnHeight;
}





