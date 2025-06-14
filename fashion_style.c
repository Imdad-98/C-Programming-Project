#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME_LEN 50
#define MAX_STYLE_LEN 100
#define MAX_STYLES 100
#define DEQUE_SIZE 21  // Enough space for worst styles before and after center
#define LINE_LEN 200

// Member struct
typedef struct {
    char nickname[MAX_NAME_LEN];
    char name[MAX_NAME_LEN];
    char gender; // 'M' or 'F'
} Member;

// Fashion Style struct
typedef struct {
    char style[MAX_STYLE_LEN];
    char reason[MAX_STYLE_LEN];
} FashionStyle;

// Deque struct
typedef struct {
    FashionStyle* items[DEQUE_SIZE];
    int front;
    int rear;
    int size;
} Deque;

// Global Arrays
FashionStyle maleStyles[MAX_STYLES];
int maleStyleCount = 0;
FashionStyle femaleStyles[MAX_STYLES];
int femaleStyleCount = 0;
Member* members = NULL;
int memberCount = 0;

// Deque functions
void initDeque(Deque* dq) {
    dq->front = dq->rear = DEQUE_SIZE / 2;
    dq->size = 0;
}

int isFull(Deque* dq) {
    return dq->size == DEQUE_SIZE;
}

int isEmpty(Deque* dq) {
    return dq->size == 0;
}

void pushFront(Deque* dq, FashionStyle* fs) {
    if (isFull(dq)) {
        printf("Deque is full!\n");
        return;
    }
    dq->front = (dq->front - 1 + DEQUE_SIZE) % DEQUE_SIZE;
    dq->items[dq->front] = fs;
    dq->size++;
}

void pushRear(Deque* dq, FashionStyle* fs) {
    if (isFull(dq)) {
        printf("Deque is full!\n");
        return;
    }
    dq->items[dq->rear] = fs;
    dq->rear = (dq->rear + 1) % DEQUE_SIZE;
    dq->size++;
}

void printDeque(Deque* dq) {
    printf("\n--- Fashion Style Deque ---\n");
    int i = dq->front;
    for (int count = 0; count < dq->size; count++) {
        printf("%s: %s\n", dq->items[i]->style, dq->items[i]->reason);
        i = (i + 1) % DEQUE_SIZE;
    }
}

// Load fashion styles
void loadFashionStyles(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Cannot open fashion.txt");
        exit(1);
    }

    char line[LINE_LEN];
    while (fgets(line, sizeof(line), file)) {
        FashionStyle fs;
        char gender;
        if (sscanf(line, "%c,%[^,],%[^\n]", &gender, fs.style, fs.reason) == 3) {
            if (gender == 'M') {
                maleStyles[maleStyleCount++] = fs;
            } else if (gender == 'F') {
                femaleStyles[femaleStyleCount++] = fs;
            }
        }
    }
    fclose(file);
}

// Load members
void loadMembers(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Cannot open milliways_member.csv");
        exit(1);
    }

    char line[LINE_LEN];
    while (fgets(line, sizeof(line), file)) {
        memberCount++;
    }
    rewind(file);

    members = malloc(sizeof(Member) * memberCount);
    int i = 0;
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^,],%[^,],%c", members[i].nickname, members[i].name, &members[i].gender);
        i++;
    }
    fclose(file);
}

Member* findMemberByNickname(const char* nickname) {
    for (int i = 0; i < memberCount; i++) {
        if (strcmp(members[i].nickname, nickname) == 0) {
            return &members[i];
        }
    }
    return NULL;
}

void understandFashion() {
    char input[MAX_NAME_LEN];
    printf("Enter trainee nickname: ");
    scanf("%s", input);

    Member* m = findMemberByNickname(input);
    if (!m) {
        printf("Nickname not found.\n");
        return;
    }

    FashionStyle* styles = (m->gender == 'M') ? maleStyles : femaleStyles;
    int styleCount = (m->gender == 'M') ? maleStyleCount : femaleStyleCount;

    printf("Worst fashion styles for %s:\n", (m->gender == 'M') ? "men" : "women");
    for (int i = 0; i < styleCount; i++) {
        printf("%d. %s (%s)\n", i + 1, styles[i].style, styles[i].reason);
    }

    srand((unsigned)time(NULL));
    int center = rand() % styleCount;
    printf("\nRandom style chosen: %s\n", styles[center].style);

    Deque dq;
    initDeque(&dq);
    pushRear(&dq, &styles[center]);

    char choice;
    printf("Choose to fill (F)ront or (R)ear: ");
    scanf(" %c", &choice);

    if (choice == 'F' || choice == 'f') {
        for (int i = center - 1; i >= 0 && !isFull(&dq); i--) {
            pushFront(&dq, &styles[i]);
        }
        for (int i = center + 1; i < styleCount && !isFull(&dq); i++) {
            pushRear(&dq, &styles[i]);
        }
    } else {
        for (int i = center + 1; i < styleCount && !isFull(&dq); i++) {
            pushRear(&dq, &styles[i]);
        }
        for (int i = center - 1; i >= 0 && !isFull(&dq); i--) {
            pushFront(&dq, &styles[i]);
        }
    }

    if (isFull(&dq)) {
        printf("\nCongratulations! Deque is full.\n");
        printDeque(&dq);
    }
}

// Call this in stage6.c menu system
// case 'C': understandFashion(); break;
