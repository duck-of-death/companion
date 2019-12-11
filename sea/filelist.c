#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

int main(int argc, char **argv)
{
    DIR *directory;
    struct dirent *dirlist;

    directory = opendir(".");

    while (dirlist = readdir(directory))
    {
        printf("file: %s\n", dirlist->d_name);
    }

    closedir(directory);

    return 0;
}

