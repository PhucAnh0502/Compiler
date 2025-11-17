#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 100
#define MAX_LINE_LENGTH 2048
#define MAX_STOP_WORDS 500
#define MAX_INDEX_SIZE 10000
#define MAX_LINE_LIST_LENGTH 1024

typedef struct
{
    char word[MAX_WORD_LENGTH];
    int count; 
    char lines[MAX_LINE_LIST_LENGTH];
    int lastLineAdded;
} IndexEntry;

char stopWords[MAX_STOP_WORDS][MAX_WORD_LENGTH];
int stopWordCount = 0;
IndexEntry index[MAX_INDEX_SIZE];
int indexCount = 0;

void loadStopWords(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Failed to open stop words file");
        return;
    }

    while (stopWordCount < MAX_STOP_WORDS && fgets(stopWords[stopWordCount], MAX_WORD_LENGTH, file))
    {
        stopWords[stopWordCount][strcspn(stopWords[stopWordCount], "\r\n")] = 0;

        for (int i = 0; stopWords[stopWordCount][i]; i++)
        {
            stopWords[stopWordCount][i] = tolower(stopWords[stopWordCount][i]);
        }
        
        if (strlen(stopWords[stopWordCount]) > 0)
        {
            stopWordCount++;
        }
    }
    fclose(file);
    printf("Loaded %d stop words.\n", stopWordCount);
}

int isStopWord(const char *word)
{
    for (int i = 0; i < stopWordCount; i++)
    {
        if (strcmp(stopWords[i], word) == 0)
        {
            return 1;
        }
    }
    return 0;
}

void addToIndex(const char *word, int lineNumber)
{
    for (int i = 0; i < indexCount; i++)
    {
        if (strcmp(index[i].word, word) == 0)
        {
            index[i].count++;

            if (index[i].lastLineAdded != lineNumber)
            {
                char lineStr[12];
                sprintf(lineStr, ", %d", lineNumber);

                if (strlen(index[i].lines) + strlen(lineStr) < MAX_LINE_LIST_LENGTH)
                {
                    strcat(index[i].lines, lineStr);
                    index[i].lastLineAdded = lineNumber;
                }
            }
            return;
        }
    }

    if (indexCount < MAX_INDEX_SIZE)
    {
        strcpy(index[indexCount].word, word);
        index[indexCount].count = 1; 

        char lineNumStr[12];
        sprintf(lineNumStr, "%d", lineNumber);
        strcpy(index[indexCount].lines, lineNumStr);
        index[indexCount].lastLineAdded = lineNumber;
        indexCount++;
    }
}

int compareEntries(const void *a, const void *b)
{
    IndexEntry *entryA = (IndexEntry *)a;
    IndexEntry *entryB = (IndexEntry *)b;
    return strcmp(entryA->word, entryB->word);
}

void processFile(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Failed to open input file");
        return;
    }

    char line[MAX_LINE_LENGTH];
    int lineNumber = 0;
    
    int isStartOfSentence = 1;

    printf("Processing file: %s\n", filename);
    while (fgets(line, sizeof(line), file))
    {
        lineNumber++;
        char *ptr = line;

        while (*ptr)
        {
            while (*ptr && !isalpha(*ptr))
            {
                if (*ptr == '.' || *ptr == '!' || *ptr == '?')
                {
                    isStartOfSentence = 1;
                }
                else if (!isspace(*ptr))
                {
                    isStartOfSentence = 0;
                }
                ptr++;
            }

            if (*ptr == '\0')
                break;

            char originalWord[MAX_WORD_LENGTH];
            int i = 0;
            while (*ptr && isalpha(*ptr))
            {
                if (i < MAX_WORD_LENGTH - 1)
                {
                    originalWord[i++] = *ptr;
                }
                ptr++;
            }
            originalWord[i] = '\0';

            if (isupper(originalWord[0]) && !isStartOfSentence)
            {
                isStartOfSentence = 0; 
                continue; 
            }

            char lowerWord[MAX_WORD_LENGTH];
            for (i = 0; (lowerWord[i] = tolower(originalWord[i])); i++)
                ; 

            if (strlen(lowerWord) == 0)
            {
                isStartOfSentence = 0;
                continue;
            }

            if (isStopWord(lowerWord))
            {
                isStartOfSentence = 0;
                continue;
            }

            addToIndex(lowerWord, lineNumber);
            
            isStartOfSentence = 0;
        }
    }
    fclose(file);
    printf("Processed %d lines.\n", lineNumber);
}

void printIndex()
{
    qsort(index, indexCount, sizeof(IndexEntry), compareEntries);

    printf("\nIndex:\n");
    printf("--------------------------------------\n");

    long totalOccurrences = 0; 

    for (int i = 0; i < indexCount; i++)
    {
        printf("%-20s %d, %s\n", index[i].word, index[i].count, index[i].lines);
        
        totalOccurrences += index[i].count;
    }
    printf("--------------------------------------\n");
    
    printf("Total unique words in index: %d\n", indexCount);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <stop_word_file> <input_file>\n", argv[0]);
        fprintf(stderr, "Example: %s stopw.txt alice30.txt\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *stopWordFile = argv[1];
    char *inputFile = argv[2];

    loadStopWords(stopWordFile);
    processFile(inputFile);
    printIndex();
    
    return 0;
}