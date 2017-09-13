//
//A generic implementation of a tree.
//
#ifndef GTREENODE_H
#define GTREENODE_H
template <class t> class gTreeNode{
   private:
      t data;
      gTreeNode<t> *leftChild;
      gTreeNode<t> *rightChild;
   public:
      gTreeNode();
      gTreeNode(t inData);
      gTreeNode(t inData, gTreeNode<t> *inLeftChild, gTreeNode<t> *inRightChild);
      gTreeNode(t inData, t inLeftData, t inRightData);
      ~gTreeNode();
      t            getData();
      void         setData(t inData);
      gTreeNode<t> *getLeftChild();
      void         setLeftChild(gTreeNode<t> *inNode);
      void         setLeftChild(t inData);
      gTreeNode<t> *getRightChild();
      void         setRightChild(gTreeNode<t> *inNode);
      void         setRightChild(t inData);
       int          getHeight();
};

template <class t> class gTree{
   private:
       gTreeNode<t> *treeHead;
   public:
       gTree(gTreeNode<t> *headNode);
       gTree(t inData);
       gTree();
       ~gTree();
       gTreeNode<t> *getHead();
       void         setHead(gTreeNode<t> *inNode);
};
#endif //GTREENODE_H
