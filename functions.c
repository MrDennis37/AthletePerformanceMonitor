#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Header.h"

void addAthlete(FILE* file) {
    ATHLETE newAthlete;
    int maxId = 0;
    ATHLETE temp;

    FILE* f = fopen(FILE_NAME, "rb");
    if (f != NULL) {
        while (fread(&temp, sizeof(ATHLETE), 1, f) == 1) {
            if (temp.id > maxId) {
                maxId = temp.id;
            }
        }
        fclose(f);
    }

    newAthlete.id = maxId + 1;

    printf("Enter athlete's name: ");
    if (scanf("%19s", newAthlete.name) != 1) {
        printf("Invalid input for name.\n");
        return;
    }

    printf("Enter athlete's surname: ");
    if (scanf("%19s", newAthlete.surname) != 1) {
        printf("Invalid input for surname.\n");
        return;
    }

    printf("Enter athlete's age: ");
    if (scanf("%d", &newAthlete.age) != 1) {
        printf("Invalid input for age.\n");
        while (getchar() != '\n');
        return;
    }

    printf("Enter athlete's height: ");
    if (scanf("%d", &newAthlete.height) != 1) {
        printf("Invalid input for height.\n");
        while (getchar() != '\n');
        return;
    }

    printf("Enter athlete's weight: ");
    if (scanf("%f", &newAthlete.weight) != 1) {
        printf("Invalid input for weight.\n");
        while (getchar() != '\n');
        return;
    }

    if (newAthlete.age < 13) {
        newAthlete.category = KIDS;
    }
    else if (newAthlete.age <= 17) {
        newAthlete.category = CADETS;
    }
    else if (newAthlete.age <= 20) {
        newAthlete.category = JUNIORS;
    }
    else {
        newAthlete.category = SENIORS;
    }

    f = fopen(FILE_NAME, "ab");
    if (f == NULL) {
        perror("Error opening file for writing");
        return;
    }

    fwrite(&newAthlete, sizeof(ATHLETE), 1, f);
    fclose(f);

    printf("Athlete successfully added with ID: %d\n", newAthlete.id);
}

void allAthleteOverview(FILE* file) {
    FILE* f = fopen(FILE_NAME, "rb");
    if (f == NULL) {
        printf("Error opening file!\n");
        return;
    }

    ATHLETE athlete;
    int count = 0;

    printf("\n========= All Athletes =========\n");

    while (fread(&athlete, sizeof(ATHLETE), 1, f) == 1) {
        count++;
        printf("\nAthlete #%d\n", count);
		printf("ID: %d\n", athlete.id);
        printf("Name: %s %s\n", athlete.surname, athlete.name);
        printf("Age: %d\n", athlete.age);
        printf("Height: %d cm\n", athlete.height);
        printf("Weight: %.1f kg\n", athlete.weight);
        printf("%s", categoryToString(athlete.category));
        printf("********************************\n");
    }

    if (count == 0) {
        printf("No athletes found.\n");
    }

    fclose(f);
}

void deleteAthlete(FILE* file) {
    int idToDelete;

    printf("Enter athlete's ID to delete: ");
    if (scanf("%d", &idToDelete) != 1) {
        printf("Wrong ID.\n");
        while (getchar() != '\n'); 
        return;
    }

    FILE* tempFile = fopen("temp.bin", "wb");
    if (!tempFile) {
        perror("Error opening temp.bin");
        return;
    }

    rewind(file);

    ATHLETE athlete;
    int found = 0;

    while (fread(&athlete, sizeof(ATHLETE), 1, file) == 1) {
        if (athlete.id != idToDelete) {
            fwrite(&athlete, sizeof(ATHLETE), 1, tempFile);
        }
        else {
            found = 1;
        }
    }

    fclose(file);
    fclose(tempFile);

    if (remove("athletes.bin") != 0) {
        perror("Error deleting athletes file");
        return;
    }

    if (rename("temp.bin", "athletes.bin") != 0) {
        perror("Error renaming file");
        return;
    }
	file = fopen(FILE_NAME, "rb+");
	if (file == NULL) {
		printf("Error reopening athletes file.\n");
		return;
	}

    if (found) {
        printf("Athlete deleted.\n");
    }
    else {
        printf("Athlete with ID %d not found.\n", idToDelete);
    }

}


