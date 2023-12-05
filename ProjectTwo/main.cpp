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
    void preOrder(Node* node);
    void ChopTree(Node* node);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void PreOrder();
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
 * Recursively deletes tree nodes
 *
 * @param node Current node in tree
 */
void BinarySearchTree::ChopTree(Node* node) {

    if (node) {
        ChopTree(node->left);
        ChopTree(node->right);
        delete node;
    }
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    // recurse from root deleting every node
    ChopTree(root);

}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
    // Calls inOrder function with root as argument
    this->inOrder(root);
}

/**
 * Traverse the tree in pre-order
 */
void BinarySearchTree::PreOrder() {
    // Calls preOrder function with root as argument
    this->preOrder(root);
}

/**
 * Insert Course into tree.
 *
 * @param Course - course object
 */
void BinarySearchTree::Insert(Course course) {
    // If the tree is empty, this node is the root.
    if (root == nullptr) {
        root = new Node(course);
        cout << root->course.courseID << "!!!" << endl;
    }
        // Otherwise, call the add node function with the root and new course.
    else {
        this->addNode(root, course);
    }
}

/**
 * Traverses the tree until a Course object has a matching courseID.
 *
 * @param string courseID - the course number being searched.
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

    // Default constructor if the course is not found.
    Course blankCourse;
    return blankCourse;
}

/**
 * Add a course to some node (recursive)
 *
 * @param node - Current node in tree
 * @param course - Course to be added
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

/**
 * Traverses the tree in a left root, right root pattern to print
 * in alphanumerical order.
 *
 * @param node - Current node in tree
 */
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

void BinarySearchTree::preOrder(Node* node) {
    if (node != nullptr) {
        cout << node->course.courseID << " - " << node->course.courseName << endl;
        cout << "Prerequisites: ";
        for (const auto& preReq : node->course.coursePrerequisites) {
            cout << preReq << " ";
        }
        cout << endl;
        cout << "--------------------------------------------" << endl;
        preOrder(node->left);
        preOrder(node->right);
    }
}

/**
 * Displays the course information to the console (std::out)
 *
 * @param - course - Course struct object containing the course info
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
 * Used for error-checking. Since each line parsed from the file is a vector,
 * this function iterates and prints each course in the vector to show
 * the user why it is not in the proper format.
 *
 * @param - string vector - errorLine, the vector containing the line that has improper format
 */
static void displayErrorFileLine(vector<string>& errorLine) {
    for (const auto& offendingCourse : errorLine) {
        cout << offendingCourse << " ";
    }
    cout << endl;
}

/**
 * Builds Course objects by parsing through each element within a string vector nested
 * inside of a parent vector.
 *
 * @param - A vector of string vectors containing course information.
 * @return - A vector of Course objects.
 */
