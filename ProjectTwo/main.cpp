/*
 * PROJECT TWO
 */

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

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

// Internal structure holds a tree node.
struct Node {
    Course course;
    Node* left;
    Node* right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // Use a course to initialize
    Node(Course someCourse) : Node() {
        this->course = someCourse;
    }
};

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Course course);
    void inOrder(Node* node);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void Insert(Course course);
    Course Search(string courseID);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    // Initialize root as null pointer
    root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    // recurse from root deleting every node
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
    // Calls inOrder function with root as argument
    this->inOrder(root);

}

/**
 * Insert Course into tree.
 */
void BinarySearchTree::Insert(Course course) {
    // If the tree is empty, this node is the root.
    if (root == nullptr) {
        root = new Node(course);
    }
        // Otherwise, call the add node function with the root and new course.
    else {
        this->addNode(root, course);
    }
}

/**
 * Search for a course
 */
Course BinarySearchTree::Search(string courseID) {
    // Sets current node equal to root
    // Starts searching from the root
    Node* current = root;

    // Keeps looping downwards until bottom reached or marching courseID is found
    while (current != nullptr) {
        // Base case: Matching course found
        if (current->course.courseID == courseID) {
            return current->course;
        }
        // If course is less than current node, then traverse left side of tree
        if (current->course.courseID > courseID) {
            current = current->left;
        }
            // Otherwise, the value is greater than the current node: traverse right side of tree
        else {
            current = current->right;
        }
    }
}

/**
 * Add a course to some node (recursive)
 *
 * @param node Current node in tree
 * @param course Course to be added
 */
void BinarySearchTree::addNode(Node* node, Course course) {
    // Node is larger than the course so add to left subtree.
    if (node->course.courseID.compare(course.courseID) > 0) {
        // Base case: If the left child does not exist, insert new code as left child
        if (node->left == nullptr) {
            node->left = new Node(course);
        }
            // If there is already a left child, recursively call addNode to find spot for new course
        else {
            this->addNode(node->left, course);
        }
    }
        // Add to right subtree
    else {
        // Base case: If the right child does not exist, insert new node as right child
        if (node->right == nullptr) {
            node->right = new Node(course);
        }
            // If it already has a right child, recursively call addNode to find spot for new course
        else {
            this->addNode(node->right, course);
        }
    }
}

void BinarySearchTree::inOrder(Node* node) {
    if (node != nullptr) {
        inOrder(node->left);
        cout << node->course.courseID << " - " << node->course.courseName << endl;
        cout << "Prerequisites: ";
        for (const auto& preReq : node->course.coursePrerequisites) {
            cout << preReq << " ";
        }
        cout << endl;
        cout << "--------------------------------------------" << endl;
        inOrder(node->right);
    }
}

/**
 * Displays the course information to the console (std::out)
 *
 * @param - Course struct containing the course info
 */
static void displayCourse(Course course) {
    cout << course.courseID << " - " << course.courseName << endl;
    cout << "Prerequisites: ";
    for (const auto& preReq : course.coursePrerequisites) {
        cout << preReq << " ";
    }
    cout << endl;
    cout << "------------------------------------" << endl;
}

/**
 * Builds Course objects by parsing through each element within a string vector nested
 * inside of a parent vector.
 *
 * @param - A vector of string vectors containing course information.
 * @return - A vector of Course objects.
 */
// Pun intended.
static vector<Course> conStructCourses(vector<vector<string>> contents) {
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
static string findBOM(string &word) {
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
static vector <string> loadDefaultCourseList(const string& filePath) {
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

static vector<string> verifyPrerequisites(vector<string> &courseTokens, vector<string> &courseCatalog) {

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
static vector<vector<string>> checkDataIntegrity(const string& filePath) {
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

static void loadCoursesIntoTree(vector<Course>& courses, BinarySearchTree* tree) {
    for (Course& course : courses) {
        tree->Insert(course);
    }
    courses.clear();
}

static void printCourseList(vector<Course> courses) {//FIXME change arguments to tree
    for (size_t i = 0; i < courses.size(); ++i) {
        displayCourse(courses.at(i));
    }
}

static bool compareCourseID(const Course& firstCourse, const Course& secondCourse) {
    return firstCourse.courseID < secondCourse.courseID;
}

static vector<Course> sortVector(vector<Course>& sortee) {

    sort(sortee.begin(), sortee.end(), compareCourseID);
    printCourseList(sortee);
    return sortee;
}

static void printSampleSchedule(BinarySearchTree* tree) {
    cout << "Here is a sample schedule:" << endl;
    cout << endl;
    tree->InOrder();
}

static void printCourseInformation(BinarySearchTree* tree, string courseNumber) {
    Course course;
    course = tree->Search(courseNumber);

    if (!course.courseID.empty()) {
        displayCourse(course);
    }
    else {
        cout << "Course " << courseNumber << " not found in catalog." << endl;
    }
}

/**
 *
 * Handles and checks command-line arguments during execution and stores the csv file path into a variable
 * for the mainMenu() function to utilize when it calls loadBids().
 *
 * @param argc - number of command line arguments
 * @param argv - vector with the actual command line arguments
 */
string processCommandLine(int argc, char* argv[]) {
    // Process command line arguments
    string filePath;
    switch (argc) {
        case 2:
            filePath = argv[1];
            break;
        default:
            filePath = "/Users/abaires/SNHU-CS-300/ProjectTwo/ABCU_Advising_Program_Input.txt";
            break;
    }
    return filePath;
}

static void mainMenu(const string& path) {
    vector<vector<string>> courseInfo = checkDataIntegrity(path);
    vector<Course> courses = conStructCourses(courseInfo);

    courseInfo.clear(); // Finished verifying data, cleared memory of courseInfo.

    BinarySearchTree* courseTree;
    courseTree = new BinarySearchTree();

    int userInput = {};
    string courseNumber = {};

    while (userInput != 9) {
        cout << "*********************************" << endl;
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
                //sortVector(courses); FIXME adjust sort
                loadCoursesIntoTree(courses, courseTree);
                printCourseList(courses);
                break;
            case 2:
                printSampleSchedule(courseTree);
                break;
            case 3:
                cout << "What course do you want to know about?" << endl;
                cin >> courseNumber;
                printCourseInformation(courseTree, courseNumber);
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

/**
 * The one and only main() method
 *
 * @param arg[1] path to CSV file to load from (optional)
 * @param arg[2] the bid Id to use when searching the list (optional)
 */
int main(int argc, char* argv[]) {
    mainMenu(processCommandLine(argc, argv));
    return 0;
}