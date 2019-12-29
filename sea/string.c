#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void main(int argc, char **argv)
{
	char *cstr = "this is a static string!";
	char *astr;
	size_t cstr_len;
	
	if (argc != 2)
	{
		fprintf(stderr, "Error: missing args\n");
		exit(1);
	}

	cstr_len = strlen(cstr);
	printf("%s %ld\n", cstr, cstr_len);

	asprintf(&astr, "this is from asprintf, %s, %s\n", cstr, argv[1]);
	printf("%s", astr);
	free(astr);
}
