#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main() {

    int min = 500;
    int max = 1000;

    float min_fl, max_fl;

    min_fl = 500.0;
    max_fl = 1000.0;
    

    srand(time(NULL));
    int number = rand() % (max - min + 1) + min;
    float float_num = (float)rand() / RAND_MAX * (max_fl - min_fl) + min_fl;
    printf("Egesz veletlen szam: %d", number);
    printf("\nLebegopontos veletlen szam: %f", float_num);
    return 0;
}