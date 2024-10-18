#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Course {
    char *name; // Name of the course
    int grade;  // Grade 0-10
};

struct CourseSet {
    struct Course *arr;
    int numCourses;
};

struct Student {
    int id;           // Student ID
    char *surname;    // Surname
    char *name;       // Name
    int age;          // AgeS
    struct CourseSet courses; // Courses
};

struct StudentSet {
    struct Student *students;  // Dynamic array of students
    int num_students;          // Number of students
};


void addStudent(struct StudentSet *set, char *name, char *surname, int age) {
    // Reallocate memory to accommodate the new student
    struct Student* temp = realloc(set->students, (set->num_students + 1) * sizeof(struct Student));
    if (temp == NULL) {
        printf("Error reallocating memory\n");
        return;
    }
    set->students = temp;

    // Find the insert position using binary search (if you are keeping students sorted)
    int low = 0, high = set->num_students - 1, insert_pos = set->num_students;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (strcmp(set->students[mid].surname, surname) < 0) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    insert_pos = low;

    // Shift elements to make space for the new student
    for (int i = set->num_students; i > insert_pos; i--) {
        set->students[i] = set->students[i - 1];
    }

    // Initialize the new student
    struct Student *new_student = &set->students[insert_pos];  // Insert at the correct position
    new_student->id = set->num_students + 1;  // Set student ID
    new_student->name = strdup(name);
    new_student->surname = strdup(surname);
    new_student->age = age;
    new_student->courses.arr = NULL;
    new_student->courses.numCourses = 0;

    set->num_students++;
}


void sortStudentsBySurname(struct StudentSet *set) {
    for (int i = 0; i < set->num_students - 1; i++) {
        for (int j = 0; j < set->num_students - i - 1; j++) {
            if (strcmp(set->students[j].surname, set->students[j + 1].surname) > 0) {
                // Αντιμετάθεση φοιτητών
                struct Student temp = set->students[j];
                set->students[j] = set->students[j + 1];
                set->students[j + 1] = temp;
            }
        }
    }
}


struct Student* searchStudentById(struct StudentSet *set, int student_id) {
    // Επανάληψη για κάθε φοιτητή
    for (int i = 0; i < set->num_students; i++) {
        if (set->students[i].id == student_id) {  // Έλεγχος αν το ID ταιριάζει
            return &set->students[i];  // Βρέθηκε ο φοιτητής
        }
    }
    // Αν δεν βρεθεί φοιτητής
    printf("Student with ID %d not found!\n", student_id);
    return NULL;
}                                                                                  
struct Student* searchStudentBySurname(struct StudentSet *set, char *surname) {
    int low = 0;
    int high = set->num_students - 1;
    
    while (low <= high) {
        int mid = low + (high - low) / 2;
        int cmp = strcmp(set->students[mid].surname, surname);
        
        if (cmp == 0) {
            return &set->students[mid];  // Βρήκαμε τον φοιτητή
        } else if (cmp < 0) {
            low = mid + 1;  // Αναζήτηση στο δεξί μισό
        } else {
            high = mid - 1;  // Αναζήτηση στο αριστερό μισό
        }
    }
    
    printf("Student with surname %s not found!\n", surname);  
    return NULL;
}

void addCourse(struct StudentSet *set) {
    int searchOption,student_id, grade;
    char surname[100];
    //char *course_name; 
    struct Student *student;
    
    printf("\nDo you want to search the student by ID or surename? (1 for ID , 2 for surename) \n");
    scanf("%d", &searchOption);
    do{ 
        if (searchOption == 1) {
            printf("Enter student ID: ");
            scanf("%d", &student_id);
            student = searchStudentById(set, student_id);

        } else if (searchOption == 2) {       
            printf("Enter student's surname: ");
            scanf("%s", surname);
            student = searchStudentBySurname(set, surname);
        } else{
            printf("invalid option select 1 or 2");
        }
    } while (searchOption != 1 && searchOption != 2) ;

    int courseCount ,i;
    char course_name[100];

     // Βρέθηκε ο φοιτητής, εισαγωγή μαθημάτων
    printf("How many courses would you like to insert? ");
    scanf("%d", &courseCount);

    // Αναδιάταξη πίνακα μαθημάτων με realloc
    student->courses.arr = realloc(student->courses.arr, (student->courses.numCourses + courseCount) * sizeof(struct Course));
    if (student->courses.arr == NULL) {
        printf("Memory allocation error!\n");
        return;
    }
    // Εισαγωγή μαθημάτων
    for (i = 0; i < courseCount; i++) {
        printf("Enter course name: ");
        scanf("%s", course_name);
        printf("Enter course grade (0-10): ");
        scanf("%d", &grade);

        // Έλεγχος εγκυρότητας βαθμού
        while (grade < 0 || grade > 10) {
            printf("Invalid grade! Enter a grade between 0 and 10: ");
            scanf("%d", &grade);
        }

        // Αποθήκευση του μαθήματος
        student->courses.arr[student->courses.numCourses].name = strdup(course_name);
        student->courses.arr[student->courses.numCourses].grade = grade;
        student->courses.numCourses++;
    }

    printf("Courses added successfully!\n");

}


