//============================================================================
// Name        : HashTable.cpp
// Author      : Alex Baires
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Module 4 Hash Table Assignment
//============================================================================

#include <algorithm>
#include <climits>
#include <iostream>
#include <string> // atoi
#include <time.h>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

const unsigned int DEFAULT_SIZE = 179;

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
// Hash Table class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a hash table with chaining.
 */
class HashTable {

private:
    // Define structures to hold bids
    struct Node {
        Bid bid;
        unsigned int key;
        Node *next;

        // default constructor
        Node() {
            key = UINT_MAX;
            next = nullptr;
        }

        // initialize with a bid
        Node(Bid aBid) : Node() {
            bid = aBid;
        }

        // initialize with a bid and a key
        Node(Bid aBid, unsigned int aKey) : Node(aBid) {
            key = aKey;
        }
    };

    vector<Node> nodes;

    unsigned int tableSize = DEFAULT_SIZE;
    unsigned int hash(unsigned int key);

public:
    HashTable();
    HashTable(unsigned int size);
    virtual ~HashTable();
    void Insert(Bid bid);
    void PrintAll();
    void Remove(string bidId);
    Bid Search(string bidId);

};

/**
 * Default constructor
 */
HashTable::HashTable() {

    // Initialize node structure by resizing tableSize
    nodes.resize(tableSize);
}

/**
 * Constructor for specifying size of the table
 * Use to improve efficiency of hashing algorithm
 * by reducing collisions without wasting memory.
 */
HashTable::HashTable(unsigned int size) {
    // Invokes local table size
    this->tableSize = size;

    // Resizes nodes vector
    nodes.resize(tableSize);
}

/**
 * Destructor
 */
HashTable::~HashTable() {
    // To be thorough, I used erase as a range
    nodes.erase(nodes.begin(), nodes.end());

}

/**
 * Calculate the hash value of a given key.
 * Note that key is specifically defined as
 * unsigned int to prevent undefined results
 * of a negative list index.
 *
 * @param key The key to hash
 * @return The calculated hash
 */
unsigned int HashTable::hash(unsigned int key) {

    return key % tableSize; // Modulo of the table size calculates the hash value.
}

/**
 * Insert a bid
 *
 * @param bid The bid to insert
 */
void HashTable::Insert(Bid bid) {

    // Converts bidID string to integer using stoi() standard string method introduced in C++11
    // Then creates a key
    unsigned key = hash(stoi(bid.bidId));

    // Retrieves node using its key.
    Node* oldNode = &(nodes.at(key)); // oldNode points to the memory address

    // if no entry found for the key
    if (oldNode == nullptr) {
        Node* newNode = new Node(bid, key);
        // Inserts node "key" places from the beginning of the vector
        nodes.insert(nodes.begin() + key, (*newNode));
    }

    else {
        // Node found, key for the node entry has not been used yet
        if (oldNode->key == UINT_MAX) {
            oldNode->key = key;
            oldNode->bid = bid;
            oldNode->next = nullptr; // No more nodes pointed to.
        }
        // Iterate through each node's next pointer. While it isn't null,
        // make it point to the next null until we find a null pointer.
        else {
            while (oldNode->next != nullptr) {
                oldNode = oldNode->next;
            }

            // Appends the new node to the linked list.
            oldNode->next = new Node(bid, key);
        }
    }
}

/**
 * Print all bids
 */
void HashTable::PrintAll() {

    // Range-based for loop provides simplicity for iterating through a vector
    for (Node aNode : nodes) {
        Node* currentNode = &aNode; // We just need a pointer to the memory address of each node

        // If the key is not equal to UINT_MAX
        if (currentNode->key != UINT_MAX) {
            cout << currentNode->key << ": " << currentNode->bid.bidId << " | "
                << currentNode->bid.title << " | " << currentNode->bid.amount << " | "
                 << currentNode->bid.fund << endl;
        }

        // Cycle through the nodes until the final one
        while (currentNode->next != nullptr) {
            cout << currentNode->key << ": " << currentNode->bid.bidId << " | "
                 << currentNode->bid.title << " | " << currentNode->bid.amount << " | "
                 << currentNode->bid.fund << endl;
            currentNode = currentNode->next; // Node is equal to the next one
        }

    }
}

