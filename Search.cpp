// Search.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <fstream>
#include <iostream>
#include <iostream>
#include <string>
#include <vector>
#include "record.h"
#include "indexRecord.h";

using namespace std;

indexRecord readline(string line);
/**<
// This function receives a line from the index file and
// returns an indexRecord object with the same data.
// @pre There is an index file.
// @post index file line is constructed into an object form.
// @param line string containing a line of the index file.
// @return indexRecord
*/

void search_indexVector(int argc, char* argv[], string data, vector <indexRecord> indexVector, vector <string> fieldOrder, vector<string> &not_foundRecords, vector<Record> &foundRecords);
/**<
// This function searches the length-indicated CSV for all zip codes provided
// via command line arguments. If the zip code is found, it is added to the foundRecords vector.
// If a zipcode is not found, it is added to the not_foundRecords vector. Any invalid input is
// ignored. 
// @pre There is a length-indicated CSV file and corresponding index file
// @post All zip codes being searched via command line argument are sorted in the the respective
// foundRecords or not_foundRecords vector.
// @param argc int count of command line arguments.
// @param argv char* of command line arguments.
// @param data string containing data of whole length-indicated file. 
// @param indexVector vector <indexRecord> vector containing all of the indexRecord objects from the index.
// @param fieldOrder vector <string> containing the fields of the CSV in sequential order.
// @param &not_foundRecords vector<string> containing the string representation of zip codes not found via the search.
// @param &foundRecords vector<Record> containing the record objects of zip codes found via search.
// @return void
*/

Record unpackRecord(vector <string> fieldOrder, string line);
/**<
// This function takes in a whole record (without length-indicator) as a string
// and constructs a corresponding Record object.
// @pre During a search, the zip code was found in the index primary key.
// @post a record data line is converted to an object verison and returned.
// @param fieldOrder vector <string> containing the fields of the CSV in sequential order.
// @param line string containing the record data from the length-indicated CSV.
// @return Record
*/

int main(int argc, char* argv[])
{
	vector<string> not_foundRecords;
	vector<Record> foundRecords;
	
	string data;
	string test = "501,Holtsville,NY,Suffolk,40.8154,-73.0451,";
	string datafile = argv[1];
	
	string indexfile = argv[1];
	for(int i = 0; i < 4; i++)
	{
		indexfile.pop_back();
	}
	indexfile += "_INDEX";

	for (int i = 0; i < 4; i++)
	{
		datafile.pop_back();
	}
	datafile += "_LENGTH-INDICATED";

	const char COMMA = ',';
	string indexHeader,temp;
    ifstream readIndex,readData;
    readIndex.open(indexfile);

	if(!readIndex.good())	//End the program if filename is not a real file.
	{
		cout << "No file named : " << datafile << " exists..." << endl;
		cout << "\nPlease enter the correct filename as your first argument and -Z for each zip code you'd like to search for." << endl;
		cout << "Example: us_postal_codes.csv -Z55449 -Z55014 -Z56301" << endl;
		return 0;
	}

    getline(readIndex, indexHeader);

    vector <string> fieldOrder;
	vector <indexRecord> indexVector;
	
	temp = "";
	for (int i = 0; i < indexHeader.size(); i++)	//Establishes the order of the fields into the field_Order vector
	{
		if (indexHeader[i] != COMMA)
		{
			temp = temp + indexHeader[i];
		}
		else
		{
			fieldOrder.push_back(temp);
			temp = "";
		}
	}
	fieldOrder.push_back(temp);

	while(!readIndex.eof())		//Unpacks the indexRecords into memory via "indexVector"
	{
		getline(readIndex, temp);
		if(temp != "")
		{
			indexVector.push_back(readline(temp));
		}
		
	}
	
	readIndex.close();
	readData.open(datafile);
	getline(readData, data);
	
	search_indexVector(argc, argv, data, indexVector, fieldOrder, not_foundRecords, foundRecords);	//Search the indexVector and unpack any records it found into foundRecords
																									//Any records not found are added as strings to the non_foundRecords

	if (foundRecords.size() > 0)
	{
		cout << "\n--------------------------------------Found Records-----------------------------------------------------\n";
		cout << "Zip Code\tPlace\t\tState\t\tCounty\t\tLatitude\tLongitude\n";
		cout << "--------------------------------------------------------------------------------------------------------\n";

		for(int i = 0; i < foundRecords.size(); i++)
		{
			cout << foundRecords[i].zipCode << "\t\t" << foundRecords[i].placeName << "\t" << foundRecords[i].state << "\t\t";
			cout << foundRecords[i].county << "\t\t" << foundRecords[i].lat << "\t\t" << foundRecords[i].lon << "\n";
		}
	}
	
	if (not_foundRecords.size() > 0)
	{
		cout << "\n\n-------------------------------------Records Not Found--------------------------------------------------\n";
		cout << "Zip Code\n";
		cout << "--------------------------------------------------------------------------------------------------------\n";

		for(int i = 0; i < not_foundRecords.size(); i++)
		{
			cout << not_foundRecords[i].substr(2, not_foundRecords[i].size()-2) << endl;
		}
	}

}

