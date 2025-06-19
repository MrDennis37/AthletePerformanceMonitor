#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Header.h"

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int main() {
    FILE* file = fopen("athletes.bin", "rb+");
    if (file == NULL) {
        file = fopen("athletes.bin", "wb+");
        if (file == NULL) {
            printf("Error opening file!\n");
            exit(1);
        }
    }

    int choice;
    while (1) {
        printf("\n");
        printf("*********************************\n");
        printf("|      Athlete Performance      |\n");
        printf("|           Tracker             |\n");
        printf("*********************************\n");
        printf("|       Choose an option:       |\n");
        printf("|   1. All Athletes Overview    |\n");
        printf("|        2. Add Athlete         |\n");
        printf("|    3. Update Athlete Info     |\n");
        printf("|    4. Delete Athlete Info     |\n");
        printf("|      5. Search athlete        |\n");
        printf("|        6. Sort Athletes       |\n");
        printf("|         7. Delete             |\n");
        printf("|          8. Exit             |\n");
        printf("*********************************\n");
        printf("|    Enter your choice (1-7):   |\n");
        printf("*********************************\n");

        if (scanf("%d", &choice) != 1) {
            printf("Error!\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
        case 1:
            allAthleteOverview(file);
            break;
        case 2:
            addAthlete(file);
            break;
        case 3:
            updateAthlete(file);
            break;
        case 4:
            deleteAthlete(file);
            break;
        case 5:
            searchAthlete(file);
            break;
        case 6:
            sortAthletes(file);
            break;
        case 7:
            fclose(file);
            remove_file();
            break;
        case 8:
            fclose(file);
            exit(0);
        default:
            printf("Invalid choice. Please try again.\n");
            break;
        }
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF);
    }

    fclose(file);
    return 0;
}