/**
 * Remove a bid
 *
 * @param bidId The bid id to search for
 */
void HashTable::Remove(string bidId) {
    // set key equal to bidID converted to integer using C++11 stoi()
    unsigned key = hash(stoi(bidId));

    // Erases the node
    nodes.erase(nodes.begin() + key); // Offset, beginning of node plus offset
}

/**
 * Search for the specified bidId
 *
 * @param bidId The bid id to search for
 */
Bid HashTable::Search(string bidId) {
    Bid bid;

    // Creates the key for the given bid transformed into an int using C++11 stoi()
    unsigned key = hash(stoi(bidId));
    // Retrieves node using its key.
    Node* node = &(nodes.at(key)); // oldNode points to the memory address

    // If no entry found for the key
    if (node == nullptr || node->key == UINT_MAX) {
        return bid;
    }
    // If matching node entry found
    if (node != nullptr && node->key != UINT_MAX && node->bid.bidId.compare(bidId) == 0) {
        return node->bid;
    }

    // Walk the linked list to find the match
    while (node != nullptr) {
        // Current node matches the node/key
        if (node->key != UINT_MAX && node->bid.bidId.compare(bidId) == 0) {
            return node->bid;
        }
        node = node->next;
    }

    return bid;
}

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information to the console (std::out)
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
         << bid.fund << endl;
    return;
}

/**
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
void loadBids(const string& csvPath, HashTable* hashTable) {
    cout << "Loading CSV file " << csvPath << endl;

    // Initializes the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    // Reads and displays the header row - optional
    vector<string> header = file.getHeader();
    for (auto const& c : header) {
        cout << c << " | ";
    }
    cout << "" << endl;

    try {
        // Loops to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            // Creates a data structure and add to the collection of bids
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            // Pushes this bid to the end
            hashTable->Insert(bid);
        }
    } catch (csv::Error &e) {
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
    // Process command line arguments
    string csvPath;
    switch (argc) {
        case 2:
            csvPath = argv[1];
            //searchValue = "98109";
            break;

        default:
            csvPath = "/Users/abaires/SNHU-CS-300/LinkedList/eBid_Monthly_Sales_Dec_2016.csv";
            break;
    }
    return csvPath;
}

/**
 * Creates Hash Table to hold bids, displays a main menu and loads/displays bids pulled from the
 * CSV file. Depending on user choice, it will allow a user to load the bids from a CSV file, search for a bid,
 * or remove a bid.
 *
 * @param path - a string containing the path where the csv file can be found.
 */
void mainMenu(const string &path) {

    const string &csvPath = path;
    string searchValue = {};
    clock_t ticks;

    // Define a hash table to hold all the bids
    HashTable* bidTable;

    Bid bid;
    bidTable = new HashTable();

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display All Bids" << endl;
        cout << "  3. Find Bid" << endl;
        cout << "  4. Remove Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                // Initialize a timer variable before loading bids
                ticks = clock();

                // Complete the method call to load the bids
                loadBids(csvPath, bidTable);

                // Calculate elapsed time and display result
                displayTelemetry(ticks);
                break;

            case 2:
                bidTable->PrintAll();
                break;

            case 3:
                // Added user input to search by bid id
                cout << "Enter bid ID:" << endl;
                cin >> searchValue;

                ticks = clock();

                bid = bidTable->Search(searchValue);

                if (!bid.bidId.empty()) {
                    displayBid(bid);
                } else {
                    cout << "Bid Id " << searchValue << " not found." << endl;
                }

                displayTelemetry(ticks);
                break;

            case 4:
                // Added user input to search by custom bid id
                cout << "Enter bid ID to remove:" << endl;
                cin >> searchValue;

                // Search function called for input validation.
                bid = bidTable->Search(searchValue);

                // Removal now only occurs if the value exists!
                if (!bid.bidId.empty()) {
                    bidTable->Remove(searchValue);
                    cout << searchValue << " removed." << endl;
                }
                else {
                    cout << searchValue << " not found." << endl;
                    cout << "Returning to Menu..." << endl;
                }
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
    mainMenu(processCommandLine(argc, argv));
    return 0;
}