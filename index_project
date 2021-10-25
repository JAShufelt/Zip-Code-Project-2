#include <iostream> 
using namespace std;
#include <string>

void searchZipCode(string indexFileName, string csvFileName, int argc, string** zCodeFlag)
{
	ifstream indexFile;
	indexFile.open(indexFileName.c_str());
	ifstream csvFile;
	csvFile.open(csvFileName.c_str());
	string index, csv;
	getline(csvFile, csv);
	getline(indexFile, index);
	bool exist;
	record zipInfo;

	if (argc < 1)
		cout << "Please enter the number of zip codes being searched as the third parameter." << endl;
	for (int i = 0; i < argc; i++)
	{
		exist = allowed(zCodeFlag[i], i + 1, argc);
		if (exist == true)
		{
			int** zCode = zCodeFlag[i].substr(2, zCodeFlag[i].size() - 2);
			zipInfo = searchAlgorithm(zCode, index, csv);
			if (zipInfo != NULL)
				cout << "Zip Code\tPlace Name\tState\tCounty\tLat\tLong " << endl << zipInfo.zipCode << "\t\t" << zipInfo.placeName << "\t\t" << zipInfo.state << "\t\t" << zipInfo.county << "\t\t" << zipInfo.lat << "\t\t" << zipInfo.lon << endl;
			else
				cout << zCode << " does not exist." << endl;
		}
	}
};

/**<
// This function is to read the zip codes in order to determine if they're in the proper format. 
*/
	bool allowed(string zCodeFlag, int i, int argc)
	{
		string flag = zCodeFlag.substr(0, 1);
		if ((flag =! '-z' or '-Z'))
		{
			cout << "Please enter flag " << i << " in the form -Z(zipcode)" << endl;
			return false;
		}
		return true;
	};

/**<
// This function
*/

record searchAlgorithm(int zCode, string index, string csv)
{
	int n = 0, m = 0, possibleZip;
	string tempString = "", str = "", temp;
	char comma = ",";
	while (!eol) {
		while (csv[n] != comma) {
			tempString += str[n];
			n++;
