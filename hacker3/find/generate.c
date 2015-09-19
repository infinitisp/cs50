/**
 * generate.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Generates pseudorandom numbers in [0,LIMIT), one per line.
 *
 * Usage: generate n [s]
 *
 * where n is number of pseudorandom numbers to print
 * and s is an optional seed
 */
       
#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LIMIT 65536

int main(int argc, string argv[])
{
    // if not 2 or 3 cmd line args print instructions & return 1
    if (argc != 2 && argc != 3)
    {
        printf("Usage: ./generate n [s]\n");
        return 1;
    }

    // convert 1st cmd line arg to integer
    int n = atoi(argv[1]);

    // if 2nd cmd line exists, make it the seed
    if (argc == 3)
    {
        srand((unsigned int) atoi(argv[2]));
    }
    else
    // make time(NULL) the seed (using cpu time)
    {
        srand((unsigned int) time(NULL));
    }

    // print out the random numbers, mod the LIMIT
    for (int i = 0; i < n; i++)
    {
        printf("%i\n", rand() % LIMIT);
    }

    // that's all folks
    return 0;
}
