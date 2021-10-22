#pragma once
#ifndef INDEXRECORD
#define INDEXRECORD
class indexRecord
{public:
	indexRecord(int zip, int reffield)
	{
		zipCode = zip;
		referenceField = reffield;
	}
		
	int zipCode;
	int referenceField;
};
#endif
