#include "huffCompressor.h"
#include <iostream>

#define ULLSize sizeof(unsigned long long int)
#define CODE_SIZE 256
#define LFTMST_BIT_SET 128 //10000000
#define BITS_IN_BYTE 8
void code::incrementSize(){
   cSize = cSize + 1;
}

//Set an element in a code's internal array
void code::setCodeElement(unsigned char cInChar){
   aCode[cSize] = cInChar;
}

//Default constructor for a code
code::code(){
   for(int i = 0; i < CODE_SIZE; i++){
      aCode[i] = 0;
   }
}

unsigned char huffNode::getSymbol(){
   return cSymbol;
}

void huffNode::setSymbol(unsigned char cInSymbol){
   cSymbol = cInSymbol; 
}

long int huffNode::getWeight(){
   return nWeight;
}

void huffNode::setWeight(unsigned long int nInWeight){
   nWeight = nInWeight;
}

void huffNode::incrementWeight(){
   nWeight = nWeight + 1;
}

//Constructor for a huffNode giving it a symbol and a weight
huffNode::huffNode(unsigned char nInSymbol,unsigned long int nInWeight){
  setSymbol(nInSymbol);
  setWeight(nInWeight); 
  bIsLeaf = true; //note that if a huffNode has a symbol, it must be a leaf
}

//Constructor for a huffNode giving it only a weight
huffNode::huffNode(unsigned long int nInWeight){
   setWeight(nInWeight);
   bIsLeaf = false;
}

//Default constructor for a huffNode
huffNode::huffNode(){
   bIsLeaf = false;
}

bool huffNode::getIsLeaf(){
   return bIsLeaf;
}

code huffNode::getCode(){
   return nodeCode;
}

void huffNode::setCode(code inCode){
   nodeCode = inCode;
}

//Default constructor for a huffNodeList
huffNodeList::huffNodeList(){
   aList = new huffNode[1];
   nSize = 0;
}

huffNodeList::~huffNodeList(){
   delete[] aList;
}

//insert a huffNode into the lsit via binary insert
void huffNodeList::insert(huffNode inNode){
   int nHi = nSize-1;
   int nLo = 0;
   int nMid = 0;
   bool nGreater = false;
   while(nLo <= nHi){
      nMid = (nLo + nHi) / 2;
      if(aList[nMid].getSymbol()<inNode.getSymbol()){
         nLo = nMid + 1; 
         nGreater = true;
      }
      else if(aList[nMid].getSymbol()>inNode.getSymbol()){
         nHi = nMid - 1;
         nGreater = false;
      }
      else if(aList[nMid].getSymbol()==inNode.getSymbol()){
         //if the node is already in the list, increment its weight
         aList[nMid].incrementWeight();
         return;
      }
   }
   
   //increment the size of the array if a new node is added
   huffNode* newArray = new huffNode[nSize+1];
   for(int i = 0; i < nSize; i++){
       newArray[i] = aList[i];
   }
   delete[] aList; 
   aList = newArray; 
   nSize = nSize + 1;
   shiftArray(nMid);
   if(nGreater){
      aList[nMid+1] = inNode;
   }
   else{
      aList[nMid] = inNode;
   }
}

//retrieves the code from the huffNode with the given symbol
//char in the huffNodeList
code huffNodeList::getCode(unsigned char cInChar){
   code retCode;
   int nHi = nSize-1;
   int nLo = 0;
   int nMid = 0;
   bool nGreater = false;
   while(nLo <= nHi){
      nMid = (nLo + nHi) / 2;
      if(aList[nMid].getSymbol()<cInChar){
         nLo = nMid + 1; 
      }
      else if(aList[nMid].getSymbol()>cInChar){
         nHi = nMid - 1; 
      }
      else if(aList[nMid].getSymbol()==cInChar){
         retCode =  aList[nMid].getCode();   
         break;
      }
   }
   return retCode;
}


//shifts the internal array down by one.
//Helper function for the inserts
void huffNodeList::shiftArray(int nIndex){
   for(int i = nSize-1; i > nIndex; i--){
     if(i > 0){
        aList[i] = aList[i-1];
     }
   }
}

