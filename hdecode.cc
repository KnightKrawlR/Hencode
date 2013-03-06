/* Jimmy Alexis
 * Lab 4
 * hencode.cc hdecode.cc
 * May 8
 * This is the decode section of my lab. It takes an encode message an decodes
 * it.
 */





#include <iostream>
#include <cstdlib>
#include <cassert>
#include <fstream>
#include <math.h>


using namespace std;



struct Hnode{

  char Hdata;  
  int freq; 
  Hnode *left;
  Hnode *right;

  Hnode(){
    freq = 0;
    left = NULL;
    right = NULL;
  }

  Hnode(char _data, int _freq){
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
   
  
  Hnode(char _data, int _freq, Hnode *_left, Hnode *_right){
    Hdata = _data;
    freq = _freq;
    left = _left;
    right = _right;
  }
   
};


typedef Hnode * Hnodep;


string byteBite(int * ByteList, const int bits, int sizeBy)
{
  // This turns the Bytes into Binary and puts it into a list. 
  string BiStr = "";
  int count = bits;
  int tempNum;
  for(int i=0; (i<sizeBy); i++){
    tempNum = ByteList[i];
    for(int j=0; j<8; j++){
      count--;
      if (count<0) break;
      if(tempNum >= int(pow(2,(7-j)))){
	tempNum = tempNum - int(pow(2,(7-j)));
	BiStr += "1";		 
      }
      else{
	BiStr += "0";		 
      }
      
      
    }
    
  } 
  return BiStr;
}
string decodeHuff(Hnodep origP, string BiStr, size_t i, 
		  string  mess)
{//This uses the hufman tree to decode binary string

  Hnodep newP = origP;
  while (i<BiStr.length()){
    // If char is zero go left on tree... else go right.
    if (BiStr[i] == '0'){
      newP = newP->left;
      if(!(newP->left)){
	mess+=newP->Hdata;
	newP = origP;
      }
    }
    else{
      newP = newP->right;
      if(!(newP->left)){
	mess+=newP->Hdata;
	newP = origP; 
      }
    }
    i++;
  }
  return mess;

}


Hnodep genHuff(istream & istr)
{
  
  char code = (unsigned char) istr.get();
  if(code == 'L'){
    char c = (unsigned char) istr.get();
    return new Hnode(c, 0,NULL, NULL);
  }
  else{
    Hnode *left = genHuff(istr);
    Hnode *right = genHuff(istr);
    return new Hnode(0,0,left,right);
  }

}


void outputTree(Hnodep p, ostream & ostr)
{
  if (p->left==NULL){
    ostr<<"L"<<p->Hdata;
  }
  else{
    ostr<<"I";   
    outputTree(p->left, ostr);
    outputTree(p->right, ostr);
  }
}



int main(int argc, char **argv)
{

  ifstream inFile(argv[1]);

  int bits;                    //// Getting the Bit Number
  inFile>>bits;
 
  //Creating the Huff tree from the overhead
  Hnodep head = genHuff(inFile); 
  
  //Establishing the size of ByteList based on the mod of the num of bits.
  int sizeBy;
  if (bits%8)
    sizeBy =  (bits / 8) + 1;
  else
    sizeBy = bits / 8;
  
  // Creating the byteList
  int *ByteList = new int[sizeBy];
  
  for(int i=0;i<sizeBy; i++){
    int c = (unsigned char) inFile.get();
    
    if (i>=sizeBy) break;
       
    ByteList[i] = c;           //Getting the Byte Numbers
    
  }  

  //Create the Binary String
  string BiStr = byteBite(ByteList, bits, sizeBy);
  
  string origMess = decodeHuff(head, BiStr, 0, "");
  
  string outHuff = string(argv[1]);
 
  //Getting the output file name.
  size_t pos = outHuff.find(".huf");    // position of "live" in str
  outHuff = outHuff.substr (0,pos);   // get from "live" to the end
  
  ofstream outFile(outHuff.c_str());
  
  //Write origMess to the File
  for(size_t i=0; i<origMess.length(); i++){
    outFile.put(origMess[i]);
  }

  unlink(argv[1]);
		
}
