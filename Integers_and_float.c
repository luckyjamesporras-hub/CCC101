#include "stdio.h"

void main(){
    int LuckyJames ;
    float Porras ;

    printf("Enter Two Numbers(int: Float:)"); 
    scanf("%i %f", &LuckyJames,&Porras);

    if (LuckyJames > 0){
        printf("the number %i is positive\n", LuckyJames);
    } else if (LuckyJames < 0){
        printf("the number %i is negative\n", LuckyJames);
    } else {
        printf("the number %i is Neutral\n", LuckyJames);
    }

     if (Porras > 0){
        printf("the number %.1f is positive\n", Porras);
    } else if (Porras < 0){
        printf("the number %.1f is negative\n", Porras);
    } else {
        printf("the number %.1f is Neutral\n", Porras);
    }
}