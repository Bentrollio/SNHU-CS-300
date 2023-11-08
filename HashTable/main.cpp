//============================================================================
// Name        : HashTable.cpp
// Author      : Alex Baires
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Hello World in C++, Ansi-style
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
    // FIXME (1): Initialize the structures used to hold bids

    // Initialize node structure by resizing tableSize
    nodes.resize(tableSize);
}

/**
 * Constructor for specifying size of the table
 * Use to improve efficiency of hashing algorithm
 * by reducing collisions without wasting memory.
 */
HashTable::HashTable(unsigned int size) {
    // invoke local tableSize to size with this->
    this->tableSize = size;
    // resize nodes size
    nodes.resize(tableSize);
}


/**
 * Destructor
 */
HashTable::~HashTable() {
    // FIXME (2): Implement logic to free storage when class is destroyed
    nodes.erase(nodes.begin(), nodes.end()); // FIXME: Alex modification, remove end if funky.
    // erase nodes beginning
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
    // FIXME (3): Implement logic to calculate a hash value
    return key % tableSize; // Modulus of the table size for hash value.
}

/**
 * Insert a bid
 *
 * @param bid The bid to insert
 */
void HashTable::Insert(Bid bid) {
    // FIXME (5): Implement logic to insert a bid
    // create the key for the given bid
    // Converts bidID string to c-string in order to convert to integer
    unsigned key = hash(atoi(bid.bidId.c_str())); //FIXME: See if we can use sstream
    // Retrieves node using its key.
    Node* oldNode = &(nodes.at(key)); // oldNode points to the memory address
    // if no entry found for the key
    if (oldNode == nullptr) {
        Node* newNode = new Node(bid, key);
        // Inserts node "key" places from the beginning of the vector
        nodes.insert(nodes.begin() + key, (*newNode));
    }
    // assign this node to the key position
    // else if node is not used
    else {
        // node found, key for the node entry has not been used yet
        // assign old node key to UNIT_MAX, set to key, set old node to bid and old node next to null pointer
        if (oldNode->key == UINT_MAX) {
            oldNode->key = key;
            oldNode->bid = bid;
            oldNode->next = nullptr; // No more nodes pointed to.
        }
        // Iterate through each node's next pointer. While it isn't null,
        // make it point to the next null until we find a null pointer. */
        else {
            while (oldNode->next != nullptr) {
                oldNode = oldNode->next;
            }
            oldNode->next = new Node(bid, key);
        }
    }
    // add new newNode to end
}

/**
 * Print all bids
 */
void HashTable::PrintAll() {
    // FIXME (6): Implement logic to print all bids
    // for node begin to end iterate
    for (Node anode : nodes) {
        Node* currentNode = &anode;

        if (currentNode->key != UINT_MAX) {
            cout << currentNode->key << ": " << currentNode->bid.bidId << " | "
                << currentNode->bid.title << " | " << currentNode->bid.amount << " | "
                 << currentNode->bid.fund << endl;
        }

        while (currentNode->next != nullptr) {
            currentNode = currentNode->next;
            cout << currentNode->key << ": " << currentNode->bid.bidId << " | "
                 << currentNode->bid.title << " | " << currentNode->bid.amount << " | "
                 << currentNode->bid.fund << endl;
        }

    }
    //   if key not equal to UINT_MAx
    // output key, bidID, title, amount and fund
    // node is equal to next iter
    // while node not equal to nullptr
    // output key, bidID, title, amount and fund
    // node is equal to next node

}

/**
 * Remove a bid
 *
 * @param bidId The bid id to search for
 */
void HashTable::Remove(string bidId) {
    // FIXME (7): Implement logic to remove a bid
    // set key equal to hash atoi bidID cstring
    unsigned key = hash(atoi(bidId.c_str()));

    // erase node begin and key
    nodes.erase(nodes.begin() + key); // Offset, beginning of node plus offset
}

/**
 * Search for the specified bidId
 *
 * @param bidId The bid id to search for
 */
Bid HashTable::Search(string bidId) {
    Bid bid;

    // FIXME (8): Implement logic to search for and return a bid

    // create the key for the given bid
    unsigned key = hash(atoi(bidId.c_str()));
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
        if (node->key != UINT_MAX && node->bid.bidId.compare(bidId) == 0) {
            return node->bid;
        }
        node = node->next;
    }
    // return bid
    // while node not equal to nullptr
    // if the current node matches, return it
    //node is equal to next node

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
void loadBids(string csvPath, HashTable* hashTable) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    // read and display header row - optional
    vector<string> header = file.getHeader();
    for (auto const& c : header) {
        cout << c << " | ";
    }
    cout << "" << endl;

    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of bids
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            //cout << "Item: " << bid.title << ", Fund: " << bid.fund << ", Amount: " << bid.amount << endl;

            // push this bid to the end
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
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, searchValue;
    switch (argc) {
        case 2:
            csvPath = argv[1];
            searchValue = "98010";
            break;
        case 3:
            csvPath = argv[1];
            searchValue = argv[2];
            break;
        default:
            csvPath = "/Users/abaires/SNHU-CS-300/HashTable/eBid_Monthly_Sales_Dec_2016.csv";
            searchValue = "98010";
    }

    // Define a timer variable
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
                ticks = clock() - ticks; // current clock ticks minus starting clock ticks
                cout << "time: " << ticks << " clock ticks" << endl;
                cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
                break;

            case 2:
                bidTable->PrintAll();
                break;

            case 3:
                ticks = clock();

                bid = bidTable->Search(searchValue);

                ticks = clock() - ticks; // current clock ticks minus starting clock ticks

                if (!bid.bidId.empty()) {
                    displayBid(bid);
                } else {
                    cout << "Bid Id " << searchValue << " not found." << endl;
                }

                cout << "time: " << ticks << " clock ticks" << endl;
                cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
                break;

            case 4:
                bidTable->Remove(searchValue);
                break;
        }
    }

    cout << "Goodbye." << endl;

    return 0;
}