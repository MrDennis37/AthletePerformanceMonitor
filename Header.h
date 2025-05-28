#define _CRT_SECURE_NO_WARNINGS
#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>

#define FILE_NAME "athletes.bin"

typedef struct Athlete {
    char name[20];
    char surname[20];
    int age;
    int height;
    float weight;
    int id;
	CATEGORY category;
} ATHLETE;

typedef enum {
    KIDS,
    CADETS,
    JUNIORS,
    SENIORS
} CATEGORY;


typedef enum {
    sortAlphabetically,
    sortReverseAlphabetically,
    categoryUpwards,
    categoryDownwards,
    ageUpwards,
    ageDownwards,
    povratak
} SortOption;


void addAthlete(FILE* file);
void allAthleteOverview(FILE* file);
void deleteAthlete(FILE* file);
void searchAthlete(FILE* file);
void updateAthlete(FILE* file);
void sortAthletes(FILE* file);

#endif
