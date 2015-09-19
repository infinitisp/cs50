/**
 * helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 */
       
#include <cs50.h>
#include <stdio.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int* array, int n)
{
    if (array[n/2] == value) return true;
        
    int* halvedArrayStart = array[n/2] > value ? &array[0] : &array[(n+1)/2];
    if (n > 1)
        if (search (value, halvedArrayStart, n/2) == true) 
            return true;
    
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    int tempValues[65536] = { };
    int sortedPosition = 0;
    
    //O(n) steps
    for (int i = 0; i < n; i++) 
        tempValues[values[i]]++;
    
    //O(constant) steps
    for (int i = 0; i < 65536; i++) 
        while (tempValues[i]-- > 0) 
            values[sortedPosition++] = i; 
         
    return;
}
