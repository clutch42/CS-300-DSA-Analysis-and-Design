// ABCUAdvising.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Brian Engel

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

const int DEFAULT_TABLE_SIZE = 11;  // default hash table size

// Got this function off github. Used to find a prime number for expanding the table size.
// Alabombarda. (2015, May 28).A simple program in C++ that finds
// the next prime number above a given number.
// GitHub.https://gist.github.com/alabombarda/f3944cd68dda390d25cb
int findNextPrime(int n) {
    int i = n + 1;
    while (true) {
        bool isPrime = true;
        for (int j = 2; j <= i / 2; j++) {
            if (i % j == 0) {
                isPrime = false;
                break;
            }
        }
        if (isPrime) {
            return i;
        }
        i++;
    }
}

// This function converts an alphanumeric string to an integer
// It assumes that the string contains only digits and letters
// It returns -1 if the string is empty or contains invalid characters 
int changeToInt(string courseNumber) {
    // gets rid of any spaces at the end of the string
    courseNumber.erase(courseNumber.find_last_not_of(' ') + 1);
    int result = 0; // Initialize the result to zero
    for (char c : courseNumber) { // Loop through each character in the string
        if (isdigit(c)) { // If the character is a digit
            result = result * 10 + (c - '0'); // Multiply the result by 10 and add the digit value
        }
        else if (isalpha(c)) { // If the character is a letter
            result = result * 36 + (toupper(c) - 'A' + 10); // Multiply the result by 36 and add the letter value
        }
        else { // If the character is invalid
            return -1; // Return -1 to indicate an error
        }
    }
    return result; // Return the final result
}

class HashTable{
    // A structure to represent a course in the hashtable
    struct Course {
        string courseNumber;    // The course number
        string name;            // The name of the course
        vector<string> prerequisites;  // A vector of strings that store the prerequisites of the course
    };
    // A structure to represent a node in the hashtable
    struct Node {
        Course course; // The course object
        int key = -1; // The hashed key of the course id
        Node* next = nullptr; // A pointer to the next node in the same key
    };

    int numberOfNodes = 0;  // integer to keep track of number of nodes in the hashtable
    vector<Node*> table;    // A vector of nodes to hold the hashtable

    // A function to hash the course id to make a key
    int hash(int id) {
        // Use a modulo operation as the hash function
        return id % table.size();
    }
    // A function to rehash the table if the ratio is too high
    void rehash() {
        // Store the old table size and data
        int oldSize = table.size();
        vector<Node*> oldData = table;

        // find next prime after double the table size and create a new empty table
        int newSize = findNextPrime(oldSize * 2);
        table.resize(newSize);
        // set each key to nullptr
        for (int i = 0; i < newSize; i++) {
            table.at(i) = nullptr;
        }
        numberOfNodes = 0;  // reset number of nodes because they will be added back in insert
        // Rehash the old data into the new table
        for (int i = 0; i < oldSize; i++) {
            Node* current = oldData.at(i);
            while (current != nullptr) {
                insertCourse(current->course);
                current = current->next;
            }
        }
    }


    // A function to insert a course into the hashtable
    void insertCourse(Course course) {
        // check the ratio of nodes to tablesize
        double ratio = numberOfNodes / (table.size() * 1.0);
        if (ratio > 0.6) {
            rehash();
        }
        // Hash the course id to make a key
        int key = hash(changeToInt(course.courseNumber));
        // Create a node and assign its key and course
        Node* node = new Node;  // create new node and set the attributes
        node->key = key;
        node->course = course;
        node->next = nullptr;
        // If the vector of nodes at the key is empty, assign the node to the vector at the key
        if (table.at(key) == nullptr) {
            table.at(key) = node;
        }
        // Else, add the node to the end of the vector
        else {
            Node* current = table.at(key);
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = node;
        }
        numberOfNodes++;    // increment node counter
    }
public:
    // default constructor
    HashTable() {
        table.resize(DEFAULT_TABLE_SIZE);
        for (int i = 0; i < numberOfNodes; i++) {
            table.at(i) = nullptr;
        }
    }
    // A constructor to initialize the hash table with a given size
    HashTable(int size) {
        table.resize(size);
        for (int i = 0; i < size; i++) {
            table.at(i) = nullptr;
        }
    }

    // function to print the hashtable 
    void PrintHashTable() {
        // Loop through each bucket of the table
        for (unsigned int i = 0; i < table.size(); i++) {
            // Print the bucket index
            cout << i << ": ";
            // Print the keys in the bucket
            Node* temp = table.at(i);
            while (temp != nullptr) {
                PrintCourse(temp->course);
                temp = temp->next;
            }
            cout << endl;
        }
    }

