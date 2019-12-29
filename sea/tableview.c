#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include "ace.h"

void err_check(UNSIGNED32 ulRetCode)
{
	if (ulRetCode != AE_SUCCESS)
	{
		printf("Error code: %d\n", ulRetCode);
		exit(1);
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

	DIR *compdir;
	struct dirent *dirlist;
	char *connStr;
	size_t filenameLen;

	if (argc != 2)
	{
		fprintf(stderr, "Error: Missing COMP ADT directory\n");
		exit(1);
	}
	else
	{
        	compdir = opendir(argv[1]);
		asprintf(&connStr, "Data Source=%s; TableType=ADT, CharType=OEM; LockMode=Compatible", argv[1]);
        	if (compdir == NULL)
        	{
            		fprintf(stderr, "Error: opendir() failed on %s\n", argv[1]);
            		exit(1);
        	}
	}
	printf("Connection string: %s\n", connStr);

	// Making a free connection
	ulRetCode = AdsConnect101(connStr, NULL, &hCon);
	err_check(ulRetCode);

	// Directory stuff needs to go here
	while ((dirlist = readdir(compdir)) != NULL)
	{
		filenameLen = strlen(dirlist->d_name);
		if (filenameLen > 4)
		{
			if (strncmp(dirlist->d_name + (filenameLen - 4), ".ADT", filenameLen) == 0)
			{
				printf("d_name = %s\n", dirlist->d_name);
				ulRetCode = AdsOpenTable101(hCon, dirlist->d_name, &hTable);
				err_check(ulRetCode);

				printf("TABLE:%s:", dirlist->d_name);
				ulRetCode = AdsGetNumFields(hTable, &usNumFields);
				err_check(ulRetCode);
				printf("F:%d:", usNumFields);	

				ulRetCode = AdsGetRecordCount(hTable, ADS_IGNOREFILTERS, &ulNumRecords);
				err_check(ulRetCode);
				printf("R:%d\n", ulNumRecords);

				aucFieldName = malloc(sizeof(UNSIGNED8) * 128);

				for (usFieldNum = 1; usFieldNum <= usNumFields; usFieldNum++)
				{
					// Fields start at 1
					usLength = 128;
					ulRetCode = AdsGetFieldName(hTable, usFieldNum, aucFieldName, &usLength); 
					err_check(ulRetCode);
					printf("\tFIELD:%s:", aucFieldName);
					
					ulRetCode = AdsGetFieldType(hTable, aucFieldName, &usFieldType);
					err_check(ulRetCode);
					type_check(usFieldType);
				}

				free(aucFieldName);
			}
		}
	}
	
	free(connStr);
	return 0;
}