int huffNodeList::getSize(){
   return nSize;
}

huffNode huffNodeList::getAt(int nIndex){
   return aList[nIndex];
}

void huffNodeList::insert(unsigned char cInChar){
   huffNode insertNode(cInChar, 1);
   insert(insertNode);
}

//Takes in a piece of data (aInData) of size nSize, and
//iterates through the data constructing a huffNodeList
//where each unique char (byte) has a corresponding
//huffNode and weight representing the number of times
//that char (byte) appears
void huffCompressor::getSymbolsAndWeights(unsigned char *aInData, unsigned long long int nSize, huffNodeList &insertList){  
   for(int i = 0; i < nSize; i++){
          insertList.insert(aInData[i]);
   }
}

//Uses a simple, common algorithm to build the
//Huffman code tree. It takes in a gPriorityQueue
//where a set of huffNodes have been wrapped with
//gTreeNodes, and priority = huffNode weight
gTree<huffNode> huffCompressor::createCodeTree(gPriorityQueue<gTreeNode<huffNode>*> &inSymbols){
   gTreeNode<huffNode> *headNode;
   gTreeNode<huffNode> *tempNodeA;
   gTreeNode<huffNode> *tempNodeB;
   int nAWeight;
   int nBWeight;
   int nInsertPriority;

   //if there is more than one node in the queue
   while(inSymbols.getSize() > 1){
      //dequeue the two nodes with the lowest priority
      tempNodeA = inSymbols.lowDequeue();
      nAWeight = tempNodeA->getData().getWeight();
      tempNodeB = inSymbols.lowDequeue();
      nBWeight = tempNodeB->getData().getWeight();

      //construct a new huffNode with a weight that
      //is = to the combined weight of the two dequeued
      //nodes
      huffNode innerNode(nAWeight+nBWeight);

      //wrap that huffNode in a new gTreeNode, which has the two dequeued nodes as its children
      gTreeNode<huffNode> *newNode = new gTreeNode<huffNode>(innerNode,tempNodeA,tempNodeB);

      //insert the new gTreeNode back into the queue with its priority = to its weight
      nInsertPriority = newNode->getData().getWeight();
      inSymbols.insertWithPriority(newNode,nInsertPriority);
   }

   //the last node in the queue must be the root of the tree
   headNode = inSymbols.dequeue();
   gTree<huffNode> outTree(headNode);
   return outTree;
}

//calls a recursive function which traverses the given code tree (*inTree) and
//creates a table of huffNodes sorted by their symbols (&traverseList), where
//each node's code is the optimal prefix code for that symbol in the code tree
void huffCompressor::createCodeTable(gTree<huffNode> *inTree, huffNodeList &traverseList){
   code startCode;
   traverseCodeTree(traverseList, inTree->getHead(), startCode);
}

//recursive function which traverses the code tree, and in doing so
//sets the codes of the huffNodes within that tree. Through this process
//we create the optimal prefix code for these symbols.
void huffCompressor::traverseCodeTree(huffNodeList &traverseList, gTreeNode<huffNode> *inNode, code inCode){
   //if the node is a leaf, set the Node's code
   //to the one passed in as a parameter
   if(inNode->getData().getIsLeaf()){
       huffNode addNode(inNode->getData().getSymbol(),0);
       code setCode = inCode;
       setCode.incrementSize();
       addNode.setCode(setCode);
       traverseList.insert(addNode);
   }
   //otherwise, add a '0' to the code going left,
   //add a '1' to the code going right, and then
   //recursively call the function with these new codes
   else{
       code leftCode = inCode;
       code rightCode = inCode;
       leftCode.incrementSize();
       rightCode.incrementSize();
       leftCode.setCodeElement('0');
       rightCode.setCodeElement('1');
       traverseCodeTree(traverseList, inNode->getLeftChild(), leftCode);
       traverseCodeTree(traverseList, inNode->getRightChild(), rightCode);
   }
}

