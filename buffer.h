#ifndef BUFFER_H
#define BUFFER_H

#include <iostream>
#include <string>
#include <fstream>
#include "record.h"
#include <vector>
#include "headerBuffer.h"
#include "indexRecord.h"
using namespace std;

class Buffer
{
 public:
 vector<vector <Record> > outer;
 
 vector<Record> recordVector;

 
 Buffer()
 {
 }
/**<
 // This is a default constructor.
 // @pre There is no buffer object.
 // @post A buffer object is created with all data members initialized to null.
 */
 
 string read(string converted_file);
 
 void unpack(string str, headerBuffer headerbuff, vector<string> fieldOrder);
 
 vector<indexRecord> constructIndex(headerBuffer &headerbuff);

 void writeIndex(headerBuffer headerbuff, vector<indexRecord>);

private:
 ifstream upc;
};

#endif
