#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_TRAINEES 10
#define MAX_MENTORS 8
#define MAX_QUESTIONS 5
#define MAX_RESPONSES 100
#define MAX_COUNSELING 30

typedef struct {
    char nickname[50];
    int ability;
    int parsedNickname;
} Trainee;

typedef struct {
    int mentorID;
    char name[50];
    int menteeID;
} Mentor;

typedef struct {
    char nickname[50];
    int age;
    char trauma[200];
} Trauma;

typedef struct {
    int id;
    char content[150];
} Question;

typedef struct {
    char nickname[50];
    int questionID;
    char response[MAX_RESPONSES];
} Counseling;

Trainee trainees[MAX_TRAINEES];
Mentor mentors[MAX_MENTORS];
Trauma traumas[MAX_TRAINEES];
Counseling counselingSessions[MAX_COUNSELING];
Question questions[MAX_QUESTIONS] = {
    {1, "In what situations have you experienced this trauma?"},
    {2, "How has this situation affected your daily life and emotions?"},
    {3, "How have you tried to overcome this trauma?"},
    {4, "What emotions do you associate with this trauma?"},
    {5, "What kind of support do you think is necessary to overcome this trauma?"}
};
int counselingCount = 0;

void parseIntMember();
int getRandomAbility();
void initializeTrainees();
void inputMentors();
void shuffleTrainees();
void matchMentoring();
void printMentoringResults();
void overcomeTrauma();
void traumaMenu();
void inputTrauma();
void counselingMenu();
void viewCompleted();

int main() {
    srand(time(NULL));
    initializeTrainees();
    inputMentors();
    matchMentoring();
    printMentoringResults();
    overcomeTrauma();
    return 0;
}

void parseIntMember() {
    for (int i = 0; i < MAX_TRAINEES; i++) {
        int sum = 0;
        for (int j = 0; trainees[i].nickname[j] != '\0'; j++) {
            sum += trainees[i].nickname[j];
        }
        trainees[i].parsedNickname = sum;
    }
}

int getRandomAbility() {
    return (rand() % 901) + 100;
}

void initializeTrainees() {
    strcpy(trainees[0].nickname, "Ariel");
    strcpy(trainees[1].nickname, "Simba");
    strcpy(trainees[2].nickname, "Belle");
    strcpy(trainees[3].nickname, "Aladdin");
    for (int i = 0; i < 4; i++) {
        trainees[i].ability = getRandomAbility();
        strcpy(traumas[i].nickname, trainees[i].nickname);
        traumas[i].age = 18;
    }
    parseIntMember();
}

void inputMentors() {
    for (int i = 0; i < MAX_MENTORS; i++) {
        mentors[i].mentorID = i + 1;
        printf("Enter mentor %d name: ", i + 1);
        fgets(mentors[i].name, sizeof(mentors[i].name), stdin);
        mentors[i].name[strcspn(mentors[i].name, "\n")] = '\0';
        mentors[i].menteeID = -1;
    }
}

void shuffleTrainees() {
    for (int i = 0; i < 4; i++) {
        int j = rand() % 4;
        Trainee temp = trainees[i];
        trainees[i] = trainees[j];
        trainees[j] = temp;
    }
}

void matchMentoring() {
    shuffleTrainees();
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < MAX_MENTORS; j++) {
            if (mentors[j].menteeID == -1) {
                mentors[j].menteeID = i;
                break;
            }
        }
    }
}

void printMentoringResults() {
    printf("\nMentor-Mentee Assignments:\n");
    for (int i = 0; i < MAX_MENTORS; i++) {
        if (mentors[i].menteeID != -1) {
            printf("Mentor ID: %d, Mentor Name: %s, Trainee Nickname: %s\n",
                   mentors[i].mentorID, mentors[i].name,
                   trainees[mentors[i].menteeID].nickname);
        }
    }
}

