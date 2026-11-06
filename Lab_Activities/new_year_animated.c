#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define SCREEN_WIDTH 120
#define SCREEN_HEIGHT 40

static void sleepMs(int ms) {
    Sleep(ms);
}

static void setCursorPosition(int x, int y) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = (SHORT)x;
    pos.Y = (SHORT)y;
    SetConsoleCursorPosition(hOut, pos);
}

static void setColor(int color) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hOut, (WORD)color);
}

static void hideCursor(void) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    if (hOut != INVALID_HANDLE_VALUE) {
        GetConsoleCursorInfo(hOut, &info);
        info.bVisible = FALSE;
        SetConsoleCursorInfo(hOut, &info);
    }
}

static void clearScreen(void) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info;
    DWORD written = 0;
    COORD start = {0, 0};

    if (hOut == INVALID_HANDLE_VALUE) {
        return;
    }

    GetConsoleScreenBufferInfo(hOut, &info);
    FillConsoleOutputCharacter(hOut, ' ', info.dwSize.X * info.dwSize.Y, start, &written);
    FillConsoleOutputAttribute(hOut, info.wAttributes, info.dwSize.X * info.dwSize.Y, start, &written);
    SetConsoleCursorPosition(hOut, start);
}

static void printAt(int x, int y, const char *text) {
    setCursorPosition(x, y);
    printf("%s", text);
}

static void drawFirework(int x, int y, int frame, int minY, int maxY) {
    const char symbols[] = "*+xo.";
    const int dx[8] = {0, 1, 2, 1, 0, -1, -2, -1};
    const int dy[8] = {-2, -1, 0, 1, 2, 1, 0, -1};
    int i;
    int phase = frame % 12;
    int radius = phase <= 5 ? phase + 1 : 11 - phase;

    setColor(9 + (frame % 7));
    if (radius == 0) {
        return;
    }

    for (i = 0; i < 8; ++i) {
        int px = x + dx[i] * radius;
        int py = y + dy[i] * radius;
        if (px >= 1 && py >= minY && px < SCREEN_WIDTH - 1 && py <= maxY) {
            setCursorPosition(px, py);
            putchar(symbols[(frame + i) % 5]);
        }
    }
}

