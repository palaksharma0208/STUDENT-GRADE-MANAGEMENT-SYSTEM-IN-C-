#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SUBJECTS 5
#define MAX_STUDENTS 1000
#define DATA_FILE "students.dat"

typedef struct {
    int rollNo;
    char name[50];
    char section[20];
    float marks[SUBJECTS];
    float total;
    float percentage;
    char grade[3];
    int passed;
} Student;

Student students[MAX_STUDENTS];
int studentCount = 0;

/* Function declarations */
void loadStudents(void);
void saveStudents(void);
void calculateResult(Student *s);
void addStudent(void);
void displayStudents(void);
void searchStudent(void);
void updateStudent(void);
void deleteStudent(void);
void generateResult(void);
void classStatistics(void);
int findStudent(int rollNo);
int readInteger(const char *message);
float readMarks(const char *message);
void displayResult(Student s);

/* Read an integer safely */
int readInteger(const char *message) {
    char input[100];
    char extra;
    int value;

    while (1) {
        printf("%s", message);

        if (fgets(input, sizeof(input), stdin) == NULL)
            exit(0);

        if (sscanf(input, " %d %c", &value, &extra) == 1)
            return value;

        printf("Invalid input. Please enter a number.\n");
    }
}

/* Read and validate marks */
float readMarks(const char *message) {
    char input[100];
    char extra;
    float marks;

    while (1) {
        printf("%s", message);

        if (fgets(input, sizeof(input), stdin) == NULL)
            exit(0);

        if (sscanf(input, " %f %c", &marks, &extra) == 1 &&
            marks >= 0 && marks <= 100)
            return marks;

        printf("Invalid marks! Enter a value between 0 and 100.\n");
    }
}

/* Calculate total, percentage and grade */
void calculateResult(Student *s) {
    int i;

    s->total = 0;
    s->passed = 1;

    for (i = 0; i < SUBJECTS; i++) {
        s->total += s->marks[i];

        if (s->marks[i] < 40)
            s->passed = 0;
    }

    s->percentage = (s->total / 500.0f) * 100.0f;

    if (!s->passed)
        strcpy(s->grade, "F");
    else if (s->percentage >= 90)
        strcpy(s->grade, "A+");
    else if (s->percentage >= 80)
        strcpy(s->grade, "A");
    else if (s->percentage >= 70)
        strcpy(s->grade, "B");
    else if (s->percentage >= 60)
        strcpy(s->grade, "C");
    else if (s->percentage >= 50)
        strcpy(s->grade, "D");
    else
        strcpy(s->grade, "F");
}

/* Find student using roll number */
int findStudent(int rollNo) {
    int i;

    for (i = 0; i < studentCount; i++) {
        if (students[i].rollNo == rollNo)
            return i;
    }

    return -1;
}

/* Save records to file */
void saveStudents(void) {
    FILE *fp = fopen(DATA_FILE, "wb");

    if (fp == NULL) {
        printf("Error saving student records.\n");
        return;
    }

    fwrite(&studentCount, sizeof(int), 1, fp);

    if (studentCount > 0) {
        fwrite(students, sizeof(Student), studentCount, fp);
    }

    fclose(fp);
}

/* Load records from file */
void loadStudents(void) {
    FILE *fp = fopen(DATA_FILE, "rb");
    int count;

    if (fp == NULL)
        return;

    if (fread(&count, sizeof(int), 1, fp) != 1 ||
        count < 0 || count > MAX_STUDENTS) {
        printf("Invalid data file. Starting with empty records.\n");
        fclose(fp);
        return;
    }

    if (count > 0 &&
        fread(students, sizeof(Student), count, fp) != (size_t)count) {
        printf("Incomplete data file. Starting with empty records.\n");
        studentCount = 0;
        fclose(fp);
        return;
    }

    studentCount = count;
    fclose(fp);
}

/* Display an individual student result */
void displayResult(Student s) {
    int i;

    printf("\n====================================\n");
    printf("       STUDENT RESULT REPORT\n");
    printf("====================================\n");

    printf("Roll Number : %d\n", s.rollNo);
    printf("Name        : %s\n", s.name);
    printf("Class       : %s\n", s.section);

    printf("\nSubject-wise Marks:\n");

    for (i = 0; i < SUBJECTS; i++) {
        printf("Subject %d: %.2f / 100\n",
               i + 1, s.marks[i]);
    }

    printf("\nTotal Marks : %.2f / 500\n", s.total);
    printf("Percentage  : %.2f%%\n", s.percentage);
    printf("Grade       : %s\n", s.grade);

    printf("Result      : %s\n",
           s.passed ? "PASS" : "FAIL");

    printf("====================================\n");
}

/* Add a new student */
void addStudent(void) {
    Student s;
    int i;
    char message[100];

    if (studentCount >= MAX_STUDENTS) {
        printf("Student storage is full.\n");
        return;
    }

    s.rollNo = readInteger("Enter Roll Number: ");

    if (s.rollNo <= 0) {
        printf("Roll number must be positive.\n");
        return;
    }

    if (findStudent(s.rollNo) != -1) {
        printf("Roll number already exists!\n");
        return;
    }

    printf("Enter Student Name: ");

    if (fgets(s.name, sizeof(s.name), stdin) == NULL)
        return;

    s.name[strcspn(s.name, "\n")] = '\0';

    printf("Enter Class/Section: ");

    if (fgets(s.section, sizeof(s.section), stdin) == NULL)
        return;

    s.section[strcspn(s.section, "\n")] = '\0';

    if (strlen(s.name) == 0 || strlen(s.section) == 0) {
        printf("Name and class cannot be empty.\n");
        return;
    }

    for (i = 0; i < SUBJECTS; i++) {
        sprintf(message, "Enter marks for Subject %d: ", i + 1);
        s.marks[i] = readMarks(message);
    }

    calculateResult(&s);

    students[studentCount++] = s;

    saveStudents();

    printf("\nStudent added successfully!\n");
    displayResult(s);
}