//encodes the data in *aInData using the code table provided by &inTable.
//The encoded data is written out into *&aWriteData. nLength is the length of
//the data in *aInData, and the function returns the length of the encoded
//data
long long int huffCompressor::encodeData(huffNodeList &inTable, unsigned char *aInData, unsigned char *&aWriteData, long long int nLength){
   long long int nWriterIndex = 0; //current index in the aWrittenData array
   unsigned char cBitCounter = 1; //bits written in the current unsigned char
   unsigned char cBitMask = LFTMST_BIT_SET;//bit mask representing 10000000
   unsigned char cSetChar = 0;
   code tempCode;

   //loop through aInData
   for(int nReaderIndex = 0; nReaderIndex < nLength; nReaderIndex++){
       //look up the given char in the code table to get the 
       //corresponding code
       tempCode = inTable.getCode(aInData[nReaderIndex]); 
       
       //bithack the code into the aWriteData array
       for(int i = 0; i < tempCode.cSize; i++){
          //if we have written 8 bits, cross over
          //into the next index of the writearray
          if(cBitCounter > BITS_IN_BYTE){
             aWriteData[nWriterIndex] = cSetChar;
             cSetChar = 0;
             nWriterIndex = nWriterIndex + 1;
             cBitCounter = 1;
             cBitMask = LFTMST_BIT_SET;
          }
          //if the code is a 1, OR a 1 into the char
          if(tempCode.aCode[i] == '1'){
             cSetChar = cSetChar | cBitMask; 
          }
          
          //advance the bitmask
          cBitCounter = cBitCounter + 1;
          cBitMask    = cBitMask >> 1;
       }
   }
   //write the last char out
   aWriteData[nWriterIndex] = cSetChar;
   return nWriterIndex+1;
}

//decodes the data in *aInData using the code tree &inTree, and writes the decoded data
//into *&aWriteData. nEncodedLength and nDecodedLength represent the length of the encoded
//data and the decoded data respectively. The function returns the size of the decoded data.
long long int huffCompressor::decodeData(gTree<huffNode> &inTree, unsigned char *aInData, unsigned char *&aWriteData, long long int nEncodedLength, long long int nDecodedLength){
   unsigned char cBitMask = LFTMST_BIT_SET;    //set bitmask to 10000000
   long long int nWriterIndex = 0;

   //gTreeNode<huffNode> pointer for traversing through the code tree
   gTreeNode<huffNode> *traverseNode = inTree.getHead();
   
   //loop through aInData
   for(int nReaderIndex = 0; nReaderIndex < nEncodedLength; nReaderIndex++){
       //go 1 bit at a time
       for(int bit = 1; bit <= BITS_IN_BYTE; bit++){
          char cBitValue = cBitMask & aInData[nReaderIndex];
          
          //if the bit is 1, go down the right side of the tree
          if(cBitValue){
             traverseNode = traverseNode->getRightChild();          
          }
          //if the bit is 0, go down the left side of the tree
          else{
             traverseNode = traverseNode->getLeftChild();
          }
          //if the gTreeNode<huffNode> is a leaf
          if(traverseNode->getData().getIsLeaf()){
             //then the next char to go into the aWriteData is the huffNode's symbol
             aWriteData[nWriterIndex] = traverseNode->getData().getSymbol(); 
             traverseNode = inTree.getHead();
             nWriterIndex = nWriterIndex + 1;
             //if we have exceeded the length of the writeArray, get out of here
             if(nWriterIndex >= nDecodedLength){
                break;
             }
          }
          cBitMask = cBitMask >> 1; 
       } 
       if(nWriterIndex >= nDecodedLength){
          break;
       }
       //reset the bitmask
       cBitMask = LFTMST_BIT_SET;
   }
   return nWriterIndex+1;
}

//helper function which opens the file with the name aFileName, and then loads its 
//data into the pointer *&aReadData. NOTE: the array passed to aReadData must be
//deleted later
long long int huffCompressor::readFileData(char* aFileName, unsigned char *&aReadData){
   if(aReadData){
      delete[] aReadData;
   }
   std::streampos size;

   std::fstream file (aFileName, std::ios::in|std::ios::binary|std::ios::ate);
   if(file.is_open()){
      size = file.tellg();
      
      aReadData= new unsigned char[size];

      file.seekg(0, std::ios::beg);
      file.read((char*)aReadData,size);
      file.close();
   }
   long long int outSize = size;
   return outSize; 
}

