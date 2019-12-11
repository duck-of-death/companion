#include <stdio.h>
#include <stdlib.h>
#include "ace.h"

int main(int argc, char **argv)
{
	ADSHANDLE hCon;
	ADSHANDLE hTable;
	UNSIGNED8 *aucFieldName;
	UNSIGNED16 usNumFields;
	UNSIGNED16 usLength;
	UNSIGNED32 ulNumRecords;
	UNSIGNED32 ulRetCode;

	ulRetCode = AdsConnect101("Data Source=/home/john/Comp4win/; TableType=ADT; CharType=OEM; LockMode=Compatible;", NULL, &hCon);
	printf("ulRetCode = %d\n", ulRetCode);

	ulRetCode = AdsOpenTable101(hCon, "COPA.ADT", &hTable);
	printf("ulRetCode = %d\n", ulRetCode);

	ulRetCode = AdsGetNumFields(hTable, &usNumFields);
	printf("ulRetCode = %d\n", ulRetCode);
	printf("Number of fields: %d\n", usNumFields);	

	ulRetCode = AdsGetRecordCount(hTable, ADS_IGNOREFILTERS, &ulNumRecords);
	printf("ulRetCode = %d\n", ulRetCode);
	printf("Number of records: %d\n", ulNumRecords);

	// Fields start at 1
	aucFieldName = malloc(sizeof(UNSIGNED8) * 128);
	usLength = 128;
	ulRetCode = AdsGetFieldName(hTable, 1, aucFieldName, &usLength); 
	printf("ulRetCode = %d\n", ulRetCode);
	printf("Field name: %s\n", aucFieldName);
	free(aucFieldName);	


	return 0;
}
