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
		//printf("Character data\n");
		printf("TEXT");
	}	
	else if (usFieldType == ADS_DATE)
	{
		// printf("Date field. 4 byte Julian date\n");
		printf("TEXT");

	}
	else if (usFieldType == ADS_LOGICAL)
	{
		//printf("1 byte logical value\n"); 
		printf("NUMERIC");
	}
	else if (usFieldType == ADS_SHORTINT)
	{
		//printf("IEEE 2 byte signed short integer\n");
		printf("INTEGER");
	}
	else if (usFieldType == ADS_MEMO)
	{
		//printf("Variable length character field\n");
		printf("TEXT");
	}
	else if (usFieldType == ADS_DOUBLE)
	{
		//printf("IEEE 8 byte floating point\n");
		printf("REAL");
	}
	else if (usFieldType == ADS_INTEGER)
	{
		//printf("IEEE 4 byte long integer\n");
		printf("INTEGER");
	}
	else if (usFieldType == ADS_IMAGE)
	{
		//printf("BLOB - bitmap\n");
		printf("BLOB");
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
	UNSIGNED8 *pucTempRecord; 
	UNSIGNED8 *pucBuf;
	UNSIGNED16 usNumFields;
	UNSIGNED16 usFieldNum;
	UNSIGNED16 usLength;
	UNSIGNED16 usFieldType;
	UNSIGNED32 ulNumRecords;
	UNSIGNED32 ulRetCode;
	UNSIGNED32 ulRecSize;
	UNSIGNED32 pulLength;

	DIR *compdir;
	struct dirent *dirlist;
	char *connStr;
	char *tblName;
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
	// printf("Connection string: %s\n", connStr);

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
				ulRetCode = AdsOpenTable101(hCon, dirlist->d_name, &hTable);
				err_check(ulRetCode);

				tblName = malloc((filenameLen - 3) * sizeof(char));
				strncpy(tblName, dirlist->d_name, (filenameLen - 3));
				tblName[filenameLen - 4] = '\0';

				printf("CREATE TABLE %s(", tblName);

				ulRetCode = AdsGetNumFields(hTable, &usNumFields);
				err_check(ulRetCode);

				aucFieldName = malloc(sizeof(UNSIGNED8) * 128);

				for (usFieldNum = 1; usFieldNum <= usNumFields; usFieldNum++)
				{
					// Fields start at 1
					if (usFieldNum != 1)
					{
						printf(", ");
					}
					usLength = 128;
					ulRetCode = AdsGetFieldName(hTable, usFieldNum, aucFieldName, &usLength); 
					err_check(ulRetCode);
					printf("\"%s\" ", aucFieldName);
					
					ulRetCode = AdsGetFieldType(hTable, aucFieldName, &usFieldType);
					err_check(ulRetCode);
					type_check(usFieldType);
				}
				printf(");\n");

				// Record Stuff... 
				// Probably could combine with for loop above and call a function and use asprintf

				ulRetCode = AdsGetRecordCount(hTable, ADS_IGNOREFILTERS, &ulNumRecords);
				err_check(ulRetCode);

				if (ulNumRecords >= 1)
				{
					AdsGotoRecord(hTable, 1);

					ulRetCode = AdsGetFieldLength(hTable, aucFieldName, &pulLength);
					err_check(ulRetCode);

					printf("%s = len(%d)\n", aucFieldName, pulLength);

					pulLength =+ 512;
					ulRetCode = AdsGetField(hTable, aucFieldName, pucBuf, &pulLength, ADS_TRIM);
					err_check(ulRetCode);

					printf("\"%s\"\n", pucBuf);
				}

				free(aucFieldName);

				ulRetCode = AdsCloseTable(hTable);
				err_check(ulRetCode);
				free(tblName);
			}
		}
	}

	ulRetCode = AdsDisconnect(hCon);
	err_check(ulRetCode);
	free(compdir);	
	free(connStr);
	return 0;
}
