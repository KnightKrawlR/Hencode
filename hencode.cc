/* Jimmy Alexis
 * hencode.cc hdecode.cc
 * May 8
 * This is the encode section of my lab. It takes an message an encodes
 * it into an array of bytes.
 */



#include <iostream>
#include <cstdlib>
#include <cassert>
#include <fstream>
#include <math.h>

using namespace std;

struct Hnode{

  unsigned char Hdata;  
  int freq; 
  Hnode *left;
  Hnode *right;

  Hnode(){
    freq = 0;
    left = NULL;
    right = NULL;
  }

  Hnode(const unsigned char & _data, int _freq){
    Hdata = _data;
    freq = _freq;
    left = NULL;
    right = NULL;
  }

  Hnode(int _freq, Hnode *_left, Hnode *_right){
    freq = _freq;
    left = _left;
    right = _right;
  }
   
  
  Hnode(const unsigned char & _data, int _freq, Hnode *_left, Hnode *_right){
    Hdata = _data;
    freq = _freq;
    left = _left;
    right = _right;
  }
   
};

typedef Hnode * Hnodep;

int Hnodepcompare(const Hnodep & a, const Hnodep & b)
{
  return (a->freq - b->freq);
}


class PriorityQueue {
public:
  PriorityQueue() {
    _head = NULL;
    _size = 0;
    compare = Hnodepcompare;
  }
  size_t size ()
  {
    return _size;
  }
  void enqueue(const Hnodep & v) {
    // POST: v is inserted in the priority queue
    //       before the first element v' such that v < v'.
    //       or at the end of the list if no such v' can be found.
    _add(v, _head);
    _size++;
  }

  Hnodep dequeue() {
    // PRE: this priority queue isn't empty
    // POST: The first element is removed and returned.
    assert(_head);
    Node *old = _head; 
    
    Hnodep result = _head->data;
    _head = _head->next;
    _size--;
    delete old;
    return result;
  }
  
private:
  struct Node {
    Hnodep data;
    Node *next;
    Node(const Hnodep & d, Node *n): data(d), next(n) {}
  };
    
  void _add(const Hnodep & v, Node * & ptr)
  {
    if (!ptr)
      ptr = new Node(v,NULL);
    else if (compare(v , ptr->data) < 0)
      ptr = new Node(v,ptr);
    else _add(v,ptr->next);
  }
  size_t _size;
  Node *_head;
  int (*compare)(const Hnodep & x, const Hnodep & y);
};



void findFreq(char **argv, int * & freqList, string & dataStr) 
{
  char * sourcefilename = argv[1];
  ifstream inFile(sourcefilename);

  while (inFile.good()){
    char c = (unsigned char) inFile.get();
    
    if (!inFile) break;
       
    dataStr += c;
    freqList[c]++;
  } 
} 

void findEnqueue(PriorityQueue & myQueue, int * freqList)
{
  // This functions enqueues all of the char into my priority Queue
  for (int i=0; i<256; i++){
    if (freqList[i]){
      unsigned char c = i;
      int freq = freqList[i];
      myQueue.enqueue(new Hnode(c,freq));
    }
  }

}

Hnodep createHuff(PriorityQueue & myQueue)
{
  while (myQueue.size() > 1){
    Hnodep Ltemp = myQueue.dequeue();
    Hnodep Rtemp = myQueue.dequeue();
    int freqTemp = Ltemp->freq + Rtemp->freq;
    myQueue.enqueue(new Hnode(freqTemp, Ltemp, Rtemp));
  }
  return myQueue.dequeue();
}



void createBin(Hnodep p, string * BiList, string Bi)
{
  if (p->left==NULL){
    BiList[p->Hdata] = Bi;
  }
  else{
    string BiL = Bi + "0";
    string BiR = Bi + "1";
    createBin(p->left, BiList, BiL);
    createBin(p->right, BiList, BiR);    
  }
}

void createCode(string * BiList, string dataStr, string & code)
{
  
  for(size_t i=0; i<dataStr.length(); i++)
    code += BiList[dataStr[i]];

}

void createByte(const string & code, int * & byteList)
{
  int index = code.length() / 8;
  int remain = code.length() % 8;
  //IF the byte is not a multiple of 8 it has and extra index in loop.
  byteList = remain?new int[index+1]:new int[index];
 
  //Initiallizing list
  for(int i=0; i<index; i++)
    byteList[i] = 0;
  if (remain) byteList[index]=0;

  int j = 0;
  int ii = 0;
  //Creating the Bytes
  for(size_t i=0; i<code.length(); i++){
    if(code[i] == '1'){
      byteList[ii] += int(pow(2,(7-j)));
    }
    
    j= (j+1)%8;
    if (!j) ii++;
    
  }

}


void outputTree(Hnodep p, ostream & outFile)
{
  
  if (p->left==NULL){
    outFile<<"L"<<p->Hdata;
  }
  else{
    outFile<<"I";   
    outputTree(p->left, outFile);
    outputTree(p->right, outFile);
  }
}

int main(int argc, char **argv)
{
  PriorityQueue myQueue; 
    
  string dataStr = "";
  int *freqList = new int[256];
  for (int i=0; i<256; i++){
    freqList[i] = 0;
  }

  // Finding the Frequency of chars and creating the big string.
  findFreq(argv, freqList, dataStr);
 
  findEnqueue(myQueue, freqList);
  
  //Create Huffman Tree
  Hnodep huffTree = createHuff(myQueue);
  
  //Initialize list
  string *BiList = new string[256];
  for(int i=0; i<256; i++)
    BiList[i] = "";
  
  // Create Binary List
  createBin(huffTree, BiList, "");
 
  //Create String of Binary
  string code;
  createCode(BiList, dataStr, code);
  
  // Find Bytes
  int * byteList;
  createByte(code, byteList);
  
  // Creating the outFile
  cout<<string(argv[1])<<endl;
  string huffname = string(argv[1])+string(".huf");
  ofstream outFile(huffname.c_str());
  
  
  int bits = code.length();
  outFile<<bits;

  outputTree(huffTree, outFile);
  
  size_t sizeBy;
  if (code.length()%8)
    sizeBy =  (code.length() / 8) + 1;
  else
    sizeBy = code.length() / 8;


  for(size_t i=0; i<sizeBy; i++){
    
    outFile.put(byteList[i]);
  }
  
  
  unlink(argv[1]); 

}