/* Display all student records */
void displayStudents(void) {
    int i;

    if (studentCount == 0) {
        printf("No student records available.\n");
        return;
    }

    printf("\n%-10s %-20s %-12s %-10s %-10s %-8s\n",
           "Roll No.", "Name", "Class",
           "Total", "Percent", "Grade");

    printf("--------------------------------------------------------------------------\n");

    for (i = 0; i < studentCount; i++) {
        printf("%-10d %-20.20s %-12.12s %-10.2f %-9.2f %-8s\n",
               students[i].rollNo,
               students[i].name,
               students[i].section,
               students[i].total,
               students[i].percentage,
               students[i].grade);
    }
}

/* Search student */
void searchStudent(void) {
    int rollNo, index;

    rollNo = readInteger("Enter Roll Number to Search: ");

    index = findStudent(rollNo);

    if (index == -1) {
        printf("Student record not found!\n");
    } else {
        displayResult(students[index]);
    }
}

/* Update student information */
void updateStudent(void) {
    int rollNo, index, i;
    char message[100];

    rollNo = readInteger("Enter Roll Number to Update: ");

    index = findStudent(rollNo);

    if (index == -1) {
        printf("Student record not found!\n");
        return;
    }

    printf("Enter New Student Name: ");

    if (fgets(students[index].name,
              sizeof(students[index].name), stdin) == NULL)
        return;

    students[index].name[
        strcspn(students[index].name, "\n")] = '\0';

    printf("Enter New Class/Section: ");

    if (fgets(students[index].section,
              sizeof(students[index].section), stdin) == NULL)
        return;

    students[index].section[
        strcspn(students[index].section, "\n")] = '\0';

    for (i = 0; i < SUBJECTS; i++) {
        sprintf(message, "Enter New Marks for Subject %d: ", i + 1);

        students[index].marks[i] = readMarks(message);
    }

    calculateResult(&students[index]);

    saveStudents();

    printf("Student record updated successfully!\n");

    displayResult(students[index]);
}

/* Delete student record */
void deleteStudent(void) {
    int rollNo, index, i;
    char choice[20];

    rollNo = readInteger("Enter Roll Number to Delete: ");

    index = findStudent(rollNo);

    if (index == -1) {
        printf("Student record not found!\n");
        return;
    }

    displayResult(students[index]);

    printf("Are you sure you want to delete this record? (y/n): ");

    if (fgets(choice, sizeof(choice), stdin) == NULL)
        return;

    if (choice[0] != 'y' && choice[0] != 'Y') {
        printf("Deletion cancelled.\n");
        return;
    }

    for (i = index; i < studentCount - 1; i++) {
        students[i] = students[i + 1];
    }

    studentCount--;

    saveStudents();

    printf("Student record deleted successfully!\n");
}

/* Generate individual result */
void generateResult(void) {
    searchStudent();
}

/* Display class statistics and topper */
void classStatistics(void) {
    int i;
    int topper = 0;
    int passCount = 0;
    float totalPercentage = 0;

    if (studentCount == 0) {
        printf("No records available.\n");
        return;
    }

    for (i = 0; i < studentCount; i++) {
        totalPercentage += students[i].percentage;

        if (students[i].passed)
            passCount++;

        if (students[i].percentage >
            students[topper].percentage) {
            topper = i;
        }
    }

    printf("\n========== CLASS STATISTICS ==========\n");

    printf("Total Students : %d\n", studentCount);

    printf("Class Average  : %.2f%%\n",
           totalPercentage / studentCount);

    printf("Passed Students: %d\n", passCount);

    printf("Failed Students: %d\n",
           studentCount - passCount);

    printf("\n========== CLASS TOPPER ==============\n");

    displayResult(students[topper]);
}

/* Main function */
int main(void) {
    int choice;

    loadStudents();

    printf("\n========================================\n");
    printf("     STUDENT GRADE MANAGEMENT SYSTEM\n");
    printf("========================================\n");

    do {
        printf("\n--------------- MAIN MENU ---------------\n");
        printf("1. Add New Student\n");
        printf("2. Display All Students\n");
        printf("3. Search Student by Roll Number\n");
        printf("4. Update Student Record\n");
        printf("5. Delete Student Record\n");
        printf("6. Generate Student Result\n");
        printf("7. View Class Statistics / Topper\n");
        printf("0. Exit\n");
        printf("-----------------------------------------\n");

        choice = readInteger("Enter Your Choice: ");

        switch (choice) {
            case 1:
                addStudent();
                break;

            case 2:
                displayStudents();
                break;

            case 3:
                searchStudent();
                break;

            case 4:
                updateStudent();
                break;

            case 5:
                deleteStudent();
                break;

            case 6:
                generateResult();
                break;

            case 7:
                classStatistics();
                break;

            case 0:
                saveStudents();
                printf("Thank you for using the system!\n");
                break;

            default:
                printf("Invalid choice! Please try again.\n");
        }

    } while (choice != 0);

    return 0;
}