/*
* PROJECT TWO
*/

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

void loadCourses(const string& filePath) {
    vector<string> courseParameters = {};
    vector<string> tokens = {};
    ifstream courseData;
    courseData.open(filePath, ios::in);
    int iteration = {};

    if (courseData.is_open()) {
        cout << "File successfully opened. Course List:" << endl;
        string line;
        string word;

        while (courseData.good()) {
            getline(courseData, line);
            stringstream courseStream(line);
            while (getline(courseStream, word, ',')) {
                tokens.push_back(word);
            }
            if (tokens.size() < 2) {
                cout << "ERROR: Course line does not have at least 2 parameters." << endl;
                break;
            }
            if (tokens.size() > 2) {
                for (size_t i = 2; i < tokens.size(); ++i) {
                    vector<string> temp = {};
                    //string preReq = tokens.at(i);
                    cout << "PREREQ TESTED: "<< tokens.at(i) << endl;
                    string verifyLine;
                    string verifyWord;
                    ifstream verifyCourseData;
                    verifyCourseData.open(filePath, ios::in);
                    while (verifyCourseData.good()) {
                        getline(verifyCourseData, verifyLine);
                        stringstream verifyCourseStream(verifyLine);
                        while (getline(verifyCourseStream, verifyWord, ',')) {
                            temp.push_back(verifyWord);
                        }
                        cout << temp.at(0) << endl;

                        temp.clear();

//                        if(temp.at(0) == preReq) {
//                            cout << preReq << " is found in course catalog" << endl;
//                        }
                    }
                    verifyCourseData.close();
 //                   ifstream courseData2(filePath);
//                    while (getline(courseData2, moreLine)) {
//                        istringstream iss(moreLine);
//                        string firstWord, secondWord, thirdWord;
//
//
//                    }
  //                  courseData2.close();
                    //temp.clear();
                }
                ++iteration; // FIXME Remove iteration variable.
                cout << "Iteration number: " << iteration << endl;
            }
            cout << endl;
            cout << "VECTOR SIZE: " << tokens.size() << endl;
            tokens.clear();
            cout << endl;
        }

        cout << "End of file reached. Closing it now..." << endl;
        courseData.close();
    }
    else {
        cout << "File could not found. Check directory" << endl;
    }
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
                loadCourses(path);
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