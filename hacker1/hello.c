#include <stdio.h>
#include <cs50.h>

int GetPositiveInt();

int main(void)
{
	int n = GetPositiveInt();
	printf("%d\n", n);
}

int GetPositiveInt() {
    while (true) {
        int x = GetInt();
        if (x>0) return x; 
    }
}
