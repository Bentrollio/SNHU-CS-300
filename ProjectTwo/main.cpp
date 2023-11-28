/*
* PROJECT TWO
*/

#include <iostream>
#include <regex>
#include <string>
#include <vector>

using namespace std;

void loadCourses(const string& filePath) {
    cout << "Load  Courses function" << endl; // FIXME load courses from csv
}

void printCourseList(const string& filePath) {//FIXME change arguments to tree
    cout << "Print the course list" << endl;
}

void printCourse(const string& filePath) { //FIXME search tree for a course
    cout << "Print a specific course!" << endl;
}

void mainMenu(const string &path) {
    int userInput = {};

    while (userInput != 9) {
        cout << "*********************************" << endl; // FIXME create main menu
        cout << "|        COURSE PLANNER         |" << endl;
        cout << "*********************************" << endl;
        cout << "|     1. Load Data Structure    |" << endl;
        cout << "|-------------------------------|" << endl;
        cout << "|     2. Print Course List      |" << endl;
        cout << "|-------------------------------|" << endl;
        cout << "|     3. Print Course           |" << endl;
        cout << "|-------------------------------|" << endl;
        cout << "|     9. Exit                   |" << endl;
        cout << "|-------------------------------|" << endl;
        cout << "*********************************" << endl;

        cout << "Choose an Option: ";
        cin >> userInput;

        switch (userInput) {
            case 1:
                loadCourses("test");
                break;
            case 2:
                printCourseList("test");
                break;
            case 3:
                printCourse("test");
                break;
            case 9:
                cout << "Thank you for using the course planner!" << endl;
                break;
            default:
                cout << userInput << " is not a valid option." << endl;
                break;
        }
    }
}

int main(int argc, char* argv[]) {
    string filePath;
    switch (argc) {
        case 2:
            filePath = argv[1];
            break;
        default:
            filePath = "/Users/abaires/SNHU-CS-300/ProjectTwo/ABCU_Advising_Program_Input.txt";
            break;
    }
    mainMenu(filePath); // FIXME adjust mainmenu arguments

    return 0;
}