//helper function which opens a file with the name aFileName, and then writes out
//the data in *aInData to that file. nDataSize is the size of the data in aInData.
void huffCompressor::writeFileData(unsigned char *aInData, long long int nDataSize, char *&aFileName){
   std::ofstream outFile;

   outFile.open(aFileName);
   for(int i = 0; i < nDataSize; i++){
      outFile << aInData[i];
   }
   outFile.close(); 
}

//Converts the code tree provided by &inCodeTree into a
//binary representation, and writes that representation into
// *&aWriteArray. This binary representation is a simple array
// representation of a tree, where the array representation has
// been compressed by representing non-leaf nodes as 0 bits, and
// leaf nodes as a 1 bit, followed by the 8 bit symbol.
// NOTE: you have to delete *&aWriteArray later
long long int huffCompressor::createTreeBlock(gTree<huffNode> &inCodeTree, unsigned char *&aWriteArray){
   long long int nOutSize = 0;  
   int  nHeight = inCodeTree.getHead()->getHeight();
   long int nIArraySize = pow(2,nHeight+1)-1;

   //the intermediate array is an array of pointers which lays out the tree
   //inCodeTree in an array representation
   gTreeNode<huffNode> **intermediateArray = new gTreeNode<huffNode>*[nIArraySize];
   for(int i = 0; i < nIArraySize; i++){
      intermediateArray[i] = NULL;
   }
   
   //array index 1 is the tree head
   intermediateArray[1] = inCodeTree.getHead();
   int nBranches = 0;
   int nLeaves = 0;

   //iterate through the array, and add a node's
   //left and right children as they are encountered
   //in the array
   for(long int i = 1; i < nIArraySize; i++){
       if(intermediateArray[i]!=NULL){
          if(intermediateArray[i]->getData().getIsLeaf()){
             nLeaves++;
          }
          else{
             nBranches++;
          }
          if(intermediateArray[i]->getLeftChild() != NULL){
             intermediateArray[2*i] = intermediateArray[i]->getLeftChild();
          }
          if(intermediateArray[i]->getRightChild() != NULL){
             intermediateArray[(2*i)+1] = intermediateArray[i]->getRightChild();
          } 
       }
       else{
          nBranches++;
       }
   }

   //loop backwards through the array until the first node with information is hit.
   //decrease the number of branches so as to avoid dead space at the end of the tree block
   int iter = nIArraySize - 1;
   while(intermediateArray[iter] == NULL){
      nBranches--;
      iter--;
   }
   
   //size we need for the tree block is going to be 9 bits for each leaf and 1 bit for
   //each branch or null space.
   long long int nWriteArraySize = (long long int)ceil(((9.0*nLeaves)+(nBranches))/8.0);
   if(aWriteArray){
      delete[] aWriteArray;
   }

   //allocate new writeArray
   aWriteArray = new unsigned char[nWriteArraySize];
   for(int i = 0; i < nWriteArraySize; i++){
      aWriteArray[i] = 0;
   }
   
   //bithack the tree into the writeArray.
   unsigned char cBitMask    = LFTMST_BIT_SET; //bit mask for 10000000
   unsigned char cBitCounter = 1;
   unsigned char cWriteChar  = 0;
   long long int nWriteIndex = 0;
   bool          bWriteLeaf = false;
   //iterate through the intermediate array. For each
   //branch or dead space, a 0 bit is written to the writeArray
   //for each leaf, a 1 and then an 8 bit symbol is written.
   for(int i = 0; i < nIArraySize; i++){
      //if we have written 8 bits, move on to the next
      //byte in the array
      if(cBitCounter > BITS_IN_BYTE){
         cBitMask = LFTMST_BIT_SET;
         cBitCounter = 1;
         aWriteArray[nWriteIndex] = cWriteChar;
         nWriteIndex = nWriteIndex + 1;
         if(nWriteIndex >= nWriteArraySize){
            break;
         }
         cWriteChar = 0;
      }
      
      //if the index is a leaf
      if(intermediateArray[i] != NULL && intermediateArray[i]->getData().getIsLeaf()){
         //write a 1, and then the char
         cWriteChar = cWriteChar | cBitMask;
         bWriteLeaf = true;
      }

      //advance the bitmask
      cBitCounter = cBitCounter + 1;
      cBitMask = cBitMask >> 1;

      if(bWriteLeaf){
         //write out the symbol data
         unsigned char cWriteSymbol = intermediateArray[i]->getData().getSymbol();
         unsigned char cBitMask2 = LFTMST_BIT_SET;
         for(int i = 1; i <= BITS_IN_BYTE; i++){ //for looping through the char
            if(cBitCounter > BITS_IN_BYTE){
               cBitMask = LFTMST_BIT_SET;
               cBitCounter = 1;
               aWriteArray[nWriteIndex] = cWriteChar;
               nWriteIndex = nWriteIndex + 1;
               if(nWriteIndex >= nWriteArraySize){
                  break;
               }
               cWriteChar = 0;
            }
            if(cWriteSymbol & cBitMask2){
               cWriteChar = cWriteChar | cBitMask;
            }
            cBitMask2 = cBitMask2 >> 1;
            cBitMask = cBitMask >> 1;
            cBitCounter = cBitCounter +1;
         }
         bWriteLeaf = false;
      }
   }
   
   delete[] intermediateArray;
   return nWriteArraySize;
}

