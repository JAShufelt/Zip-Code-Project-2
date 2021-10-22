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
	
	void read(string filename);

	void write(string filename);

	int getHeaderRecordSize();

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