void searchAthlete(FILE* file) {
    ATHLETE athlete = { 0 };
    int category = 0, age = 0;
    char name[20] = { 0 }, surname[20] = { 0 };
    rewind(file);

    int choice = 0, found = 0;

    printf("***************************\n");
    printf("|    Search athlete by:   |\n");
    printf("***************************\n");
    printf("|         1. Name         |\n");
    printf("|        2. Surname       |\n");
    printf("|         3. Age          |\n");
    printf("|        4. Category      |\n");
    printf("***************************\n");
    printf("|    Enter your choice:   |\n");
    printf("***************************\n");

    if (scanf("%d", &choice) != 1) {
        printf("Error reading choice.\n");
        while (getchar() != '\n');
        return;
    }

    switch (choice) {
    case 1:
        printf("Enter athlete's name: ");
        if (scanf("%19s", name) != 1) {
            printf("Invalid input for name.\n");
            while (getchar() != '\n');
            return;
        }
        break;
    case 2:
        printf("Enter athlete's surname: ");
        if (scanf("%19s", surname) != 1) {
            printf("Invalid input for surname.\n");
            while (getchar() != '\n');
            return;
        }
        break;
    case 3:
        printf("Enter athlete's age: ");
        while (scanf("%d", &age) != 1) {
            printf("Invalid input. Enter a number: ");
            while (getchar() != '\n');
        }
        break;
    case 4:
        printf("Enter athlete category:\n");
        printf("1. Kids (<18)\n2. Junior (18-20)\n3. Senior (20+)\n");
        while (scanf("%d", &category) != 1 || category < 1 || category > 3) {
            printf("Invalid input. Enter 1, 2, or 3: ");
            while (getchar() != '\n');
        }
        break;
    default:
        printf("Invalid choice.\n");
        return;
    }

    rewind(file);
    while (fread(&athlete, sizeof(ATHLETE), 1, file) == 1) {
        int match = 0;

        switch (choice) {
        case 1:
            match = strcmp(athlete.name, name) == 0;
            break;
        case 2:
            match = strcmp(athlete.surname, surname) == 0;
            break;
        case 3:
            match = (athlete.age == age);
            break;
        case 4:
            if (category == 1 && athlete.age < 18) match = 1;
            else if (category == 2 && athlete.age >= 18 && athlete.age <= 20) match = 1;
            else if (category == 3 && athlete.age > 20) match = 1;
            break;
        }

        if (match) {
            found = 1;
            printf("\n***************************\n");
            printf("Athlete found:\n");
            printf("ID: %d\n", athlete.id);
            printf("Name: %s\n", athlete.surname);
            printf("Surname: %s\n", athlete.name);
            printf("Age: %d\n", athlete.age);
            printf("Height: %d cm\n", athlete.height);
            printf("Weight: %.1f kg\n", athlete.weight);
            printf("***************************\n");
        }
    }

    if (!found) {
        printf("No matching athlete found.\n");
    }
}

void updateAthlete(FILE* file) {
    int id, found = 0;
    ATHLETE athlete;

    FILE* f = fopen(FILE_NAME, "rb+");
    if (f == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter the ID of the athlete you want to update: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid input for ID.\n");
        fclose(f);
        return;
    }

    while (fread(&athlete, sizeof(ATHLETE), 1, f) == 1) {
        if (athlete.id == id) {
            found = 1;

            printf("Athlete found:\n");
            printf("ID: %d\nName: %s\nSurname: %s\nAge: %d\nHeight: %d\nWeight: %.1f\n",
                athlete.id, athlete.name, athlete.surname, athlete.age, athlete.height, athlete.weight);

            int choice;
            printf("\nWhat do you want to update?\n");
            printf("1. Name\n2. Surname\n3. Age\n4. Height\n5. Weight\n6. All fields\nChoice: ");
            if (scanf("%d", &choice) != 1) {
                printf("Invalid input for choice.\n");
                fclose(f);
                return;
            }

            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);

            switch (choice) {
            case 1:
                printf("Enter new name: ");
                if (scanf("%19s", athlete.name) != 1) {
                    printf("Invalid input for name.\n");
                    while (getchar() != '\n');
                    return;
                }
                break;
            case 2:
                printf("Enter new surname: ");
                if (scanf("%19s", athlete.surname) != 1) {
                    printf("Invalid input for surname.\n");
                    while (getchar() != '\n');
                    return;
                }
                break;
            case 3:
                printf("Enter new age: ");
                if (scanf("%d", &athlete.age) != 1) {
                    printf("Invalid input for age.\n");
                    while (getchar() != '\n');
                    return;
                }
                break;
            case 4:
                printf("Enter new height: ");
                if (scanf("%d", &athlete.height) != 1) {
                    printf("Invalid input for height.\n");
                    while (getchar() != '\n');
                    return;
                }
                break;
            case 5:
                printf("Enter new weight: ");
                if (scanf("%f", &athlete.weight) != 1) {
                    printf("Invalid input for weight.\n");
                    while (getchar() != '\n');
                    return;
                }
                break;
            case 6:
                printf("Enter new name: ");
                if (scanf("%19s", athlete.name) != 1) {
                    printf("Invalid input for name.\n");
                    while (getchar() != '\n');
                    return;
                }
                printf("Enter new surname: ");
                if (scanf("%19s", athlete.surname) != 1) {
                    printf("Invalid input for surname.\n");
                    while (getchar() != '\n');
                    return;
                }
                printf("Enter new age: ");
                if (scanf("%d", &athlete.age) != 1) {
                    printf("Invalid input for age.\n");
                    while (getchar() != '\n');
                    return;
                }
                printf("Enter new height: ");
                if (scanf("%d", &athlete.height) != 1) {
                    printf("Invalid input for height.\n");
                    while (getchar() != '\n');
                    return;
                }
                printf("Enter new weight: ");
                if (scanf("%f", &athlete.weight) != 1) {
                    printf("Invalid input for weight.\n");
                    while (getchar() != '\n');
                    return;
                }
                break;
            default:
                printf("Invalid choice.\n");
                fclose(f);
                return;
            }

            
            if (athlete.age < 12) {
                athlete.category = KIDS;
            }
            else if (athlete.age <= 15) {
                athlete.category = CADETS;
            }
            else if (athlete.age <= 18) {
                athlete.category = JUNIORS;
            }
            else {
                athlete.category = SENIORS;
            }

            fseek(f, -(long)sizeof(ATHLETE), SEEK_CUR);
            fwrite(&athlete, sizeof(ATHLETE), 1, f);
            printf("Athlete updated successfully.\n");
            break;
        }
    }

    if (!found) {
        printf("Athlete with ID %d not found.\n", id);
    }

    fclose(f);
}

