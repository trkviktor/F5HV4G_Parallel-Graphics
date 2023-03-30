#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "xlsxwriter.h"

int main()
{

    lxw_workbook  *workbook  = workbook_new("task07.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    int row = 1, col = 0;
    worksheet_write_string(worksheet, 0, col, "running time", NULL);
    worksheet_write_string(worksheet, 0, col+1, "prime number count", NULL);

    
    int isPrime, max, primenumbers = 0;

    printf("Input a number larger than 999: ");
    scanf("%d", &max);

    printf("MAX:::: %d\n", max);
    clock_t start = clock();
    for (int i = 0; i < max; i++)
    {
        
        if (i > 1)
        {
            isPrime = 1;
            for (int j = 2; j <= i / 2; ++j)
            {

                if (i % j == 0)
                {
                    isPrime = 0;
                    break;
                }
            }
        }
        if(isPrime) primenumbers++;
        if (i % 1000 == 0)
        {
            clock_t end = clock();
            float seconds = (float)(end - start) / CLOCKS_PER_SEC;
            printf("\n running time: %f prime number count: %d", seconds, primenumbers);
            worksheet_write_number(worksheet, row, col,     seconds, NULL);
            worksheet_write_number(worksheet, row, col + 1, primenumbers, NULL);
            row++;
        }
    }

    return workbook_close(workbook);

    
}
