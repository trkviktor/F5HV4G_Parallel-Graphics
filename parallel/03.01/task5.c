#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    int min, max, number_of_inputs, ok;
    printf("Adja meg a minimum es a maximum erteket (pl: 2 5): ");

    srand(time(NULL));//randomizáló

    do
    {
        ok = 1;
        number_of_inputs = scanf("%d %d", &min, &max);
        if (number_of_inputs != 2)
        {
            ok = 0;
            printf("2 szamot szukseges megadni!\n");
            while(getchar() != '\n');
        }
        else if (min < 0)
        {

            printf("Pozitiv intervallumot lehet csak megadni!!\n");
            ok = 0;
        }
        else if (max < min)
        {

            printf("Az intervallum zaro elemenek nagyobbnak kell lennie mint a nyito elemnek!\n");
            ok = 0;
        }
    }
    while(!ok);

    int random = rand() % (max - min + 1) + min;

    printf("A %d-%d intervallumon generalt random szam: %d", min, max, random);

    printf("\n");

    getchar();

    return 0;
}