static inline void printAthlete(const ATHLETE* athlete) {
    printf("Name: %s, Surname: %s, Age: %d, Category: %s\n",
        athlete->name, athlete->surname, athlete->age, categoryToString(athlete->category));
}


int compareByFirstName(const void* a, const void* b) {
    return strcmp(((ATHLETE*)a)->name, ((ATHLETE*)b)->name);
}

int compareByLastName(const void* a, const void* b) {
    return strcmp(((ATHLETE*)a)->surname, ((ATHLETE*)b)->surname);
}

int compareByAge(const void* a, const void* b) {
    return ((ATHLETE*)a)->age - ((ATHLETE*)b)->age;
}

int compareByCategory(const void* a, const void* b) {
    return ((ATHLETE*)a)->category - ((ATHLETE*)b)->category;
}



const char* categoryToString(CATEGORY category) {
    switch (category) {
    case KIDS: return "KIDS";
    case CADETS: return "CADETS";
    case JUNIORS: return "JUNIORS";
    case SENIORS: return "SENIORS";
    default: return "UNKNOWN";
    }
}


void sortAthletes(FILE* file) {
    rewind(file);
    ATHLETE* athletes = NULL;
    int count = 0;
    ATHLETE temp;

    while (fread(&temp, sizeof(ATHLETE), 1, file) == 1) {
        ATHLETE* tempPtr = realloc(athletes, (count + 1) * sizeof(ATHLETE));
        if (!tempPtr) {
            printf("Memory allocation failed.\n");
            free(athletes);
            return;
        }
        athletes = tempPtr;
        athletes[count++] = temp;
    }

    if (count == 0) {
        printf("No athletes found.\n");
        return;
    }

    int choice;
    printf("\nSort athletes by:\n");
    printf("1. First Name\n");
    printf("2. Last Name\n");
    printf("3. Age\n");
    printf("4. Category\n");
    printf("Enter your choice: ");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        while (getchar() != '\n');
        free(athletes);
        return;
    }

    switch (choice) {
    case 1:
        qsort(athletes, count, sizeof(ATHLETE), compareByFirstName);
        break;
    case 2:
        qsort(athletes, count, sizeof(ATHLETE), compareByLastName);
        break;
    case 3:
        qsort(athletes, count, sizeof(ATHLETE), compareByAge);
        break;
    case 4:
        qsort(athletes, count, sizeof(ATHLETE), compareByCategory);
        break;
    default:
        printf("Invalid choice.\n");
        free(athletes);
        return;
    }

    printf("\nSorted Athletes:\n");
    for (int i = 0; i < count; i++) {
        printf("-----------------------------------\n");
        printf("ID: %d\n", athletes[i].id);
        printf("Name: %s\n", athletes[i].name);
        printf("Surname: %s\n", athletes[i].surname);
        printf("Age: %d\n", athletes[i].age);
        printf("Height: %d\n", athletes[i].height);
        printf("Weight: %.2f\n", athletes[i].weight);
        printf("Category: %s\n", categoryToString(athletes[i].category));
    }

    free(athletes);
}



