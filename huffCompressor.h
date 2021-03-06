//
//Implementation of Huffman Coding compression
//
#include "gTree.cpp"
#include "gPriorityQueue.cpp"
#include <math.h>
#include <fstream>

#ifndef HUFF_H
#define HUFF_H
#define CODE_SIZE 256


//struct which represents the coded value of a
//symbol
struct code{
    unsigned char aCode[CODE_SIZE];
    unsigned char cSize = -1;
    void incrementSize();
    void setCodeElement(unsigned char cInChar);
    code();
};

//Node containing compression-related
//information
class huffNode{
   private:
       bool          bIsLeaf = false; //bool notifying that this is a leaf in the code tree
       unsigned char cSymbol = 0;     //8-bit symbol represented by this node (if it is a leaf)
       long int      nWeight = 1;   
       code          nodeCode;        //the code generated by the tree
   public:
       huffNode();
       huffNode(unsigned long int nInWeight);
       huffNode(unsigned char cInSymbol, unsigned long int nInWeight);
       unsigned char      getSymbol();
       void               setSymbol(unsigned char cInSymbol);
       long int           getWeight();
       void               setWeight(unsigned long int nInWeight);
       void               incrementWeight();
       code               getCode();
       void               setCode(code inCode);
       bool               getIsLeaf();
};

//A specialized list for containing huffNodes.
//internally, it is sorted by the huffNode symbols.
class huffNodeList{
   private:
      huffNode* aList;
      int nSize;
      void shiftArray(int nIndex);
   public:
      huffNodeList();
      ~huffNodeList();
      void     insert(huffNode inNode);
      void     insert(unsigned char cInChar);
      code     getCode(unsigned char cInChar); //gets the code assosciated with the given char
      int      getSize();
      huffNode getAt(int nIndex);   
};

//The class which manages all the functions
//and steps of the compression
class huffCompressor{
   private:
      long long int    readFileData(char* aFileName, unsigned char *&aReadData);
      void             getSymbolsAndWeights(unsigned char *aInData,unsigned long long int nSize, huffNodeList &insertList);
      gTree<huffNode>  createCodeTree(gPriorityQueue<gTreeNode<huffNode>*> &inSymbols);
      long long int    createTreeBlock(gTree<huffNode> &inCodeTree, unsigned char *&aWriteArray);
      void             readTreeBlock(gTree<huffNode> &resultCodeTree, unsigned char *&aTreeBlock, int nSize);
      void             createCodeTable(gTree<huffNode> *inTree, huffNodeList &traverseList); 
      void             writeFileData(unsigned char *aInData, long long int nDataSize, char *&afileName);
      unsigned char*   getTreeBlock(unsigned char *aInData);
      gTree<huffNode>* getCodeTree(unsigned char* aInTreeBlock);
      void             traverseCodeTree(huffNodeList &traverseList, gTreeNode<huffNode> *inNode, code inCode);
      long long int    encodeData(huffNodeList &inTable, unsigned char *aInData, unsigned char *&aWriteData, long long int nLength);
      long long int    decodeData(gTree<huffNode> &inTree, unsigned char *aInData, unsigned char *&aWriteData, long long int nEncodedLength, long long int nDecodedLength);
   public:
      int              compress(char *aInFileName);
      int              decompress(char *aInFileName);
};
#undef CODE_SIZE
#endif //HUFF_H
