#include <iostream>
#include <fstream>
#include <string>
#include "buffer.h"
#include "record.h"
#include "headerBuffer.h"
#include "indexRecord.h"
using namespace std;

int main(int arc, char* argv[])
{
    string filename = argv[1];  //Sets filename from the command line argument
    string converted_file;      //Variable for Length-Indicated CSV filename
    Buffer recordBuff;          //Create a record buffer
    headerBuffer headerBuff;    //Create a header record buffer
    
    converted_file = headerBuff.convert(filename);  //Convert the file into a Length-Indicated CSV
    headerBuff.read(filename);  //Header record buffer reads the original CSV and prepares the appropriate header record for the new Length-Indicated CSV
    headerBuff.write(converted_file);   //Add the header record to the Length-Indicated CSV
 
    recordBuff.unpack(recordBuff.read(converted_file), headerBuff, headerBuff.fieldOrder);  //Open the length indicated CSV, and unpack records into objects
    recordBuff.writeIndex(headerBuff, recordBuff.constructIndex(headerBuff));   //Create a corresponding index for the Length-Indicated CSV, and write it to an index file
    cout << "Initialization Done" << endl;
}
/**<
// This is a main function that simply creates all of the structures needed to execute the program.
// This does not impliment any of the dynamic elements exect for the file name.
// @pre A zip code CSV file exists.
// @post A new length-indicated version of the file and an inex file exist.
// @param arc the number of characters in the file name.
// @param argv[] The file name stored as an array of characters.
*/