void overcomeTrauma() {
    char choice;
    while (1) {
        traumaMenu();
        scanf(" %c", &choice);
        getchar();
        switch (tolower(choice)) {
            case '1': inputTrauma(); break;
            case '2': counselingMenu(); break;
            case '3': viewCompleted(); break;
            case 'q': return;
            default: printf("Invalid choice. Try again.\n");
        }
    }
}

void traumaMenu() {
    printf("\n[Trauma Management]\n");
    printf("1. Input Trauma Description\n");
    printf("2. Counseling Session\n");
    printf("3. View Completed Counseling\n");
    printf("Q. Quit\n");
    printf("Select: ");
}

void inputTrauma() {
    char name[50];
    while (1) {
        printf("Enter nickname (or type 'exit'): ");
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = '\0';
        if (strcmp(name, "exit") == 0) return;
        int found = 0;
        for (int i = 0; i < 4; i++) {
            if (strcmp(name, traumas[i].nickname) == 0) {
                printf("Enter trauma description: ");
                fgets(traumas[i].trauma, sizeof(traumas[i].trauma), stdin);
                traumas[i].trauma[strcspn(traumas[i].trauma, "\n")] = '\0';
                found = 1;
                break;
            }
        }
        found ? printf("Trauma recorded.\n") : printf("Nickname not found.\n");
    }
}

void counselingMenu() {
    char name[50];
    int available = 0;
    printf("\nMembers with trauma:\n");
    for (int i = 0; i < 4; i++) {
        if (strlen(traumas[i].trauma) > 0) {
            printf("- %s\n", traumas[i].nickname);
            available = 1;
        }
    }
    if (!available) {
        printf("No members with trauma entries.\n");
        return;
    }

    printf("Enter nickname to start counseling (or type 'exit'): ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';
    if (strcmp(name, "exit") == 0) return;

    int completed[5] = {0};
    for (int i = 0; i < counselingCount; i++) {
        if (strcmp(counselingSessions[i].nickname, name) == 0) {
            completed[counselingSessions[i].questionID - 1] = 1;
        }
    }

    int asked = 0;
    while (asked < 3) {
        int q = rand() % 5;
        if (completed[q]) continue;

        printf("Q%d: %s\n", questions[q].id, questions[q].content);
        char answer[MAX_RESPONSES];
        while (1) {
            printf("Answer (or type 'pause'): ");
            fgets(answer, sizeof(answer), stdin);
            answer[strcspn(answer, "\n")] = '\0';
            if (strcmp(answer, "pause") == 0) return;
            if (strlen(answer) == 0 || strlen(answer) >= MAX_RESPONSES) {
                printf("Invalid input. Try again.\n");
            } else break;
        }

        strcpy(counselingSessions[counselingCount].nickname, name);
        counselingSessions[counselingCount].questionID = questions[q].id;
        strcpy(counselingSessions[counselingCount].response, answer);
        counselingCount++;
        completed[q] = 1;
        asked++;
    }
    printf("Counseling session for %s completed.\n", name);
}

void viewCompleted() {
    char name[50];
    printf("\nCompleted Counseling Members:\n");
    int listed = 0;
    for (int i = 0; i < counselingCount; i++) {
        int isLast = (i == counselingCount - 1);
        if ((i == 0 || strcmp(counselingSessions[i].nickname, counselingSessions[i-1].nickname) != 0)) {
            printf("- %s\n", counselingSessions[i].nickname);
            listed = 1;
        }
    }
    if (!listed) {
        printf("No completed sessions.\n");
        return;
    }
    printf("Enter nickname to view session: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';

    for (int i = 0; i < 4; i++) {
        if (strcmp(name, traumas[i].nickname) == 0) {
            printf("\nNickname: %s\nAge: %d\nTrauma: %s\n",
                   traumas[i].nickname, traumas[i].age, traumas[i].trauma);
            break;
        }
    }
    for (int i = 0; i < counselingCount; i++) {
        if (strcmp(counselingSessions[i].nickname, name) == 0) {
            printf("Q: %s\nA: %s\n",
                   questions[counselingSessions[i].questionID - 1].content,
                   counselingSessions[i].response);
        }
    }
}
