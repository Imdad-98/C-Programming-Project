#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SENTENCE_COUNT 10
#define WORD_COUNT 10
#define MAX_WORD_LEN 20
#define MAX_TRAINEES 10
#define NAME_LEN 20

typedef struct {
    char eng[256];
    char kor[WORD_COUNT][MAX_WORD_LEN];
    int korWordCount;
} SentenceSet;

typedef struct {
    char nickname[NAME_LEN];
    int grammarScore;
    int hasPlayed;
} Learner;

SentenceSet sentences[SENTENCE_COUNT] = {
    {"I like to eat apples", {"나는", "사과를", "먹는", "것을", "좋아해", "요"}, 6},
    {"She is reading a book", {"그녀는", "책을", "읽고", "있어요", "정말", "재미있게"}, 6},
    {"We went to the market", {"우리는", "시장에", "갔어요", "어제", "아침에", "같이"}, 6},
    {"They play soccer every weekend", {"그들은", "매주", "주말마다", "축구를", "해요", "즐겁게"}, 6},
    {"He studies Korean everyday", {"그는", "매일", "한국어를", "공부해요", "집에서", "혼자서"}, 6},
    {"You are very kind", {"당신은", "정말", "친절해요", "항상", "언제나", "미소로"}, 6},
    {"The cat is sleeping", {"고양이가", "자고", "있어요", "소파에서", "편안하게", "조용히"}, 6},
    {"I am learning C language", {"나는", "C", "언어를", "배우고", "있어요", "열심히"}, 6},
    {"My brother is tall", {"내", "형은", "키가", "커요", "정말", "많이"}, 6},
    {"We are friends", {"우리는", "친구예요", "오래된", "좋은", "진짜", "친구"}, 6}
};

Learner trainees[MAX_TRAINEES] = {
    {"Alice", 0, 0}, {"Bob", 0, 0}, {"Charlie", 0, 0}, {"Diana", 0, 0},
    {"Eve", 0, 0}, {"Frank", 0, 0}, {"Grace", 0, 0}, {"Hank", 0, 0},
    {"Ivy", 0, 0}, {"Jack", 0, 0}
};

void shuffleWords(char arr[][MAX_WORD_LEN], int n) {
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            if (strcmp(arr[j], arr[minIdx]) < 0) {
                minIdx = j;
            }
        }
        if (minIdx != i) {
            char temp[MAX_WORD_LEN];
            strcpy(temp, arr[i]);
            strcpy(arr[i], arr[minIdx]);
            strcpy(arr[minIdx], temp);
        }
    }
}

void composeSentences(int learnerIndex) {
    int score = 0;
    int used[SENTENCE_COUNT] = {0};
    for (int i = 0; i < 5; i++) {
        int idx;
        do {
            idx = rand() % SENTENCE_COUNT;
        } while (used[idx]);
        used[idx] = 1;

        char words[WORD_COUNT][MAX_WORD_LEN];
        int count = sentences[idx].korWordCount;
        for (int j = 0; j < count; j++) {
            strcpy(words[j], sentences[idx].kor[j]);
        }

        shuffleWords(words, count);
        printf("\nEnglish: %s\n", sentences[idx].eng);
        printf("Korean words: ");
        for (int j = 0; j < count; j++) {
            printf("%s ", words[j]);
        }
        printf("\nYour sentence: ");

        char answer[256];
        getchar(); // flush newline
        fgets(answer, sizeof(answer), stdin);
        answer[strcspn(answer, "\n")] = '\0';

        char correct[256] = "";
        for (int j = 0; j < count; j++) {
            strcat(correct, sentences[idx].kor[j]);
            if (j < count - 1) strcat(correct, " ");
        }

        if (strcmp(answer, correct) == 0) {
            printf("Correct!\n");
            score += 20;
        } else {
            printf("Incorrect. Correct sentence: %s\n", correct);
        }
    }
    trainees[learnerIndex].grammarScore = score;
    trainees[learnerIndex].hasPlayed = 1;
    printf("\nYour total score: %d\n", score);
}

void printScore(int learnerIndex) {
    if (!trainees[learnerIndex].hasPlayed) {
        printf("You must complete the quiz first!\n");
        return;
    }

    printf("Your score: %d\n", trainees[learnerIndex].grammarScore);

    // Sort by score descending
    for (int i = 0; i < MAX_TRAINEES - 1; i++) {
        for (int j = i + 1; j < MAX_TRAINEES; j++) {
            if (trainees[i].grammarScore < trainees[j].grammarScore) {
                Learner temp = trainees[i];
                trainees[i] = trainees[j];
                trainees[j] = temp;
            }
        }
    }

    printf("\nLeaderboard:\n");
    for (int i = 0; i < MAX_TRAINEES; i++) {
        if (trainees[i].hasPlayed) {
            printf("%d. %s - %d points\n", i + 1, trainees[i].nickname, trainees[i].grammarScore);
        }
    }
}

void learnKoreanGrammar() {
    char nickname[NAME_LEN];
    int foundIndex = -1, attempts = 0;

    while (attempts < 3 && foundIndex == -1) {
        printf("Enter your nickname: ");
        scanf("%s", nickname);
        for (int i = 0; i < MAX_TRAINEES; i++) {
            if (strcmp(trainees[i].nickname, nickname) == 0) {
                foundIndex = i;
                break;
            }
        }
        if (foundIndex == -1) {
            printf("Nickname not found. Try again.\n");
            attempts++;
        }
    }
    if (foundIndex == -1) {
        printf("Too many failed attempts.\n");
        return;
    }

    int choice;
    printf("\n1. Compose Korean Sentences\n2. Check Scores\nSelect option: ");
    scanf("%d", &choice);

    if (choice == 1) {
        composeSentences(foundIndex);
    } else if (choice == 2) {
        printScore(foundIndex);
    } else {
        printf("Invalid choice. Exiting.\n");
    }
}

int main() {
    srand((unsigned int)time(NULL));
    int running = 1, menuChoice;

    while (running) {
        printf("\n--- Stage 3: Korean Grammar Game ---\n");
        printf("1. Learn Korean Grammar\n0. Exit\nSelect option: ");
        scanf("%d", &menuChoice);
        switch (menuChoice) {
            case 1:
                learnKoreanGrammar();
                break;
            case 0:
                running = 0;
                break;
            default:
                printf("Invalid input. Try again.\n");
        }
    }
    return 0;
}
