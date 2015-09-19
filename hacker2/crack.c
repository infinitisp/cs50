#include <stdio.h>
#include <crypt.h>
#include <string.h>
#include <cs50.h>
#include <math.h>

//Not sure why these are here. Program runs fine without them.
#define _XOPEN_SOURCE
#include <unistd.h>

//Function Declarations
int dictLookup();
int checkModded();
int checkLineSalt();
void addCharToStr (char base[], int length, char added);
int testWAddedChars (char line[16], int len, int num);

//Global Variables
char* passInput;
char salt[3];

//*********************************
//** SCOPE of Added Permutations **
//*********************************
// addedChars currently consists of: NULL, s, and [0-9]
// --> will find things like "aaa12345" but NOT "aaa123k"
// to find the latter, need to add "k" to this list
const int numAddedChars = 12;
const char addedChars[numAddedChars] = {0, 115, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57};

//*********************************
//** DEPTH of Added Permutations **
//*********************************
/*  maxCharsToAdd = DEPTH OF SEARCH
    Accepts anything between 1 and 7 (8+ OK, but does nothing).  
    E.g. 3 will not find "aaa12345" but will find "aaa123"
    
    NOTE right now this only does a single pass -- and I think
    using values of 4 and 5 are way too slow / inefficient for
    a single pass through :: I'm just too lazy to optimize further
    by added 2nd and 3rd passes (and it still eventually works).
*/  
const int maxCharsToAdd = 3;

//MAIN METHOD
int main(int argc, char *argv[]) {
    
    passInput = argv[1];
    
    //quit & return -1 if not a single command line arg
    if (argc != 2) return -1;
    
    //get salt || assumes input is valid (has salt)
    salt[0] = passInput[0];
    salt[1] = passInput[1];
    salt[2] = 0; 
   
    //first see if password matches dictionary + mods to dict
    if (dictLookup() == 0) return 0;
    
    /*TODO other optimizations here:
    - do progressive passes through dictionary with increasing scope + depth
    - 1337 speak permutations of cain.txt
    - trim cain.txt (I think it's unnecessarily long)
    */
    
}

//DICTLOOKUP()
//returns 0 if word is found through dictionary + mods to dict
int dictLookup() {

    //open password dictionary file (found on internet)
    FILE* passdict = fopen("cain.txt", "r");
   
    //read each line, trim \n, and check if line or mods match password
    char line [16];
    while (fgets (line, sizeof line, passdict) != NULL) {
        line[strlen(line)-1] = '\0';
        if (checkModded(line) == 0) return 0;
    }
    
    //close password dictionary file
    fclose(passdict);
    return 1;
}

//CHECKMODDED()
//return 0 if password matches line or any derivatives of line
//formed by appending up to "maxCharsToAdd" of "addedChars[]"
int checkModded(char line[16]) {
    if (checkLineSalt(line) == 0) return 0;
    
    //for each unit difference between strlen(line) and 8:
    for (int i = strlen(line), len = strlen(line); i < 8; i++) {
       
        //makes sure you don't add so many that it would take you over max of 8
        int chars2Add = 8-i > maxCharsToAdd ? maxCharsToAdd : 8-i;
        //calls testWAddedChars, see comment there
        if (testWAddedChars(line, len, chars2Add) == 0) return 0;
             
    }

    return 1;
}

//prints line & returns 0 if a given line/salt combo matches the password
int checkLineSalt (char line[16]) {
    if (strcmp(crypt(line, salt), passInput) == 0) {
        printf("%s\n", line);
        return 0;
    }
    return 1;
}

//adds a specified character to the end of an existing base string
void addCharToStr (char base[], int length, char added) {
    base[length] = added; 
    base[length+1] = 0;
}

//This function basically finds all permutations of "num"
//"addedChars[]". E.g., if num is 3, it expands to 3 nested-
//for loops by calling itself recursively. 
int testWAddedChars (char line[16], int len, int num) {
    for (int i = 0; i<numAddedChars; i++) {
        //add a character
        addCharToStr(line, len, addedChars[i]);
        
        if (num == 1) {
            //if that's the last character we need to add, test it now
            if (checkLineSalt(line) == 0) return 0;
        } else
            //we need to add at least another character, so function calls
            //itself with one less character to add
            if (testWAddedChars (line, len+1, num-1) == 0) return 0;

        //subtract added character
        line[len] = 0;  
    }
    return 1;
}
