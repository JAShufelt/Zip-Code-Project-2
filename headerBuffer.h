#pragma once
#ifndef HEADERBUFFER_H
#define HEADERBUFFER_H
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;
class headerBuffer
{
public:
	string convert(string filename);
/**<
// This function converts the original file into a length-indicated CSV
// file. First, it creates a new name for the length-indicated file. Then,
// it gets the data from the original file and adds a self-counting length
// indicator and a comma. Finally, it writes everything to the file and
// closes it.
// @pre There is a CSV file.
// @post A length-indicated version of the file exists.
// @param filename A string containing the original file's name.
// @return The file name of the converted file.
*/
	
	void read(string filename);
/**<
// This function uses the original file's header to help organize all of the
// files that spawn from it. It creates new headers, a vector containing the
// field order, a name for the index file, and establishes the header
// record.
// @pre A zip code CSV file exists.
// @post all new headers, file names, and field orders are established.
// @param filename A string containing the original file's name.
*/

	void write(string filename);
/**<
// This function writes the header at the top of the length-indicated file.
// @pre The length-indicated file exists and a vector contains the field order.
// @post A header is written at the top of the length-indicated file.
// @param filename A string containing the name of the length-indicated file.

	int getHeaderRecordSize();
/**<
// This function returns the size of the header record.
// @pre Header Record exists.
// @post Size of header record is returned.
// @return Size of the header record as integer.

public:
	string structure_Type;	//Length-Indicated Records
	double file_Version;	//1.0
	int header_record_Size;	//Discovered Dynamically while reading
	int bytes_per_record_Size;	//4
	string size_Format;	//ASCII
	string index_file_Name;	//Discovered Dynamically while reading
	string index_Schema;	//"Zip-Code"|Reference field
	int record_Count;	//Discovered Dynamically while reading
	int fields_Count;	//Discovered Dynamically while reading
	int primary_Key;	//Discovered Dynamically while reading

	string header_record;	//Discovered Dynamically while reading
	vector<string> fieldOrder;	//Constructed while reading for maintaining field order.
};
#endif
