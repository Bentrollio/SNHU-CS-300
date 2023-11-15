//============================================================================
// Name        : BinarySearchTree.cpp
// Author      : Alex Baires
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Module 5: Binary Search Tree Assignment
//============================================================================

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

// Internal structure for tree node
struct Node {
    Bid bid;
    Node *left;
    Node *right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a given bid
    Node(Bid aBid) : Node() {
        this->bid = aBid;
    }
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Bid bid);
    void inOrder(Node* node);
    Node* removeNode(Node* node, string bidId);
    void postOrder(Node* node);
    void preOrder(Node* node);


public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void PostOrder();
    void PreOrder();
    void Insert(Bid bid);
    void Remove(string bidId);
    Bid Search(string bidId);
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
 * Traverse the tree in post-order
 */
void BinarySearchTree::PostOrder() {
    // Calls postOrder function with root as argument
    this->postOrder(root);
}

/**
 * Traverse the tree in pre-order
 */
void BinarySearchTree::PreOrder() {
    // Calls preOrder function with root as argument
    this->preOrder(root);
}

/**
 * Insert a bid
 */
void BinarySearchTree::Insert(Bid bid) {
    // If the tree is empty, this node is the root.
    if (root == nullptr) {
        root = new Node(bid);
    }
    // Otherwise, call the add node function with the root and new bid
    else {
        this->addNode(root, bid);
    }
}

/**
 * Remove a bid
 */
void BinarySearchTree::Remove(string bidId) {
    // remove node root bidID
    this->removeNode(root, bidId);
}

/**
 * Search for a bid
 */
Bid BinarySearchTree::Search(string bidId) {
    // Sets current node equal to root
    // Starts searching from the root
    Node* current = root;

    // Keeps looping downwards until bottom reached or matching bidId found
    while (current != nullptr) {
        // Base case: We found a matching bid
        if (current->bid.bidId.compare(bidId) == 0) {
            return current->bid;
        }
        // If bid is smaller than current node, then traverse left
        if (bidId.compare(current->bid.bidId) < 0) {
            current = current->left;
        }
        // Otherwise, the value is greater than the current node, Traverse right.
        else {
            current = current->right;
        }
    }

    // Returns an empty bid because it was not found in left/right nodes
    Bid bid;
    return bid;
}

/**
 * Add a bid to some node (recursive)
 *
 * @param node Current node in tree
 * @param bid Bid to be added
 */
void BinarySearchTree::addNode(Node* node, Bid bid) {
    // if node is larger than the bid, add to left subtree
    if (node->bid.bidId.compare(bid.bidId) > 0) {
        // Base case: If the left child does not exist, insert the new bid as the left child
        if (node->left == nullptr) {
            node->left = new Node(bid);
        }
        // If it already has a left child, we recursively call addNode
        // to search for a spot for the new bid
        else {
            this->addNode(node->left, bid);
        }
    }
    // Add to right subtree
    else {
        // Base case: If the right child does not exist, insert the new bid as the right child
        if (node->right == nullptr) {
            node->right = new Node(bid);
        }
        // If it already has a right child, we recursively call addNode
        // to search for a spot for the new bid
        else {
            this->addNode(node->right, bid);
        }

    }
}

Node* BinarySearchTree::removeNode(Node* node, string bidId) {
    // Base case: Bottom of the tree, parent has no children
    if (node == nullptr) {
        return node;
    }
    // Recurse down the left subtree (node smaller than bid)
    if (bidId.compare(node->bid.bidId) < 0) {
        node->left = removeNode(node->left, bidId);
    }
    // Recurse down the right subtree (node bigger than bid)
    else if ((bidId.compare(node->bid.bidId) > 0)) {
        node->right = removeNode(node->right, bidId);
    }
    else {
        // Leaf node with no children
        if (node->left == nullptr && node->right == nullptr) {
            delete node;
            node = nullptr;
        }
        // One child to the left
        else if (node->left != nullptr && node->right == nullptr) {
            Node* temp = node; // Temporary node pointer to copy the node
            node = node->left;
            delete temp;
        }
        // One child to the right
        else if (node->right != nullptr && node->left == nullptr) {
            Node *temp = node; // Temporary node pointer to copy the node
            node = node->right;
            delete temp;
        }
        // Two children
        else {
            Node* temp = node->right;
            while (temp->left != nullptr) {
                temp = temp->left;
            }
            node->bid = temp->bid;
            node->right = removeNode(node->right, temp->bid.bidId);
        }
    }
    return node;
}
void BinarySearchTree::inOrder(Node* node) {
    if (node != nullptr) {
        inOrder(node->left);
        cout << node->bid.bidId << ": " << node->bid.title << " | " << node->bid.amount << " | "
             << node->bid.fund << endl;
        inOrder(node->right);
    }
}
void BinarySearchTree::postOrder(Node* node) {
    if (node != nullptr) {
        postOrder(node->left);
        postOrder(node->right);
        cout << node->bid.bidId << ": " << node->bid.title << " | " << node->bid.amount << " | "
             << node->bid.fund << endl;
    }
}

void BinarySearchTree::preOrder(Node* node) {
    if (node != nullptr) {
        cout << node->bid.bidId << ": " << node->bid.title << " | " << node->bid.amount << " | "
             << node->bid.fund << endl;
        preOrder(node->left);
        preOrder(node->right);
    }
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
void loadBids(string csvPath, BinarySearchTree* bst) {
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
            bst->Insert(bid);
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
            csvPath = "/Users/abaires/SNHU-CS-300/BinarySearchTree/eBid_Monthly_Sales_Dec_2016.csv";
            break;
    }
    return csvPath;
}

/**
 * Creates Binary Search Tree to hold bids, displays a main menu and loads/displays bids pulled from the
 * CSV file. Depending on user choice, it will allow a user to load the bids from a CSV file, search for a bid,
 * or remove a bid.
 *
 * @param path - a string containing the path where the csv file can be found.
 */
void mainMenu(const string &path) {

    const string &csvPath = path;
    string bidKey = {};
    clock_t ticks;

    // Define a binary search tree to hold all bids
    BinarySearchTree* bst;
    bst = new BinarySearchTree();
    Bid bid;

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
                loadBids(csvPath, bst);

                //cout << bst->Size() << " bids read" << endl;

                // Calculate elapsed time and display result
                displayTelemetry(ticks);
                break;

            case 2:
                bst->InOrder();
                break;

            case 3:
                // Added user input to search by bid id
                cout << "Enter bid ID:" << endl;
                cin >> bidKey;

                ticks = clock();

                bid = bst->Search(bidKey);

                if (!bid.bidId.empty()) {
                    displayBid(bid);
                } else {
                    cout << "Bid Id " << bidKey << " not found." << endl;
                }

                displayTelemetry(ticks);
                break;

            case 4:
                cout << "Enter bid ID to remove:" << endl;
                cin >> bidKey;

                // Search function called for input validation
                bid = bst->Search(bidKey);

                // Removal now only occurs if the value exists!
                if(!bid.bidId.empty()) {
                    bst->Remove(bidKey);
                    cout << bidKey << " removed." << endl;
                }
                else {
                    cout << bidKey << " not found." << endl;
                    cout << "Returning to Menu..." << endl;
                }
                break;
        }
    }
    cout << "Goodbye." << endl;
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
