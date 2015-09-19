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

#include "dictionary.h"

typedef struct node {
    bool isWord;
    struct node* next[27];
} node;

node* start;    
unsigned int sizeD = 0;

void dictAdd(char* w);
int toIndex(char c);
void freeNode (node* n);

//array to quickly free
node* listOfNodes[1000000];
int listPos = 0;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    int i = 0;
    node* current = start;
    while (true) {
        if (word[i] != 0)
        {
            if (current->next[(int)toIndex(word[i])] == NULL){
                return false;
                }
            else {
                current = current->next[(int)toIndex(word[i])];
                i++;

            }
        }
        else {
            return current->isWord;
        }
    }
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

    start = malloc (sizeof *start); 
    listOfNodes[listPos++] = start;
    char w[LENGTH+2];

    while (fscanf (fp, "%s", w) != EOF) {
        dictAdd(w);
    }   
    
    fclose(fp);
    return true;
}

//convert character to index of node children
int toIndex(char c) {
    if (c == '\'') return 26;
    else if (c > 96)
        return (int) c - 97;
    else 
        return (int) c - 65;
}

//add word to the data structure
void dictAdd(char* w){
    //start with first character and start node
    int i = 0;
    node* current = start;
    
    //loop through characters; making nodes and stuff, ultimately adding word
    while (true) {
        if (w[i] != 0)
        {
            int x = toIndex(w[i++]);
            if (current->next[x] == NULL) {
                current->next[x] = malloc(sizeof *current);
                listOfNodes[listPos++] = current->next[x];
                }
            current = current->next[x];
        }
        else 
        {
            current->isWord = true;
            sizeD++;
            break;
        }  
    }
} 

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return sizeD;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    //freeNode(start);

    for (int i = 0; i < listPos; i++)
        free(listOfNodes[i]);

    return true;
}

//recursive function frees all nodes. I also considered keeping track of all nodes in a separate structure, but it was slower than this.
/*
void freeNode (node* n) {
    for (int i = 0; i < 27; i++)
        if (n->next[i] != NULL) 
            freeNode(n->next[i]);
    free(n);
}
*/
