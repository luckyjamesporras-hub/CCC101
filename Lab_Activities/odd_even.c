#include <stdio.h>
#include <stdlib.h>

int is_even(int n) {
    n = abs(n);            
    while (n >= 2) {
        n -= 2;           
    }
    return n == 0;        
}

void main() {
    int number;
    printf("Enter an integer: ");
    scanf("%d", &number);

    if (is_even(number))
        printf("%d is even\n", number);
    else
        printf("%d is odd\n", number);
}