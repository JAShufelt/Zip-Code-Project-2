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
 /**<
 // This function opens the record file, reads it, save it into memory, and
 // and returns it as a string.
 // @pre The record file exists.
 // @post The record file is saved as string in memory.
 // @param converted_file Name of the file being read.
 // @return The record file as one long string.
 */
 
 void unpack(string str, headerBuffer headerbuff, vector<string> fieldOrder);
 /**<
 // This function goes through the string made in read() and looks at the
 // header. The metadata is saved as corresponding characters to be used in
 // a switch-case later. Looking at the order of types of data, this function
 // reads each data member, determines its length, and creates a new, length-
 // indicated record file.
 // @pre Original record file exists as strinbg in memory.
 // @post Record now exists as new, length-indicated file.
 // @param str The record string in memory.
 // @param headerbuff Data of type headerBuffer used to determine size.
 // @param fieldOrder A string vector that holds the order of the data.
 */
 
 vector<indexRecord> constructIndex(headerBuffer &headerbuff);
 /**<
 // This function goes through the length indicated record file, extracts every
 // zip code, and lists each zip code next to its byte offset.
 // @pre Length-indicated record exists.
 // @post An index file exists in memory in the form of a vector.
 // @param headerbuff A pointer to the original file header.
 // @return A vector containing the index
 */

 void writeIndex(headerBuffer headerbuff, vector<indexRecord>);
/**<
// This function takes the index vector from constructIndex(), and writes it
// into an index file. It uses the length indicated by the headerbuff to
// determine which number are zip codes and which are byte offsets. It writes
// into the idex file with the form ZIPCODE,BYTEOFFSET.
// @pre There is a vector in memory containing the index.
// @post There is an index file.
// @param headerbuff Shows the length of the data.
// @param indexVector Holds the index in memory.
*/
private:
 ifstream upc;
};

#endif
