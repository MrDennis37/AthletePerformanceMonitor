#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
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
	FILE* file = NULL;
	file = fopen("athletes.bin", "rb");
	if (file == NULL) {
		file = fopen("athletes.bin", "wb");
		if (file == NULL) {
			printf("Error opening file!\n");
			exit(1);
		}
		fclose(file);
	}


	int choice;
	while (1) {
		printf("\n");
		printf("*********************************\n");
		printf("|      Athlete Performance      |\n");
		printf("|           Tracker             |\n");
		printf("*********************************\n");
		printf("|       Choose an option:       |\n");
		printf("|        1. Training log        |\n");
		printf("|   2. All Athletes Overview    |\n");
		printf("|        3. Add Athlete         |\n");
		printf("|    4. Update Athlete Info     |\n");
		printf("|    5. Delete Athlete Info     |\n");
		printf("|      6. Search athlete        |\n");
		printf("|           7. Exit             |\n");
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
			printf("Training log\n");
			break;
		case 2:
			allAthleteOverview(file);
			break;
		case 3:
			addAthlete(file);
			break;
		case 4:
			updateAthlete(file);
			break;
		case 5:
			deleteAthlete(file);
			break;
		case 6:
			searchAthlete(file);
			break;
		case 7:
			fclose(file);
			break;
		default:
			printf("Invalid choice. Please try again.\n");
			break;
		}
		int ch;
		while ((ch = getchar()) != '\n' && ch != EOF);

	}
}