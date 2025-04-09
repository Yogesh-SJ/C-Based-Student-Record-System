#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int studentId;
    char studentName[50];
    int studentAge;
    float studentMarks;
} Student;

void addNewStudent() {
    Student newStudent;
    FILE *filePointer = fopen("students.dat", "ab");
    if (filePointer == NULL) {
        printf("Error opening file.\n");
        return;
    }

    printf("Enter Student ID: ");
    scanf("%d", &newStudent.studentId);
    printf("Enter Name: ");
    scanf(" %[^\n]", newStudent.studentName);
    printf("Enter Age: ");
    scanf("%d", &newStudent.studentAge);
    printf("Enter Marks: ");
    scanf("%f", &newStudent.studentMarks);

    fwrite(&newStudent, sizeof(Student), 1, filePointer);
    fclose(filePointer);
    printf("Student added successfully!\n");
}

void showAllStudents() {
    Student currentStudent;
    FILE *filePointer = fopen("students.dat", "rb");
    if (filePointer == NULL) {
        printf("No records found.\n");
        return;
    }

    printf("\n%-10s %-20s %-10s %-10s\n", "ID", "Name", "Age", "Marks");
    printf("---------------------------------------------------------\n");

    while (fread(&currentStudent, sizeof(Student), 1, filePointer)) {
        printf("%-10d %-20s %-10d %-10.2f\n", currentStudent.studentId, currentStudent.studentName, currentStudent.studentAge, currentStudent.studentMarks);
    }

    fclose(filePointer);
}

void modifyStudent() {
    int searchId, recordFound = 0;
    Student studentToUpdate;
    FILE *filePointer = fopen("students.dat", "rb+");
    if (filePointer == NULL) {
        printf("Error opening file.\n");
        return;
    }

    printf("Enter ID of student to update: ");
    scanf("%d", &searchId);

    while (fread(&studentToUpdate, sizeof(Student), 1, filePointer)) {
        if (studentToUpdate.studentId == searchId) {
            printf("Enter new name: ");
            scanf(" %[^\n]", studentToUpdate.studentName);
            printf("Enter new age: ");
            scanf("%d", &studentToUpdate.studentAge);
            printf("Enter new marks: ");
            scanf("%f", &studentToUpdate.studentMarks);

            fseek(filePointer, -sizeof(Student), SEEK_CUR);
            fwrite(&studentToUpdate, sizeof(Student), 1, filePointer);
            recordFound = 1;
            printf("Record updated successfully.\n");
            break;
        }
    }

    if (!recordFound) {
        printf("Student with ID %d not found.\n", searchId);
    }

    fclose(filePointer);
}

void removeStudent() {
    int deleteId, recordFound = 0;
    Student currentStudent;
    FILE *filePointer = fopen("students.dat", "rb");
    FILE *tempFile = fopen("temp.dat", "wb");

    if (filePointer == NULL || tempFile == NULL) {
        printf("Error opening file.\n");
        return;
    }

    printf("Enter ID of student to delete: ");
    scanf("%d", &deleteId);

    while (fread(&currentStudent, sizeof(Student), 1, filePointer)) {
        if (currentStudent.studentId == deleteId) {
            recordFound = 1;
            continue;
        }
        fwrite(&currentStudent, sizeof(Student), 1, tempFile);
    }

    fclose(filePointer);
    fclose(tempFile);

    remove("students.dat");
    rename("temp.dat", "students.dat");

    if (recordFound)
        printf("Student deleted successfully.\n");
    else
        printf("Student with ID %d not found.\n", deleteId);
}

int main() {
    int userChoice;
    do {
        printf("\n=== Student Management System ===\n");
        printf("1. Add Student\n");
        printf("2. View All Students\n");
        printf("3. Update Student\n");
        printf("4. Delete Student\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &userChoice);

        switch (userChoice) {
            case 1: addNewStudent(); break;
            case 2: showAllStudents(); break;
            case 3: modifyStudent(); break;
            case 4: removeStudent(); break;
            case 5: printf("Exiting...\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while (userChoice != 5);

    return 0;
}
