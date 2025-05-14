#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMBER_COUNT 5
#define TEST_COUNT 7

// Member info: names and nicknames
const char *member_names[MEMBER_COUNT] = {
    "Kim Goeun", "Park Woncheol", "Lee Youngjin", "Choi Hyekyung", "Jeon Arthur"
};
const char *member_nicknames[MEMBER_COUNT] = {
    "Goeun", "Woncheol", "Youngjin", "Hyekyung", "Arthur"
};

// Fitness data: [member][test index]
float health_scores[MEMBER_COUNT][TEST_COUNT];

// Stage completion status
char stageStatus[8] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};

// Helper to find member index by nickname
int findMemberIndex(const char *nickname) {
    for (int i = 0; i < MEMBER_COUNT; i++) {
        if (strcmp(nickname, member_nicknames[i]) == 0)
            return i;
    }
    return -1;
}

void setHealth() {
    char input[256];
    printf("\nEnter member nickname: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    int index = findMemberIndex(input);

    if (index == -1) {
        printf("Member not found.\n");
        return;
    }

    printf("Enter 7 fitness test results (comma-separated):\n");
    printf("1-Mile Run (min), 100m Sprint (sec), Push-ups (min), Squats (min), Arm Strength (min), Swimming (min), Weightlifting (multiplier):\n> ");

    fgets(input, sizeof(input), stdin);

    char *token = strtok(input, ",");
    int count = 0;
    while (token != NULL && count < TEST_COUNT) {
        health_scores[index][count++] = atof(token);
        token = strtok(NULL, ",");
    }

    if (count != TEST_COUNT) {
        printf("Error: Expected 7 test values.\n");
        return;
    }

    printf("Fitness data recorded for %s.\n", member_nicknames[index]);
}

void getHealth() {
    char input[50];
    printf("\n1. View all\n2. View member\n3. View specific test of a member\n> ");
    fgets(input, sizeof(input), stdin);
    int option = atoi(input);

    if (option == 1) {
        for (int i = 0; i < MEMBER_COUNT; i++) {
            printf("\n%s (%s):\n", member_names[i], member_nicknames[i]);
            for (int j = 0; j < TEST_COUNT; j++)
                printf("  Test %d: %.2f\n", j + 1, health_scores[i][j]);
        }
    } else if (option == 2) {
        printf("Enter member nickname: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;
        int idx = findMemberIndex(input);
        if (idx == -1) {
            printf("Member not found.\n");
            return;
        }
        printf("\n%s (%s):\n", member_names[idx], member_nicknames[idx]);
        for (int j = 0; j < TEST_COUNT; j++)
            printf("  Test %d: %.2f\n", j + 1, health_scores[idx][j]);

    } else if (option == 3) {
        printf("Enter member nickname: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;
        int idx = findMemberIndex(input);
        if (idx == -1) {
            printf("Member not found.\n");
            return;
        }
        printf("Enter test number (1-7): ");
        fgets(input, sizeof(input), stdin);
        int testIdx = atoi(input) - 1;
        if (testIdx < 0 || testIdx >= TEST_COUNT) {
            printf("Invalid test index.\n");
            return;
        }
        printf("%s (%s) Test %d: %.2f\n", member_names[idx], member_nicknames[idx], testIdx + 1, health_scores[idx][testIdx]);
    } else {
        printf("Invalid option.\n");
    }
}

int canAccessStage(int stage) {
    if (stage == 1) return 1;
    if (stage == 2 && stageStatus[0] == 'P') return 1;
    if (stage >= 3 && stage <= 8 && stageStatus[0] == 'P' && stageStatus[1] == 'P') return 1;
    return 0;
}

void physicalStrengthSubMenu() {
    while (1) {
        printf("\nA. Enter Fitness Data\nB. View Fitness Data\nQ. Back\n> ");
        char input[10];
        fgets(input, sizeof(input), stdin);
        if (input[0] == 'A' || input[0] == 'a') setHealth();
        else if (input[0] == 'B' || input[0] == 'b') getHealth();
        else if (input[0] == 'Q' || input[0] == 'q') break;
        else printf("Invalid input.\n");
    }
}

void handleTrainingStage(int stage) {
    if (!canAccessStage(stage)) {
        printf("You must pass previous stages first.\n");
        return;
    }

    if (stageStatus[stage - 1] == 'P') {
        printf("Stage already completed.\n");
        return;
    }

    if (stage == 1) physicalStrengthSubMenu();

    printf("Would you like to enter the evaluation result? (Y/N): ");
    char input[10];
    fgets(input, sizeof(input), stdin);
    if (input[0] == 'Y' || input[0] == 'y') {
        printf("Did you complete the training and pass the certification? (Y/N): ");
        fgets(input, sizeof(input), stdin);
        if (input[0] == 'Y' || input[0] == 'y') {
            stageStatus[stage - 1] = 'P';
            printf("Stage %d marked as passed.\n", stage);
        } else {
            stageStatus[stage - 1] = 'F';
            printf("Stage %d marked as failed.\n", stage);
        }
    }
}

int allStagesPassed() {
    for (int i = 0; i < 8; i++)
        if (stageStatus[i] != 'P') return 0;
    return 1;
}

void trainingMenu() {
    while (1) {
        printf("\n--- Training Menu ---\n");
        printf("1. Physical Strength & Knowledge\n");
        printf("2. Self-Management & Teamwork\n");
        printf("3. Language & Pronunciation\n");
        printf("4. Vocal\n");
        printf("5. Dance\n");
        printf("6. Visual & Image\n");
        printf("7. Acting & Stage Performance\n");
        printf("8. Fan Communication\n");
        printf("Q. Back\n> ");

        char input[10];
        fgets(input, sizeof(input), stdin);
        if (input[0] == 'Q' || input[0] == 'q') break;

        int stage = atoi(input);
        if (stage >= 1 && stage <= 8)
            handleTrainingStage(stage);
        else
            printf("Invalid selection.\n");
    }
}

void mainMenu() {
    while (1) {
        printf("\n=== Main Menu ===\n");
        printf("I. Audition Management\n");
        printf("II. Training\n");
        printf("III. Debut\n");
        printf("Q. Quit\n> ");

        char input[10];
        fgets(input, sizeof(input), stdin);

        if (input[0] == 'Q' || input[0] == 'q' || input[0] == '0' || input[0] == '\n') break;
        else if (input[0] == 'I' || input[0] == 'i') printf("Audition management not yet implemented.\n");
        else if (input[0] == 'I' && input[1] == 'I') trainingMenu();
        else if (input[0] == 'I' && input[1] == 'I' && input[2] == 'I') {
            if (allStagesPassed()) printf("Congratulations! You are ready for debut.\n");
            else printf("You must pass all training stages first.\n");
        } else if (input[0] == '2') trainingMenu();
        else if (input[0] == '3') {
            if (allStagesPassed()) printf("Congratulations! You are ready for debut.\n");
            else printf("You must pass all training stages first.\n");
        }
        else printf("Invalid input.\n");
    }
}

int main() {
    mainMenu();
    printf("Exiting program.\n");
    return 0;
}
