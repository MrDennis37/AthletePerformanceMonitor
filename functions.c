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

    printf("\n========= All Athletes =========\n");

    printAthletes(f, 0);  

    fclose(f);
}

void printAthletes(FILE* f, int count) {
    ATHLETE athlete;

    if (count == 0 && fread(&athlete, sizeof(ATHLETE), 1, f) != 1) {
        printf("No athlete found.\n");
        return;
    }

    if (count != 0) {
        if (fread(&athlete, sizeof(ATHLETE), 1, f) != 1)
            return;
    }

    printf("\nAthlete #%d\n", count + 1);
    printf("ID: %d\n", athlete.id);
    printf("Name: %s %s\n", athlete.surname, athlete.name);
    printf("Age: %d\n", athlete.age);
    printf("Height: %d cm\n", athlete.height);
    printf("Weight: %.1f kg\n", athlete.weight);
    printf("%s", categoryToString(athlete.category));
    printf("********************************\n");

    printAthletes(f, count + 1);
}

void deleteAthlete(FILE* file) {
    int idToDelete;

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    if (fileSize == 0) {
        printf("No athletes found.\n");
        return;
    }
    rewind(file);

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
    static int searchCallCount = 0;
    searchCallCount++;


    ATHLETE athlete = { 0 };
    int age = 0;
    char name[20] = { 0 }, surname[20] = { 0 };
    rewind(file);
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    if (fileSize == 0) {
        printf("No athletes found.\n");
        return;
    }
    rewind(file);

    int choice = 0, found = 0;

    printf("***************************\n");
    printf("|    Search athlete by:   |\n");
    printf("***************************\n");
    printf("|         1. Name         |\n");
    printf("|        2. Surname       |\n");
    printf("|         3. Age          |\n");
    printf("|       4. Category       |\n");
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
    case 4: {
        char inputCategory[20];
        CATEGORY searchCategory;
        printf("Enter athlete category (KIDS, CADETS, JUNIORS, SENIORS): ");
        if (scanf("%19s", inputCategory) != 1) {
            printf("Invalid input.\n");
            return;
        }

        if (strcmp(inputCategory, "KIDS") == 0)
            searchCategory = KIDS;
        else if (strcmp(inputCategory, "CADETS") == 0)
            searchCategory = CADETS;
        else if (strcmp(inputCategory, "JUNIORS") == 0)
            searchCategory = JUNIORS;
        else if (strcmp(inputCategory, "SENIORS") == 0)
            searchCategory = SENIORS;
        else {
            printf("Unknown category.\n");
            return;
        }

        ATHLETE* athletes = NULL;
        int count = 0;
        ATHLETE temp;

        rewind(file);
        while (fread(&temp, sizeof(ATHLETE), 1, file) == 1) {
            ATHLETE* newPtr = realloc(athletes, (count + 1) * sizeof(ATHLETE));
            if (!newPtr) {
                printf("Memory allocation failed.\n");
                free(athletes);
                return;
            }
            athletes = newPtr;
            athletes[count++] = temp;
        }

        if (count == 0) {
            printf("No athletes available.\n");
            free(athletes);
            return;
        }

        qsort(athletes, count, sizeof(ATHLETE), compareByCategory);

        ATHLETE key = { 0 };
        key.category = searchCategory;

        ATHLETE* result = bsearch(&key, athletes, count, sizeof(ATHLETE), compareByCategory);

        if (!result) {
            printf("No athletes found in category %s.\n", categoryToString(searchCategory));
            free(athletes);
            return;
        }

        int index = (int)(result - athletes);

        while (index > 0 && athletes[index - 1].category == searchCategory) {
            index--;
        }

        printf("\nAthletes in category %s:\n", categoryToString(searchCategory));
        while (index < count && athletes[index].category == searchCategory) {
            printf("-----------------------------\n");
            printf("ID: %d\n", athletes[index].id);
            printf("Name: %s %s\n", athletes[index].name, athletes[index].surname);
            printf("Age: %d\n", athletes[index].age);
            printf("Height: %d cm\n", athletes[index].height);
            printf("Weight: %.1f kg\n", athletes[index].weight);
            printf("-----------------------------\n");
            index++;
        }

        free(athletes);
        return;
    }
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
        }

        if (match) {
            found = 1;
            printf("\n***************************\n");
            printf("Athlete found:\n");
            printf("ID: %d\n", athlete.id);
            printf("Name: %s %s\n", athlete.name, athlete.surname);
            printf("Age: %d\n", athlete.age);
            printf("Height: %d cm\n", athlete.height);
            printf("Weight: %.1f kg\n", athlete.weight);
            printf("*****************************\n");
        }
    }

    if (!found && choice != 4) {
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
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    if (fileSize == 0) {
        printf("No athletes found.\n");
        return;
    }
    rewind(file);

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

    int choice, order;
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

    printf("\nOrder:\n");
    printf("1. Ascending (A to Z or lowest to highest)\n");
    printf("2. Descending (Z to A or highest to lowest)\n");
    printf("Enter order: ");
    if (scanf("%d", &order) != 1 || (order != 1 && order != 2)) {
        printf("Invalid input.\n");
        while (getchar() != '\n');
        free(athletes);
        return;
    }

    switch (choice) {
    case 1:
        qsort(athletes, count, sizeof(ATHLETE),
            order == 1 ? compareByFirstNameAsc : compareByFirstNameDesc);
        break;
    case 2:
        qsort(athletes, count, sizeof(ATHLETE),
            order == 1 ? compareByLastNameAsc : compareByLastNameDesc);
        break;
    case 3:
        qsort(athletes, count, sizeof(ATHLETE),
            order == 1 ? compareByAgeAsc : compareByAgeDesc);
        break;
    case 4:
        qsort(athletes, count, sizeof(ATHLETE),
            order == 1 ? compareByCategoryAsc : compareByCategoryDesc);
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
        printf("Height: %d cm\n", athletes[i].height);
        printf("Weight: %.2f kg\n", athletes[i].weight);
        printf("Category: %s\n", categoryToString(athletes[i].category));
    }

    free(athletes);
}


