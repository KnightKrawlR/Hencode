#include <iostream>
#include <cstdlib>
#include <cassert>

using namespace std;



struct Node 
{
  Node()
  {
    prev = NULL;
    next = NULL;
  }
  
  Node(Node * p, Node * n)
  {
    prev = p;
    next = n;
  }
  
  Node(int d, Node * p, Node * n)
  {
    data = d;
    prev = p;
    next = n;
  }
  
  
  int data;
  Node *prev;
  Node *next;
};





void deleteAll(Node *p)
{
  if(p==NULL) return; return deleteAll(p->next);
  delete p;
}

/* Both index functions searches from either side of list depending on the 
 * index and returns the address of the node before the position in question.
 */

Node* findIndexF(Node* p, int index)
{
  return index==0?p:findIndexF(p->next, index-1);
}

Node* findIndexR(Node* p, int index)
{
  return index==0?p:findIndexR(p->prev, index-1);
}


int findElement(Node* p, int item, int index, int size)
{// Returns the index of first item found or the size if item not found.
  if (index == size){
    return size;
  }
  return item == p->data ? index : findElement(p->next, item, index+1, size);
}


void sortFunc(char * & A, int i, int size, 
	      int(*compare)(const char & x, 
			    const char & y))
{
  // Professor Campbell's idea of structure for sort function.
  if (i == size) return;
  
  int p = i;
 
  for(int j=i+1; j<size; j++)
    if(compare(A[j] , A[p]) < 0)  // Uses an outside compare function
      p = j;
  
  int temp = A[i]; A[i] = A[p]; A[p] = temp;
  
  sortFunc(A, i+1, size, compare);
}


void setPrev(Node * p)
{// Sets the prev field of all Nodes in a given list to its previous.
  if (p->next==NULL) return;
  p->next->prev = p; setPrev(p->next);
}


Node * copyAll(Node * p)
{
  return p->next==NULL?NULL:new Node(p->data,NULL,copyAll(p->next));
}



class List 
{
public:
  List()
  {
    front = new Node; // Always sets prev and next to NULL in node.cc
    rear = new Node;
    
    front->next = rear;
    rear->prev = front;
    oldFront = rear;
    
    oldIndex = 0;
    size = 0;
  }
  
  // Create an empty doubly-linked list
  
  ~List()
  {
    rear = NULL;
    deleteAll(front);
    front = NULL;
    oldFront = NULL;
    oldIndex = 0;
  }
  
  // Delete all nodes

  List(const List & orig)
  // Make a deep copy of a list
  { 
    front = new Node(NULL, copyAll(orig.front->next));
    rear = new Node;
    
    Node *temp = findIndexF(front, orig.size); 
    // Placing sentinel on the end.
    rear->prev = temp;
    temp->next = rear;
    
    setPrev(front);

    oldFront = findIndexF(front, orig.oldIndex);
    
    oldIndex = orig.oldIndex;
    
    size = orig.size;
  }
  
  void add(const char & item,int index)
  {
    assert(index<=size);
    assert(index>=0);
    oldFront = NULL;
    
    // Constant intime
    Node *temp = index>size/2?findIndexF(front, index):
                              findIndexR(rear->prev, size - index);
    Node *temp2 = new Node(item, temp,temp->next);
    
    temp->next->prev = temp2;
    temp->next= temp2;
    
    oldFront = findIndexF(front, oldIndex);
    
    size++;
  }
  
  
  void removeAt(int index)
  {
    assert(index<size);
    assert(index>=0);
    
    oldFront = NULL;
    
    Node *temp = index>size/2?findIndexF(front, index):
                              findIndexR(rear->prev, size - index);
    Node *temp2 = temp->next;
    temp->next = temp2->next;
    temp->next->prev = temp; 
    delete temp2; 

    if (oldIndex>=size){
      oldIndex=0;
      oldFront = front->next;
    }
    else 
      oldFront = findIndexF(front, oldIndex); 
    
    size--;
  }
  
  void remove(const char & item)
  // Let i be the smallest integer where get(i)==item. 
  // Post: same as for removeAt(i)
  // If no such i exists, no change to the list.
  {
    int index = findElement(front->next, item, 0, size);
    
    if (index != size)   /// If index equals size... Not in the list
      removeAt(index);
  }
  
  
  int find(const char & item) const
  // return smallest integer where get(i)==item,
  // or getSize() if no such integer
  {
    return findElement(front->next, item, 0, size); 
  }
  
  
  int get(int index)
  // return the element at position i.
  // Guarantees constant time access to either end of the list.
  {
    assert(index<size);
    assert(index>=0);
      
    int oldDis = oldIndex - index;
    int oldDisPos = abs(oldDis);

    int newDis= index>(size/2)?index:size - index;
    int newDisPos = abs(newDis);

    Node *temp;

    if ((newDisPos <= oldDisPos)) {
      temp = index>size/2?findIndexF(front, index):
	                           findIndexR(rear->prev, size - index);
    }
    // Using the stored point to find the item
    else 
      temp = oldIndex<index?findIndexF(oldFront, oldDisPos):
	                    findIndexR(oldFront, oldDisPos);
    // Storing the pointer for quicker run time
    oldFront = temp;
    oldIndex = index;

    return temp->next->data; 
  }
    
    int getSize() const
  // return the number of elements of the list
  // runs in constant time
  {  
    return size;
  }
  
  std::ostream & output(std::ostream & ostr)
  // output the list to ostr using format
  // <v0, v1, v2, ..., vn-1>
  {
    ostr << "<";
    for (int i = 0;i<size;i++) {
      ostr << get(i);
      if (i+1 < size)
	ostr << ", ";
    }
    ostr << ">";
    return ostr;
  }
  bool empty() const
  {
    return size==0;
  }
  void clear()
  {
    oldFront = front;
    oldIndex = 0;
      
    while (!empty())
      removeAt(0);
  }
  
  string getString(int start, int end)
  {
    string theString = "";
    for(int i=start; i<end; i++)
      theString += get(i);

    return theString;
  }

  void sort(int (*compare)(const char & item1,
			   const char & item2))
  // reorder elements of list according to given comparison function
  {
    // Professor Campbell's idea of structure for a sort method.
    char *data = new char[size];
    
    int originalSize = size;
    
    // Putting Elements of list into an array and deleting the nodes.
    for(int i=0; i<originalSize; i++){
      data[i] = get(0);
      removeAt(0);
    }
    // reorder elements of list according to given comparison function
    sortFunc(data, 0, originalSize, compare);
    
    for(int j=0;j<originalSize; j++){
      add(data[j], j);
    }
    delete [] data;
  }
 

 private:
  int size;
  int oldIndex;
  Node *front;
  Node *rear;
  Node *oldFront;
  
};


ostream & operator<<(ostream & ostr,List & list)
{
  list.output(ostr);
  return ostr;
}
