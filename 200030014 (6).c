#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int count=0;
int flag=0;
// Structure for a node in the linked list
struct Node {
    char word[100];
    struct Node* next;
};

// Function to compute the hash value for a given word
int computeHash(const char* word, int tableSize) {
    int sum = 0;
    for (int i = 0; word[i] != '\0'; i++) {
        sum += (int)word[i];
    }
    return sum % tableSize;
}

// Function to insert a word into the hash table
void insertWord(struct Node** hashTable, const char* word, int tableSize) {
    int hashValue = computeHash(word, tableSize);
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    strcpy(newNode->word, word);
    newNode->next = hashTable[hashValue];
    hashTable[hashValue] = newNode;
}

// Function to check if two words are anagrams
int areAnagrams(const char* word1, const char* word2) {
    int count[256] = {0};

    for (int i = 0; word1[i] != '\0'; i++) {
        count[(int)word1[i]]++;
    }

    for (int i = 0; word2[i] != '\0'; i++) {
        count[(int)word2[i]]--;
    }

    for (int i = 0; i < 256; i++) {
        if (count[i] != 0) {
            return 0; // Not an anagram
        }
    }

    return 1; // Anagram
}

// Function to find anagrams of a word in the hash table
void findAnagrams(struct Node** hashTable, const char* word, int tableSize, FILE* outputFile) {
    int hashValue = computeHash(word, tableSize);
    struct Node* currentNode = hashTable[hashValue];
    char anagrams[10000];
    anagrams[0] = '\0';

    while (currentNode != NULL) {
        if (areAnagrams(word, currentNode->word)) {
            strcat(anagrams, currentNode->word);
            strcat(anagrams, " ");
        }
        currentNode = currentNode->next;
    }
    char anagram[10000];
    int size = 0;
    for (int i = 0; anagrams[i] != '\0'; i++) {
        size++;
    }
    for (int i = 0; i <= 999; i++) {
        anagram[i] = '\0';
    }
    int i;
    for (i = 0; i < size - 1; i++) {
        anagram[i] = anagrams[i];
    }
    if(count==0) fprintf(outputFile, "%s",anagram);
    else{
        if(anagram[0]=='\0'){
            fprintf(outputFile, "\n");
            flag=count;
        } else{
            fprintf(outputFile, "\n%s",anagram);
        }
    } 
    count++;
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        printf("Insufficient arguments.\n");
        printf("Usage: ./a.out words.txt table_size query.txt\n");
        return 1;
    }

    const char* wordsFile = argv[1];
    int tableSize = atoi(argv[2]);
    const char* queryFile = argv[3];

    // Create the hash table
    struct Node** hashTable = (struct Node**)malloc(tableSize * sizeof(struct Node));
    for (int i = 0; i < tableSize; i++) {
        hashTable[i] = NULL;
    }

    // Read words from the input file and insert them into the hash table
    FILE* wordsInputFile = fopen(wordsFile, "r");
    if (wordsInputFile == NULL) {
        printf("Unable to open words file: %s\n", wordsFile);
        return 1;
    }

    char word[100];
    while (fgets(word, sizeof(word), wordsInputFile) != NULL) {
        word[strcspn(word, "\n")] = '\0'; // Remove trailing newline character
        insertWord(hashTable, word, tableSize);
    }

    fclose(wordsInputFile);

    // Process the query file and find anagrams
    FILE* queryInputFile = fopen(queryFile, "r");
    if (queryInputFile == NULL) {
        printf("Unable to open query file: %s\n", queryFile);
        return 1;
    }

    FILE* outputFile = fopen("anagrams.txt", "w");
    if (outputFile == NULL) {
        printf("Unable to create output file: anagrams.txt\n");
        return 1;
    }

    while (fgets(word, sizeof(word), queryInputFile) != NULL) {
        word[strcspn(word, "\n")] = '\0'; // Remove trailing newline character
        findAnagrams(hashTable, word, tableSize, outputFile);
    }
    if(count==flag+1) fprintf(outputFile, "\n");
    fclose(queryInputFile);
    fclose(outputFile);

    // Free the memory allocated for the hash table
    for (int i = 0; i < tableSize; i++) {
        struct Node* currentNode = hashTable[i];
        while (currentNode != NULL) {
            struct Node* temp = currentNode;
            currentNode = currentNode->next;
            free(temp);
        }
    }
    free(hashTable);

    return 0;
}
