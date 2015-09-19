#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(){
    string s = GetString();
    if (s[0] != ' ') printf("%c", toupper(s[0]));
    for (int i = 1, n = strlen(s); i < n-1; i++) {
        if (s[i] == ' ' && s[i+1] != ' ') printf("%c", toupper(s[i+1]));        
    }
    printf ("\n");
}
