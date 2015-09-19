#include <stdio.h>
#include <crypt.h>
#include <string.h>

//not sure why these are here:
#define _XOPEN_SOURCE
#include <unistd.h>

int main(int argc, char *argv[]) 
{
    printf("%s\n", crypt(argv[1],"50"));
}