//takes in a block of data ostensibly representing our
//compressed tree binary, *&aTreeBlock, where nSize is the
//size of the block. The resulting tree is stored in &resultCodeTree
void huffCompressor::readTreeBlock(gTree<huffNode> &resultCodeTree, unsigned char *&aTreeBlock, int nSize){
    long long int nISize = nSize * BITS_IN_BYTE;

    //intermediate array which lays out the gTreeNodes in an array representation
    gTreeNode<huffNode> **intermediateArray = new gTreeNode<huffNode>*[nISize];
     
    unsigned char cBitMask = LFTMST_BIT_SET; //set bitmask to 10000000
    unsigned char cBitCounter = 1;
    unsigned char cSymbol = 0;
    int nReaderIndex = 0;
    int nIntermediateIndex = 0;

    //loop through aTreeBlock. for each 0, add a new branch,
    //for each 1, add a new leaf node with the next 8 bits as 
    //the symbol
    while(nReaderIndex < nSize){//loop through bitstream
       if(cBitCounter > BITS_IN_BYTE){
          cBitMask = LFTMST_BIT_SET;
          cBitCounter = 1;
          nReaderIndex = nReaderIndex + 1;
          if(nReaderIndex >= nSize){
             break;
          }
       }
       //if it is a branch
       if(!(aTreeBlock[nReaderIndex] & cBitMask)){
           huffNode branchNode; 
           intermediateArray[nIntermediateIndex] = new gTreeNode<huffNode>(branchNode);  
           cBitMask = cBitMask >> 1;
           cBitCounter = cBitCounter + 1;
       }
       //if it is a leaf
       else{
          //bithack out synbol value and construct the leaf
          cBitMask = cBitMask >> 1;
          cBitCounter = cBitCounter + 1;
          if(cBitCounter > BITS_IN_BYTE){
             cBitMask = LFTMST_BIT_SET;
             cBitCounter = 1;
             nReaderIndex = nReaderIndex + 1;
             if(nReaderIndex >= nSize){
                break;
             }
          }
          unsigned char cBitMask2 = LFTMST_BIT_SET;
          for(int i = 1; i <= BITS_IN_BYTE; i++){
             if(cBitCounter > BITS_IN_BYTE){
                cBitMask = LFTMST_BIT_SET;
                cBitCounter = 1;
                nReaderIndex = nReaderIndex + 1;
                if(nReaderIndex >= nSize){
                   break;
                }
             }
             if(aTreeBlock[nReaderIndex] & cBitMask){
                cSymbol = cSymbol | cBitMask2;
             }
             cBitMask = cBitMask >> 1;
             cBitMask2 = cBitMask2 >> 1;
             cBitCounter = cBitCounter + 1;
          }
          huffNode leafNode(cSymbol,0);
          intermediateArray[nIntermediateIndex] = new gTreeNode<huffNode>(leafNode);
          cSymbol = 0;
       }
       nIntermediateIndex = nIntermediateIndex + 1;
    }

    //now iterate backwards through the intermediate array. For each leaf node
    //or node with children, go to its parent at i/2 and set the node as one
    //of its children. 
    for(int i = nISize - 1; i > 1; i--){
       gTreeNode<huffNode> *currentNode = intermediateArray[i];
       if(currentNode){
          //if the node is a leaf, or either of its children aren't null
          if(currentNode->getData().getIsLeaf() || currentNode->getLeftChild() || currentNode->getRightChild()){
             currentNode = intermediateArray[i/2]; //set the currentNode to the parent

             if(i % 2 == 0){
                //if i is a multiple of 2 it must be a left child
                currentNode->setLeftChild(intermediateArray[i]);
             }
             else{
                //if i is not it must be a right child
                currentNode->setRightChild(intermediateArray[i]);
             }
             
             //set this index to NULL so that the
             //object is not later deleted
             intermediateArray[i] = NULL;
          }
       }
    }
    
    //the node at position 1 must always be the tree head
    resultCodeTree.setHead(intermediateArray[1]);
    intermediateArray[1] = NULL;

    //get rid of filler objects
    for(int i = 0; i < nISize; i++){
        if(intermediateArray[i]){
           delete intermediateArray[i];
        }
    }

    delete[] intermediateArray;
}

