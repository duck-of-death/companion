#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include "ace.h"

// Check for errors from libace functions
void err_check(UNSIGNED32 ulRetCode)
{
	if (ulRetCode != AE_SUCCESS)
	{
		printf("Error code: %d\n", ulRetCode);
		exit(1);
	}
}

// Print the closest SQLite datatype for a given ADS type
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

void cleanbuf(UNSIGNED8 *buffer)
{
	int i = 0;

	while (buffer[i] != '\0')
	{
		if (buffer[i] == 0x22)
		{
			// fprintf(stderr, "Found \", replacing with \'\n");
			buffer[i] = 0x27;
		}

		i++;
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
	UNSIGNED32 ulRecNum;

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

	printf("BEGIN TRANSACTION;\n");

	// Directory stuff needs to go here
	while ((dirlist = readdir(compdir)) != NULL)
	{
		// File name length has to be more than 4 to be checked if it
		// ends in ".ADT"
		filenameLen = strlen(dirlist->d_name);
		if (filenameLen > 4)
		{
			// Does the filename end in ADT?
			if (strncmp(dirlist->d_name + (filenameLen - 4), ".ADT", filenameLen) == 0 || 
					strncmp(dirlist->d_name + (filenameLen - 4), ".adt", filenameLen) == 0)
			{
				ulRetCode = AdsOpenTable101(hCon, dirlist->d_name, &hTable);
				err_check(ulRetCode);

				tblName = malloc((filenameLen - 3) * sizeof(char));
				strncpy(tblName, dirlist->d_name, (filenameLen - 3));
				tblName[filenameLen - 4] = '\0';

				printf("CREATE TABLE %s(", tblName);

				ulRetCode = AdsGetNumFields(hTable, &usNumFields);
				err_check(ulRetCode);

				// Max field length is 128 characters

				for (usFieldNum = 1; usFieldNum <= usNumFields; usFieldNum++)
				{
					aucFieldName = malloc(sizeof(UNSIGNED8) * 128);

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

					free(aucFieldName);
				}
				printf(");\n");



				ulRetCode = AdsCloseTable(hTable);
				err_check(ulRetCode);
				free(tblName);
			}
		}
	}	

	printf("COMMIT;\n");
	ulRetCode = AdsCloseAllTables();
	err_check(ulRetCode);
	ulRetCode = AdsDisconnect(hCon);
	err_check(ulRetCode);
	free(compdir);	
	free(connStr);
	return 0;
}
