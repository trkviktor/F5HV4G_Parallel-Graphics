#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main()
{

    srand(time(NULL));
    int number = rand() % 100000;

    int num_of_digits = snprintf(NULL, 0, "%d", number);
    int num_of_spaces = 8 - num_of_digits;
    if (num_of_spaces < 0)
    {
        num_of_spaces = 0;
    }

    printf("%*d", num_of_digits + num_of_spaces, number);
    getchar();
    return 0;
}