static void drawPortrait(void) {
    const char *face[75] = {
"                                               ..:::.::^::^^:....                                   ",
"                                           :^!777!!!!!!!!!!777777!^.                                ",
"                                         :~7777!!!!!~~~~~!!!!!777??7^:                              ",
"                                        ^!7?7!!!!!!!!!!!!!!!!!77777???\\?!~^.                         ",
"                                       .~7?7!!7!!!!!!!!!!!!!!!!7777????JYYYJ~.                      ",
"                             ........ .:77!!!!7!!!!!!!!!!!!!!!77777????JJJY55Y?~.                   ",
"                       .:^^~!!777!!!!!!!~~~~~~~~~~!!!!!!!!!!!!!77777?????JJYYY5P5!:                 ",
"                    .:~!77!!!~~~~~~~~~~^^^^^^^~~~~~~~~~~~!!!!!!7!!777????JJJJYY55PY:                ",
"                 .^~!~~^^^^^^^^^^^^^^^^^^^^^^^^^^^~~~~~~~~~!!!!!777777????JJJYYYY5P5^               ",
"               :^!!~~^^^^^^^^^^^^^^^^^^^^^^^^^~~~~~~~~~~!!!!!!!!!777777????JJJYYY55PP7              ",
"              .^~!!~^^^^^^^^^^^^^^^^^^^^^^^~~!!!!!!!~~~!!!!!!!!!!!7777????JJJJYYYY55PG5^            ",
"              .~!!~~^^^^^^^^^^^^^^^^^^^^~~!!!77777!!!!!!!!!!!!!!!777777????JJJJYYY555PGG:           ",
"             ..!~~~^^^^^^^^^^^^:^^^^^~~~!!!777?777777777!!!!!!77777....??JJJJYYY555PPGBY           ",
"              ~7~~^^^^^^^^^^^::^^^^^~~!!777???????????77777777777777777?????JJJYYYY55PPGBY          ",
"              !!~~^~^^^^^^::::^^^^~~!!777?????JJJJJ????????7777777777777??????JJJJYY555PPB5         ",
"             ^?7!!~~^^^^^^:::^^^^~~!!77?????JJJJJJJJJJJJJ?????777777777777?????JJJJYYY55PGP^        ",
"             ~??7!~^~~~~^:^:^^^~~!!!77??????JJJJJJJJJJJJJJJJ????77777777777??????JJJJYY5PPP:        ",
"             .!7?7!^~!!~^:^^^~~~!!777?????JJJJJJJJJYYYJJJJJJJJ????7777777777???????JJJYY5P5:        ",
"              .~?7!~~~!~^^^~^~!!77?????JJJJYYYYYYYYYYYYYYYYYYYJJJ????777????????7????JJJY57         ",
"               :!^!~~~^^^^~~!77???J??JJJJYYYY5555555555YYY5555YYYYJJJ??77??J????777???JJJJ.         ",
"                ..^!~^^^^~!77JYYJ?77777????JYY5555PP55555PP5555555YYYJJJ?7?JJJJ?77777??JY^          ",
"                   ~~^^^^~?PPPY7!~^^^^^^~~~~!!?JY55555555PP555YJJJJ?777??777?JJJ?77777?J^           ",
"                   .7~^^^!G&&GJ7!!!~~~^^^^^^~~!!?JYYY555555YJ7!!!~~!!!!!!77777???7!77?J!            ",
"                    :7~^^5@@&#PYJ???????7!~~~!!!7?JJYYYYYYJ?7!!!!!!77???JJJ???7?J?777?J:            ",
"                     :!!~#@@@#GP5YJJJJJJJJ??77777?JJYYYYYJJ?777???JJY55YYYYYYJJ5PY7!7J!             ",
"                      ^7?@@@&BGP5YJ?7!!777777??77?JJYY55YYJ?????77??JJJJJJJJYYY5GGJ7?7              ",
"                      ~7G@@&BPYJ7!~7J7^:^^!7!!?J??JYY555YJJ?JJ7!77~~!!!77777?JY5PBG?Y:              ",
"                     .J7#@@#PYJ?7777?\\?!~~7???7????JY5555YYJ?JJ????7!!!??7777?JY5PGB55?.             ",
"                     :??&@@#P5YJJJJ????????JJJJJJJYY55555YJJJJYYJJ????JJJJJYYYY5PB#P5Y!             ",
"                     ^?5&@@#P5YYYYYYYYYYYYYYYYJJJYYYYY55YYYYYYYY555555555555555PG#&GY57             ",
"                     .?G&@&BP5555555555YYYYYYYJJYYYYYY55YYYYYYYY5555PPPPPPPPP555G#&PY5^             ",
"                      ?P&&&BP555555555YYYYYYYJJJY5YYYY55YYJJJYYY55555PPPPPPPPP55P#&5YJ              ",
"                      ^P#&&BP555Y5555555555YJ??JY5555555YYJJ??YY5555555555555555PB#Y5^              ",
"                       JB&##GP555555Y55555YJ?Y5555555PP555YYYJJJ5555555555555555P#G5!               ",
"                       J#&##GGP5YYYYYYY55YY?YP5JJYYYYYYYYJJJYYJJY555555555555555P#P!                ",
"                       ~&&##BGP5YYYYYYYYYYYJ?7!~~!!777777!!!!7?YY555555555555555GB7                 ",
"                        :G&#BGP5YYYYYYYYYYYJJ?777!!7????77???JY5555555555555YY5PB?                  ",
"                         ^&#BGP5YYYJJJJJJYJ???77!!7?JJJJ?77??JYY5555555555YYYY5P^                   ",
"                         G#BBP5YYJJJJJ??7!~~!777???JJJ???777777?JYYYYYYYYYYYYP5                    ",
"                          :#BBGP5YJJJ?7!^^^~!!!~~~~!!7!!~~~!777!~!77?JJJYYYJJYP:                    ",
"                           7#BGP5YJJ?7!^:^^^^^^^~~~~~~~~~^^^^~!~~^~~7?JJJJJJY5:                     ",
"                            J#GPPYJJ?7~^^~~~~!!777??????77!!!~~~^^:~!???JJJJY:                      ",
"                             JBP5YJ?7!~~!!!77777777777777!777777!~^~!7????J?.                       ",
"                              7PYJJ?7!!!!!7777!!~~~~~^^~~!!!!777!!!!!!777?7.                        ",
"                               ?Y??77777!777777!!!~~~^~!!!!!!!!!~^^~!!!!!^                          ",
"                               ~P?77777????7777777!!!!!77??77777!!!~~~~~.                           ",
"                               JGJ7!777?JYYJ???777!!7!!!77??????7!!~~~!:                            ",
"                       ..   .^JPPY?!~~!77???7777!!!!!!!~!!77?777!~~^~~?^                            ",
"                   ..^!77YPPGGGYY5J?!~^~~!!!!!~~~~~~~~~~~~~!!!~~^^~~!!J&&BGY7~.                     ",
"             ..:^~!~~~~~75GGGGG5?JJ?7!~~^^^^^^^^:::::^^^^^^^^^^~~!!!!!?&@&&B??JJ7!:.                ",
"         .~!~~~!!~~^:^~^^~JPGGG5?7??7!!~~~^^^^::::::::::::^^~~!!!!77!~Y&&&5^:^!!7?J!!!^:..          ",
"^^~!!!!7!!!~::^^~~^:::^^:::!PP55J!^!7!!!!~~~~~^^^^^^^^^~~~~!!!77777!~^G&&@J.:::::^77~!!~~7???7~^:::^",
"~~~~~!!~~^^^:.:^^^:..:::::.!5PPPJ7^:~!!!!!!~!~~~~~~~~~~~!!!!!!!!!~~^:7B&&&B:.....:^!7!~~^^:^~7J?7777",
"^^::^^^^^::::.::^^^:...::..!55YYJ!^..:::::::::::::::::::::::........:P5G#&#^......:^7!^^^^::.:^!7!!!",
"::::::^^:::::..:::::....:..!J5PBBG~:................................7G5PPB#7::.....:^!~:::::....:^~~",
":...:::::...:...::::.......~?BBBG7^:................................Y555PP#?::::.....:~~:.........:^",
"....:::::........:::.......:~!!!!^^:...............................^5Y5PPG#?...::......^^:..........",
".....:::..........::.......:~^~7?!^^:..............................7YY5YJYB?............:^:.........",
"......::....................7!7??7^^:.............................:5JYYJJJG7.............:^:........",
"............................~7?JJ?!^^:............................!5JJYYY5G~...............^:.......",
"............................:^!JJJ?!~^...........................:55Y5JJJ5G:................:.......",
".............................~~!JY?77~...........................!GYJYYYYPY..................:......",
"................:^~!!7??7777~~!!JYJ!!!^..........................5PYY??YJP7.........................",
".........^~!!!~~7????777777?!^!!JYY7!~~. ...................... !GP55YYYJP7^::::....................",
"......::~7YYJJ???JJJJJJYYJJJ7^~!?JJ?!~^:!!:...................^:5GPPPPP55YJ5555?777777~:............",
"..^~777777777777777??77777777^^!7??J7!^:?P5?^. ............^7JJ?GPY55555PJ5PP5PPP5555PP5Y!:.........",
":?PPPPPPPPPPPPPP55YYYYYY55YJJ!^~!?JYJ?!^!5P5J!^. .........~JJ?!PBP5PP5Y5P?JJJJJJJJJ?JJJJYYY7:.......",
"??????JJ?????JJJ?JYYYYYYYYJJJ?^~~?55YJ?!~PBBBBB5~......^Y5PGG5?BBBGP55555YPPPP5YJJJJJYYYYYJ??7~:....",
"55555YYYYYYYYYYYYJJ?77!!!!!?YJ~^~?YYY?77!?GGGGGP5!....^Y5PGGPJPB55555555JGBBBBBGGPPPGGGGGGGPYJJY7:..",
"PPPPPJJY555PPGGGGP555PPPPPPGB5^~77JJJ?7!!7GBBBBBBB5: ^GBBBBBP?BB555555GPPBBBBBBBBBBBBBBBBBBBBBBGBY..",
"P5P5PJ.....:^!7?^....::^!5GPPGY!777???7!~~JGGGPPPPPJ^J55PPGP?PBPPPPGP5G5PGGGGGGGGGPGGGGGGGGGGGGY~^:.",
"JJJJJJ7..................?5Y55Y~77?Y5Y7!7!!555YY5555YYY5555JJBG55YY55PG?555PPPPP5555555555PP555?^^~~",
"777JYYY?7??????????JYYYY555555~.!7?Y5Y7!!!~J5YYY5555YYYYY5J!PGP55Y5P5PPJ5YY?!!7?Y555P5YYJYYJ!!!!!777",
"~!!!!!!!!!!!!!!!!?J5GBGGGGGGP!..^~7JYYJ7!!~JBGGGGBBBBGGGBB??B5YPPPPP5GYGBB!.....!GBBBBGPPPPPPPPPPPP5",
"7777!!!!!!!!!!!!!J5555PPP5J^. .?~~!755Y7!!^~PPPPPPPPPPPPGY^PBG5YY555PPYGGP:......~GGGGGGGGBBGGGGGG7^"
    };

    int outputRow;
    for (outputRow = 0; outputRow < 20; ++outputRow) {
        char scaled[SCREEN_WIDTH];
        int sourceRow = outputRow * 74 / 19;
        int sourceCol;
        int outputCol = 0;

        for (sourceCol = 0; face[sourceRow][sourceCol] != '\0' && outputCol < SCREEN_WIDTH - 10; ++sourceCol) {
            scaled[outputCol++] = face[sourceRow][sourceCol];
        }
        scaled[outputCol] = '\0';
        printAt(9, 8 + outputRow, scaled);
    }
}

