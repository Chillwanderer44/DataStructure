#include "helpers.hpp"
#include "linkedlist.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono> // for performance measurement
#include <iomanip> // for formatting
#include <string.h>
#include <algorithm>


using namespace std;
using namespace std::chrono;

// Constructor 
TransactionList::TransactionList() : head(nullptr), size(0){}

//Destructor
TransactionList::~TransactionList(){
    clear();
}

// Clear list and free memory
void TransactionList::clear() {
    NodeTransaction* current = head;
    while (current != nullptr){
        NodeTransaction* temp = current;
        current = current->next;
        delete temp;
    }
    head = nullptr;
    size = 0;
}

//insert a transaction at the beginning of the list
void TransactionList::insert(Transaction transaction) {
    NodeTransaction* newnode = new NodeTransaction;
    newnode->data = transaction;
    newnode->next = head;
    head = newnode;
    size++;
}
//Display all transactions
void TransactionList::display() {
    NodeTransaction* current = head;
    cout << "-------------------------------------------------------------------------------------------\n";
    cout << "|     Date     | Customer ID |     Product     |   Category   |  Price  |   Payment Method  |\n";
    cout << "-------------------------------------------------------------------------------------------\n";
    
    while (current != nullptr) {
        cout << "| " << current->data.date
             << " | " << current->data.customerID
             << " | " << current->data.product
             << " | " << current->data.category
             << " | " << current->data.price
             << " | " << current->data.paymentMethod << " |\n";
        current = current->next;
    }
    cout << "-------------------------------------------------------------------------------------------\n";
}

// Get size of the list
int TransactionList::getSize() const {
    return size;
}

// Get head of the list
NodeTransaction* TransactionList::getHead() const {
    return head;
}

//load transactions from CSV
void TransactionList::loadFromCSV(const char* filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return;
    }

    // Clear existing list
    clear();

    string line;
    int lineCount = 0;
    
    // Check if file is empty
    if (file.peek() == ifstream::traits_type::eof()) {
        cerr << "Error: File is empty!" << endl;
        file.close();
        return;
    }
    
    // Skip header row if exists
    getline(file, line);
    
    // Process each line
    while (getline(file, line)) {
        lineCount++;
        
        // Skip empty lines
        if (line.empty()) {
            continue;
        }
        
        stringstream ss(line);
        Transaction transaction;
        
        // Initialize fields to empty to avoid garbage data
        transaction.customerID[0] = '\0';
        transaction.product[0] = '\0';
        transaction.category[0] = '\0';
        transaction.date[0] = '\0';
        transaction.paymentMethod[0] = '\0';
        transaction.price = 0.0;
        
        // Parse each field from the CSV
        if (getline(ss, line, ',')) {
            strncpy(transaction.customerID, line.c_str(), MAX_STRING_LENGTH - 1);
            transaction.customerID[MAX_STRING_LENGTH - 1] = '\0';
        }
        
        if (getline(ss, line, ',')) {
            strncpy(transaction.product, line.c_str(), MAX_STRING_LENGTH - 1);
            transaction.product[MAX_STRING_LENGTH - 1] = '\0';
        }
        
        if (getline(ss, line, ',')) {
            strncpy(transaction.category, line.c_str(), MAX_STRING_LENGTH - 1);
            transaction.category[MAX_STRING_LENGTH - 1] = '\0';
        }
        
        if (getline(ss, line, ',')) {
            try {
                transaction.price = stod(line);
            } catch (const exception& e) {
                cerr << "Error converting price on line " << lineCount << ": " << e.what() << endl;
                transaction.price = 0.0;
            }
        }
        
        if (getline(ss, line, ',')) {
            strncpy(transaction.date, line.c_str(), MAX_STRING_LENGTH - 1);
            transaction.date[MAX_STRING_LENGTH - 1] = '\0';
        }
        
        if (getline(ss, line)) {  // Get the rest of the line for payment method
            strncpy(transaction.paymentMethod, line.c_str(), MAX_STRING_LENGTH - 1);
            transaction.paymentMethod[MAX_STRING_LENGTH - 1] = '\0';
        }
        
        // Insert at the beginning (we will sort later if needed)
        insert(transaction);
    }

    file.close();
    cout << "Loaded " << size << " transactions from " << filename << endl;
}

