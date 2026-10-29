#include <stdio.h>
#include <windows.h>

void gotoxy(int x, int y){
    COORD p = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p);
}
int main(){
    char name[] = "Lucky James Porras";
    int x, y;
    system("cls");
    for(x = 0 ; x <= 235 ; x++){
        system("cls");
        gotoxy(x, 0);
        printf("%s", name);
        Sleep(10);
    }
     for(y = 1 ; y <= 50 ; y++){
        system("cls");
        gotoxy(235, y);
        printf("%s", name);
        Sleep(10);
    }
     for(x = 235 ; x >= 0 ; x--){
        system("cls");
        gotoxy(x, 50);
        printf("%s", name);
        Sleep(10);
    }
     for(y = 50 ; y > 0 ; y--){
        system("cls");
        gotoxy(0, y);
        printf("%s", name);
        Sleep(10);
    }
    gotoxy(x, 50);
    getchar();
    return 0;
}