#include <stdio.h>
#include <stdlib.h>
#include "ace.h"

void err_check(UNSIGNED32 ulRetCode)
{
	if (ulRetCode != AE_SUCCESS)
	{
		printf("Error code: %d\n", ulRetCode);
	}
}

void type_check(UNSIGNED16 usFieldType)
{
	if (usFieldType == ADS_STRING)
	{
		printf("Character data\n");
	}	
	else if (usFieldType == ADS_DATE)
	{
		printf("Date field. 4 byte Julian date\n");
	}
	else if (usFieldType == ADS_LOGICAL)
	{
		printf("1 byte logical value\n"); 
	}
	else if (usFieldType == ADS_SHORTINT)
	{
		printf("IEEE 2 byte signed short integer\n");
	}
	else if (usFieldType == ADS_MEMO)
	{
		printf("Variable length character field\n");
	}
	else if (usFieldType == ADS_DOUBLE)
	{
		printf("IEEE 8 byte floating point\n");
	}
	else if (usFieldType == ADS_INTEGER)
	{
		printf("IEEE 4 byte long integer\n");
	}
	else if (usFieldType == ADS_IMAGE)
	{
		printf("BLOB - bitmap\n");
	}
	else
	{
		printf("Unknown type: %d\n", usFieldType);
	}
}

int main(int argc, char **argv)
{
	ADSHANDLE hCon;
	ADSHANDLE hTable;
	UNSIGNED8 *aucFieldName;
	UNSIGNED16 usNumFields;
	UNSIGNED16 usFieldNum;
	UNSIGNED16 usLength;
	UNSIGNED16 usFieldType;
	UNSIGNED32 ulNumRecords;
	UNSIGNED32 ulRetCode;

	if (argc != 2)
	{
		fprintf(stderr, "Error: ADT filename argument\n");
		exit(1);
	}

	ulRetCode = AdsConnect101("Data Source=/home/john/Comp4win/; TableType=ADT; CharType=OEM; LockMode=Compatible;", NULL, &hCon);
	err_check(ulRetCode);

	ulRetCode = AdsOpenTable101(hCon, argv[1], &hTable);
	err_check(ulRetCode);

	ulRetCode = AdsGetNumFields(hTable, &usNumFields);
	err_check(ulRetCode);
	printf("Number of fields: %d\n", usNumFields);	

	ulRetCode = AdsGetRecordCount(hTable, ADS_IGNOREFILTERS, &ulNumRecords);
	err_check(ulRetCode);
	printf("Number of records: %d\n", ulNumRecords);

	aucFieldName = malloc(sizeof(UNSIGNED8) * 128);

	for (usFieldNum = 1; usFieldNum <= usNumFields; usFieldNum++)
	{
		// Fields start at 1
		usLength = 128;
		ulRetCode = AdsGetFieldName(hTable, usFieldNum, aucFieldName, &usLength); 
		err_check(ulRetCode);
		printf("FIELD:%s:", aucFieldName);
		
		ulRetCode = AdsGetFieldType(hTable, aucFieldName, &usFieldType);
		err_check(ulRetCode);
		type_check(usFieldType);
	}

	free(aucFieldName);

	return 0;
}
