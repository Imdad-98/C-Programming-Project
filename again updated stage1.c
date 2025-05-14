#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MEMBERS 5
#define MAX_NAME_LEN 50
#define DAYS 6
#define MAX_EXERCISE_LEN 50

char *milliways_members[MAX_MEMBERS][2] = {
    {"Goeun", "G"},
    {"Woncheol", "W"},
    {"Youngjin", "Y"},
    {"Hyekyung", "H"},
    {"Arthur", "A"}
};

float health_scores[MAX_MEMBERS][7];
char member_routine[MAX_MEMBERS][DAYS][MAX_EXERCISE_LEN];

const char *exercise_types[5][3] = {
    {"Running", "Cycling", "Fast Walking"},
    {"Push-ups", "Squats", ""},
    {"Leg Press", "Leg Curl", ""},
    {"Pull-ups", "Chin-ups", ""},
    {"Plank", "Crunches", ""}
};

void listMembers() {
    printf("\n--- Milliways Members ---\n");
    for (int i = 0; i < MAX_MEMBERS; i++) {
        printf("%s (%s)\n", milliways_members[i][0], milliways_members[i][1]);
    }
}

int findMemberIndexByName(char *name) {
    for (int i = 0; i < MAX_MEMBERS; i++) {
        if (strcmp(milliways_members[i][0], name) == 0) return i;
    }
    return -1;
}

void setExerciseRoutine() {
    char name[MAX_NAME_LEN];
    listMembers();
    printf("\nEnter member name: ");
    scanf("%s", name);
    int idx = findMemberIndexByName(name);
    if (idx == -1) {
        printf("Member not found.\n");
        return;
    }

    printf("\nSelect exercises for %s (%s)\n", milliways_members[idx][0], milliways_members[idx][1]);
    int core_used = 0;
    for (int day = 0; day < DAYS; day++) {
        printf("\nDay %d:\n", day + 1);

        printf("Select one Cardio exercise: 0=Running, 1=Cycling, 2=Fast Walking\n> ");
        int cardio;
        scanf("%d", &cardio);

        printf("Select one Strength/Core exercise:\n");
        printf("1=Push-ups, 2=Squats, 3=Leg Press, 4=Leg Curl, 5=Pull-ups, 6=Chin-ups, 7=Plank, 8=Crunches\n> ");
        int strength;
        scanf("%d", &strength);

        if ((strength == 7 || strength == 8) && core_used) {
            printf("Core exercise already used this week. Choose another.\n");
            day--;
            continue;
        } else if (strength == 7 || strength == 8) {
            core_used = 1;
        }

        const char *cardio_str = exercise_types[0][cardio];
        const char *strength_str;
        if (strength <= 2)
            strength_str = exercise_types[1][strength - 1];
        else if (strength <= 4)
            strength_str = exercise_types[2][strength - 3];
        else if (strength <= 6)
            strength_str = exercise_types[3][strength - 5];
        else
            strength_str = exercise_types[4][strength - 7];

        snprintf(member_routine[idx][day], MAX_EXERCISE_LEN, "%s & %s", cardio_str, strength_str);
    }
    printf("Workout routine set successfully for %s (%s).\n", milliways_members[idx][0], milliways_members[idx][1]);
}

void getExerciseRoutine() {
    char name[MAX_NAME_LEN];
    listMembers();
    printf("\nEnter member name to view routine: ");
    scanf("%s", name);
    int idx = findMemberIndexByName(name);
    if (idx == -1) {
        printf("Member not found.\n");
        return;
    }
    printf("\nWorkout Routine for %s (%s):\n", milliways_members[idx][0], milliways_members[idx][1]);
    const char *days[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    for (int i = 0; i < DAYS; i++) {
        printf("%s: %s\n", days[i], member_routine[idx][i]);
    }
}

int main() {
    int choice;
    while (1) {
        printf("\n--- Training Menu ---\n");
        printf("1. Set Basic Workout Routine\n");
        printf("2. View Basic Workout Routine\n");
        printf("0. Exit\n> ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                setExerciseRoutine();
                break;
            case 2:
                getExerciseRoutine();
                break;
            case 0:
                return 0;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
    return 0;
}
