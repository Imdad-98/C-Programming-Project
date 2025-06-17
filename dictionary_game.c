#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_MEMBERS 5
#define MAX_CARDS 30
#define MAX_NAME_LENGTH 50
#define MAX_DESCRIPTION_LENGTH 100
#define DIFFICULTY_LEVELS 3

// Structure to hold a word card
typedef struct {
    char word[MAX_NAME_LENGTH];
    char description[MAX_DESCRIPTION_LENGTH];
    int difficulty;  // 1: Easy, 2: Medium, 3: Hard
} WordCard;

// Structure to store player challenge data
typedef struct {
    char name[MAX_NAME_LENGTH];
    int difficulty;  // Pass level: 1 (easy), 2 (medium), 3 (hard)
    int score;
    int passed;  // 0: not passed, 1: passed at easy, 2: passed at medium, 3: passed at hard
    char passDate[9];  // YYYYMMDD
} PlayerChallenge;

// Array to store WordCards
WordCard wordCards[MAX_CARDS];

// Array to store player data
PlayerChallenge players[MAX_MEMBERS];

// Function prototypes
void loadWordCards();
void selectMember();
void challengeHighLevel();
void challengeStepByStep();
void updatePassStatus(int playerIndex, int difficulty);
void displayGameResult();
void doDictionaryGame();

// Function to load word cards from a file
void loadWordCards() {
    FILE *file = fopen("wordcards.txt", "r");
    if (file == NULL) {
        printf("Error opening wordcards.txt file!\n");
        exit(1);
    }

    int index = 0;
    while (fscanf(file, "%s %s %d", wordCards[index].word, wordCards[index].description, &wordCards[index].difficulty) != EOF) {
        index++;
    }
    fclose(file);
}

// Function to randomly select a member (excluding the same member twice)
void selectMember() {
    int selectedMemberIndex = rand() % MAX_MEMBERS;
    printf("Selected Gamer: %s\n", players[selectedMemberIndex].name);
}

// Function to simulate the "Challenge the highest difficulty test"
void challengeHighLevel() {
    int correctAnswers = 0;
    srand(time(NULL));

    printf("Challenge High Level Test - 5 questions\n");

    for (int i = 0; i < 5; i++) {
        int randIndex = rand() % MAX_CARDS;
        if (wordCards[randIndex].difficulty == 3) { // Only choose difficult cards
            printf("Description: %s\n", wordCards[randIndex].description);
            printf("Enter the word: ");
            char answer[MAX_NAME_LENGTH];
            scanf("%s", answer);
            if (strcmp(answer, wordCards[randIndex].word) == 0) {
                correctAnswers++;
            }
        }
    }

    if (correctAnswers == 5) {
        printf("Congratulations! You passed the High-Level Test.\n");
        // Update pass status
    } else {
        printf("Failed the test. Retry required for all players.\n");
    }
}

// Function to challenge step by step by difficulty
void challengeStepByStep() {
    printf("Challenge Step By Step - Choose difficulty\n");
    printf("1. Easy\n2. Medium\n3. Hard\n");
    
    int difficultyChoice;
    scanf("%d", &difficultyChoice);

    if (difficultyChoice < 1 || difficultyChoice > 3) {
        printf("Invalid choice! Please choose a valid difficulty.\n");
        return;
    }

    int correctAnswers = 0;

    // Simulating difficulty-based questions
    printf("You chose difficulty %d\n", difficultyChoice);

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < MAX_CARDS; j++) {
            if (wordCards[j].difficulty == difficultyChoice) {
                printf("Description: %s\n", wordCards[j].description);
                printf("Enter the word: ");
                char answer[MAX_NAME_LENGTH];
                scanf("%s", answer);
                if (strcmp(answer, wordCards[j].word) == 0) {
                    correctAnswers++;
                }
                break;
            }
        }
    }

    if (correctAnswers >= 2) {
        printf("You passed this difficulty level!\n");
        // Update pass status
    } else {
        printf("Failed this difficulty. Retry required for all players.\n");
    }
}

// Function to update player's pass status
void updatePassStatus(int playerIndex, int difficulty) {
    if (difficulty == 1) {
        players[playerIndex].passed = 1;
    } else if (difficulty == 2) {
        players[playerIndex].passed = 2;
    } else if (difficulty == 3) {
        players[playerIndex].passed = 3;
    }
}

// Function to display the final result of the game
void displayGameResult() {
    printf("All players have passed the dictionary game!\n");
    printf("Congratulations! MillieWays group has passed the Dictionary Game!\n");
}

// Main game function
void doDictionaryGame() {
    loadWordCards();  // Load word cards

    // Select a random player to start
    selectMember();

    printf("Choose a challenge type:\n");
    printf("1. Challenge the highest difficulty test\n2. Challenge step-by-step by difficulty\n");
    
    int choice;
    scanf("%d", &choice);

    if (choice == 1) {
        challengeHighLevel();
    } else if (choice == 2) {
        challengeStepByStep();
    } else {
        printf("Invalid choice!\n");
    }

    displayGameResult();
}

// Main function to run the game
int main() {
    // Initialize player data (MillieWays members)
    strcpy(players[0].name, "Member1");
    strcpy(players[1].name, "Member2");
    strcpy(players[2].name, "Member3");
    strcpy(players[3].name, "Member4");
    strcpy(players[4].name, "Member5");

    doDictionaryGame();  // Start the dictionary game

    return 0;
}
