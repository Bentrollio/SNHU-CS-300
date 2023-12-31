//============================================================================
// Name        : LinkedList.cpp
// Author      : Alex Baires
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Lab 3-3 Lists and Searching
//============================================================================

#include <algorithm>
#include <iostream>
#include <time.h>

#include "CSVparser.h"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold bid information
struct Bid {
    string bidId; // unique identifier
    string title;
    string fund;
    double amount;
    Bid() {
        amount = 0.0;
    }
};

//============================================================================
// Linked-List class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a linked-list.
 */
class LinkedList {

private:
    //Internal structure for list entries, housekeeping variables
    struct Node {
        Bid bid;
        struct Node* next;

        // default constructor
        Node() {
            next = nullptr;
        }

        // initialize with a bid
        explicit Node(Bid &aBid) {
            bid = aBid;
            next = nullptr;
        }
    };

    Node* head;
    Node* tail;
    int size = 0;

public:
    LinkedList();
    virtual ~LinkedList();
    void Append(Bid bid);
    void Prepend(Bid bid);
    void PrintList();
    void Remove(string bidId);
    Bid Search(string bidId);
    int Size();
};

/**
 * Default constructor
 */
LinkedList::LinkedList() {
    // Initializes an empty linked list because both head and tail point to null.
    head = nullptr;
    tail = nullptr;
}

/**
 * Destructor
 */
LinkedList::~LinkedList() {
    // start at the head
    Node* current = head;
    Node* temp;

    // loop over each node, detach from list then delete
    while (current != nullptr) {
        temp = current; // hang on to current node
        current = current->next; // make current the next node
        delete temp; // delete the orphan node
    }
}

/**
 * Append a new bid to the end of the list
 */
void LinkedList::Append(Bid bid) {
    // Creating a new node
    Node* newNode = new Node(bid);

    // Empty list, assign the newest node to the head.
    if (head == nullptr) {
        head = newNode;
    }

    // Else, populated list with an existing tail
    else {
        if (tail != nullptr) {

            // Current tail node points to the newest node.
            tail->next = newNode;
        }
    }
    // The newest node is the tail because it is last in the list.
    tail = newNode;

    // Each time we add a node, the size of the list grows.
    ++size;
}

/**
 * Prepend a new bid to the start of the list
 */
void LinkedList::Prepend(Bid bid) {
    // Create the new node
    Node* newNode = new Node(bid);

    // If the list is populated and has a head.
    if (head != nullptr) {
        // The new node is pointing at the head because it is now the 1st element
        newNode->next = head;
    }

    // Updates the head pointer to the appended node.
    head = newNode;

    // Each time we add a node, the size of the list grows.
    ++size;
}

/**
 * Simple output of all bids in the list
 */
void LinkedList::PrintList() {
    // Start at the head of the list
    Node* currentNode = head;

    // While loop iterates until the tail.
    while (currentNode != nullptr) {
        cout << currentNode->bid.bidId << ": " << currentNode->bid.title << " | "
             << currentNode->bid.amount << " : " << currentNode->bid.fund << endl;
        // Increments the node output.
        currentNode = currentNode->next;
    }
}

/**
 * Remove a specified bid
 *
 * @param bidId The bid id to remove from the list
 */
void LinkedList::Remove(const string bidId) {
    // If the list is not empty.
    if (head != nullptr) {

        // Deletes first element if the head is the matching node
        if (head->bid.bidId.compare(bidId) == 0) {
            Node* temp = head->next; // Temporarily stores address of node after head
            delete head;
            head = temp; // The node after the old head is now the new head
            --size;
            return;
        }
    }
    // The below executes if the matching node is found outside of the head
    Node* currentBid = head; // currentBid iterator temporarily points to the head.

    while (currentBid->next != nullptr) {

        // Peaks at the bid after the current bid's bidID to find a matching bidID
        if (currentBid->next->bid.bidId.compare(bidId) == 0) {
            Node *temp = currentBid->next; // Temporarily stores the next node
            currentBid->next = temp->next; // Moves the node after the next up one element
            delete temp; // Deletes the orphan node.

            --size;
            return;
        }
        currentBid = currentBid->next; // Moves onto the next node.
    }
}

/**
 * Search for the specified bidId
 *
 * @param bidId The bid id to search for
 */
