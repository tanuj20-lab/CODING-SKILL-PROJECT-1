#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STUDENT_FILE "students.txt"
#define CREDENTIAL_FILE "credentials.txt"

struct Student { 
    int roll; 
    char name[50]; 
    float marks; 
};

char currentRole[10];
char currentUser[50];

// Function declarations
void mainMenu();
void adminMenu();
void staffMenu();
void guestMenu();
int loginSystem();
void addStudent();
void displayStudents();
void searchStudent();
void updateStudent();
void deleteStudent();

int loginSystem() {
    char username[20], password[20];
    char fileUser[20], filePass[20], fileRole[10];
    int attempts = 0;
    int maxAttempts = 3;
    
    while (attempts < maxAttempts) {
        printf("\n=========Login Screen========\n");
        printf("Attempt %d of %d\n", attempts + 1, maxAttempts);
        printf("Username: ");
        scanf("%s", username);
        printf("Password: ");
        scanf("%s", password);
        
        FILE *fp = fopen(CREDENTIAL_FILE, "r");
        if (!fp) {
            printf("Error: credentials.txt not found\n");
            return 0;
        }
        
        while (fscanf(fp, "%s %s %s", fileUser, filePass, fileRole) == 3) {
            if (strcmp(username, fileUser) == 0 && strcmp(password, filePass) == 0) {
                strcpy(currentRole, fileRole);
                strcpy(currentUser, fileUser);
                fclose(fp);
                printf("Login successful! Welcome %s (%s)\n", currentUser, currentRole);
                return 1;
            }
        }
        
        fclose(fp);
        attempts++;
        
        if (attempts < maxAttempts) {
            printf("Invalid username or password! %d attempt(s) remaining.\n", maxAttempts - attempts);
        } else {
            printf("Invalid username or password!\n");
            printf("Maximum login attempts exceeded. Access denied.\n");
        }
    }
    
    return 0;
}

void mainMenu() {
    if (strcmp(currentRole, "ADMIN") == 0) {
        adminMenu();
    } else if (strcmp(currentRole, "STAFF") == 0) {
        staffMenu();
    } else {
        guestMenu();
    }
}

void adminMenu() {
    int choice;
    do {
        printf("\n====ADMIN MENU=====\n");
        printf("1. Add New Student\n");
        printf("2. Display All Records\n");
        printf("3. Search Record\n");
        printf("4. Update Student Details\n");
        printf("5. Delete Student Record\n");
        printf("6. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: printf("Logging out...\n"); return;
            default: printf("Invalid choice\n");
        }
    } while (1);
}

void staffMenu() {
    int choice;
    do {
        printf("\n====STAFF MENU=====\n");
        printf("1. Add New Student\n");
        printf("2. Display All Records\n");
        printf("3. Search Record\n");
        printf("4. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: printf("Logging out...\n"); return;
            default: printf("Invalid choice\n");
        }
    } while (1);
}

void guestMenu() {
    int choice;
    do {
        printf("\n====GUEST MENU=====\n");
        printf("1. Display All Records\n");
        printf("2. Search Record\n");
        printf("3. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: displayStudents(); break;
            case 2: searchStudent(); break;
            case 3: printf("Logging out...\n"); return;
            default: printf("Invalid choice\n");
        }
    } while (1);
}

void addStudent() {
    struct Student s;
    FILE *fp = fopen(STUDENT_FILE, "a");
    
    if (!fp) {
        printf("Error: Unable to open file\n");
        return;
    }
    
    printf("\nEnter Roll Number: ");
    scanf("%d", &s.roll);
    printf("Enter Name: ");
    scanf(" %[^\n]", s.name);
    printf("Enter Marks: ");
    scanf("%f", &s.marks);
    
    fprintf(fp, "%d %s %.2f\n", s.roll, s.name, s.marks);
    fclose(fp);
    
    printf("Student added successfully!\n");
}

void displayStudents() {
    struct Student s;
    FILE *fp = fopen(STUDENT_FILE, "r");
    
    if (!fp) {
        printf("Error: Unable to open file or no records found\n");
        return;
    }
    
    printf("\n%-10s %-30s %-10s\n", "Roll No", "Name", "Marks");
    printf("---------------------------------------------------\n");
    
    while (fscanf(fp, "%d %s %f", &s.roll, s.name, &s.marks) == 3) {
        printf("%-10d %-30s %-10.2f\n", s.roll, s.name, s.marks);
    }
    
    fclose(fp);
}

void searchStudent() {
    char searchName[50];
    int found = 0;
    struct Student s;
    
    printf("\nEnter Student Name to search: ");
    scanf(" %[^\n]", searchName);
    
    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) {
        printf("Error: Unable to open file\n");
        return;
    }
    
    printf("\n%-10s %-30s %-10s\n", "Roll No", "Name", "Marks");
    printf("---------------------------------------------------\n");
    
    while (fscanf(fp, "%d %s %f", &s.roll, s.name, &s.marks) == 3) {
        if (strstr(s.name, searchName) != NULL || strcmp(s.name, searchName) == 0) {
            printf("%-10d %-30s %-10.2f\n", s.roll, s.name, s.marks);
            found = 1;
        }
    }
    
    if (!found) {
        printf("No student found with name matching '%s'\n", searchName);
    }
    
    fclose(fp);
}

void updateStudent() {
    int roll, found = 0;
    struct Student s;
    
    printf("\nEnter Roll Number to update: ");
    scanf("%d", &roll);
    
    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    
    if (!fp || !temp) {
        printf("Error: Unable to open file\n");
        return;
    }
    
    while (fscanf(fp, "%d %s %f", &s.roll, s.name, &s.marks) == 3) {
        if (s.roll == roll) {
            found = 1;
            printf("Current Details - Name: %s, Marks: %.2f\n", s.name, s.marks);
            printf("Enter New Name: ");
            scanf(" %[^\n]", s.name);
            printf("Enter New Marks: ");
            scanf("%f", &s.marks);
            printf("Record updated successfully!\n");
        }
        fprintf(temp, "%d %s %.2f\n", s.roll, s.name, s.marks);
    }
    
    fclose(fp);
    fclose(temp);
    
    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);
    
    if (!found) {
        printf("Student with Roll No %d not found\n", roll);
    }
}

void deleteStudent() {
    int roll, found = 0;
    struct Student s;
    
    printf("\nEnter Roll Number to delete: ");
    scanf("%d", &roll);
    
    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    
    if (!fp || !temp) {
        printf("Error: Unable to open file\n");
        return;
    }
    
    while (fscanf(fp, "%d %s %f", &s.roll, s.name, &s.marks) == 3) {
        if (s.roll == roll) {
            found = 1;
            printf("Student record deleted successfully!\n");
        } else {
            fprintf(temp, "%d %s %.2f\n", s.roll, s.name, s.marks);
        }
    }
    
    fclose(fp);
    fclose(temp);
    
    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);
    
    if (!found) {
        printf("Student with Roll No %d not found\n", roll);
    }
}

int main() {
    if (loginSystem()) {
        mainMenu();
    } else {
        printf("\nLogin Failed. Exiting...\n");
    }
    return 0;
}