// Pun intended.
static vector<Course> conStructCourses(vector<vector<string>> &contents) {
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

    // Exception handling in case file does not open.
    if (!courseData.is_open()) {
        throw "File not opened. Please check directory.";
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

    courseData.close();
    return defaultCourses; // Default course catalog created
}

/**
 * Ensures that the vector containing a line of course information is in proper format.
 * The function checks that each line in the text file is in the below format:
 *      Course ID, Course name
 *      OR
 *      Course ID, Course name, Course Prerequisite 1, Course Prerequisite 2, etc.
 * It then checks that the pre-req exists inside the default course catalog.
 *
 * @param - courseTokens - string vector that has prerequisites.
 * @param - courseCatalog - string vector that contains the default course catalog each prereq is compared with
 * @return - courseTokens - string vector that has been verified and error-checked
 */
static vector<string> verifyDataIntegrity(vector<string> &courseTokens, vector<string> &courseCatalog) {

    if (courseTokens.size() < 2) { // Verifies that each line has at least 2 elements
        // Outputs the invalid course line in file for user to review
        displayErrorFileLine(courseTokens);
        // Exception handling, will end program
        throw "Above Course Line Incomplete, please update file with at least COURSE ID and COURSE TITLE";
    }

    if (courseTokens.size() > 2) { // Checks for prerequisites
        for (size_t i = 2; i < courseTokens.size(); ++i) { // Iterate through last 2 elements of line (pre-reqs)
            int matchingCourse = {};
            for (const auto& individualCourse : courseCatalog) { // Iterates through the default course catalog
                if (individualCourse == courseTokens.at(i)) { // If the pre-req is found within the course catalog
                    ++matchingCourse;
                    break;
                }
            }

            if (matchingCourse != 1) { // The pre-req does not exist
                displayErrorFileLine(courseTokens);
                throw "Above Course Line has non-existent prerequisite course.";

            }
        }
    }
    return courseTokens;
}

/**
 * Parses through a file and tokenizes each line. Each line is stored as a vector within
 * a vector.
 *
 * @param - A string containing the path to the text or csv file being parsed.
 * @return - A vector of vectors that stores each course and its prereqs.
 */
static vector<vector<string>> fileParser(const string& filePath) {
    vector<string> courseParameters = loadDefaultCourseList(filePath);
    vector<string> tokens = {};
    vector<vector<string>> fileContents = {};

    string line;
    string word;

    ifstream courseData;
    courseData.open(filePath, ios::in);

    if (courseData.fail()) { // Error handling in case file does not open
        throw "Check file path.";
    }

    while (getline(courseData, line)) { // Parses through each line in file
        stringstream courseStream(line);
        tokens.clear(); // Prepares vector of tokenized words from line
        while (getline(courseStream, word, ',')) { // Parses through each word in line
            findBOM(word); // Cleans the token
            tokens.push_back(word);
        }

        verifyDataIntegrity(tokens, courseParameters);
        fileContents.push_back(tokens);
    }

    cout << "File successfully processed." << endl;
    courseData.close();
    return fileContents;
}

/**
 * Builds Course objects by parsing through each element within a string vector nested
 * inside of a parent vector.
 *
 * @param - courses - A vector of Course objects.
 * @param - tree - An empty binary search tree the courses will be attach to.
 */
static void loadCoursesIntoTree(vector<Course>& courses, BinarySearchTree* tree) {
    for (Course& course : courses) {
        tree->Insert(course);
    }
    courses.clear();
}

/**
 * FIX ME. Function used for testing purposes.
 *
 * @param - courses - A vector of Course objects.
 *
 */
static void printCourseList(vector<Course> courses) {//FIXME change arguments to tree
    for (size_t i = 0; i < courses.size(); ++i) {
        displayCourse(courses.at(i));
    }
}

/**
 * Compares adjacent course IDs inside of a vector for a linear sort.
 *
 * @param - two course objects.
 *
 */
static bool compareCourseID(const Course& firstCourse, const Course& secondCourse) {
    return firstCourse.courseID < secondCourse.courseID;
}

/**
 * Sorts a vector in ascending order. This will be used to create a balanced BST from the file to
 * alleviate the worst case runtime analysis of O(n).
 *
 * @param - sortee - a vector of Course objects.
 *
 */
static vector<Course> sortVector(vector<Course>& sortee) {
    sort(sortee.begin(), sortee.end(), compareCourseID);

    printCourseList(sortee);

    return sortee;
}

static void loadBalancedTree(vector<Course>& courses, BinarySearchTree* tree, int start, int end) {
    // Base case
    if (start > end) {
        return;
    }

    int mid = (start + end) / 2;

    tree->Insert(courses.at(mid)); // Middle element is the root.
    loadBalancedTree(courses, tree, start, mid - 1); // Get middle element of left side of vector partition and insert
    loadBalancedTree(courses, tree, mid + 1, end); // Get middle element of right side of vector partition and insert
}

/**
 * Calls the InOrder() function to print a sample schedule.
 *
 * @param - tree - the binary search tree storing course objects.
 *
 */
static void printSampleSchedule(BinarySearchTree* tree) {
    cout << "Here is a sample schedule:" << endl;
    cout << endl;
    tree->InOrder();
}

/**
 * Searches the tree for a specific course number.
 *
 * @param - tree - the binary search tree storing course objects.
 * @param - courseNumber - string with the course ID being searched for.
 *
 */
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
 * Gets user input and displays a main menu. The user can load the course objects into the
 * Binary Search tree. They have the choice of printing a list of courses, or searching
 * for a specific course. If the user enters 9, the program exits.
 *
 * @param - courseVector, a vector of Course object and tree, an empty Binary Search Tree
 */
static void mainMenu(vector <Course> &courseVector, BinarySearchTree* tree) {
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
                loadCoursesIntoTree(courseVector, tree);
                printCourseList(courseVector);
                break;

            case 2:
                printSampleSchedule(tree);
                break;

            case 3:
                cout << "What course do you want to know about?" << endl;
                cin.ignore();
                cin>>courseNumber;
                transform(courseNumber.begin(), courseNumber.end(), courseNumber.begin(), ::toupper);
                cout << courseNumber << endl;
                printCourseInformation(tree, courseNumber);
                break;
            case 4:
                sortVector(courseVector);
                loadBalancedTree(courseVector, tree, 0, courseVector.size() - 1);
                cout << "PreOrder" << endl;
                tree->PreOrder();
                cout << endl;
                cout << "InOrder" << endl;
                tree -> InOrder();
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
 *
 * Handles and checks command-line arguments during execution and stores the text file path into a variable
 * for the programDriver() function to utilize when it calls loadBids().
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
            cout << "Enter the file name to be loaded. Or press 'd' to load default file." << endl;
            cin >> filePath;

            if (filePath == "d") { // Opens default file for project
                filePath = "/Users/abaires/SNHU-CS-300/ProjectTwo/ABCU_Advising_Program_Input.txt";
                break;
            }

            break;
    }
    return filePath;
}

/**
 *
 * @param path - a string containing the path where the csv file can be found.
 */
static void programDriver(const string& path) {
    try {
        vector<vector<string>> courseInfo = fileParser(path);
        vector<Course> courses = conStructCourses(courseInfo);

        courseInfo.clear(); // Finished verifying data, cleared memory of courseInfo.

        BinarySearchTree* courseTree;
        courseTree = new BinarySearchTree();

        mainMenu(courses, courseTree);
    }
    catch (const char* exp) { // Exception handling
        cout << "EXCEPTION: " << exp << endl;
    }
    return;
}

/**
 * The one and only main() method
 *
 * @param arg[1] path to CSV file to load from (optional)
 * @param arg[2] the bid Id to use when searching the list (optional)
 */
int main(int argc, char* argv[]) {

    programDriver(processCommandLine(argc, argv));

    return 0;
}