    // function to search for a course by course number and return the course object
    Course SearchCourse(string prereq) {
        // make sure letters are capitalized
        for (unsigned int i = 0; i < prereq.length(); i++) {
            prereq[i] = toupper(prereq[i]);
        }
        Course course;  // create course object and set number to blank
        course.courseNumber = "";
        int key = hash(changeToInt(prereq));    // get the hash value of prereq
        Node* temp = table.at(key);             // set temp to table at beginning of key's ll
        if (temp == nullptr) {                  // nothing in key return empty course
            return course;
        }
        // traverse the linked list table at key
        while (temp != nullptr) {
            if (temp->course.courseNumber.compare(prereq) == 0) {   // if course number and prereq match
                return temp->course;    // return that course
            }
            else  {
                temp = temp->next;      // move to next node in ll
            }
        }
        return course;                  // not found in ll return empty course
    }

    // function to print a single course that is passed
    void PrintCourse(Course course) {
        // print course number and description
        cout << "Course Number: " << course.courseNumber << ", Course Description: " << course.name;
        if (course.prerequisites.size() > 0) {  // if there are prerequisites
            cout << ", Prerequisites: ";
        }
        for (string p : course.prerequisites) { // loop through prerequisites and print them
            cout << p << " ";
        }
        cout << endl;
    }

    // function to sort the hashtable in a vector and print afterward
    void SortAndPrintTable(HashTable& hashtable) {
        vector<string> c;   // vector for holding course numbers
        // loop through all the buckets in the hashtable
        for (unsigned int i = 0; i < hashtable.table.size(); i++) {
            Node* temp = hashtable.table.at(i);
            while (temp != nullptr) {   // loop through all the nodes in the ll
                c.push_back(temp->course.courseNumber); // add the course number to the vector
                temp = temp->next;
            }
        }
        sort(c.begin(), c.end());   // sort the vector
        for (string all : c) {      // print all the courses
            PrintCourse(SearchCourse(all));
        }
        
    }
    
    // A function that reads a file and stores it in a hashtable of courses
    void ReadFile(string fileName) {
        ifstream file;  // Open the file
        file.open(fileName);
        // Check if the file opened successfully
        if (!file) {
            // Output an error message
            cout << "Could not open file" << endl;
            // return to main
            return;
        }
        if (file) {
            cout << "Opening file" << endl;
        }
        // Declare a string to store each line in the file
        string lineInFile;
        // Loop through each line in the file
        while (getline(file, lineInFile)) {
            // Create a course object
            Course course;
            // Create a string stream to parse the line
            stringstream ss(lineInFile);
            // Get the course number from the line
            getline(ss, course.courseNumber, ',');
            // capitalize any lower case letters
            for (unsigned int i = 0; i < course.courseNumber.length(); i++) {
                course.courseNumber.at(i) = toupper(course.courseNumber.at(i));
            }
            // Check if the course number is empty
            if (course.courseNumber.empty()) {
                // Output an error message
                cout << "Invalid course number" << endl;
                // Return to main
                return;
            }
            // Get the course name from the line
            getline(ss, course.name, ',');
            // Check if the course name is empty
            if (course.name.empty()) {
                // Output an error message
                cout << "Invalid course name" << endl;
                // Return to main
                return;
            }
            // Declare a string to store each prerequisite
            string prereq;
            // Loop through each prerequisite in the line
            while (getline(ss, prereq, ',')) {
                // Check if the prerequisite is in the hash table
                if (SearchCourse(prereq).courseNumber.compare(prereq) == 0) {
                    // Add the prerequisite to the course object
                    course.prerequisites.push_back(prereq);
                }
                else {
                    // Output an error message if not in table
                    cout << "Invalid prerequisite" << endl;
                    // Return to main
                    return;
                }
            }
            // Insert the course into the hash table and update the previous node pointer
            insertCourse(course);
        }
        // Close the file
        file.close();
        cout << "File successfully read" << endl;
    }
};

int main(int argc, char* argv[]) {
    // this is used to set a argument to the .exe file
    string csv_file = "input.csv"; // The default csv file name
    if (argc > 1) {
        csv_file = argv[1]; // The optional argument for a csv file name
    }
    HashTable test; // create HashTable
    char choice = ' ';  // set an initial char for the menu loop
    while (choice != '9') {
        cout << "1. Load data to hash table" << endl;
        cout << "2. Print course list in order" << endl;
        cout << "3. Print course details" << endl;
        cout << "4. Show hash table" << endl;
        cout << "9. Exit" << endl;
        cin >> choice;
        switch(choice) {
            case '1': { // if 1 read the default or passed argument
                test.ReadFile(csv_file);
                break;
            }
            case '2': { // if 2 sort and print hashtable
                test.SortAndPrintTable(test);
                break;
            }
            case '3': { // if 3 search for course and print details if found
                string c;
                cout << "Enter course to search for: ";
                cin >> c;
                if (test.SearchCourse(c).courseNumber != "") {
                    test.PrintCourse(test.SearchCourse(c));
                }
                else {
                    cout << "Course not found" << endl;
                }
                break;
            }
            case '4': { // this isn't on the project, but I had to create it for debugging and i thought it was pretty cool
                test.PrintHashTable();
                break;
            }
            case '9': { // if 9 exit the program
                cout << "Goodbye" << endl;
                break;
            }
            default:    // if none of the above error message and loop back around
                cout << "Invalid entry. Try again." << endl;
        }
    }
    return 0;
}