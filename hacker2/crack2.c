#include <stdio.h>
#include <crypt.h>
#include <string.h>
#include <cs50.h>
#include <math.h>

int dictLookup();
int checkModded();
int checkLineSalt();
void addCharToStr (char base[], int length, char added);
int testWAddedChars (char line[16], int len, int num);

char* passInput;
char salt[3];

//*********************************
//** SCOPE of Added Permutations **
//*********************************
const int numAddedChars = 12;
const char addedChars[numAddedChars] = {0, 115, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57};

//*********************************
//** DEPTH of Added Permutations **
//*********************************
const int maxCharsToAdd = 3;

int main(int argc, char *argv[]) {
    passInput = argv[1];
    if (argc != 2) return -1;
    salt[0] = passInput[0];
    salt[1] = passInput[1];
    salt[2] = 0;
    if (dictLookup() == 0) return 0;
}

int dictLookup() {
    FILE* passdict = fopen("cain.txt", "r");
    char line [16];
    while (fgets (line, sizeof line, passdict) != NULL) {
        line[strlen(line)-1] = '\0';
        if (checkModded(line) == 0) return 0;
    }
    fclose(passdict);
    return 1;
}

int checkModded(char line[16]) {
    if (checkLineSalt(line) == 0) return 0;
    
    for (int i = strlen(line), len = strlen(line); i < 8; i++) {
        int chars2Add = 8-i > maxCharsToAdd ? maxCharsToAdd : 8-i;
        if (testWAddedChars(line, len, chars2Add) == 0) return 0;
    }
    return 1;
}

int checkLineSalt (char line[16]) {
    if (strcmp(crypt(line, salt), passInput) == 0) {
        printf("%s\n", line);
        return 0;
    }
    return 1;
}

void addCharToStr (char base[], int length, char added) {
    base[length] = added; 
    base[length+1] = 0;
}

int testWAddedChars (char line[16], int len, int num) {
    for (int i = 0; i<numAddedChars; i++) {
        addCharToStr(line, len, addedChars[i]);
        
        if (num == 1) {
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
