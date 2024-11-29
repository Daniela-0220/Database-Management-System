#include "catalog.h"
#include "query.h"


// forward declaration
const Status ScanSelect(const string & result, 
            const int projCnt, 
            const AttrDesc projNames[],
            const AttrDesc *attrDesc, 
            const Operator op, 
            const char *filter,
            const int reclen);

/*
 * Selects records from the specified relation.
 *
 * Returns:
 *  OK on success
 *  an error code otherwise
 */

const Status QU_Select(const string & result, 
               const int projCnt, 
               const attrInfo projNames[],
               const attrInfo *attr, 
               const Operator op, 
               const char *attrValue)
{
   // Qu_Select sets up things and then calls ScanSelect to do the actual work
    cout << "Doing QU_Select " << endl;
    Status status = OK;
    AttrDesc attrDescNames[projCnt];
    AttrDesc attrDesc;
    int reclen = 0;
    int intVal;
    float floatVal;

    // check names
    for(int i = 0; i < projCnt; i++){
        if((status = attrCat->getInfo(string(projNames[i].relName), string(projNames[i].attrName), attrDescNames[i])) != OK) return status;
        reclen += attrDescNames[i].attrLen;
    }
    // check attr
    if(attr != NULL){
        if ((status = attrCat->getInfo(string(attr->relName), string(attr->attrName), attrDesc)) != OK) return status;

        if(attr->attrType == INTEGER){
            intVal = atoi(attrValue);
            attrValue = (char *)&intVal;
        }else if(attr->attrType == FLOAT){
            floatVal = atof(attrValue);
            attrValue = (char *)&floatVal;
        }
    }else{
        attrDesc.attrOffset = 0;
        attrDesc.attrLen = 0;
        attrDesc.attrType = STRING;
        attrValue = NULL;
    }


    return ScanSelect(result, projCnt, attrDescNames, &attrDesc, op, attrValue, reclen);
}


const Status ScanSelect(const string & result, 
#include "stdio.h"
#include "stdlib.h"
            const int projCnt, 
            const AttrDesc projNames[],
            const AttrDesc *attrDesc, 
            const Operator op, 
            const char *filter,
            const int reclen)
{
    cout << "Doing HeapFileScan Selection using ScanSelect()" << endl;
    Status status = OK;

    HeapFileScan hfs(projNames[0].relName, status);
    if(status != OK) return status;

    // build result relation
    InsertFileScan ifs(result, status);
    if(status != OK) return status;
    
    status = hfs.startScan(attrDesc->attrOffset, attrDesc->attrLen, (Datatype) attrDesc->attrType, filter, op);
    if(status != OK) return status;

    RID rid;
    Record record;
    RID outputRid;
    char outputData[reclen];
    Record output;
    output.length = reclen;
    output.data = (void *) outputData;

    while ((status = hfs.scanNext(rid)) == OK){
        if((status = hfs.getRecord(record)) != OK) return status;

        //copy each attr
        int outputOffset = 0;
        for (int i = 0; i < projCnt; i++) {
            memcpy(outputData + outputOffset, (char *)record.data + projNames[i].attrOffset, projNames[i].attrLen); 
            outputOffset += projNames[i].attrLen;
        }
        if((status = ifs.insertRecord(output, outputRid)) != OK) return status;
    }

    if((status = hfs.endScan()) != OK) return status;
    return OK;
}