static inline void printAthlete(const ATHLETE* athlete) {
    printf("Name: %s, Surname: %s, Age: %d, Category: %s\n",
        athlete->name, athlete->surname, athlete->age, categoryToString(athlete->category));
}

int compareByFirstNameAsc(const void* a, const void* b) {
    return strcmp(((ATHLETE*)a)->name, ((ATHLETE*)b)->name);
}
int compareByFirstNameDesc(const void* a, const void* b) {
    return strcmp(((ATHLETE*)b)->name, ((ATHLETE*)a)->name);
}

int compareByLastNameAsc(const void* a, const void* b) {
    return strcmp(((ATHLETE*)a)->surname, ((ATHLETE*)b)->surname);
}
int compareByLastNameDesc(const void* a, const void* b) {
    return strcmp(((ATHLETE*)b)->surname, ((ATHLETE*)a)->surname);
}

int compareByAgeAsc(const void* a, const void* b) {
    return ((ATHLETE*)a)->age - ((ATHLETE*)b)->age;
}
int compareByAgeDesc(const void* a, const void* b) {
    return ((ATHLETE*)b)->age - ((ATHLETE*)a)->age;
}

int compareByCategoryAsc(const void* a, const void* b) {
    return ((ATHLETE*)a)->category - ((ATHLETE*)b)->category;
}
int compareByCategoryDesc(const void* a, const void* b) {
    return ((ATHLETE*)b)->category - ((ATHLETE*)a)->category;
}
int compareByCategory(const void* a, const void* b) {
    const ATHLETE* A = (const ATHLETE*)a;
    const ATHLETE* B = (const ATHLETE*)b;
    return (int)A->category - (int)B->category;
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






