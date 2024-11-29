#include "catalog.h"
#include "query.h"


/*
 * Inserts a record into the specified relation.
 *
 * Returns:
 * 	OK on success
 * 	an error code otherwise
 */

const Status QU_Insert(const string & relation, 
	const int attrCnt, 
	const attrInfo attrList[])
{
	Status status;
	AttrDesc* attrs;
	InsertFileScan* ifs = new InsertFileScan(relation, status);
	if(status != OK) return status;
	int cnt;
	if((status = attrCat->getRelInfo (relation, cnt, attrs)) != OK) return status;

	int recLen = 0;
	for(int j=0; j<cnt; j++){
		recLen += attrs[j].attrLen;
	}

	char newList[recLen];
	int intVal;
    float floatVal;
	
	for(int i=0; i<attrCnt; i++){
		if(attrList[i].attrValue == NULL) return BADCATPARM;

		for(int j=0; j<cnt; j++){
			if(strcmp(attrList[i].attrName, attrs[j].attrName) == 0){

				if(attrList[i].attrType == INTEGER){
            		intVal = atoi((char*)attrList[i].attrValue);
					memcpy(&newList[attrs[j].attrOffset], &intVal, attrs[j].attrLen);
        		}else if(attrList[i].attrType == FLOAT){
            		floatVal = atof((char*)attrList[i].attrValue);
            		memcpy(&newList[attrs[j].attrOffset], &floatVal, attrs[j].attrLen);
    			}else{
					memcpy(&newList[attrs[j].attrOffset], attrList[i].attrValue, attrs[j].attrLen);
    			}
				
				break;
			}

		}
	}

	RID rid;
	Record rec = {newList, recLen};
    status = ifs->insertRecord(rec, rid);

	return OK;

}

