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

int getLocPoint(const char* word) {
    if (word[1] == 0) return toIndex(word[0]) * 729;
    else return toIndex(word[0]) * 729 + toIndex(word[1]) * 27 + toIndex(word[2]);
}

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    char c[LENGTH+2];
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

        //set Location pointer, filling in gaps for check algo
        int lp = getLocPoint(w);
        while (tempLocPoint != lp) {
            locationPoint[++tempLocPoint] = wordCount;
        }
        
        //copy word to dictionary
        char* c = dictArray[wordCount++].word; //create pointer to memory
        int i = 0;
        do {
            c[i] = w[i]; //copy word character by character
        } while (w[i++] != 0);

    }   
    
    //set end of dictionary pointer, filling in gaps for check algo
    while (tempLocPoint != 19684)
        locationPoint[++tempLocPoint] = wordCount; 
        
    fclose(fp);
    return true;
}

//convert character to index of node children
int toIndex(char c) {
    if (c == '\'' || c == 0) return 0;
    else if (c > 96)
        return (int) c - 96;
    else 
        return (int) c - 64;
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
