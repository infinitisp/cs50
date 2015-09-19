#include <stdio.h>

int toIndex(char c) {
    if (c == '\'' || c == 0) return 0;
    else if (c > 96)
        return (int) c - 96;
    else 
        return (int) c - 64;
}

int main (void) {
    for (int i = 0; i < 128; i++) {
        printf("%i, ", toIndex((char) i));
        }
}


