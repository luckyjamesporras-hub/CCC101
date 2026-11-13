#include <stdio.h>
#include <stdlib.h>


void main() {
    int luckyjames;
    printf("Enter an integer: ");
    scanf("%d", &luckyjames);

    if (luckyjames % 2 == 0){
        printf("%i is even\n", luckyjames);
    }else{
        printf("%i is odd\n", luckyjames);
    }
}