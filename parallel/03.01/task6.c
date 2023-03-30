#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    int ok, input, sum;

    srand(time(NULL)); // randomizáló

    int random[2];
    random[0] = rand() % (RAND_MAX - 1 + 1) + 1;
    random[1] = rand() % (RAND_MAX - 1 + 1) + 1;

    sum = random[0] + random[1];

    printf("Mennyi %d+%d? ", random[0], random[1]);

    clock_t start, end;
    double execution_time;
    start = clock();

    do
    {
        ok = 1;

        if (scanf("%d", &input) == 1)
        {

            if (input == sum)
            {
                ok = 1;
                end = clock();
                execution_time = ((double)(end - start)) / CLOCKS_PER_SEC;
                printf("\nHelyes megoldas! A feladat %lf masodpercbe telt! ", execution_time);
            }
            else
            {
                ok = 0;
                printf("\nA megoldas helytelen! Mas eredmeny: ");
            }
        }
        else
        {
            printf("Egy megoldast kerek csak!! Masik megoldas: ");
        }

    } while (!ok);

    printf("\n");

    getchar();

    return 0;
}