#include "linkedlist.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono> // for performance measurement
#include <iomanip> // for formatting

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
void TransactionList::loadFromCSV (const char* filename){
    ifstream file(filename);
    if(!file.is_open()){
        cerr << "Error: Unable to open file " << filename << endl;
        return;
    }

    string line;
    // Read from CSV file line by line
    while(getline(file, line)){
        Transaction transaction;
        stringstream ss;

        //Parse each field from the CSV
        ss.getline(transaction.customerID, MAX_STRING_LENGTH, ',');
        ss.getline(transaction.product, MAX_STRING_LENGTH, ',');
        ss.getline(transaction.category, MAX_STRING_LENGTH, ',');
        ss >> transaction.price;
        ss.ignore();  // Ignore comma
        ss.getline(transaction.date, MAX_STRING_LENGTH, ',');
        ss.getline(transaction.paymentMethod, MAX_STRING_LENGTH, ',');
        
        // Insert at the beginning (we will sort later if needed)
        insert(transaction);

    }

    file.close();
    cout << "Loaded " << size << " transactions from " << filename << endl;

}

// function to search for customer using customer ID
void TransactionList::searchByCustomerID(const char* customerID){
    bool found = false;
    NodeTransaction* current = head;

    while(current != nullptr){
        if(strcmp(current->data.customerID, customerID) == 0){
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

    if(!found){
        cout << "Customer ID NOT found." << endl;
    }
}

// Helper function to parse data (same as in array.cpp)
Date parseDate(const char* dateStr){
    Date date;
    sscanf(dateStr, "%d/%d/%d", &date.day, &date.month, &date.year);
    return date;
}

// Helper function to compare two dates
int compareDates(const char* date1, const char* date2){
    Date d1 = parseDate(date1);
    Date d2 = parseDate(date2);

    //compare first year, then month, then day
    if(d1.year != d2.year) return d1.year - d2.year;
    if(d1.month != d2.month) return d1.month - d2.month;
    return d1.day - d2.day;
}

// helper function check if date is within a range
bool isDateInRange(const char* transactionDate, const char* startDate, const char* endDate){
    if(strcmp(startDate, "-") == 0 || strcmp(endDate, "-") == 0) return true;

    //check if date is between start and end dates
    return compareDates(transactionDate, startDate) >= 0 && 
    compareDates(transactionDate, endDate) <= 0;
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

// Add these to your linkedlist.cpp file

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

// Performance measurement function
void TransactionList::measureSortingPerformance() {
    // Create three copies of the current list for testing
    TransactionList bubbleList, insertionList, mergeList;
    
    // Copy current list to test lists
    NodeTransaction* current = head;
    while (current != nullptr) {
        bubbleList.insert(current->data);
        insertionList.insert(current->data);
        mergeList.insert(current->data);
        current = current->next;
    }
    
    cout << "Measuring sorting performance for " << getSize() << " transactions:" << endl;
    cout << "------------------------------------------------------" << endl;
    
    // Measure Bubble Sort
    auto start1 = high_resolution_clock::now();
    bubbleList.bubbleSort();
    auto end1 = high_resolution_clock::now();
    auto duration1 = duration_cast<microseconds>(end1 - start1);
    
    // Measure Insertion Sort
    auto start2 = high_resolution_clock::now();
    insertionList.insertionSort();
    auto end2 = high_resolution_clock::now();
    auto duration2 = duration_cast<microseconds>(end2 - start2);
    
    // Measure Merge Sort
    auto start3 = high_resolution_clock::now();
    mergeList.mergeSort();
    auto end3 = high_resolution_clock::now();
    auto duration3 = duration_cast<microseconds>(end3 - start3);
    
    // Report results
    cout << "Bubble Sort:    " << duration1.count() << " microseconds" << endl;
    cout << "Insertion Sort: " << duration2.count() << " microseconds" << endl;
    cout << "Merge Sort:     " << duration3.count() << " microseconds" << endl;
    cout << "------------------------------------------------------" << endl;
    
    // Show the most efficient method
    if (duration1.count() <= duration2.count() && duration1.count() <= duration3.count()) {
        cout << "Bubble Sort was fastest for this dataset." << endl;
    } else if (duration2.count() <= duration1.count() && duration2.count() <= duration3.count()) {
        cout << "Insertion Sort was fastest for this dataset." << endl;
    } else {
        cout << "Merge Sort was fastest for this dataset." << endl;
    }
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

//clean word - remove punctuation and convert to lowercase
void cleanWord(string& word){
    //convert to lowercase
    for (int i =0; i < word.length(); i++){
        word[i] = tolower(word[i]);
    }
    //remove punctuation
    string result;
    for (int i = 0; i < word.length(); i++){
        if(!ispunct(word[i])){
            result += word[i];
        }
        word = result;
    }
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

// Display menu for linked list operations
// Add this function to linkedlist.cpp

// Display menu for linked list operations
void displayLinkedListMenu() {
    cout << "\n===== LINKED LIST OPERATIONS =====\n";
    cout << "1. Search by Customer ID\n";
    cout << "2. Filter Transactions\n";
    cout << "3. Sort Transactions by Date\n";
    cout << "4. Analyze Reviews\n";
    cout << "5. Compare Sorting Performance\n";
    cout << "6. Return to Main Menu\n";
    cout << "Enter your choice: ";
}












