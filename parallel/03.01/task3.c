#include <stdio.h>
#include <unistd.h>

int main() {

    printf("Ez az uzenet azonnal megjelenik");
    sleep(2);
    printf("\nEz az uzenet 2 masodperccel kesobb jelenik meg");
    sleep(1);
    printf("\nEz az uzenet az elozo utan 1 seccel jelenik meg.");
    getchar();
    return 0;
}