void printStudent(struct Student *student) {
    if (student == NULL) return;
    printf("ID: %d\nName: %s %s\nAge: %d\nCourses:\n", student->id, student->name, student->surname, student->age);
    for (int i = 0; i < student->courses.numCourses; i++) {
        printf("  %s: %d\n", student->courses.arr[i].name, student->courses.arr[i].grade);
    }
}

void calculateAverage(struct Student *student) {
    if (student == NULL) return;
    if (student->courses.numCourses == 0) {
        printf("Student has no courses!\n");
        return;
    }
    int total = 0;
    for (int i = 0; i < student->courses.numCourses; i++) {
        total += student->courses.arr[i].grade;
    }
    double average = (double)total / student->courses.numCourses;
    printf("Average grade: %.2f\n", average);
}

void deleteStudent(struct StudentSet *set, int student_id) {
    // Find the student by ID
    int index = -1;
    for (int i = 0; i < set->num_students; i++) {
        if (set->students[i].id == student_id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Student with ID %d not found!\n", student_id);
        return;
    }

    struct Student *student = &set->students[index];

    // Free the dynamically allocated memory for the student's courses, name, and surname
    for (int i = 0; i < student->courses.numCourses; i++) {
        free(student->courses.arr[i].name);
    }
    free(student->courses.arr);
    free(student->name);
    free(student->surname);

    // Shift all students after the one being deleted one position to the left
    for (int i = index; i < set->num_students - 1; i++) {
        set->students[i] = set->students[i + 1];
    }

    // Resize the array to remove the last student (which is now a duplicate)
    set->students = realloc(set->students, (set->num_students - 1) * sizeof(struct Student));

    if (set->students == NULL && set->num_students - 1 > 0) {
        printf("Memory reallocation failed after deleting student!\n");
        return;
    }

    set->num_students--;
    printf("Student with ID %d successfully deleted.\n", student_id);
}
void deleteCourse(struct Student *student, char *course_name) {
    if (student == NULL || student->courses.numCourses == 0) {
        printf("No courses to delete!\n");
        return;
    }

    int found = -1;
    for (int i = 0; i < student->courses.numCourses; i++) {
        if (strcmp(student->courses.arr[i].name, course_name) == 0) {
            found = i;
            free(student->courses.arr[i].name);
            break;
        }
    }

    if (found == -1) {
        printf("Course %s not found!\n", course_name);
        return;
    }

    for (int i = found; i < student->courses.numCourses - 1; i++) {
        student->courses.arr[i] = student->courses.arr[i + 1];
    }

    student->courses.arr = realloc(student->courses.arr, (student->courses.numCourses - 1) * sizeof(struct Course));
    student->courses.numCourses--;
    printf("\nCourse deleted sucsesfully\n");
}


// Helper function to free dynamically allocated memory for a student
void freeStudent(struct Student *student) {
    free(student->name);     // Free dynamically allocated name
    free(student->surname);  // Free dynamically allocated surname
    for (int j = 0; j < student->courses.numCourses; j++) {
        free(student->courses.arr[j].name);  // Free each course name
    }
    free(student->courses.arr);  // Free courses array
}

// Free previously allocated data in the StudentSet
void freeStudentSet(struct StudentSet *set) {
    for (int i = 0; i < set->num_students; i++) {
        freeStudent(&set->students[i]);
    }
    free(set->students);  // Free students array
}

void loadFromFile(struct StudentSet *set, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    int file_num_students;
    // Read the number of students from the file
    if (fscanf(file, "Number of Students : %d\n", &file_num_students) != 1) {
        printf("Error reading number of students!\n");
        fclose(file);
        return;
    }

    // Reallocate memory to accommodate new students
    int new_total_students = set->num_students + file_num_students;
    struct Student* temp = realloc(set->students, new_total_students * sizeof(struct Student));
    if (temp == NULL) {
        printf("Memory allocation failed!\n");
        fclose(file);
        return;
    }
    set->students = temp;

    // Load students from the file and add them to the set
    for (int i = set->num_students; i < new_total_students; i++) {
        struct Student *student = &set->students[i];
        char name[100], surname[100];
        
        // Reading student data
        if (fscanf(file, "ID : %d\n Name : %s\n Surname : %s\n Age : %d\n Number of courses : %d\n", 
            &student->id, name, surname, &student->age, &student->courses.numCourses) != 5) {
            printf("Error reading student details!\n");
            fclose(file);
            return;
        }

        // Copy the name and surname
        student->name = strdup(name);
        student->surname = strdup(surname);

        // Allocate memory for courses
        if (student->courses.numCourses > 0) {
            student->courses.arr = malloc(student->courses.numCourses * sizeof(struct Course));
            if (student->courses.arr == NULL) {
                printf("Memory allocation failed for courses!\n");
                fclose(file);
                return;
            }

            // Reading course names and grades
            for (int j = 0; j < student->courses.numCourses; j++) {
                char courseName[100];
                if (fscanf(file, "%s %d\n", courseName, &student->courses.arr[j].grade) != 2) {
                    printf("Error reading course details!\n");
                    fclose(file);
                    return;
                }
                student->courses.arr[j].name = strdup(courseName);
            }
        } else {
            student->courses.arr = NULL;
        }
    }

    // Update the total number of students
    set->num_students = new_total_students;

    fclose(file);
}

void saveToFile(struct StudentSet *set, const char *filename) {
    FILE *file = fopen(filename, "w");  // Use "w" only if you want to overwrite
    if (file == NULL) {
        printf("Error opening file for saving!\n");
        return;
    }

    // Write the number of students to the file
    fprintf(file, "Number of Students : %d\n", set->num_students);

    // Loop through all students and write their information
    for (int i = 0; i < set->num_students; i++) {
        struct Student *student = &set->students[i];
        
        fprintf(file, "ID : %d\n", student->id);
        fprintf(file, "Name : %s\n", student->name);
        fprintf(file, "Surname : %s\n", student->surname);
        fprintf(file, "Age : %d\n", student->age);
        fprintf(file, "Number of courses : %d\n", student->courses.numCourses);

        // Write the courses
        for (int j = 0; j < student->courses.numCourses; j++) {
            fprintf(file, "%s %d\n", student->courses.arr[j].name, student->courses.arr[j].grade);
        }

        fprintf(file, "\n");  // Add a newline between students
    }

    fclose(file);
    printf("Data saved successfully to %s\n", filename);
}


int main() {
    struct StudentSet studentSet;
    studentSet.students = NULL;
    studentSet.num_students = 0;

    loadFromFile(&studentSet, "students.txt");

    int choice;
    do {
        printf("\nStudent Management System:\n");
        printf("1. Add Student\n");
        printf("2. Add Course to Student\n");
        printf("3. Search Student by ID\n");
        printf("4. Search Student by Surname\n");
        printf("5. Calculate Average Grade\n");
        printf("6. Delete Student\n");
        printf("7. Delete Course from Student\n");
        printf("8. Exit and Save\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            char name[100], surname[100];
            int age;
            printf("Enter student's name: ");
            scanf("%s", name);
            printf("Enter student's surname: ");
            scanf("%s", surname);
            printf("Enter student's age: ");
            scanf("%d", &age);
            addStudent(&studentSet, name, surname, age);
        } else if (choice == 2) {
            addCourse(&studentSet);
        } else if (choice == 3) {
            int student_id;
            printf("Enter student ID: ");
            scanf("%d", &student_id);
            struct Student *student = searchStudentById(&studentSet, student_id);
            printStudent(student);
        } else if (choice == 4) {
            char surname[100];
            printf("Enter student's surname: ");
            scanf("%s", surname);
            struct Student *student = searchStudentBySurname(&studentSet, surname);
            printStudent(student);
        } else if (choice == 5) {
            int student_id;
            printf("Enter student ID: ");
            scanf("%d", &student_id);
            struct Student *student = searchStudentById(&studentSet, student_id);
            calculateAverage(student);
        } else if (choice == 6) {
            int student_id;
            printf("Enter student ID to delete: ");
            scanf("%d", &student_id);
            deleteStudent(&studentSet, student_id);
        } else if (choice == 7) {
            int student_id;
            char course_name[100];
            printf("Enter student ID: ");
            scanf("%d", &student_id);
            printf("Enter course name to delete: ");
            scanf("%s", course_name);
            struct Student *student = searchStudentById(&studentSet, student_id);
            deleteCourse(student, course_name);
        } else if (choice >= 9 || choice <= 0) {
            printf("\n Please choose a choice between 1-8!! \n");

        }
    } while (choice != 8);

    
    saveToFile(&studentSet, "students.txt"); // Αποθήκευση δεδομένων σε αρχείο πριν την έξοδο
     // Καθαρισμός μνήμης
    for (int i = 0; i < studentSet.num_students; i++) {
        free(studentSet.students[i].name);
        free(studentSet.students[i].surname);
        for (int j = 0; j < studentSet.students[i].courses.numCourses; j++) {
            free(studentSet.students[i].courses.arr[j].name);
        }
        free(studentSet.students[i].courses.arr);
    }
    free(studentSet.students);

    return 0;
}