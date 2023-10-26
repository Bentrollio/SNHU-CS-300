//============================================================================
// Name        : VectorSorting.cpp
// Author      : Alex Baires
// Version     : 1.0
// Copyright   : Copyright � 2017 SNHU COCE
// Description : Vector Sorting Algorithms
//============================================================================

#include <algorithm>
#include <iostream>
#include <time.h>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// Forward declarations
double strToDouble(string str, char ch);

// Defines a structure to hold bid information
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
// Static methods used for testing
//============================================================================

/**
 * Displays the bid information to the console (std::out)
 *
 * @param - bid struct containing the bid info
 */
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
        << bid.fund << endl;
    return;
}

/**
 * Prompts the user for bid information using console (std::in)
 *
 * @return - Bid struct containing the bid info
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
 * Loads a CSV file containing bids into a container
 *
 * @param csvPath - the path to the CSV file to load
 * @return - a container holding all the bids read
 */
vector<Bid> loadBids(string csvPath) {
    cout << "Loading CSV file " << csvPath << endl;

    // Defines a vector data structure to hold a collection of bids.
    vector<Bid> bids;

    // Initializes the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    try {
        // Loop to read rows of a CSV file
        for (int i = 0; i < file.rowCount(); i++) {
            // Create a data structure and add to the collection of bids
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');
            // push this bid to the end
            bids.push_back(bid);
        }
    }
    catch (csv::Error& e) {
        std::cerr << e.what() << std::endl;
    }
    return bids;
}

/**
 * Partition the vector of bids into two parts, low and high
 *
 * @param bids Address of the vector<Bid> instance to be partitioned
 * @param begin Beginning index to partition
 * @param end Ending index to partition
 * 
 * @return - int of highest index in low partition
 */
int partition(vector<Bid>& bids, int begin, int end) {
    int low = begin;
    int high = end;

    // Choosing the middle index as the pivot.
    int mid = begin + (end - begin) / 2;

    // This stores the 'title' string value at the middle index of the bids array.
    string pivot = bids.at(mid).title;
    
    // Intentional infinite loop that exits with conditional that triggers a break
    while (true) {
        // Keeps incrementing low index while the title string value is less than the pivot.
        while (bids.at(low).title < pivot) {
            ++low;
        }

        // Keeps decrementing high index while the title string value is greater than the pivot.
        while (bids.at(high).title > pivot) {
            --high;
        }

        // Checks whether the left index reaches or passes the right index
        if (low >= high) {
            break;
        }
        /* ELSE swaps values when element in left partition is greater than the pivot
         * and an element in the right partition is less than the pivot to properly sort both
         * then moves the low and high indices closer together.*/
        else {
            swap(bids.at(low), bids.at(high));
            ++low;
            --high;
        }
    }
    return high; // returns highest index of low partition
}

/**
 * Perform a quick sort on bid title
 * Average performance: O(n log(n))
 * Worst case performance O(n^2))
 *
 * @param bids address of the vector<Bid> instance to be sorted
 * @param begin the beginning index to sort on
 * @param end the ending index to sort on
 */
void quickSort(vector<Bid>& bids, int begin, int end) {
    int mid = 0;

    // Base case; the subarray has 0 or 1 elements.
    if (end - begin <= 0) {
        return;
    }

    // Partitions the range of elements and provides the index of
    // the last element in the lower subarray partition.
    mid = partition(bids, begin, end);

    // Recursively sorts low partition
    quickSort(bids, begin, mid);

    // Recursively sorts high partition
    quickSort(bids, mid + 1, end);
}

/**
 * Perform a selection sort on bid title
 * Average performance: O(n^2))
 * Worst case performance O(n^2))
 *
 * @param bid address of the vector<Bid>
 *            instance to be sorted
 */
void selectionSort(vector<Bid>& bids) {
    int minimumBid = {}; // Tracks the lowest bid ecountered

    // pos is the demarcation between sorted and unsorted elements.
    // Outer loop iterates through array except for final element
    for (size_t pos = 0; pos < bids.size() - 1; ++pos) {
        minimumBid = pos;
        // Inner loop checks remaining values to find lower bid than minimum bid.
        for (size_t rightPos = pos + 1; rightPos < bids.size(); ++rightPos) {

            // New minimum bid found.
            if (bids.at(rightPos).title < bids.at(minimumBid).title) {
                minimumBid = rightPos;
            }
        }
        // Swaps if the minimum bid is not in the correct position
        if (minimumBid != pos) {
            swap(bids.at(pos), bids.at(minimumBid));
        }
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
        break;
    default:
        csvPath = "eBid_Monthly_Sales.csv";
    }
    return csvPath;
}

/**
 * Defines vector to hold bids, displays a main menu and loads/displays bids pulled from the CSV file.
 * Depending on user choice, it will call either a Selection Sort or Quick Sort algorithm and display
 * the processing time for comparison purposes.
 *
 * @param path - a string containing the path where the csv file can be found.
 */
void mainMenu(string path) {

    // Define a vector to hold all the bids
    vector<Bid> bids;

    // Define a timer variable
    clock_t ticks;

    string csvPath = path;

    int choice = 0;

    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display All Bids" << endl;
        cout << "  3. Selection Sort All Bids" << endl;
        cout << "  4. Quick Sort All Bids" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            // Initialize a timer variable before loading bids
            ticks = clock();

            // Complete the method call to load the bids
            bids = loadBids(csvPath);

            cout << bids.size() << " bids read" << endl;

            // Calculate elapsed time and display result
            displayTelemetry(ticks);
            break;

        case 2:
            // Loop and display the bids read
            for (int i = 0; i < bids.size(); ++i) {
                displayBid(bids[i]);
            }
            cout << endl;
            break;

            // Invokes the selection sort while reporting the timing results.
        case 3:
            // Initializes a timer variable before using selection sort on bids.
            ticks = clock();

            // Calls selection sort method on the bids vector
            selectionSort(bids);

            cout << bids.size() << " bids read" << endl;

            // Calculates the elapsed time and displays the result.
            displayTelemetry(ticks);
            break;

            // Invokes the quick sort algorithm while reporting the timing results.
        case 4:
            // Initializes a timer variable before using quick sort on bids.
            ticks = clock();

            // Calls quick sort method on the bids vector, with 0 as the first element and one minus the size of the vector as the last
            quickSort(bids, 0, bids.size() - 1);

            cout << bids.size() << " bids read" << endl;

            // Calculates the elapsed time and displays the result.
            displayTelemetry(ticks);
            break;
        }
    }
    cout << "Goodbye." << endl; // The space drove me crazy.
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    mainMenu(processCommandLine(argc, argv));

    return 0;
}