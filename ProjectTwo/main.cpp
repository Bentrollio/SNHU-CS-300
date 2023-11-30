/*
* PROJECT TWO
*/

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

vector <string> loadDefaultCourseList(const string& filePath) {
    vector<string> defaultCourses = {};
    vector<string> temp = {};
    ifstream courseData;
    courseData.open(filePath, ios::in);

    if (!courseData.is_open()) {
        cout << "File could not found. Check directory" << endl;
        return defaultCourses; // Vector is empty.
    }
    cout << "File successfully opened." << endl;
    string line;
    string word;

    while (getline(courseData, line)) {
        stringstream courseStream(line);
        temp.clear();
        while (getline(courseStream, word, ',')) {

            // Reading the file gave me the UTF-8 BOM appended to my token.
            // The below checks each token for the BOM.
            // Source: https://cplusplus.com/forum/general/111203/
            if (word.compare(0, 3, "\xEF\xBB\xBF") == 0) {
                word.erase(0, 3);
            }
            temp.push_back(word);
        }
        defaultCourses.push_back(temp.at(0));
    }
    cout << "End of file reached. Closing it now..." << endl;
    courseData.close();

    return defaultCourses;
}

void checkDataIntegrity(const string& filePath) {
    vector<string> courseParameters = loadDefaultCourseList(filePath);
    string line;
    string word;
    cout << "COURSE LIST" << endl;
    for (const auto& w : courseParameters) {
        cout << w << "size: " << w.size() << endl;
    }
    vector<string> tokens = {};
    ifstream courseData;
    courseData.open(filePath, ios::in);
    int iteration = {};

    if (courseData.fail()) {
        cout << "ERROR: Check file path." << endl;
        return;
    }
    while (getline(courseData, line)) {
        stringstream courseStream(line);
        tokens.clear();
        while (getline(courseStream, word, ',')) {
            // Reading the file gave me the UTF-8 BOM appended to the first token throwing off the size.
            // The below checks each token for the BOM and strips it from the string.
            // Source: https://cplusplus.com/forum/general/111203/
            if (word.compare(0, 3, "\xEF\xBB\xBF") == 0) {
                word.erase(0, 3);
            }
            tokens.push_back(word);
        }
        if (tokens.size() < 2) {
            cout << "ERROR: Course line incomplete." << endl;
            break;
        }
        if (tokens.size() > 2) {
            for (size_t i = 2; i < tokens.size(); ++i) {
                cout << "Prerequisite course " << tokens.at(i) << " size " << tokens.at(i).size() << endl;
                for (const auto& courses : courseParameters) {
                    if (courses == tokens.at(i)) {
                        cout << "Course Param: " << courses << endl;
                        cout << "Param Match: " << tokens.at(i) << endl;
                        // FIX ME CALL A STRUCT FUNCTION??
                    }
                }
            }
            ++iteration; // FIXME Remove this in final product.
            cout << "Iteration number: " << iteration << endl;
        }
        cout << endl;
        cout << "VECTOR SIZE: " << tokens.size() << endl;
    }
    cout << "End of file reached. Closing it now." << endl;
    courseData.close();
}

void printCourseList(const string& filePath) {//FIXME change arguments to tree
    cout << "Print the course list" << endl;
}

void printCourse(const string& filePath) { //FIXME search tree for a course
    cout << "Print a specific course!" << endl;
}

void mainMenu(const string& path) {
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
                checkDataIntegrity(path);
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