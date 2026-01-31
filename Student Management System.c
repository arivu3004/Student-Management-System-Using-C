#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 50

typedef struct {
    int rollNo;
    char name[50];
    char branch[30];
    int marks[5];
    int total;
    float percentage;
    char grade;
} Student;

// Function prototypes
void addStudent(Student students[], int *count);
void enterMarks(Student students[], int count);
void calculateResults(Student *s);
void displayStudent(Student students[], int count, int rollNo);
void displayAll(Student students[], int count);
void searchByName(Student students[], int count, char *name);
void saveToFile(Student students[], int count);
int loadFromFile(Student students[]);



void addStudent(Student students[], int *count) {
    if (*count >= MAX) {
        printf("Maximum student limit reached!\n");
        return;
    }

    Student s;
    printf("Enter Roll Number: ");
    scanf("%d", &s.rollNo);

    // Validate unique roll number
    for (int i = 0; i < *count; i++) {
        if (students[i].rollNo == s.rollNo) {
            printf("Roll number already exists!\n");
            return;
        }
    }

    printf("Enter Name: ");
    scanf(" %[^\n]", s.name);
    printf("Enter Branch: ");
    scanf(" %[^\n]", s.branch);

    
    for (int i = 0; i < 5; i++) {
        s.marks[i] = -1; // not entered yet
    }
    s.total = 0;
    s.percentage = 0.0;
    s.grade = '-';

    students[*count] = s;
    (*count)++;
    printf("Student added successfully!\n");
}

void enterMarks(Student students[], int count) {
    int rollNo;
    printf("Enter Roll Number: ");
    scanf("%d", &rollNo);

    for (int i = 0; i < count; i++) {
        if (students[i].rollNo == rollNo) {
            printf("Enter marks for 5 subjects (0-100):\n");
            for (int j = 0; j < 5; j++) {
                int mark;
                do {
                    printf("Subject %d: ", j + 1);
                    scanf("%d", &mark);
                    if (mark < 0 || mark > 100) {
                        printf("Invalid! Marks must be between 0 and 100.\n");
                    }
                } while (mark < 0 || mark > 100);
                students[i].marks[j] = mark;
            }
            calculateResults(&students[i]);
            printf("Marks entered and results calculated!\n");
            return;
        }
    }
    printf("Roll number not found!\n");
}

void calculateResults(Student *s) {
    s->total = 0;
    for (int i = 0; i < 5; i++) {
        s->total += s->marks[i];
    }
    s->percentage = s->total / 5.0;

    if (s->percentage >= 75) s->grade = 'A';
    else if (s->percentage >= 60) s->grade = 'B';
    else if (s->percentage >= 50) s->grade = 'C';
    else s->grade = 'F';
}

void displayStudent(Student students[], int count, int rollNo) {
    for (int i = 0; i < count; i++) {
        if (students[i].rollNo == rollNo) {
            printf("\n--- Student Result ---\n");
            printf("Roll No: %d\n", students[i].rollNo);
            printf("Name: %s\n", students[i].name);
            printf("Branch: %s\n", students[i].branch);
            printf("Marks: ");
            for (int j = 0; j < 5; j++) {
                printf("%d ", students[i].marks[j]);
            }
            printf("\nTotal: %d\n", students[i].total);
            printf("Percentage: %.2f\n", students[i].percentage);
            printf("Grade: %c\n", students[i].grade);
            return;
        }
    }
    printf("Student not found!\n");
}

void displayAll(Student students[], int count) {
    // Sort by percentage (descending)
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (students[j].percentage > students[i].percentage) {
                Student temp = students[i];
                students[i] = students[j];
                students[j] = temp;
            }
        }
    }

    printf("\n--- All Students Sorted by Percentage ---\n");
    for (int i = 0; i < count; i++) {
        printf("%d\t%s\t%s\t%.2f%%\tGrade: %c\n",
               students[i].rollNo,
               students[i].name,
               students[i].branch,
               students[i].percentage,
               students[i].grade);
    }
}

void searchByName(Student students[], int count, char *name) {
    printf("\n--- Search Results ---\n");
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strstr(students[i].name, name) != NULL) {
            printf("%d\t%s\t%s\t%.2f%%\tGrade: %c\n",
                   students[i].rollNo,
                   students[i].name,
                   students[i].branch,
                   students[i].percentage,
                   students[i].grade);
            found = 1;
        }
    }
    if (!found) {
        printf("No student found with name containing '%s'\n", name);
    }
}

void saveToFile(Student students[], int count) {
    FILE *fp = fopen("students.dat", "wb");
    if (fp == NULL) {
        printf("Error saving file!\n");
        return;
    }
    fwrite(&count, sizeof(int), 1, fp);
    fwrite(students, sizeof(Student), count, fp);
    fclose(fp);
    printf("Records saved successfully!\n");
}

int loadFromFile(Student students[]) {
    FILE *fp = fopen("students.dat", "rb");
    if (fp == NULL) {
        return 0; 
    }
    int count;
    fread(&count, sizeof(int), 1, fp);
    fread(students, sizeof(Student), count, fp);
    fclose(fp);
    return count;
}



int main() {
    Student students[MAX];
    int count = loadFromFile(students);
    int choice, rollNo;
    char name[50];

    do {
        printf("\n--- Student Examination Record System ---\n");
        printf("1. Add Student\n");
        printf("2. Enter Marks\n");
        printf("3. Display Individual Result\n");
        printf("4. Display All Students (Sorted)\n");
        printf("5. Search by Name\n");
        printf("6. Save Records\n");
        printf("7. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: addStudent(students, &count); break;
            case 2: enterMarks(students, count); break;
            case 3: 
                printf("Enter Roll No: ");
                scanf("%d", &rollNo);
                displayStudent(students, count, rollNo);
                break;
            case 4: displayAll(students, count); break;
            case 5: 
                printf("Enter Name to Search: ");
                scanf(" %[^\n]", name);
                searchByName(students, count, name);
                break;
            case 6: saveToFile(students, count); break;
            case 7: printf("Exiting...\n"); break;
            default: printf("Invalid choice!\n");
        }
    } while(choice != 7);

    return 0;
}