#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100

// Define Student Structure
struct Student {
    int roll;
    char name[50];
    float marks;
};

// Function to Display Header
void displayHeader() {
    printf("\n==========================================\n");
    printf("       STUDENT MANAGEMENT SYSTEM          \n");
    printf("==========================================\n");
}

// Function to Authenticate Admin
int authenticate() {
    char username[20], password[20];

    printf("\nAdmin Login Required\n");
    printf("Enter Username: ");
    scanf("%s", username);
    printf("Enter Password: ");
    scanf("%s", password);

    // Simple hardcoded authentication
    if (strcmp(username, "admin") == 0 && strcmp(password, "1234") == 0) {
        printf("Login Successful!\n");
        return 1;
    } else {
        printf("Invalid Credentials! Exiting...\n");
        return 0;
    }
}

// Function to Add Student
void addStudent() {
    FILE *file = fopen("students.dat", "ab");
    if (!file) {
        printf("Error opening file!\n");
        return;
    }

    struct Student s;
    printf("Enter Roll Number: ");
    scanf("%d", &s.roll);
    printf("Enter Name: ");
    scanf(" %[^\n]", s.name);
    printf("Enter Marks: ");
    scanf("%f", &s.marks);

    fwrite(&s, sizeof(struct Student), 1, file);
    fclose(file);
    printf("Student record added successfully!\n");
}

// Function to Display All Students
void displayStudents() {
    FILE *file = fopen("students.dat", "rb");
    if (!file) {
        printf("No records found!\n");
        return;
    }

    struct Student s;
    printf("\nStudent Records:\n");
    printf("------------------------------------------------\n");
    printf("Roll Number | Name               | Marks\n");
    printf("------------------------------------------------\n");

    while (fread(&s, sizeof(struct Student), 1, file)) {
        printf("%-11d | %-18s | %.2f\n", s.roll, s.name, s.marks);
    }

    printf("------------------------------------------------\n");
    fclose(file);
}

// Function to Search for a Student by Roll Number
void searchStudent() {
    FILE *file = fopen("students.dat", "rb");
    if (!file) {
        printf("No records found!\n");
        return;
    }

    int rollNumber, found = 0;
    struct Student s;
    printf("Enter Roll Number to Search: ");
    scanf("%d", &rollNumber);

    while (fread(&s, sizeof(struct Student), 1, file)) {
        if (s.roll == rollNumber) {
            printf("\nStudent Found!\n");
            printf("Roll: %d | Name: %s | Marks: %.2f\n", s.roll, s.name, s.marks);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Student with Roll Number %d not found.\n", rollNumber);

    fclose(file);
}

// Function to Update a Student Record
void updateStudent() {
    FILE *file = fopen("students.dat", "rb+");
    if (!file) {
        printf("No records found!\n");
        return;
    }

    struct Student s;
    int rollNumber, found = 0;

    printf("Enter Roll Number to Update: ");
    scanf("%d", &rollNumber);

    while (fread(&s, sizeof(struct Student), 1, file)) {
        if (s.roll == rollNumber) {
            found = 1;
            printf("Enter New Name: ");
            scanf(" %[^\n]", s.name);
            printf("Enter New Marks: ");
            scanf("%f", &s.marks);

            fseek(file, -sizeof(struct Student), SEEK_CUR);
            fwrite(&s, sizeof(struct Student), 1, file);
            printf("Student record updated successfully!\n");
            break;
        }
    }

    if (!found)
        printf("Student with Roll Number %d not found.\n", rollNumber);

    fclose(file);
}

// Function to Delete a Student Record
void deleteStudent() {
    FILE *file = fopen("students.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");

    if (!file || !temp) {
        printf("Error opening file!\n");
        return;
    }

    int rollNumber, found = 0;
    struct Student s;

    printf("Enter Roll Number to Delete: ");
    scanf("%d", &rollNumber);

    while (fread(&s, sizeof(struct Student), 1, file)) {
        if (s.roll == rollNumber) {
            found = 1;
            printf("Student with Roll Number %d deleted successfully!\n", rollNumber);
        } else {
            fwrite(&s, sizeof(struct Student), 1, temp);
        }
    }

    fclose(file);
    fclose(temp);

    remove("students.dat");
    rename("temp.dat", "students.dat");

    if (!found) {
        printf("Student with Roll Number %d not found.\n", rollNumber);
    }
}

// Function to Sort Students by Marks (Highest to Lowest)
void sortStudentsByMarks() {
    FILE *file = fopen("students.dat", "rb");
    if (!file) {
        printf("No records found!\n");
        return;
    }

    struct Student s[MAX_STUDENTS];
    int count = 0;

    while (fread(&s[count], sizeof(struct Student), 1, file)) {
        count++;
    }
    fclose(file);

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (s[j].marks < s[j + 1].marks) {
                struct Student temp = s[j];
                s[j] = s[j + 1];
                s[j + 1] = temp;
            }
        }
    }

    printf("\nSorted Student Records (by Marks, Highest First):\n");
    for (int i = 0; i < count; i++) {
        printf("Roll: %d | Name: %s | Marks: %.2f\n", s[i].roll, s[i].name, s[i].marks);
    }
}

// Function to Calculate Class Average Marks
void calculateAverageMarks() {
    FILE *file = fopen("students.dat", "rb");
    if (!file) {
        printf("No records found!\n");
        return;
    }

    struct Student s;
    int count = 0;
    float totalMarks = 0;

    while (fread(&s, sizeof(struct Student), 1, file)) {
        totalMarks += s.marks;
        count++;
    }

    fclose(file);

    if (count == 0) {
        printf("No student records found!\n");
    } else {
        printf("Class Average Marks: %.2f\n", totalMarks / count);
    }
}

// Main Menu
int main() {
    if (!authenticate()) return 0; // Exit if authentication fails

    int choice;

    while (1) {
        displayHeader();
        printf("\n1. Add Student\n2. Display Students\n3. Search Student\n4. Update Student\n5. Delete Student\n6. Sort by Marks\n7. Class Average Marks\n8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: sortStudentsByMarks(); break;
            case 7: calculateAverageMarks(); break;
            case 8: printf("Exiting program...\n"); return 0;
            default: printf("Invalid choice! Try again.\n");
        }
    }
}
