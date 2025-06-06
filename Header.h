#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "athletes.bin"

typedef enum {
    KIDS,
    CADETS,
    JUNIORS,
    SENIORS
} CATEGORY;

typedef struct Athlete {
    char name[20];
    char surname[20];
    int age;
    int height;
    float weight;
    int id;
    CATEGORY category;
} ATHLETE;

void addAthlete(FILE* file);
void allAthleteOverview(FILE* file);
void deleteAthlete(FILE* file);
void searchAthlete(FILE* file);
void updateAthlete(FILE* file);
void sortAthletes(FILE* file);

int compareByFirstName(const void* a, const void* b);
int compareByLastName(const void* a, const void* b);
int compareByAge(const void* a, const void* b);
int compareByCategory(const void* a, const void* b);
const char* categoryToString(CATEGORY category);
extern FILE* file;


#endif 