// function to search for customer using customer ID
void TransactionList::searchByCustomerID(const char* customerID) {
    int choice;
    cout << "Choose search algorithm:" << endl;
    cout << "1. Linear Search" << endl;
    cout << "2. Binary Search (requires sorted data)" << endl;
    cout << "Enter choice: ";
    cin >> choice;
    
    switch (choice) {
        case 1:
            linearSearchByCustomerID(customerID);
            break;
        case 2:
            binarySearchByCustomerID(customerID);
            break;
        default:
            cout << "Invalid choice. Using Linear Search by default." << endl;
            linearSearchByCustomerID(customerID);
            break;
    }
}
//linear search by customer ID
// This function will search through the linked list for a specific customer ID

void TransactionList::linearSearchByCustomerID(const char* customerID) {
    auto start = high_resolution_clock::now();  // Start timing
    
    bool found = false;
    NodeTransaction* current = head;
    int comparisons = 0;
    
    // Create a normalized copy of the search term (trim whitespace)
    char searchTerm[MAX_STRING_LENGTH];
    strncpy(searchTerm, customerID, MAX_STRING_LENGTH);
    searchTerm[MAX_STRING_LENGTH - 1] = '\0'; // Ensure null termination
    
    // Trim leading and trailing spaces from search term
    char* start_ptr = searchTerm;
    while (*start_ptr && isspace(*start_ptr)) start_ptr++;
    
    char* end_ptr = start_ptr + strlen(start_ptr) - 1;
    while (end_ptr > start_ptr && isspace(*end_ptr)) *end_ptr-- = '\0';
    
    cout << "Searching for Customer ID: '" << start_ptr << "'" << endl;
    
    // Debug: Print the first few customer IDs to check format
    cout << "Debug - First 5 customer IDs in the list:" << endl;
    NodeTransaction* debugNode = head;
    int debugCount = 0;
    while (debugNode != nullptr && debugCount < 5) {
        cout << "'" << debugNode->data.customerID << "'" << endl;
        debugNode = debugNode->next;
        debugCount++;
    }

    while(current != nullptr) {
        comparisons++;
        
        // Create a normalized copy of the current ID (trim whitespace)
        char currentID[MAX_STRING_LENGTH];
        strncpy(currentID, current->data.customerID, MAX_STRING_LENGTH);
        currentID[MAX_STRING_LENGTH - 1] = '\0'; // Ensure null termination
        
        // Trim leading and trailing spaces
        char* c_start = currentID;
        while (*c_start && isspace(*c_start)) c_start++;
        
        char* c_end = c_start + strlen(c_start) - 1;
        while (c_end > c_start && isspace(*c_end)) *c_end-- = '\0';
        
        // Case-insensitive comparison for better matching
        if (strcasecmp(c_start, start_ptr) == 0) {
            found = true;

            cout << "--------------------------------------" << endl;
            cout << "Customer ID       : " << current->data.customerID << endl;
            cout << "Product           : " << current->data.product << endl;
            cout << "Category          : " << current->data.category << endl;
            cout << "Price             : " << fixed << setprecision(2) << current->data.price << endl;
            cout << "Date              : " << current->data.date << endl;
            cout << "Payment Method    : " << current->data.paymentMethod << endl;
            cout << "--------------------------------------" << endl;
        }
        current = current->next;
    }

    auto end = high_resolution_clock::now();  // End timing
    auto duration = duration_cast<microseconds>(end - start);

    if(!found) {
        cout << "Customer ID NOT found." << endl;
        cout << "Try the following suggestions:" << endl;
        cout << "1. Check for typos in the customer ID" << endl;
        cout << "2. Make sure the data file contains this customer" << endl;
        cout << "3. Check if the customer ID is case-sensitive" << endl;
    }
    
    cout << "Linear Search completed." << endl;
    cout << "Comparisons: " << comparisons << endl;
    cout << "Time taken: " << duration.count() << " microseconds." << endl;
}

