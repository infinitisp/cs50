#include <stdio.h>
#include <cs50.h>
#include <math.h> // to include log10(), used below 

/*
NOTE: I ran style50 on this, and I dislike the output.
See my mario.c for what I consider to be more readable style.
Why? Because you can see more code on the screen at once. 
With new lines for every else and {, the code gets stretched
vertically and the structure (which is more checimportant than 
details) becomes more difficult to understand. 
*/

int main(void)
{

    printf("Number: ");
    long long ccn = GetLongLong();
    int length = (int) log10(ccn) + 1;  // I like this better than converting to string and using strlen()

    int digits[length]; // declare array of digits

    int sumDoubles = 0;
    int sumOthers = 0;

    // construct array of digits from ccn
    // I used google to find several ways to do this and picked the one I liked most (this one)
    for (int i = 0 ; i < length ; i++)
    {
        digits[length - i - 1] = ccn % 10;
        ccn /= 10;
        if (i % 2 == 1)
        {
            sumDoubles += (2 * digits[length - i - 1]) % 10;
            if (2 * digits[length - i - 1] > 9)
            {
                sumDoubles += 1;
            }
        }
        else
        {
            sumOthers += digits[length - i - 1];
        }
    }

    // checksum: is facially valid ccn?
    if ((sumOthers + sumDoubles) % 10 != 0)
    {
        printf("INVALID\n");
        return 0;
    }

    // check if matches to major card
    switch (length)
    {
        case 13:
            if (digits[0] == 4)
            {
                printf("VISA\n");
                return 0;
            }
            else
            {
                printf("INVALID\n");
                return 0;
            }
        case 15:
            if (digits[0] == 3 && (digits[1] == 4 || digits[1] == 7))
            {
                printf("AMEX\n");
                return 0;
            }
            else
            {
                printf("INVALID\n");
                return 0;
            }
        case 16:
            if (digits[0] == 4)
            {
                printf("VISA\n");
                return 0;
            }
            else if (digits[0] == 5 && digits[1] > 0 && digits[1] < 6)
            {
                printf("MASTERCARD\n");
                return 0;
            }
            else
            {
                printf("INVALID\n");
                return 0;
            }
        default:
            printf("INVALID\n");
            return 0;
    }

    printf("OK!\n"); //this line never runs
}
