// MontyHall.c
//  cd 5250
//  gcc MontyHall.c -o MontyHall
//  ./MontyHall

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    int trials = 100000;
    int switchWins = 0;
    int stayWins = 0;

    srand(time(NULL));

    for (int t = 0; t < trials; t++)
    {
        int prize = rand() % 3;      // prize door
        int choice = rand() % 3;     // player initial choice

        // host opens a door that is:
        //  - not the prize door
        //  - not the player's choice
        int host;
        for (int d = 0; d < 3; d++) {
            if (d != prize && d != choice) {
                host = d;
                break;
            }
        }

        // switching means choosing the remaining unopened door...
        int switchChoice = 3 - choice - host;

        if (switchChoice == prize) switchWins++;
        if (choice == prize) stayWins++;
    }

    printf("Trials: %d\n", trials);
    printf("Win rate (stay):   %.5f\n", (double)stayWins / trials);
    printf("Win rate (switch): %.5f\n", (double)switchWins / trials);

    return 0;
}
