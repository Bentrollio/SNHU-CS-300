/*
 * PROJECT TWO
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// This structure holds course information.
struct Course {
    string courseID;
    string courseName;
    vector<string> coursePrerequisites;

    // Default constructor.
    Course() {
    }
};

/**
 * Displays the course information to the console (std::out)
 *
 * @param - Course struct containing the course info
 */
void displayCourse(Course course) {
    cout << course.courseID << " - " << course.courseName << endl;
    cout << "Prerequisites: ";
    for (const auto& preReq : course.coursePrerequisites) {
        cout << preReq << " ";
    }
    cout << endl;
    cout << "----------------" << endl;
}

/**
 * Builds Course objects by parsing through each element within a string vector nested
 * inside of a parent vector.
 *
 * @param - A vector of string vectors containing course information.
 * @return - A vector of Course objects.
 */
// Pun intended.
vector<Course> conStructCourses(vector<vector<string>> contents) {
    vector<Course> courses;
    // Iterate through each row
    for (const auto& row : contents) {
        Course course;
        // Iterate through the vector within the row of the parent vector.
        for (size_t i = 0; i < row.size(); ++i) {
            course.courseID = row.at(0);
            course.courseName = row.at(1);
            course.coursePrerequisites = {};
            // Checks if the course has prerequisites
            if (!(row.size() > 2)) {
                course.coursePrerequisites.push_back("No Prerequisites.");
                break;
            }
            // Course has pre-reqs, assigned to coursePrerequisites member vector in course
            for (size_t j = 2; j < row.size(); ++j) {
                course.coursePrerequisites.push_back(row.at(j));
            }
        }
        courses.push_back(course);
    }
    return courses; // Returns vector of course objects.
}

/**
 * Checks a string that was tokenized from a file for the UTF-8 BOM prepended to it.
 * If found, it trims the string to maintain data integrity.
 *
 * Source: https://cplusplus.com/forum/general/111203/
 * @param - A string
 * @return - A string with the first 3 characters trimmed from it.
 */
string findBOM(string &word) {
    if (word.compare(0, 3, "\xEF\xBB\xBF") == 0) {
        word.erase(0, 3);
    }
    return word;
}

/**
 * Parses through a text or CSV file and creates a vector of default courses
 * in the course catalog for comparison purposes.
 *
 * The function assumes each line in the text file is comma seperated and will
 * tokenize only the first comma separated word from each line.
 *
 * @param - A string containing the path to the text or csv file being parsed
 * @return - A vector containing a list of CourseIDs parsed from a file.
 */
vector <string> loadDefaultCourseList(const string& filePath) {
    vector<string> defaultCourses = {};
    vector<string> temp = {};

    ifstream courseData;
    courseData.open(filePath, ios::in);

    // Returns empty vector if file does not open.
    if (!courseData.is_open()) {
        return defaultCourses;
    }
    cout << "File successfully opened." << endl;

    string line;
    string word;

    while (getline(courseData, line)) { // Parses through each line in file
        stringstream courseStream(line);
        temp.clear(); // Prepares the temp vector to store a new line
        while (getline(courseStream, word, ',')) { // Parses through each word stripping comma

            findBOM(word); // Cleans the token
            temp.push_back(word); // Each word in the line is stored in a temporary vector
        }
        defaultCourses.push_back(temp.at(0)); // First element is the default course in catalog
    }
    cout << "End of file reached. Closing it now..." << endl;
    courseData.close();

    return defaultCourses;
}

vector<string> verifyPrerequisites(vector<string> &courseTokens, vector<string> &courseCatalog) {

    if (courseTokens.size() < 2) { // Verifies that each line has at least 2 elements.
        //cout << "ERROR: Course line incomplete." << endl; //FIXME Error handling
        courseTokens = {};
        return courseTokens;
    }
    if (courseTokens.size() > 2) { // Checks for prerequisites
        for (size_t i = 2; i < courseTokens.size(); ++i) {
            cout << "Prerequisite course " << courseTokens.at(i) << " size " << courseTokens.at(i).size() << endl;
            int iterator = {};
            for (const auto& courses : courseCatalog) { // Iterates through the default courses
                cout << "Checking " << courses << " to " << courseTokens.at(i) << endl; // FIXME remove in final product
                if (courses == courseTokens.at(i)) { // If the pre-req is found within the course catalog
                    ++iterator;
                    cout << iterator << " match found." << endl; //FIXME remove in live product
                    break;
                }
            }
            if (iterator != 1) { // The pre-req does not exist
                //cout << "No matches found!" << endl; //FIXME add error handling
                courseTokens = {};
                return courseTokens;
            }
        }
    }
    return courseTokens;
}

/**
 * Parses through a text or CSV file, ensuring proper formatting for compatibility
 * with the program.
 *
 * The function checks that each line in the text file is in the below format:
 *      Course ID, Course name
 *      OR
 *      Course ID, Course name, Course Prerequisite 1, Course Prerequisite 2, etc.
 *
 * @param - A string containing the path to the text or csv file being parsed.
 * @return - A vector of vectors that stores each course and its prereqs.
 */
vector<vector<string>> checkDataIntegrity(const string& filePath) {
    vector<string> courseParameters = loadDefaultCourseList(filePath);
    vector<string> tokens = {};
    vector<vector<string>> fileContents = {};

    if (courseParameters.empty()) { // Error handling for loadDefaultCourseList()
        cerr << "File could not found. Check directory" << endl; // FIXME Should throw an error + exit
        return fileContents;
    }

    string line;
    string word;

    ifstream courseData;
    courseData.open(filePath, ios::in);

    if (courseData.fail()) {
        cout << "ERROR: Check file path." << endl;
        return fileContents; // Returns empty vector if file does not reopen.
    }
    while (getline(courseData, line)) { // Parses through each line in file
        stringstream courseStream(line);
        tokens.clear(); // Prepares vector of tokenized words from line
        while (getline(courseStream, word, ',')) { // Parses through each word in line
            findBOM(word); // Cleans the token
            tokens.push_back(word);
        }
        verifyPrerequisites(tokens, courseParameters);
        if (tokens.empty()) {
            cerr << "Prerequisite mismatch with Course Catalog." << endl; // FIXME should throw an error +
            fileContents = {};
            return fileContents;
        }
        fileContents.push_back(tokens);
    }
    cout << "End of file reached. Closing it now." << endl;
    courseData.close();
    return fileContents;
}

void printCourseList(vector<Course> courses) {//FIXME change arguments to tree
    for (int i = 0; i < courses.size(); ++i) {
        displayCourse(courses.at(i));
    }
}

void printCourse(const string& filePath) { //FIXME search tree for a course
    cout << "Print a specific course!" << endl;
}

void mainMenu(const string& path) {
    vector<vector<string>> courseInfo = checkDataIntegrity(path);
    vector<Course> courses = conStructCourses(courseInfo);
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


                break;
            case 2:
                printCourseList(courses); //FIX ME: Needs to traverse through tree, not vector.
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