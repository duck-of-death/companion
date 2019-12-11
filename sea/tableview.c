#include <stdio.h>
#include "ace.h"

int main(int argc, char **argv)
{
	ADSHANDLE hCon;
	ADSHANDLE hTable;
	UNSIGNED16 usNumFields;
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



	return 0;
}