//Compression function. Takes in a char array representing a filename,
//opens that file, compresses it, and writes out the compressed data as
//the filename+".huff"
int huffCompressor::compress(char *aInFileName){
   int ret = 0;
   try{
      unsigned char* aData = NULL;
      unsigned long long int nFileSize = readFileData(aInFileName, aData);
      //BUILD CODE TREE
      std::cout<<"Building code tree...\n";
      huffNodeList nodeList;
      gPriorityQueue<gTreeNode<huffNode>*> treeBuilderQueue;
      getSymbolsAndWeights(aData,nFileSize,nodeList);
      for(int i = 0; i < nodeList.getSize(); i++){
         gTreeNode<huffNode> *newNode = new gTreeNode<huffNode>(nodeList.getAt(i));
         treeBuilderQueue.insertWithPriority(newNode, newNode->getData().getWeight());
      }
      gTree<huffNode> codeTree = createCodeTree(treeBuilderQueue);
       
      //GET CODE TABLE
      huffNodeList codeTable;
      createCodeTable(&codeTree,codeTable);

      //ENCODE DATA
      std::cout<<"Encoding data...\n";
      unsigned char* aEncodedData = new unsigned char[nFileSize];
      unsigned long long int nEncodedSize = encodeData(codeTable, aData, aEncodedData, nFileSize);

      //GET TREE BLOCK
      unsigned char *aTreeBlock = NULL;
      unsigned int nTreeBlockSize = createTreeBlock(codeTree,aTreeBlock);

      //CREATE OUTPUT DATA
      std::cout<<"Compiling output data...\n";
      unsigned long long int nOutputSize = ULLSize; //bytes for the filesize
      nOutputSize = nOutputSize + 2; //TODO probably should use sizeof here as well 
      nOutputSize = nOutputSize + nTreeBlockSize + nEncodedSize;
      unsigned char *aOutData = new unsigned char [nOutputSize];
      //place the filesize at the beginning - big-endian

      for(int i = 0; i < ULLSize; i++){
          unsigned long long int nNumChunk = nFileSize << BITS_IN_BYTE*i;
          nNumChunk = nNumChunk >> BITS_IN_BYTE*(ULLSize - 1);
          unsigned char cBitChunk = nNumChunk; 
          aOutData[i] = cBitChunk;
      }
      unsigned char cMSigTBsz = nTreeBlockSize >> BITS_IN_BYTE;
      unsigned char cLSigTBsz = (nTreeBlockSize << BITS_IN_BYTE) >> BITS_IN_BYTE;
      //place the treesize next - big-endian
      aOutData[ULLSize]     = cMSigTBsz;
      aOutData[ULLSize + 1] = cLSigTBsz;
     
      unsigned int nWriteStart = ULLSize + 2;

      //write out the tree block
      for(int i = nWriteStart; i-nWriteStart < nTreeBlockSize; i++){
         aOutData[i] = aTreeBlock[i-nWriteStart];
      }
      
      //write out the encoded block
      unsigned int nEndOfTree = nWriteStart + nTreeBlockSize;
      for(int i = nEndOfTree; i - nEndOfTree < nEncodedSize; i++){
         aOutData[i] = aEncodedData[i-nEndOfTree];
      }

      //WRITE OUT THE FILE
      std::string sOldFileName = aInFileName;
      std::string sFileTag = ".huff";
      std::string sNewFileName = sOldFileName + sFileTag;
      
      char* aOutFileName = (char*)sNewFileName.c_str();
      writeFileData(aOutData,nOutputSize, aOutFileName); 
      
      std::cout<<nFileSize<<" bytes converted to "<<nOutputSize<<" bytes";

      ret = 100-((nOutputSize*100)/nFileSize); 
      
      delete[] aTreeBlock;
      delete[] aOutData;
      delete[] aEncodedData;
      delete[] aData;
   }
   catch(std::exception e){
      ret = -1;   
   }
   return ret;
}

