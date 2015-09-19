#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // intialize to -1 so while loops runs (yes I could have used do loop)
	int height = -1; 
	
	// get height integer
	while (!(height <24 && height >= 0)) {
	    printf("Height: ");   
	    height = GetInt(); 
	}
	
	// draw blocks
	for (int row = 1; row <= height; row++) {
	    for (int col = 1; col <= height*2 + 3; col++) {
	        if (col <= height-row) {
	            printf(" ");
	            continue;
	        } else if (col <= height) {
	            printf("#");
	            continue;
	        } else if (col == height + 1 || col == height + 2) {
	            printf(" ");
	            continue;
	        } else if (col <= height + 2 + row) {
	            printf("#");
	            continue;
	        } else {
	            printf("\n");
	            break;
	        }
	    }
	}
}

