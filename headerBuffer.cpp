#include "headerBuffer.h"
#include <cassert>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

//Converts the original CSV file into a Length-Indicated CSV file
string headerBuffer::convert(string filename)
{
	string temp_filename = filename;	//Name of the original CSV, passed by commandline in main
	
	temp_filename.pop_back();		//This removes the ".csv" from the filename
	temp_filename.pop_back();		//
	temp_filename.pop_back();		//
	temp_filename.pop_back();		//
	
	string converted_filename = temp_filename + "_LENGTH-INDICATED";	//This adds "_LENGTH-INDICATED" to the file name. For example "us_postal_codes.csv"
	int stringLength;													//would become "us_postal_codes_LENGTH-INDICATED" (generic file type)
	string line = "";
	string temp, recordLength;
	ifstream upc;
	ofstream out;
	upc.open(filename);
	out.open(converted_filename);
	for (int i = 0; i < 3; i++)
		getline(upc, temp);

	cout << "Please wait while converting "<< filename <<" to a length indicated record format..." << endl;
	while (!upc.eof())
	{
		getline(upc, temp);		//grab a line of text from upc
		stringLength = temp.size() + 5;		//get the size of the current line in upc and add 5 (for the 4 character length indicator, plus a comma)
		recordLength = to_string(stringLength);		//recordLength = number of characters in the line plus the "self counting" length indicator

		if (temp != "")
		{
			for (int i = (to_string(stringLength)).size(); i < 4; i++)	//Process for creating the formatted "self counting" length indicator
			{
				recordLength = "0" + recordLength;
			}

			line = line + recordLength + "," + temp + ",";		//line gains the current record beginning with its "self counting" length indicator
		}
	}

	out << line;	//Write the line to a file
	upc.close();	//Close the file
	out.close();	//Close the file
	cout << "CSV successfully converted to file: " << converted_filename << endl;

	return converted_filename;
}

void headerBuffer::read(string filename)
{
	//Hardcoded Header Attributes
	structure_Type = "Length-Indicated Records";
	file_Version = 1.0;
	bytes_per_record_Size = 4;
	size_Format = "ASCII";
	index_Schema = "Zip-Code|Reference field";

	//Dynamic Header Attributes
	header_record_Size = 0;
	index_file_Name = "";
	record_Count = 0;
	fields_Count = 0;
	primary_Key = 0;

	char COMMA = ',';
	string temp;
	string original_header = "";
	
	ifstream upc;
	upc.open(filename);
	for (int i = 0; i < 3; i++)	//Reads the original CSV header (3 lines) and stores into one string "original_header"
	{
		getline(upc, temp);
		original_header = original_header + temp;
	}
	upc.close();

	for (int i = 0; i < original_header.size(); i++)	//Calculates how many unique fields there are and stores in int fields_Count
	{
		if (original_header[i] == COMMA)
		{
			fields_Count++;
		}
	}
	fields_Count++;

	upc.open(filename);	//Calculates how many records there are by reading each line of the CSV and storing it in int record_Count
	while (!upc.eof())
	{
		getline(upc,temp);
		record_Count++;
	}
	record_Count -= 4;	//Subtracting 4 from total number of lines in the CSV (3 Lines are header, and the last line is empty and doesn't contain a record)
	upc.close();

	index_file_Name = filename;			//This block dynamically creates an index file name for the given CSV
	index_file_Name.pop_back();
	index_file_Name.pop_back();
	index_file_Name.pop_back();
	index_file_Name.pop_back();
	index_file_Name += "_INDEX";	//Index file name is the name of the CSV file + "_INDEX" Example "us_postal_codes_INDEX" generic file type

	temp = "";
	for(int i = 0; i < original_header.size(); i++)	//Establishes the order of the fields into the field_Order vector
	{
		if (original_header[i] != COMMA)
		{
			temp = temp + original_header[i];
		}
		else
		{
			fieldOrder.push_back(temp);
			temp = "";
		}
	}
	fieldOrder.push_back(temp);	//Pushes the last field into the field_order vector

	string ZIPCODE = "\"ZipCode\"";		
	for (int i = 0; i < fieldOrder.size(); i++)	//Determines the primary key ordinal location ("ZipCode")
	{
		if (fieldOrder[i] == ZIPCODE)
		{
			primary_Key = (i + 1);
		}
	}

	temp = "";
	for (int i = 0; i < fieldOrder.size(); i++)		//Determines the order of the fields and prints them in the same order within the header. This is for
													//the header fields and schema, aswell as the Index File Header
	{
		if (fieldOrder[i] == "\"ZipCode\"")
			temp = temp + fieldOrder[i] + " -integer" + COMMA;
		else if (fieldOrder[i] == "\"PlaceName\"")
			temp = temp + fieldOrder[i] + " -string" + COMMA;
		else if (fieldOrder[i] == "State")
			temp = temp + fieldOrder[i] + " -string" + COMMA;
		else if (fieldOrder[i] == "County")
			temp = temp + fieldOrder[i] + "-string" + COMMA;
		else if (fieldOrder[i] == "Lat")
			temp = temp + fieldOrder[i] + " -double" + COMMA;
		else if (fieldOrder[i] == "Long")
			temp = temp + fieldOrder[i] + " -double" + COMMA;
	}
	
	string fields_schema = temp;	//Save the fields schema in proper order
	temp = "";

	//string header is the whole header without the Header Record Byte Size
	string header = "File Structure Type: " + structure_Type + ",Version: " + to_string(file_Version) + ",Record Length Indicator Byte Size: " + to_string(bytes_per_record_Size) +
		",Size Format: " + size_Format + ",Index File Name: " + index_file_Name + ",Index File Schema: " + index_Schema + ",Record Count: " + to_string(record_Count) +
		",Fields Per Record: " + to_string(fields_Count) + ",Primary Key Field (Ordinal): " + to_string(primary_Key) + ",Fields Sequentially: " + fields_schema;

	//Add the Header Record Byte Size field to the header an
	temp = "Header Record Byte Size: ";
	header_record_Size = (header.size() + 26 ) + to_string(header.size() + 26).size();
	header = temp + to_string(header_record_Size) + "," + header;

	header_record = header;	//Establishes the header record in preparation for writing
}

//Write function writes the header for the Length-Indicated CSV 
void headerBuffer::write(string filename)
{
	string contents;
	ifstream upc;
	ofstream out;

	upc.open(filename);		//Open the Length-Indicated CSV 
	getline(upc, contents);	//Getline retrieves all the contents of the CSV since it is all in one line, and stores it in string contents 
	upc.close();			//Close the file

	contents = header_record + contents;		//Add the header record to beginning of contents

	out.open(filename, ios::out | ios::trunc);	//Open the Length-Indicated CSV for writing (it is cleared by truncating function)
	out << contents;							//Rewrite the contents with the header to the Length-Indicated CSV
	out.close();								//Close the file
}

//Returns the length of the header record of the Length-Indicated CSV
int headerBuffer::getHeaderRecordSize()
{
	return header_record_Size;
}

