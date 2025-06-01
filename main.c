#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int rollNo;
    char name[50];
    float marks[3];
    float average;
} Student;

#define FILE_NAME "C:\\Users\\chuan\\Downloads\\JANICEEEE\\students.txt"

int loadFromFile(Student **students) {
    FILE *fp = fopen(FILE_NAME, "r");
    if (fp == NULL) {
        *students = NULL;
        return 0;
    }

    int count = 0;
    Student temp;
    *students = NULL;

    while (fscanf(fp, "ID: %d\nName: %[^\n]\nMark1: %f\nMark2: %f\nMark3: %f\nAverage: %f\n---\n",
                  &temp.rollNo,
                  temp.name,
                  &temp.marks[0],
                  &temp.marks[1],
                  &temp.marks[2],
                  &temp.average) == 6) {
        *students = (Student *)realloc(*students, (count + 1) * sizeof(Student));
        (*students)[count++] = temp;
    }

    fclose(fp);
    return count;
}

void saveToFile(Student *students, int count) {
    FILE *fp = fopen(FILE_NAME, "w");
    if (fp == NULL) {
        printf("Error writing to file.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(fp, "ID: %d\nName: %s\nMark1: %.2f\nMark2: %.2f\nMark3: %.2f\nAverage: %.2f\n---\n",
                students[i].rollNo,
                students[i].name,
                students[i].marks[0],
                students[i].marks[1],
                students[i].marks[2],
                students[i].average);
    }

    fclose(fp);
}

void displayAllNames(Student *students, int count) {
    printf("\nCurrent Students:\n");
    for (int i = 0; i < count; i++) {
        printf("%d: %s\n", students[i].rollNo, students[i].name);
    }
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int main() {
    Student *students = NULL;
    int count = loadFromFile(&students);
    int choice = 0;

    while (choice != 6) {
        clearScreen();
        printf("\n--- Student Record System ---\n");
        printf("1. Add Student\n2. Edit Student\n3. Delete Student\n4. View Students\n5. Search Student\n6. Exit\nChoice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            int idInput, idExists = 0;
            printf("\nEnter new ID No: ");
            scanf("%d", &idInput);

            for (int i = 0; i < count; i++) {
                if (students[i].rollNo == idInput) {
                    idExists = 1;
                    printf("ID %d already exists for %s.\n", idInput, students[i].name);
                    printf("Do you want to edit (2) or replace (1) this record? ");
                    int subChoice;
                    scanf("%d", &subChoice);
                    if (subChoice == 2) {
                        choice = 2;
                    } else if (subChoice == 1) {
                        printf("Replacing existing record.\n");
                        printf("Name: ");
                        scanf(" %[^\n]", students[i].name);
                        float total = 0;
                        for (int j = 0; j < 3; j++) {
                            printf("Marks %d: ", j + 1);
                            scanf("%f", &students[i].marks[j]);
                            total += students[i].marks[j];
                        }
                        students[i].average = total / 3;
                        saveToFile(students, count);
                    }
                    break;
                }
            }

            if (!idExists) {
                students = (Student *)realloc(students, (count + 1) * sizeof(Student));
                if (students == NULL) {
                    printf("Memory allocation failed.\n");
                    exit(1);
                }
                students[count].rollNo = idInput;
                printf("Name: ");
                scanf(" %[^\n]", students[count].name);
                float total = 0;
                for (int j = 0; j < 3; j++) {
                    printf("Marks %d: ", j + 1);
                    scanf("%f", &students[count].marks[j]);
                    total += students[count].marks[j];
                }
                students[count].average = total / 3;
                printf("Added! Average: %.2f\n", students[count].average);
                count++;
                saveToFile(students, count);
            }
        }

        else if (choice == 2) {
            if (count == 0) {
                printf("No records to edit.\n");
            } else {
                displayAllNames(students, count);
                int roll, found = 0;
                printf("\nEnter ID No to edit: ");
                scanf("%d", &roll);
                for (int i = 0; i < count; i++) {
                    if (students[i].rollNo == roll) {
                        printf("New Name: ");
                        scanf(" %[^\n]", students[i].name);
                        float total = 0;
                        for (int j = 0; j < 3; j++) {
                            printf("New Marks %d: ", j + 1);
                            scanf("%f", &students[i].marks[j]);
                            total += students[i].marks[j];
                        }
                        students[i].average = total / 3;
                        printf("Updated! New Average: %.2f\n", students[i].average);
                        found = 1;
                        saveToFile(students, count);
                        break;
                    }
                }
                if (!found) printf("ID not found.\n");
            }
        }

        else if (choice == 3) {
            if (count == 0) {
                printf("No records to delete.\n");
            } else {
                displayAllNames(students, count);
                int roll, found = 0;
                printf("\nEnter ID No to delete: ");
                scanf("%d", &roll);
                for (int i = 0; i < count; i++) {
                    if (students[i].rollNo == roll) {
                        for (int j = i; j < count - 1; j++) {
                            students[j] = students[j + 1];
                        }
                        count--;
                        students = (Student *)realloc(students, count * sizeof(Student));
                        printf("Deleted!\n");
                        found = 1;
                        saveToFile(students, count);
                        break;
                    }
                }
                if (!found) printf("ID not found.\n");
            }
        }

        else if (choice == 4) {
            if (count == 0) {
                printf("No records.\n");
            } else {
                for (int i = 0; i < count; i++) {
                    printf("\nID: %d\nName: %s\nMarks: %.2f, %.2f, %.2f\nAverage: %.2f\n",
                           students[i].rollNo,
                           students[i].name,
                           students[i].marks[0],
                           students[i].marks[1],
                           students[i].marks[2],
                           students[i].average);
                }
            }
        }

        else if (choice == 5) {
            if (count == 0) {
                printf("No records.\n");
            } else {
                int roll, found = 0;
                printf("\nEnter ID No to search: ");
                scanf("%d", &roll);
                for (int i = 0; i < count; i++) {
                    if (students[i].rollNo == roll) {
                        printf("\nID: %d\nName: %s\nMarks: %.2f, %.2f, %.2f\nAverage: %.2f\n",
                               students[i].rollNo,
                               students[i].name,
                               students[i].marks[0],
                               students[i].marks[1],
                               students[i].marks[2],
                               students[i].average);
                        found = 1;
                        break;
                    }
                }
                if (!found) printf("ID not found.\n");
            }
        }

        else if (choice != 6) {
            printf("Invalid choice.\n");
        }

        printf("\nPress Enter to continue...");
        getchar();
        getchar();
    }

    free(students);
    return 0;
}

