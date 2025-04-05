#include <iostream>
#include <fstream>
#include <cstring>
#include <limits>

using namespace std;

const int MAX_STUDENTS = 100;

// Структура студента
struct Student {
    char firstName[50];
    char lastName[50];
    int height;
    double averageGrade;
    int cleanings;
};


int loadStudents(Student students[], int& count) {
    ifstream file("list.txt");
    if (!file) {
        cout << "File not found!\n";
        return 0;
    }

    count = 0;
    while (file >> students[count].firstName >> students[count].lastName >> students[count].height >> students[count].averageGrade >> students[count].cleanings) {
        count++;
        if (count >= MAX_STUDENTS) break;
    }

    file.close();
    return count;
}


void inputGrades(double& averageGrade) {
    int sum = 0, count = 0, grade;
    cout << "Enter grades (1-12), enter 0 to finish: ";
    while (cin >> grade and grade != 0) {
        if (grade >= 1 and grade <= 12) {
            sum += grade;
            count++;
        }
        else {
            cout << "Invalid grade! Enter a number between 1 and 12: ";
        }
    }
    averageGrade = count > 0 ? double(sum) / count : 0.0;
}

bool studentExists(const Student students[], int count, const char* firstName, const char* lastName) {
    for (int i = 0; i < count; i++) {
        if (strcmp(students[i].firstName, firstName) == 0 and strcmp(students[i].lastName, lastName) == 0) {
            return true;
        }
    }
    return false;
}


// Додавання студентів у циклі
void addStudents() {

    Student students[MAX_STUDENTS];
    int count;
    loadStudents(students, count);

    ofstream file("list.txt", ios::app);
    if (!file) {
        cout << "Error opening file!\n";
        return;
    }

    while (true) {
        Student student;

        cout << "\nEnter student details (or enter 0 to stop):\n";

        cout << "Enter first name: ";
        cin >> student.firstName;
        if (strcmp(student.firstName, "0") == 0) break;

        cout << "Enter last name: ";
        cin >> student.lastName;
        if (strcmp(student.lastName, "0") == 0) break;

        if (studentExists(students, count, student.firstName, student.lastName)) {
            cout << "A student with this name already exists! Try again.\n";
            continue;
        }
        
        cout << "Enter height: ";
        cin >> student.height;
        if (student.height == 0) break;

        inputGrades(student.averageGrade);
        if (student.averageGrade == 0) break;

        cout << "Enter number of cleanings: ";
        cin >> student.cleanings;
        if (student.cleanings == 0) break;

        // Записуємо студента у файл
        file << student.firstName << " " << student.lastName << " "
            << student.height << " " << student.averageGrade << " "
            << student.cleanings << endl;

        cout << "Student added successfully!\n";
    }

    file.close();
}



// Виведення всіх студентів
void displayAllStudents() {
    Student students[MAX_STUDENTS];
    int count;

    if (loadStudents(students, count) == 0) {
        cout << "No students found!\n";
        return;
    }

    cout << "\nList of Students:\n";
    for (int i = 0; i < count; i++) {
        cout << students[i].firstName << " " << students[i].lastName
            << ", Height: " << students[i].height
            << ", Avg. Grade: " << students[i].averageGrade
            << ", Cleanings: " << students[i].cleanings << endl;
    }
}

// Пошук студента за параметром
void searchStudent() {
    Student students[MAX_STUDENTS];
    int count;

    if (loadStudents(students, count) == 0) return;

    char keyword[50];
    int height;
    cout << "Enter first name, last name, or height to search: ";
    cin >> keyword;

    bool found = false;
    for (int i = 0; i < count; i++) {
        if (strcmp(students[i].firstName, keyword) == 0 or strcmp(students[i].lastName, keyword) == 0 or (atoi(keyword) == students[i].height)) {
            cout << students[i].firstName << " " << students[i].lastName
                << ", Height: " << students[i].height
                << ", Avg. Grade: " << students[i].averageGrade
                << ", Cleanings: " << students[i].cleanings << endl;
            found = true;
        }
    }

    if (!found) cout << "No student found!\n";
}

// Видалення студента
void deleteStudent() {
    Student students[MAX_STUDENTS];
    int count;

    if (loadStudents(students, count) == 0) return;

    char firstName[50], lastName[50];
    cout << "Enter first name and last name of the student to delete: ";
    cin >> firstName >> lastName;

    ofstream file("list.txt");
    bool deleted = false;

    for (int i = 0; i < count; i++) {
        if (strcmp(students[i].firstName, firstName) == 0 and strcmp(students[i].lastName, lastName) == 0) {
            deleted = true;
            continue;
        }
        file << students[i].firstName << " " << students[i].lastName
            << " " << students[i].height << " " << students[i].averageGrade << " " << students[i].cleanings << endl;
    }

    file.close();
    if (deleted) cout << "Student deleted successfully!\n";
    else cout << "Student not found!\n";
}

// Оновлення інформації про студента
void updateStudent() {
    Student students[MAX_STUDENTS];
    int count;

    if (loadStudents(students, count) == 0) return;

    char firstName[50], lastName[50];
    cout << "Enter first name and last name of the student to update: ";
    cin >> firstName >> lastName;

    bool updated = false;
    ofstream file("list.txt");

    for (int i = 0; i < count; i++) {
        if (strcmp(students[i].firstName, firstName) == 0 and strcmp(students[i].lastName, lastName) == 0) {
            cout << "Enter new height: ";
            cin >> students[i].height;
            double newAverage;
            inputGrades(newAverage);

            students[i].averageGrade = (students[i].averageGrade + newAverage) / 2;

            cout << "Enter new number of cleanings: ";
            cin >> students[i].cleanings;
            updated = true;
        }
        file << students[i].firstName << " " << students[i].lastName
            << " " << students[i].height << " " << students[i].averageGrade << " " << students[i].cleanings << endl;
    }

    file.close();
    if (updated) cout << "Student information updated!\n";
    else cout << "Student not found!\n";
}

// Виведення відмінників (середній бал >= 10)
void displayHonorStudents() {
    Student students[MAX_STUDENTS];
    int count;

    if (loadStudents(students, count) == 0) return;

    cout << "\nHonor Students (Grade 10+):\n";
    bool found = false;

    for (int i = 0; i < count; i++) {
        if (students[i].averageGrade >= 10) {
            cout << students[i].firstName << " " << students[i].lastName
                << ", Height: " << students[i].height
                << ", Avg. Grade: " << students[i].averageGrade
                << ", Cleanings: " << students[i].cleanings << endl;
            found = true;
        }
    }

    if (!found) cout << "No honor students found!\n";
}

// Головне меню
int main() {
    while (true) {
        cout << "\nStudent Management System\n";
        cout << "1. Add Students\n";
        cout << "2. Delete Student\n";
        cout << "3. Update Student Information\n";
        cout << "4. Display All Students\n";
        cout << "5. Search Student\n";
        cout << "6. Display Honor Students\n";
        cout << "7. Exit\n";
        cout << "Choose an option: ";

        int choice;
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid choice\n";
            continue;
        }

        switch (choice) {
        case 1: addStudents(); break;
        case 2: deleteStudent(); break;
        case 3: updateStudent(); break;
        case 4: displayAllStudents(); break;
        case 5: searchStudent(); break;
        case 6: displayHonorStudents(); break;
        case 7: cout << "Exiting...\n"; return 0;
        default: cout << "Invalid option! Try again.\n";
        }
    }
}