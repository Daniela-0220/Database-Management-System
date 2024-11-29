#include "catalog.h"
#include "query.h"


/*
 * Deletes records from a specified relation.
 *
 * Returns:
 * 	OK on success
 * 	an error code otherwise
 */

const Status QU_Delete(const string & relation, 
		       const string & attrName, 
		       const Operator op,
		       const Datatype type, 
		       const char *attrValue)
{
	// part 6
	Status status;
	AttrDesc record;
	RID rid;
	HeapFileScan *hfs;

	int offset = 0;
	int length = 0;
	int intValue;
	float floatValue;

	// Construct heap file scan
	hfs = new HeapFileScan(relation, status);
  	if (status != OK)
		return status;
	
	// Preparation for scan
	if (attrName.empty())
	{
		if (hfs->startScan(offset, length, STRING, NULL, EQ) != OK)
			return status;
	}
	else
	{
		// Get attribute data record
		if ((status = attrCat->getInfo(relation, attrName, record)) != OK)
			return status;

		offset = record.attrOffset;
		length = record.attrLen;

		switch (type)
		{
		case STRING:
			status = hfs->startScan(offset, length, type, attrValue, op);
			break;

		case INTEGER:
			intValue = atoi(attrValue);
			status = hfs->startScan(offset, length, type, (char *)&intValue, op);
			break;

		case FLOAT:
			floatValue = atof(attrValue);
			status = hfs->startScan(offset, length, type, (char *)&floatValue, op);
			break;
		}

		if (status != OK)
		{
			delete hfs;
			return status;
		}
	}

	// Implement scan logic
	while ((status = hfs->scanNext(rid)) != FILEEOF)
	{
		// Scan
		if (status != OK)
		{
			hfs->endScan();
			delete hfs;
			return status;
		}

		hfs->deleteRecord();
	}
	
	hfs->endScan();
	delete hfs;

	return OK;
}