static void drawBanner(const char *title, int color) {
    int i;
    setColor(color);
    setCursorPosition(0, 0);
    for (i = 0; i < SCREEN_WIDTH; ++i) {
        putchar('=');
    }
    printAt(42, 1, title);
    setCursorPosition(0, 2);
    for (i = 0; i < SCREEN_WIDTH; ++i) {
        putchar('=');
    }
}

static void countdown(void) {
    int n;
    for (n = 5; n >= 1; --n) {
        clearScreen();
        drawBanner("COUNTDOWN TO 2027", 14);
        setColor(11);
        printAt(58, 15, "  ");
        setCursorPosition(58, 15);
        printf("%d", n);
        setColor(15);
        printAt(40, SCREEN_HEIGHT - 2, "Get ready for a fresh new beginning...");
        fflush(stdout);
        sleepMs(900);
    }
}

static void celebration(void) {
    int frame;
    for (frame = 0; frame < 30; ++frame) {
        clearScreen();
        drawBanner("HAPPY NEW YEAR!", 10 + (frame % 6));

        drawFirework(30, 5, frame, 3, 6);
        drawFirework(90, 5, frame + 3, 3, 6);
        drawFirework(30, 31, frame + 6, 29, 33);
        drawFirework(90, 31, frame + 9, 29, 33);

        drawPortrait();

        setColor(10);
        printAt(18, 35, "May this year bring joy, success, and peace.");
        printAt(30, 37, "Cheers to a brighter tomorrow!");
        fflush(stdout);
        sleepMs(180);
    }
}

int main(void) {
    hideCursor();
    countdown();
    clearScreen();
    setColor(14);
    printAt(30, 12, "WELCOME TO THE NEW YEAR!");
    setColor(9);
    printAt(28, 14, "Let the celebrations begin...");
    fflush(stdout);
    sleepMs(1000);

    celebration();

    clearScreen();
    setColor(14);
    printAt(25, 12, "HAPPY NEW YEAR EVERYONE!");
    printAt(18, 14, "May your dreams be bigger, your heart be lighter,");
    printAt(25, 15, "and your year be full of blessings.");
    setColor(15);
    setCursorPosition(0, SCREEN_HEIGHT - 1);
    return 0;
}