//Sort linked list by customer ID (required for binary search)
void TransactionList::sortByCustomerID(){
    if (head == nullptr || head->next == nullptr)
        return; // 0 or 1 node - already sorted

    auto start = high_resolution_clock::now(); // start timer

    bool swapped;
    NodeTransaction* ptr1;
    NodeTransaction* lptr = nullptr;

    do {
        swapped = false;
        ptr1 = head;

        while(ptr1->next != lptr) {
            if (strcmp(ptr1->data.customerID, ptr1->next->data.customerID) > 0) {
                // Swap data (not pointers)
                Transaction temp = ptr1->data;
                ptr1->data = ptr1->next->data;
                ptr1->next->data = temp;
                swapped = true;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1; 
    } while (swapped);

    auto end = high_resolution_clock::now(); // end timer
    auto duration = duration_cast<microseconds>(end - start);

    cout << "Sorted Customer IDs using Bubble Sort" << endl;
    cout << "Time taken: " << duration.count() << " microseconds." << endl;
}

// Binary Search by Customer ID
// Sorting is required before binary search
void TransactionList::binarySearchByCustomerID(const char* customerID){
    //check if the list is sorted
    NodeTransaction* prev = head;
    NodeTransaction* current = head ? head->next : nullptr;
    bool isSorted = true;

    while (current != nullptr) {
        if (strcmp(prev->data.customerID, current->data.customerID) > 0){
            isSorted = false;
            break;
        }
        prev = current;
        current = current->next;
    }

    if (!isSorted) {
        cout << "List is not sorted. Please sort the list before searching." << endl;
        sortByCustomerID();
    }

    auto start = high_resolution_clock::now();  // Start timer

    //convert list to array for binary search
    Transaction* arr = new Transaction[size];
    NodeTransaction* node = head;
    int index = 0;

    while (node != nullptr){
        arr[index++] = node->data;
        node = node->next;
    }

    // Binary search implementation
    int left = 0;
    int right = size -1;
    bool found = false;
    int comparisons = 0;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        comparisons++;
        
        int comparison = strcmp(arr[mid].customerID, customerID);
        
        if (comparison == 0) {
            found = true;
            
            cout << "--------------------------------------" << endl;
            cout << "Customer ID       : " << arr[mid].customerID << endl;
            cout << "Product           : " << arr[mid].product << endl;
            cout << "Category          : " << arr[mid].category << endl;
            cout << "Price             : " << fixed << setprecision(2) << arr[mid].price << endl;
            cout << "Date              : " << arr[mid].date << endl;
            cout << "Payment Method    : " << arr[mid].paymentMethod << endl;
            cout << "--------------------------------------" << endl;
            
            // Check for other instances with the same customer ID
            // Check left
            int temp = mid - 1;
            while (temp >= 0 && strcmp(arr[temp].customerID, customerID) == 0) {
                comparisons++;
                cout << "--------------------------------------" << endl;
                cout << "Customer ID       : " << arr[temp].customerID << endl;
                cout << "Product           : " << arr[temp].product << endl;
                cout << "Category          : " << arr[temp].category << endl;
                cout << "Price             : " << fixed << setprecision(2) << arr[temp].price << endl;
                cout << "Date              : " << arr[temp].date << endl;
                cout << "Payment Method    : " << arr[temp].paymentMethod << endl;
                cout << "--------------------------------------" << endl;
                temp--;
            }
            
            // Check right
            temp = mid + 1;
            while (temp < size && strcmp(arr[temp].customerID, customerID) == 0) {
                comparisons++;
                cout << "--------------------------------------" << endl;
                cout << "Customer ID       : " << arr[temp].customerID << endl;
                cout << "Product           : " << arr[temp].product << endl;
                cout << "Category          : " << arr[temp].category << endl;
                cout << "Price             : " << fixed << setprecision(2) << arr[temp].price << endl;
                cout << "Date              : " << arr[temp].date << endl;
                cout << "Payment Method    : " << arr[temp].paymentMethod << endl;
                cout << "--------------------------------------" << endl;
                temp++;
            }
            
            break;
        } else if (comparison < 0) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    
    delete[] arr;  // Free memory
    
    auto end = high_resolution_clock::now();  // End timer
    auto duration = duration_cast<microseconds>(end - start);
    
    if (!found) {
        cout << "Customer ID NOT found." << endl;
    }
    
    cout << "Binary Search completed." << endl;
    cout << "Comparisons: " << comparisons << endl;
    cout << "Time taken: " << duration.count() << " microseconds." << endl;
}

//compare search algo
// Add this function to linkedlist.cpp to compare search algorithms

void TransactionList::compareSearchAlgorithms(const char* customerID) {
    if (size == 0) {
        cout << "No transactions to search, please check your data" << endl;
        return;
    }
    
    cout << "\n===== LINKED LIST SEARCH ALGORITHM COMPARISON =====\n";
    
    // Measure linear search
    auto start1 = high_resolution_clock::now();
    bool found1 = false;
    NodeTransaction* current = head;
    int linearComparisons = 0;
    
    while(current != nullptr) {
        linearComparisons++;
        if(strcmp(current->data.customerID, customerID) == 0) {
            found1 = true;
            // In real search we would display the transaction details here
            // but for timing purposes, we just set the found flag
        }
        current = current->next;
    }
    auto end1 = high_resolution_clock::now();
    auto duration1 = duration_cast<microseconds>(end1 - start1);
    
    // Make sure the list is sorted for binary search
    NodeTransaction* prev = head;
    NodeTransaction* curr = head ? head->next : nullptr;
    bool isSorted = true;
    
    while (curr != nullptr) {
        if (strcmp(prev->data.customerID, curr->data.customerID) > 0) {
            isSorted = false;
            break;
        }
        prev = curr;
        curr = curr->next;
    }
    
    if (!isSorted) {
        cout << "Sorting list by Customer ID for binary search...\n";
        sortByCustomerID();
    }
    
    // Measure binary search
    auto start2 = high_resolution_clock::now();
    
    // Convert linked list to array for binary search
    Transaction* arr = new Transaction[size];
    NodeTransaction* node = head;
    int index = 0;
    
    while (node != nullptr) {
        arr[index++] = node->data;
        node = node->next;
    }
    
    // Binary search implementation
    int left = 0;
    int right = size - 1;
    bool found2 = false;
    int binaryComparisons = 0;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        binaryComparisons++;
        
        int comparison = strcmp(arr[mid].customerID, customerID);
        
        if (comparison == 0) {
            found2 = true;
            // In real search we would display the transaction here
            break;
        } else if (comparison < 0) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    
    delete[] arr;  // Free memory
    
    auto end2 = high_resolution_clock::now();
    auto duration2 = duration_cast<microseconds>(end2 - start2);
    
    // Display results
    cout << "\n===== SEARCH RESULTS =====\n";
    cout << "Searching for Customer ID: " << customerID << endl;
    cout << "Customer found: " << (found1 ? "Yes" : "No") << endl << endl;
    
    cout << "Linear Search:\n";
    cout << "- Comparisons: " << linearComparisons << endl;
    cout << "- Time taken: " << duration1.count() << " microseconds\n\n";
    
    cout << "Binary Search:\n";
    cout << "- Comparisons: " << binaryComparisons << endl;
    cout << "- Time taken: " << duration2.count() << " microseconds\n\n";
    
    if (duration1.count() < duration2.count()) {
        cout << "For this dataset and search key, Linear Search was faster.\n";
    } else if (duration2.count() < duration1.count()) {
        cout << "For this dataset and search key, Binary Search was faster.\n";
    } else {
        cout << "Both algorithms performed equally fast for this search.\n";
    }
}

// filter transactions based on multiple criteria (e.g product, category, price)
void TransactionList::filterTransactions(){
    char productFilter[MAX_STRING_LENGTH], categoryFilter[MAX_STRING_LENGTH], paymentMethodFilter[MAX_STRING_LENGTH];
    char startDate[MAX_STRING_LENGTH], endDate[MAX_STRING_LENGTH];
    double minPrice, maxPrice;

    cout << "Enter product to filter (use '-' for no filter): ";
    cin >> productFilter;
    cout << "Enter category to filter (- for no filter): ";
    cin >> categoryFilter;
    cout << "Enter min price (-1 for no filter): ";
    cin >> minPrice;
    cout << "Enter max price (-1 for no filter): ";
    cin >> maxPrice;
    
    // Date range filtering
    cout << "Enter start date (- for no filter): ";
    cin >> startDate;
    cout << "Enter end date (- for no filter): ";
    cin >> endDate;

    cout << "Enter payment method to filter (- for no filter): ";
    cin.ignore();
    cin.getline(paymentMethodFilter, MAX_STRING_LENGTH);

    // trim payment method filter
    char* pmt = paymentMethodFilter;
    while (isspace(*pmt)) pmt++;
    char* end = pmt + strlen(pmt) - 1;
    while (end > pmt && isspace(*end)) *end-- = '\0';

    int matchCount = 0;
    NodeTransaction* current = head;
    
    cout << "\nMatching Transactions:\n";
    cout << "-------------------------------------------------------------------------------------------\n";
    cout << "| Customer ID |     Product     |   Category   |  Price  |     Date     |   Payment Method  |\n";
    cout << "-------------------------------------------------------------------------------------------\n";
    
    while (current != nullptr) {
        // Trim payment method in current node
        char paymentMethod[MAX_STRING_LENGTH];
        strcpy(paymentMethod, current->data.paymentMethod);
        char* p = paymentMethod;
        while (isspace(*p)) p++;
        char* e = p + strlen(p) - 1;
        while (e > p && isspace(*e)) *e-- = '\0';
        
        bool matchesProduct = (strcmp(productFilter, "-") == 0 || 
                              strcmp(current->data.product, productFilter) == 0);
        bool matchesCategory = (strcmp(categoryFilter, "-") == 0 || 
                               strcmp(current->data.category, categoryFilter) == 0);
        bool matchesPrice = (minPrice == -1 || (current->data.price >= minPrice && 
                             (maxPrice == -1 || current->data.price <= maxPrice)));
        bool matchesPaymentMethod = (strcmp(paymentMethodFilter, "-") == 0 || 
                                    strcmp(p, pmt) == 0);
        bool matchesDate = isDateInRange(current->data.date, startDate, endDate);

        if (matchesProduct && matchesCategory && matchesPrice && matchesDate && matchesPaymentMethod) {
            matchCount++;
            cout << "| " << setw(11) << current->data.customerID
                 << " | " << setw(14) << current->data.product
                 << " | " << setw(12) << current->data.category
                 << " | " << setw(7) << fixed << setprecision(2) << current->data.price
                 << " | " << setw(12) << current->data.date
                 << " | " << setw(16) << current->data.paymentMethod << " |\n";
        }
        
        current = current->next;
    }
    
    cout << "-------------------------------------------------------------------------------------------\n";
    cout << "\nTotal: " << matchCount << " transactions match the filters.\n";

}

// Bubble sort implementation for linked list
void TransactionList::bubbleSort() {
    if (head == nullptr || head->next == nullptr)
        return; // 0 or 1 node - already sorted
    
    bool swapped;
    NodeTransaction* ptr1;
    NodeTransaction* lptr = nullptr;
    
    auto start = high_resolution_clock::now();  // Start timing
    
    do {
        swapped = false;
        ptr1 = head;
        
        while (ptr1->next != lptr) {
            if (compareDates(ptr1->data.date, ptr1->next->data.date) > 0) {
                // Swap data (not pointers)
                Transaction temp = ptr1->data;
                ptr1->data = ptr1->next->data;
                ptr1->next->data = temp;
                swapped = true;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
    
    auto end = high_resolution_clock::now();  // End timing
    auto duration = duration_cast<microseconds>(end - start);
    
    cout << "Sorted using Bubble Sort (Linked List)." << endl;
    cout << "Time taken: " << duration.count() << " microseconds." << endl;
}

// Insertion sort implementation for linked list
void TransactionList::insertionSort() {
    if (head == nullptr || head->next == nullptr)
        return; // 0 or 1 node - already sorted
    
    auto start = high_resolution_clock::now();  // Start timing
    
    NodeTransaction* sorted = nullptr;
    NodeTransaction* current = head;
    
    while (current != nullptr) {
        NodeTransaction* next = current->next;
        
        if (sorted == nullptr || compareDates(current->data.date, sorted->data.date) < 0) {
            // Insert at beginning of sorted list
            current->next = sorted;
            sorted = current;
        } else {
            // Find position to insert
            NodeTransaction* temp = sorted;
            while (temp->next != nullptr && compareDates(temp->next->data.date, current->data.date) <= 0) {
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }
        
        current = next;
    }
    
    head = sorted;
    
    auto end = high_resolution_clock::now();  // End timing
    auto duration = duration_cast<microseconds>(end - start);
    
    cout << "Sorted using Insertion Sort (Linked List)." << endl;
    cout << "Time taken: " << duration.count() << " microseconds." << endl;
}

// Helper function for merge sort
NodeTransaction* merge(NodeTransaction* left, NodeTransaction* right) {
    NodeTransaction dummy;
    NodeTransaction* tail = &dummy;
    dummy.next = nullptr;
    
    // Merge until one list is empty
    while (left != nullptr && right != nullptr) {
        if (compareDates(left->data.date, right->data.date) <= 0) {
            tail->next = left;
            left = left->next;
        } else {
            tail->next = right;
            right = right->next;
        }
        tail = tail->next;
    }
    
    // Attach remaining nodes
    tail->next = (left != nullptr) ? left : right;
    
    return dummy.next;
}

// Helper function for merge sort
NodeTransaction* getMidNode(NodeTransaction* head) {
    if (head == nullptr) return nullptr;
    
    NodeTransaction* slow = head;
    NodeTransaction* fast = head->next;
    
    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;
    }
    
    return slow;
}

// Helper function for merge sort
NodeTransaction* mergeSortHelper(NodeTransaction* head) {
    // Base case: 0 or 1 node
    if (head == nullptr || head->next == nullptr) {
        return head;
    }
    
    // Find middle of the list
    NodeTransaction* mid = getMidNode(head);
    NodeTransaction* right = mid->next;
    mid->next = nullptr;  // Split the list
    
    // Recursively sort both halves
    NodeTransaction* left_sorted = mergeSortHelper(head);
    NodeTransaction* right_sorted = mergeSortHelper(right);
    
    // Merge the sorted halves
    return merge(left_sorted, right_sorted);
}

// Merge sort implementation
void TransactionList::mergeSort() {
    if (head == nullptr || head->next == nullptr)
        return; // 0 or 1 node - already sorted
    
    auto start = high_resolution_clock::now();  // Start timing
    
    head = mergeSortHelper(head);
    
    auto end = high_resolution_clock::now();  // End timing
    auto duration = duration_cast<microseconds>(end - start);
    
    cout << "Sorted using Merge Sort (Linked List)." << endl;
    cout << "Time taken: " << duration.count() << " microseconds." << endl;
}

// Add a function to choose and execute a sorting algorithm
void TransactionList::sortByDate() {
    int choice;
    cout << "Choose sorting algorithm:" << endl;
    cout << "1. Bubble Sort" << endl;
    cout << "2. Insertion Sort" << endl;
    cout << "3. Merge Sort" << endl;
    cout << "Enter choice: ";
    cin >> choice;
    
    switch (choice) {
        case 1:
            bubbleSort();
            break;
        case 2:
            insertionSort();
            break;
        case 3:
            mergeSort();
            break;
        default:
            cout << "Invalid choice. No sorting applied." << endl;
            return;
    }
    
    cout << "Transactions sorted by date." << endl;
    // Uncomment if you want to display the sorted list immediately
    // display();
}

//ReviewList constructor
ReviewList::ReviewList() : head(nullptr), size(0){}

//ReviewList destructor
ReviewList::~ReviewList(){
    clear();
}

// clear the review list memory
void ReviewList::clear(){
    NodeReview* current = head;
    while (current != nullptr){
        NodeReview* temp = current;
        current = current->next;
        delete temp;

    }
    head = nullptr;
    size = 0;
}

// insert a review at the begining of the list 
void ReviewList::insert(CustomerReview review) {
    NodeReview* newnode = new NodeReview;
    newnode->data = review;
    newnode->next = head;
    head = newnode;
    size++;
}

//Display all reviews
void ReviewList::display() {
    NodeReview* current = head;
    cout << "-------------------------------------------------------------------\n";
    cout << "| Product ID | Customer ID | Rating |          Review Text         |\n";
    cout << "-------------------------------------------------------------------\n";
    
    while (current != nullptr) {
        cout << "| " << setw(10) << current->data.productID
             << " | " << setw(11) << current->data.customerID
             << " | " << setw(6) << current->data.rating
             << " | " << setw(30) << current->data.reviewText << " |\n";
        current = current->next;
    }
    cout << "-------------------------------------------------------------------\n";
}

//get size of review list
int ReviewList::getSize() const {
    return size;
}
// get head of the review list
NodeReview* ReviewList::getHead() const {
    return head;
}

// load reviews from CSV
void ReviewList::loadFromCSV(const char* filename){
    ifstream file(filename);
    if(!file.is_open()){
        cerr << "Error: Unable to open file " << filename << endl;
        return;
    }
    string line;
    getline(file, line); //skip header row

    while(getline(file, line)){
        CustomerReview review;
        stringstream ss(line);
        
        ss.getline(review.productID, MAX_STRING_LENGTH, ',');
        ss.getline(review.customerID, MAX_STRING_LENGTH, ',');
        
        // Read rating as string then convert to int
        string ratingStr;
        getline(ss, ratingStr, ',');
        review.rating = stoi(ratingStr);
        
        // Get the rest as review text
        ss.getline(review.reviewText, MAX_STRING_LENGTH, ',');
        
        insert(review);
    }
    file.close();
    std::cout << "loaded" << size << "reviews from " << filename << endl;
}

//processs reviews to find the most common word
void ReviewList::processReviews(){
    int chosenRating;
    cout << "Enter rating to analyze (1-5): ";
    cin >> chosenRating;

    if(chosenRating < 1 || chosenRating > 5){
        cout << "Invalid input. Please enter a rating between 1 and 5./n";
        return;
    }
    
    // Linked list for word frequency
    NodeWordFreq* wordFreqHead = nullptr;

    //process review using chosen rating
    NodeReview* current = head;

    while (current != nullptr){
        if(current->data.rating == chosenRating){
            string reviewText = current->data.reviewText;
            stringstream ss(reviewText);
            string word;

            //extract words from review text
            while(ss >> word){
                cleanWord(word); //clean word
                if(word.empty()) continue; //skip empty words

                //check if word already exist in the linked list
                NodeWordFreq* wordNode = wordFreqHead;
                bool found = false;

                while(wordNode != nullptr){
                    if(strcmp(wordNode->word, word.c_str()) == 0){
                        wordNode->frequency++;
                        found = true;
                        break;
                    }
                    wordNode = wordNode->next;
                }
                //if word not found, create a new ndoe
                if(!found){
                    NodeWordFreq* newWordNode = new NodeWordFreq;
                    strcpy(newWordNode->word, word.c_str());
                    newWordNode->frequency = 1;
                    newWordNode->next = wordFreqHead;
                    wordFreqHead = newWordNode;
                }
            }
        }
        current = current->next;

    }
    // sort words by frequency (using bubble sort)
    if (wordFreqHead != nullptr && wordFreqHead->next != nullptr) {
        bool swapped;
        NodeWordFreq* ptr1;
        NodeWordFreq* lptr = nullptr;
        
        do {
            swapped = false;
            ptr1 = wordFreqHead;
            
            while (ptr1->next != lptr) {
                if (ptr1->frequency < ptr1->next->frequency) {
                    // Swap data
                    char tempWord[MAX_STRING_LENGTH];
                    strcpy(tempWord, ptr1->word);
                    int tempFreq = ptr1->frequency;
                    
                    strcpy(ptr1->word, ptr1->next->word);
                    ptr1->frequency = ptr1->next->frequency;
                    
                    strcpy(ptr1->next->word, tempWord);
                    ptr1->next->frequency = tempFreq;
                    
                    swapped = true;
                }
                ptr1 = ptr1->next;
            }
            lptr = ptr1;
        } while (swapped);
    }

    // Display top word
    cout << "\nTop 10 words in " << chosenRating << "-star reviews:\n";
    cout << "-------------------------------\n";
    cout << "|   Word      |   Frequency   |\n";
    cout << "-------------------------------\n";
    
    NodeWordFreq* wordNode = wordFreqHead;
    int count = 0;
    
    while (wordNode != nullptr && count < 10) {
        cout << "| " << setw(12) << left << wordNode->word
             << "| " << setw(13) << wordNode->frequency << " |\n";
        wordNode = wordNode->next;
        count++;
    }
    cout << "-------------------------------\n";

    // clean word frequency list
    while(wordFreqHead != nullptr){
        NodeWordFreq* temp = wordFreqHead;
        wordFreqHead = wordFreqHead->next;
        delete temp;
    }
}


// sorting performance measurement
// 3 algorithms are used to compare the performance
// insertion sort, bubble sort and merge sort

void TransactionList::measureSortingPerformance(){
    if (size == 0) {
        cout << "No transactions to sort, please check your data" << endl;
        return;
    }

    // Create 3 copies of the list to test each algorithm
    TransactionList bubbleList, insertionList, mergeList;

    //copt current list to test lists
    NodeTransaction** nodesArray = new NodeTransaction*[size];
    NodeTransaction* current = head;
    int i = 0;
    while (current != nullptr) {
        nodesArray[i++] = current;
        current = current->next;
    }

    // Populate the lists in reverse order of the array
    for (int j = size - 1; j >= 0; j--) {
        bubbleList.insert(nodesArray[j]->data);
        insertionList.insert(nodesArray[j]->data);
        mergeList.insert(nodesArray[j]->data);
    }

    delete[] nodesArray; // Free the array of pointers

    cout << "Measuring sorting performance for " << size << " transactions:" << endl;
    cout << "------------------------------------------------------" << endl;

    // Measure bubble sort
    auto start1 = high_resolution_clock::now();
    bubbleList.bubbleSort();
    auto end1 = high_resolution_clock::now();
    auto duration1 = duration_cast<microseconds>(end1 - start1);

    // measur insetion sort
    auto start2 = high_resolution_clock::now();
    insertionList.insertionSort(); 
    auto end2 = high_resolution_clock::now();
    auto duration2 = duration_cast<microseconds>(end2 - start2);

    //measure merge sort
    auto start3 = high_resolution_clock::now();
    mergeList.mergeSort();
    auto end3 = high_resolution_clock::now();
    auto duration3 = duration_cast<microseconds>(end3 - start3);

    // resutls
    cout << "Bubble Sort:    " << duration1.count() << " microseconds" << endl;
    cout << "Insertion Sort: " << duration2.count() << " microseconds" << endl;
    cout << "Merge Sort:     " << duration3.count() << " microseconds" << endl;
    cout << "------------------------------------------------------" << endl;

    // Show the best method
    if(duration1.count() <= duration2.count() && duration1.count() <= duration3.count()){
        cout << "Bubble Sort was fastest for this dataset." << endl;
    } else if (duration2.count() <= duration1.count() && duration2.count() <= duration3.count()){
        cout << "Insertion Sort was fastest for this dataset." << endl;
    } else {
        cout << "Merge Sort was fastest for this dataset." << endl;
    }
}
// End of linkedlist.cpp
// testing


// DEBUG FUNCTIONS
// Function to check if a customer ID exists in the list
bool TransactionList::hasCustomerID(const char* customerID) {
    NodeTransaction* current = head;
    int count = 0;
    
    cout << "\nChecking availability of customer IDs:" << endl;
    
    while (current != nullptr && count < 50) {  // Check first 50 for debugging
        if (strlen(current->data.customerID) > 0) {
            cout << "Found customer ID: " << current->data.customerID << endl;
            
            // Compare ignoring case
            if (strcasecmp(current->data.customerID, customerID) == 0) {
                cout << "Match found for " << customerID << "!" << endl;
                return true;
            }
        } else {
            cout << "Found empty customer ID at position " << count << endl;
        }
        
        current = current->next;
        count++;
    }
    
    cout << "Checked " << count << " records. Customer ID " << customerID << " not found in sample." << endl;
    return false;
}

// Function to inspect the CSV file format
void TransactionList::inspectCSVFormat(const char* filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return;
    }
    
    string line;
    int lineCount = 0;
    
    cout << "\n======= CSV File Inspection =======" << endl;
    
    // Get first line (likely header)
    if (getline(file, line)) {
        cout << "Header row: " << line << endl;
        cout << "Fields count: " << count(line.begin(), line.end(), ',') + 1 << endl;
    }
    
    // Check a few data lines
    cout << "\nSample data rows:" << endl;
    while (getline(file, line) && lineCount < 5) {
        lineCount++;
        cout << "Line " << lineCount << ": " << line << endl;
        cout << "Fields: " << count(line.begin(), line.end(), ',') + 1 << endl;
    }
    
    file.close();
    cout << "======= End of Inspection =======" << endl;
}







