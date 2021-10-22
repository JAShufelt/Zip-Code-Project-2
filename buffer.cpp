#include "buffer.h"
#include <cassert>
#include <fstream>
#include <stdlib.h>
#include "indexRecord.h"
using namespace std;

string Buffer::read(string convertedfile)
{
    string line;
    ifstream upc;
    upc.open(convertedfile);
    
    getline(upc, line);
    return line;
};

void Buffer::unpack(string str, headerBuffer headerbuff, vector<string> fieldOrder)
{
    string recordData;
    
    int recordLength;
    string state;
    double lat;
    double lon;
    int zip;
    string place;
    string county;
    int fieldOrderPointer = 0;
    
    char COMMA = ',';
    string partialField = "";
    int beginning_of_Record;
    int currentRecord_Length;
    int recordPointer;
    string temp;
    int header_record_length = headerbuff.getHeaderRecordSize();
    vector<char> fieldOrder_Decoded;

    //Converting the fieldOrder into characters for use in a switch statement via fieldOrder_Decoded
    int i = 0;
    while(i < fieldOrder.size())
    {
        if (fieldOrder[i] == "\"ZipCode\"")
        {
            fieldOrder_Decoded.push_back('A');
        }
        else if (fieldOrder[i] == "\"PlaceName\"")
        {
            fieldOrder_Decoded.push_back('B');
        }
        else if (fieldOrder[i] == "State")
        {
            fieldOrder_Decoded.push_back('C');
        }
        else if (fieldOrder[i] == "County")
        {
            fieldOrder_Decoded.push_back('D');
        }
        else if (fieldOrder[i] == "Lat")
        {
            fieldOrder_Decoded.push_back('E');
        }
        else if (fieldOrder[i] == "Long")
        {
            fieldOrder_Decoded.push_back('F');
        }
        i++;
    }
    
    //Navigate past the header record to the first record
    beginning_of_Record = header_record_length;
    
    currentRecord_Length = stoi(str.substr(beginning_of_Record, 4));    //Get the length of the current record string
    temp = str.substr(beginning_of_Record, currentRecord_Length + 1);     //temp becomes the current record string
    temp = temp.substr(5, temp.size() - 1);
    
    for (int j = 0; j < headerbuff.record_Count; j++)
    {
        fieldOrderPointer = 0;                      //Reset the fieldOrderPointer to the first field (element 0 of the decoded fieldOrder vector)
        i = 0;
        while (i < temp.size())
        {
            if (temp[i] != COMMA)
            {
                partialField = partialField + temp[i];
            }
            else
            {
                if (fieldOrder_Decoded[fieldOrderPointer] == 'A')
                {
                    zip = stoi(partialField);
                    partialField = "";
                }
                if (fieldOrder_Decoded[fieldOrderPointer] == 'B')
                {
                    place = partialField;
                    partialField = "";
                }
                if (fieldOrder_Decoded[fieldOrderPointer] == 'C')
                {
                    state = partialField;
                    partialField = "";
                }
                if (fieldOrder_Decoded[fieldOrderPointer] == 'D')
                {
                    county = partialField;
                    partialField = "";
                }
                if (fieldOrder_Decoded[fieldOrderPointer] == 'E')
                {
                    lat = stod(partialField);
                    partialField = "";
                }
                if (fieldOrder_Decoded[fieldOrderPointer] == 'F')
                {
                    lon = stod(partialField);
                    partialField = "";
                }
                fieldOrderPointer++;
            }
            i++;

        }
        Record newRecord = Record(currentRecord_Length, state, lat, lon, zip, place, county);
        recordVector.push_back(newRecord);
        
        
        if (j < headerbuff.record_Count - 1)
        {
            beginning_of_Record = beginning_of_Record + currentRecord_Length + 1;
            temp = str.substr(beginning_of_Record, 100);
            currentRecord_Length = stoi(temp.substr(0, 4));
            temp = temp.substr(5, currentRecord_Length - 4);
        }
        
    }
}

vector<indexRecord> Buffer::constructIndex(headerBuffer &headerbuff)
{
    cout << "\nConstructing index... " << endl;
    
    vector<indexRecord> indexVector;
    int currentZipCode;
    int byteOffset;
    
    int beginning_of_Record = headerbuff.header_record_Size + 1;    //beginning of the first record is 1 char after the header due to comma

    for (int i = 0; i < recordVector.size(); i++) 
    {
        currentZipCode = recordVector[i].zipCode;
        byteOffset = beginning_of_Record;

        indexRecord newRecord = indexRecord(currentZipCode, byteOffset);
        indexVector.push_back(newRecord);

        if (i < recordVector.size() - 1)
        {
            beginning_of_Record = beginning_of_Record + recordVector[i].lengthIndicator + 1;
        }
    }
    return indexVector;
};

void Buffer::writeIndex(headerBuffer headerbuff, vector<indexRecord> indexVector)
{
    string line = "";
    ofstream out;
    out.open(headerbuff.index_file_Name);
    
    for (int i = 0; i <headerbuff.fieldOrder.size(); i++)
    {
        if (i < headerbuff.fieldOrder.size() - 1)
            line = line + headerbuff.fieldOrder[i] + ",";
        else
            line = line + headerbuff.fieldOrder[i];
    }
    
    out << line << endl;
    line = "";
    
    for (int i = 0; i < indexVector.size(); i++)
    {
        line = to_string(indexVector[i].zipCode) + "," + to_string(indexVector[i].referenceField);
        out << line << endl;
    }
    
    cout << "Index created in file: " << headerbuff.index_file_Name << endl;
    out.close();
}