Bid LinkedList::Search(const string bidId) {
    // Start at head node
    Node *currentBid = head;
    Node* temp = new Node; // creates temporary, blank node
    temp->bid.bidId = "";

    // Searches from head to tail
    while (currentBid != nullptr) {
        if (currentBid->bid.bidId.compare(bidId) == 0) {
            return currentBid->bid; // Returns if matching bidId found.
        }
        currentBid = currentBid->next; // Iterates through the nodes
    }
    return temp->bid; // Returns the blank node if bidID not found.
}

/**
 * Returns the current size (number of elements) in the list
 */
int LinkedList::Size() {
    return size;
}

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount
         << " | " << bid.fund << endl;
}

/**
 * Prompt user for bid information
 *
 * @return Bid struct containing the bid info
 */
Bid getBid() {
    Bid bid;

    cout << "Enter Id: ";
    cin.ignore();
    getline(cin, bid.bidId);

    cout << "Enter title: ";
    getline(cin, bid.title);

    cout << "Enter fund: ";
    cin >> bid.fund;

    cout << "Enter amount: ";
    cin.ignore();
    string strAmount;
    getline(cin, strAmount);
    bid.amount = strToDouble(strAmount, '$');

    return bid;
}

/**
 * Load a CSV file containing bids into a LinkedList
 *
 * @return a LinkedList containing all the bids read
 */
void loadBids(const string &csvPath, LinkedList* list) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser
    csv::Parser file = csv::Parser(csvPath);

    try {
        // loop to read rows of a CSV file
        for (int i = 0; i < file.rowCount(); i++) {

            // initialize a bid using data from current row (i)
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            //cout << bid.bidId << ": " << bid.title << " | " << bid.fund << " | " << bid.amount << endl;

            // add this bid to the end
            list->Append(bid);
        }
    }
    catch (csv::Error& e) {
        std::cerr << e.what() << std::endl;
    }
}

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

/**
 * Displays elapsed time and ticks
 *
 * @param ticks - The starting clock ticks defined as clock_t type
 */
void displayTelemetry(clock_t ticks) {
    // Calculates the elapsed time and displays the result.
    ticks = clock() - ticks; // current clock ticks minus starting clock ticks.
    cout << "time: " << ticks << " clock ticks" << endl;
    cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
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
    // process command line arguments
    string csvPath;
    switch (argc) {
        case 2:
            csvPath = argv[1];
            //bidKey = "98109";
            break;

        default:
            csvPath = "/Users/abaires/SNHU-CS-300/LinkedList/eBid_Monthly_Sales_Dec_2016.csv";
            break;
    }
    return csvPath;
}

/**
 * Creates Linked list to hold bids, displays a main menu and loads/displays bids pulled from the CSV file.
 * Depending on user choice, it will allow a user to append a bid to the rear of the list,
 * load the bids from a csv file, display all of the loaded bids, search for a bid, delete a bid.
 * or prepend a bid to the rear of the list.
 *
 * @param path - a string containing the path where the csv file can be found.
 */
 void mainMenu(const string &path) {

    const string &csvPath = path;
    string bidKey = {};
    clock_t ticks;

    LinkedList bidList;

    Bid bid;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Enter a Bid" << endl;
        cout << "  2. Load Bids" << endl;
        cout << "  3. Display All Bids" << endl;
        cout << "  4. Find Bid" << endl;
        cout << "  5. Remove Bid" << endl;
        cout << "  6. Prepend Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                bid = getBid();
                bidList.Append(bid);
                displayBid(bid);

                break;

            case 2:
                // Initialize a timer variable before loading bids.
                ticks = clock();

                loadBids(csvPath, &bidList);
                cout << bidList.Size() << " bids read" << endl;

                // Calculate elapsed time and display result
                displayTelemetry(ticks);
                break;

            case 3:
                bidList.PrintList();
                break;

            case 4:
                // Added user input to search by custom bidkey
                cout << "Enter bid key:" << endl;
                cin >> bidKey;
                cin.ignore();

                ticks = clock();

                bid = bidList.Search(bidKey);

                if (!bid.bidId.empty()) {
                    displayBid(bid);
                } else {
                    cout << "Bid Id " << bidKey << " not found." << endl;
                }

                displayTelemetry(ticks);
                break;

            case 5:
                // Added user input to search by custom bidkey
                cout << "Enter bid key:" << endl;
                cin >> bidKey;
                bidList.Remove(bidKey);
                break;

            case 6: // Added case 6 to add access to new Prepend() function.
                bid = getBid();
                bidList.Prepend(bid);
                displayBid(bid);
                break;

            default:
                cout << "Goodbye." << endl;
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
    mainMenu(processCommandLine(argc, argv)); // Tidied up the main function
    return 0;
}
