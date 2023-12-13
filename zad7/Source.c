#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 50

struct _directory;
typedef struct _directory* PositionDirectory;
typedef struct _directory {
    char name[MAX_NAME_LENGTH];
    PositionDirectory subDirectories;
    PositionDirectory next;
} Directory;

struct _levelStack;
typedef struct _levelStack* PositionLevelStack;
typedef struct _levelStack {
    PositionDirectory directoryLevel;
    PositionLevelStack next;
} LevelStack;

PositionDirectory createDirectory(char name[MAX_NAME_LENGTH]);
PositionDirectory createSubdirectory(char name[MAX_NAME_LENGTH], PositionDirectory currentDirectory);
PositionDirectory changeDirectory(char name[MAX_NAME_LENGTH], PositionDirectory currentDirectory);
int listDirectoryContents(PositionDirectory currentDirectory);

PositionDirectory pop(PositionLevelStack headLevelStack);
int push(PositionLevelStack headLevelStack, PositionDirectory directoryLevel);
PositionLevelStack createNewLevelStackElement(PositionDirectory directoryLevel);

int main() {
    Directory headDirectory = {
        .name = {0},
        .subDirectories = NULL,
        .next = NULL
    };
    PositionDirectory rootDirectory = createDirectory("C:");
    headDirectory.next = rootDirectory;

    PositionDirectory currentDirectory = rootDirectory;

    LevelStack headLevelStack = {
        .directoryLevel = NULL,
        .next = NULL
    };
    push(&headLevelStack, currentDirectory);

    while (1) {
        printf("\nMenu:\n");
        printf("1 - md (Create Directory)\n");
        printf("2 - cd dir (Change Directory)\n");
        printf("3 - cd.. (Go Up)\n");
        printf("4 - dir (List Contents)\n");
        printf("5 - exit\n");

        char choice[10];
        printf("Enter your choice: ");
        scanf("%s", choice);

        if (strcmp(choice, "1") == 0) {
            char directoryName[MAX_NAME_LENGTH];
            printf("\nEnter directory name:");
            scanf("%s", directoryName);
            createSubdirectory(directoryName, currentDirectory);
        }
        else if (strcmp(choice, "2") == 0) {
            char directoryName[MAX_NAME_LENGTH];
            printf("\nEnter directory name:");
            scanf("%s", directoryName);
            currentDirectory = changeDirectory(directoryName, currentDirectory);
            push(&headLevelStack, currentDirectory);
        }
        else if (strcmp(choice, "3") == 0) {
            if (currentDirectory != rootDirectory) {
                currentDirectory = pop(&headLevelStack);
                printf("\nCurrently in '%s'", currentDirectory->name);
            }
            else {
                printf("Already in the root directory.\n");
                return currentDirectory;
            }
        }
        else if (strcmp(choice, "4") == 0) {
            listDirectoryContents(currentDirectory);
        }
        else if (strcmp(choice, "5") == 0) {
            printf("Exiting the program.\n");
            break;
        }
        else {
            printf("\nInvalid choice. Please enter a valid option.\n");
        }
    }


    free(rootDirectory);

    return 0;
}



PositionDirectory createDirectory(char name[MAX_NAME_LENGTH]) {
    PositionDirectory newDirectory = NULL;
    newDirectory = (PositionDirectory)malloc(sizeof(Directory));
    if (!newDirectory) {
        printf("Can't allocate memory!\n");
        return NULL;
    }
    strcpy(newDirectory->name, name);
    newDirectory->subDirectories = NULL;
    newDirectory->next = NULL;
    return newDirectory;
}

PositionDirectory createSubdirectory(char name[MAX_NAME_LENGTH], PositionDirectory currentDirectory) {
    PositionDirectory newDirectory = NULL;
    newDirectory = createDirectory(name);
    if (!newDirectory) {
        printf("New directory wasn't created!\n");
        return NULL;
    }
    newDirectory->next = currentDirectory->subDirectories;
    currentDirectory->subDirectories = newDirectory;
    return newDirectory;
}

PositionDirectory changeDirectory(char name[MAX_NAME_LENGTH], PositionDirectory currentDirectory) {
    PositionDirectory subdirectory = currentDirectory->subDirectories;
    while (subdirectory != NULL) {
        if (strcmp(subdirectory->name, name) == 0) {
            return subdirectory;
        }
        subdirectory = subdirectory->next;
    }
    printf("Directory '%s' not found.\n", name);
    return currentDirectory;
}

int listDirectoryContents(PositionDirectory currentDirectory) {
    printf("Contents of directory '%s':\n", currentDirectory->name);
    PositionDirectory subdirectory = currentDirectory->subDirectories;
    while (subdirectory != NULL) {
        printf("- %s\n", subdirectory->name);
        subdirectory = subdirectory->next;
    }
    if (currentDirectory->subDirectories == NULL) {
        printf("(empty)\n");
    }
    return EXIT_SUCCESS;
}



PositionDirectory pop(PositionLevelStack headLevelStack) {
    PositionLevelStack toDelete = NULL;
    PositionDirectory directoryLevel = NULL;

    toDelete = headLevelStack->next;
    if (!toDelete) {
        printf("Stack is empty! Nothing to pop!\n");
        return NULL;
    }

    headLevelStack->next = toDelete->next;
    directoryLevel = toDelete->directoryLevel;
    free(toDelete);

    return directoryLevel;
}

int push(PositionLevelStack headLevelStack, PositionDirectory directoryLevel) {
    PositionLevelStack newLevelStackElement = NULL;

    newLevelStackElement = createNewLevelStackElement(directoryLevel);
    if (!newLevelStackElement) {
        perror("Error in creating new element!\n");
        return NULL;
    }

    newLevelStackElement->next = headLevelStack->next;
    headLevelStack->next = newLevelStackElement;
}

PositionLevelStack createNewLevelStackElement(PositionDirectory directoryLevel) {
    PositionLevelStack newLevelStackElement = NULL;

    newLevelStackElement = (PositionLevelStack)malloc(sizeof(LevelStack));
    if (!newLevelStackElement) {
        perror("Can't allocate memory!\n");
        return NULL;
    }

    newLevelStackElement->directoryLevel = directoryLevel;
    newLevelStackElement->next = NULL;

    return newLevelStackElement;
}