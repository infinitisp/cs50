/****************************************************************************
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 6
 *
 * Implements a dictionary's functionality.
 ***************************************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"

typedef struct word {
    char word[46];
} word;

#define MAXDICTIONARY 150000
//array for dictionary of up to MAXDICTIONARY words
word dictArray[MAXDICTIONARY];
int wordCount = 0;
int locationPoint[19684]; //  = 27^3 + 1 for all two letter combos and 1 for end of dict
int getLocPoint(const char* word);
int toIndex(char c);

int indexArray[] = {0, -63, -62, -61, -60, -59, -58, -57, -56, -55, -54, -53, -52, -51, -50, -49, -48, -47, -46, -45, -44, -43, -42, -41, -40, -39, -38, -37, -36, -35, -34, -33, -32, -31, -30, -29, -28, -27, -26, 0, -24, -23, -22, -21, -20, -19, -18, -17, -16, -15, -14, -13, -12, -11, -10, -9, -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};

int getLocPoint(const char* word) {
    if (word[1] == 0) return toIndex(word[0]) * 729;
    else return toIndex(word[0]) * 729 + toIndex(word[1]) * 27 + toIndex(word[2]);
}

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    char c[LENGTH+2] = {};
    int i = 0;
    do {
        c[i] = tolower(word[i]);
    } while (word[i++] != 0);

    int lp = getLocPoint(c);
    for (int i = locationPoint[lp], n = locationPoint[lp+1]; i < n; i++)
        if (strcmp(dictArray[i].word,c) == 0)
            return true;
    return false;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    FILE* fp = fopen(dictionary, "r");
    if (fp == NULL)
    {
       return false;
    }

    char w[LENGTH+2];
    int tempLocPoint = -1;
    
    while (fscanf (fp, "%s", w) != EOF) {
    
        wordCount++;
        
        //set Location pointer, filling in gaps for check algo
        int lp = getLocPoint(w);
        for (int i = tempLocPoint+1; i <= lp; i++) {
            locationPoint[i] = wordCount; 
        }
        tempLocPoint = lp;
        
        //copy word to dictionary
        char* c = dictArray[wordCount].word; //create pointer to memory
        int i = 0;
        do {
            c[i] = w[i]; //copy word character by character
        } while (w[i++] != 0);

    }   
    
    //set end of dictionary pointer, filling in gaps for check algo
    for (int i = tempLocPoint+1; i < 19684; i++)
        locationPoint[i] = wordCount;
        
    fclose(fp);
    return true;
}

//convert character to index of node children
int toIndex(char c) {
    return indexArray[(int) c];
/*
    if (c == '\'' || c == 0) return 0;
    else if (c > 96)
        return (int) c - 96;
    else 
        return (int) c - 64;
*/
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return wordCount;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    return true;
}
