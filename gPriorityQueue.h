//
//A generic implementation of a priority que
//
#ifndef GPPRIORITYQUEUE_H
#define GPRIORITYQUEUE_H
template <class t> class gPriorityQueueNode{
   private:
       t   data;
       int nPriority = 0;
   public:
       gPriorityQueueNode();
       gPriorityQueueNode(t inData, int nInPriority);
       int  getPriority();
       void setPriority(int nInPriority);
       t    getData();
       void setData(t inData);
};

template <class t> class gPriorityQueue{
   private:
       gPriorityQueueNode<t> *aData;
       void growArray(int nGrowSize);
       void shrinkArray(int nShrinkSize);
       void shiftArray(int nIndex);
       void shiftArrayLeft(int nIndex);
       int nSize;
       int nMaxSize;
   public:
       gPriorityQueue();
       ~gPriorityQueue();
       void                  insertWithPriority(t insertData, int nPriority);
       void                  insertWithPriority(gPriorityQueueNode<t> insertNode, int nPriority);

       t                     dequeue();
       t                     lowDequeue();
       t                     peek();
       int                   getSize();
       int                   getMaxSize();
};
#endif //GPRIORITYQUEUE_H