indexRecord readline(string line)
{
	int byteOffset = 10;
	int zipCode;
	string temp = "";
	const char COMMA = ',';

	for (int i = 0; i < line.size(); i++)
	{
		if (line[i] != COMMA)
		{
			temp = temp + line[i];
		}
		else
		{
			zipCode = stoi(temp);
			byteOffset = stoi(line.substr(i + 1, line.size()-i));
			break;
		}
	}

	indexRecord new_index_record = indexRecord(zipCode, byteOffset);
	return new_index_record;
}

void search_indexVector(int argc, char* argv[], string data, vector <indexRecord> indexVector, vector <string> fieldOrder, vector<string>& not_foundRecords, vector<Record>& foundRecords)
{
	bool found = false;
	int searchingZip;
	string current_argument;

	for(int i = 2; i < argc; i++)
	{
		current_argument = argv[i];
		cout << "Searching for argument: " << argv[i] << endl;

		
		if (current_argument.size() >= 3 && current_argument.substr(0, 2) == "-Z")	//Check if the argument is a valid search argument, otherwise it is ignored.
		{
			searchingZip = stoi(current_argument.substr(2, current_argument.size() - 2));	//Argument is valid, remove the flag -Z
			int recordLength = 0;															//Initialize record length to 0
			string temp = "";																//temporary string initialized
			
			for(int j = 0; j < indexVector.size(); j++)
			{
				if(searchingZip == indexVector[j].zipCode)
				{
					found = true;
					recordLength = stoi(data.substr(indexVector[j].referenceField, 4));
					temp = data.substr(indexVector[j].referenceField + 4, recordLength - 4);
					foundRecords.push_back(unpackRecord(fieldOrder, temp));
					break;
				}
			}
			if(found == false)
			{
				not_foundRecords.push_back(argv[i]);
			}
			found = false;
		}
		
	}
	
}

Record unpackRecord(vector <string> fieldOrder, string line)
{
	int zipCode;
	string placeName, county, state;
	double lat, lon;
	
	
	int fieldNumber = 0;
	string temp = "";
	char const COMMA = ',';
	
	for (int i = 0; i < line.size(); i++)
	{
		if(line[i] != COMMA)
		{
			temp = temp + line[i];
		}
		else
		{
			if (fieldOrder[fieldNumber] == "\"ZipCode\"")
			{
				zipCode = stoi(temp);
			}
			else if (fieldOrder[fieldNumber] == "\"PlaceName\"")
			{
				placeName = temp;
			}
			else if (fieldOrder[fieldNumber] == "State")
			{
				state = temp;
			}
			else if (fieldOrder[fieldNumber] == "County")
			{
				county = temp;
			}
			else if (fieldOrder[fieldNumber] == "Lat")
			{
				lat = stod(temp);
			}
			else if (fieldOrder[fieldNumber] == "Long")
			{
				lon = stod(temp);
			}
			temp = "";
			fieldNumber++;
		}
	}

	Record newRecord = Record(0, state, lat, lon, zipCode, placeName, county);
	return newRecord;
}