//decompression function. Opens the file with the name aInFileName,
//decompresses its contents if possible, and then writes it out to
//a file with the name aInFileName-5 characters (hopefully, these 5 chars
//are the .huff extension)
int huffCompressor::decompress(char *aInFileName){
   int ret = 0;

   try{
      unsigned char* aData = NULL;
      unsigned long long int nFileSize = readFileData(aInFileName, aData);

      unsigned long long int nDecodedDataSize = 0;
      for (int i = 0; i < ULLSize; i++){
         unsigned long long int nTemp = aData[i];
         nTemp = nTemp << BITS_IN_BYTE*((ULLSize - 1) - i);
         nDecodedDataSize = nDecodedDataSize + nTemp;
      }

      unsigned int nTreeBlockSize = aData[ULLSize] << BITS_IN_BYTE;
      nTreeBlockSize = nTreeBlockSize + aData[ULLSize+1];
      unsigned long long int nEncodedDataSize = nFileSize - (nTreeBlockSize + ULLSize + 2);

      unsigned char* aTreeBlock   = new unsigned char[nTreeBlockSize];
      unsigned char* aEncodedData = new unsigned char[nEncodedDataSize];  
      unsigned char* aDecodedData = new unsigned char[nDecodedDataSize];
      
      //POPULATE THE TREE BLOCK
      std::cout<<"Reading tree block...\n";
      unsigned int nTreeStart = ULLSize + 2;
      for(int i = nTreeStart; i - nTreeStart < nTreeBlockSize; i++){
         aTreeBlock[i - nTreeStart] = aData[i];
      }
      
      //POPULATE THE ENCODED DATA BLOCK
      std::cout<<"Reading encoded data...\n";
      unsigned int nStartPosition = nTreeStart + nTreeBlockSize;
      for(int i = nStartPosition; i - nStartPosition < nEncodedDataSize; i++){
         aEncodedData[i - nStartPosition] = aData[i];
      }

      //GET THE CODE TREE
      std::cout<<"Constructing code tree...\n";
      gTree<huffNode> codeTree;
      readTreeBlock(codeTree, aTreeBlock, nTreeBlockSize);

      //DECODE THE ENCODED DATA
      std::cout<<"Decoding data...\n";
      decodeData(codeTree,aEncodedData,aDecodedData,nEncodedDataSize, nDecodedDataSize);


      //WRITE OUT THE FILE
      std::string sNewFileName = aInFileName;
      sNewFileName = sNewFileName.substr(0,sNewFileName.length()-5);
      
      char* aOutFileName = (char*)sNewFileName.c_str();
      writeFileData(aDecodedData, nDecodedDataSize, aOutFileName);
      

      delete[] aDecodedData;
      delete[] aTreeBlock;
      delete[] aEncodedData;
      delete[] aData;
   }
   catch(std::exception e){
      ret = -1;
   }
   return ret;
}

#undef LEFTMST_BIT_SET
#undef BITS_IN_BYTE
#undef ULLSize
#undef CODE_SIZE
