#include <iostream>
#include <fstream>
#include "array.hpp"

using namespace std;
// Linked-List Implementation

// Transaction node structure for linked list
struct NodeTransaction 
{
    Transaction data;
    NodeTransaction* next;
};
// Review node structure for linked list
struct NodeReview 
{
    CustomerReview data;
    NodeReview* next;
};

// Linked list class for transactions
class TransactionList 
{
public:
    NodeTransaction* head;
    TransactionList() : head(nullptr) {}

    void insert(Transaction transaction) 
    {
        NodeTransaction* newNode = new NodeTransaction;
        newNode->data = transaction;
        newNode->next = head;
        head = newNode;
    }

    void display() 
    {
        NodeTransaction* current = head;
        while (current != nullptr) 
        {
            cout << "Customer ID: " << current->data.customerID << endl;
            cout << "Product: " << current->data.product << endl;
            cout << "Category: " << current->data.category << endl;
            cout << "Price: " << current->data.price << endl;
            cout << "Date: " << current->data.date << endl;
            cout << "Payment Method: " << current->data.paymentMethod << endl;
            cout << "-------------------------" << endl;
            current = current->next;
        }
    }
}; 
// Linked List class for reviews 
class ReviewList
{
public:
    NodeReview* head;
    ReviewList() : head(nullptr) {}

    void insert(CustomerReview review) 
    {
        NodeReview* newNode = new NodeReview;
        newNode->data = review;
        newNode->next = head;
        head = newNode;
    }

    void display() 
    {
        NodeReview* current = head;
        while (current != nullptr) 
        {
            cout << "Product ID: " << current->data.productID << endl;
            cout << "Customer ID: " << current->data.customerID << endl;
            cout << "Rating: " << current->data.rating << endl;
            cout << "Review Text: " << current->data.reviewText << endl;
            cout << "-------------------------" << endl;
            current = current->next;
        }
    }
    void clear() 
    {
        NodeReview* current = head;
        while (current != nullptr) 
        {
            NodeReview* temp = current;
            current = current->next;
            delete temp;
        }
        head = nullptr;
    }
};

#include "linkedlist.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <iomanip>

using namespace std;
using namespace std::chrono;

// TransactionList implementation
TransactionList::TransactionList() : head(nullptr), size(0) {}

TransactionList::~TransactionList() {
    clear();
}

void TransactionList::clear() {
    NodeTransaction* current = head;
    while (current != nullptr) {
        NodeTransaction* temp = current;
        current = current->next;
        delete temp;
    }
    head = nullptr;
    size = 0;
}

void TransactionList::insert(Transaction transaction) {
    NodeTransaction* newNode = new NodeTransaction;
    newNode->data = transaction;
    newNode->next = head;
    head = newNode;
    size++;
}

void TransactionList::display() {
    NodeTransaction* current = head;
    cout << "-------------------------------------------------------------------------------------------\n";
    cout << "|     Date     | Customer ID |     Product     |   Category   |  Price  |   Payment Method  |\n";
    cout << "-------------------------------------------------------------------------------------------\n";
    
    while (current != nullptr) {
        cout << setw(11) << current->data.date
             << " | " << setw(11) << current->data.customerID
             << " | " << setw(14) << current->data.product
             << " | " << setw(15) << current->data.category
             << " | " << setw(7) << fixed << setprecision(2) << current->data.price
             << " | " << setw(16) << current->data.paymentMethod << "\n";
        current = current->next;
    }
    cout << "-------------------------------------------------------------------------------------------\n";
}

int TransactionList::getSize() const {
    return size;
}

NodeTransaction* TransactionList::getHead() const {
    return head;
}

void TransactionList::loadFromCSV(const char* filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return;
    }

    string line;
    // Read through the CSV
    while (getline(file, line)) {
        Transaction transaction;
        stringstream ss(line);
        
        ss.getline(transaction.customerID, MAX_STRING_LENGTH, ',');
        ss.getline(transaction.product, MAX_STRING_LENGTH, ',');
        ss.getline(transaction.category, MAX_STRING_LENGTH, ',');
        ss >> transaction.price;
        ss.ignore();  // Ignore comma
        ss.getline(transaction.date, MAX_STRING_LENGTH, ',');
        ss.getline(transaction.paymentMethod, MAX_STRING_LENGTH, ',');
        
        insert(transaction); // Insert at beginning (we can sort later)
    }
    
    file.close();
    cout << "Loaded " << size << " transactions into linked list." << endl;
}

void TransactionList::searchByCustomerID(const char* customerID) {
    bool found = false;
    NodeTransaction* current = head;
    
    while (current != nullptr) {
        if (strcmp(current->data.customerID, customerID) == 0) {
            found = true;
            cout << "--------------------------------------" << endl;
            cout << "Customer ID       : " << current->data.customerID << endl;
            cout << "Product           : " << current->data.product << endl;
            cout << "Category          : " << current->data.category << endl;
            cout << "Price             : " << current->data.price << endl;
            cout << "Date              : " << current->data.date << endl;
            cout << "Payment Method    : " << current->data.paymentMethod << endl;
            cout << "--------------------------------------" << endl;
        }
        current = current->next;
    }
    
    if (!found) {
        cout << "Customer ID not found." << endl;
    }
}

void TransactionList::filterTransactions() {
    char productFilter[MAX_STRING_LENGTH], categoryFilter[MAX_STRING_LENGTH], paymentMethodFilter[MAX_STRING_LENGTH];
    char startDate[MAX_STRING_LENGTH], endDate[MAX_STRING_LENGTH];
    double minPrice, maxPrice;
    
    cout << "Enter product to filter (- for no filter): ";
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

    // Trim payment method filter
    char* pmt = paymentMethodFilter;
    while (isspace(*pmt)) pmt++;
    char* end = pmt + strlen(pmt) - 1;
    while (end > pmt && isspace(*end)) *end-- = '\0';

    int matchCount = 0;
    NodeTransaction* current = head;
    
    cout << "\nMatching Transactions:\n";
    cout << "-------------------------------------------------------------------------------------------\n";
    
    while (current != nullptr) {
        // Trim payment method in current node
        char paymentMethod[MAX_STRING_LENGTH];
        strcpy(paymentMethod, current->data.paymentMethod);
        char* p = paymentMethod;
        while (isspace(*p)) p++;
        char* e = p + strlen(p) - 1;
        while (e > p && isspace(*e)) *e-- = '\0';
        
        bool matchesProduct = (strcmp(productFilter, "-") == 0 || strcmp(current->data.product, productFilter) == 0);
        bool matchesCategory = (strcmp(categoryFilter, "-") == 0 || strcmp(current->data.category, categoryFilter) == 0);
        bool matchesPrice = (minPrice == -1 || (current->data.price >= minPrice && 
                             (maxPrice == -1 || current->data.price <= maxPrice)));
        bool matchesPaymentMethod = (strcmp(paymentMethodFilter, "-") == 0 || strcmp(p, pmt) == 0);
        bool matchesDate = isDateInRange(current->data.date, startDate, endDate);

        if (matchesProduct && matchesCategory && matchesPrice && matchesDate && matchesPaymentMethod) {
            matchCount++;
            cout << current->data.customerID << " | " 
                 << current->data.product << " | "
                 << current->data.category << " | " 
                 << current->data.price << " | "
                 << current->data.date << " | " 
                 << current->data.paymentMethod << endl;
        }
        
        current = current->next;
    }

    cout << "\nTotal: " << matchCount << " transactions match the filters.\n";
}

// Bubble sort implementation for linked list
void TransactionList::bubbleSort() {
    if (head == nullptr || head->next == nullptr)
        return; // 0 or 1 node - already sorted
    
    bool swapped;
    NodeTransaction* ptr1;
    NodeTransaction* lptr = nullptr;
    
    do {
        swapped = false;
        ptr1 = head;
        
        while (ptr1->next != lptr) {
            // Manual date comparison
            Date date1 = parseDate(ptr1->data.date);
            Date date2 = parseDate(ptr1->next->data.date);
            
            bool shouldSwap = false;
            if (date1.year > date2.year) {
                shouldSwap = true;
            } else if (date1.year == date2.year) {
                if (date1.month > date2.month) {
                    shouldSwap = true;
                } else if (date1.month == date2.month) {
                    if (date1.day > date2.day) {
                        shouldSwap = true;
                    }
                }
            }
            
            if (shouldSwap) {
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
    
    cout << "Sorted using Bubble Sort (Linked List).\n";
}

// Selection sort implementation for linked list
void TransactionList::selectionSort() {
    NodeTransaction* current = head;
    
    while (current != nullptr) {
        NodeTransaction* min = current;
        NodeTransaction* r = current->next;
        
        while (r != nullptr) {
            // Manual date comparison
            Date dateR = parseDate(r->data.date);
            Date dateMin = parseDate(min->data.date);
            
            bool isEarlier = false;
            if (dateR.year < dateMin.year) {
                isEarlier = true;
            } else if (dateR.year == dateMin.year) {
                if (dateR.month < dateMin.month) {
                    isEarlier = true;
                } else if (dateR.month == dateMin.month) {
                    if (dateR.day < dateMin.day) {
                        isEarlier = true;
                    }
                }
            }
            
            if (isEarlier) {
                min = r;
            }
            r = r->next;
        }
        
        // Swap data if min is not current
        if (min != current) {
            Transaction temp = current->data;
            current->data = min->data;
            min->data = temp;
        }
        
        current = current->next;
    }
    
    cout << "Sorted using Selection Sort (Linked List).\n";
}

// Insertion sort implementation for linked list
void TransactionList::insertionSort() {
    if (head == nullptr || head->next == nullptr)
        return; // 0 or 1 node - already sorted
    
    NodeTransaction* sorted = nullptr;
    NodeTransaction* current = head;
    
    while (current != nullptr) {
        NodeTransaction* next = current->next;
        
        // Manual date comparison for insertion at beginning
        bool insertAtBeginning = false;
        if (sorted == nullptr) {
            insertAtBeginning = true;
        } else {
            Date dateCurrent = parseDate(current->data.date);
            Date dateSorted = parseDate(sorted->data.date);
            
            if (dateCurrent.year < dateSorted.year) {
                insertAtBeginning = true;
            } else if (dateCurrent.year == dateSorted.year) {
                if (dateCurrent.month < dateSorted.month) {
                    insertAtBeginning = true;
                } else if (dateCurrent.month == dateSorted.month) {
                    if (dateCurrent.day < dateSorted.day) {
                        insertAtBeginning = true;
                    }
                }
            }
        }
        
        if (insertAtBeginning) {
            // Insert at beginning of sorted list
            current->next = sorted;
            sorted = current;
        } else {
            // Find position to insert using manual date comparison
            NodeTransaction* temp = sorted;
            
            while (temp->next != nullptr) {
                Date dateNext = parseDate(temp->next->data.date);
                Date dateCurrent = parseDate(current->data.date);
                
                bool shouldContinue = false;
                if (dateNext.year < dateCurrent.year) {
                    shouldContinue = true;
                } else if (dateNext.year == dateCurrent.year) {
                    if (dateNext.month < dateCurrent.month) {
                        shouldContinue = true;
                    } else if (dateNext.month == dateCurrent.month) {
                        if (dateNext.day <= dateCurrent.day) {
                            shouldContinue = true;
                        }
                    }
                }
                
                if (shouldContinue) {
                    temp = temp->next;
                } else {
                    break;
                }
            }
            
            current->next = temp->next;
            temp->next = current;
        }
        
        current = next;
    }
    
    head = sorted;
    cout << "Sorted using Insertion Sort (Linked List).\n";
}

void TransactionList::sortByDate() {
    int choice;
    cout << "Choose sorting algorithm:\n";
    cout << "1. Bubble Sort\n";
    cout << "2. Selection Sort\n";
    cout << "3. Insertion Sort\n";
    cout << "Enter choice: ";
    cin >> choice;

    auto start = high_resolution_clock::now();  // Start timing

    switch (choice) {
        case 1:
            bubbleSort();
            break;
        case 2:
            selectionSort();
            break;
        case 3:
            insertionSort();
            break;
        default:
            cout << "Invalid choice. No sorting applied.\n";
            return;
    }

    auto end = high_resolution_clock::now();  // End timing
    auto duration = duration_cast<microseconds>(end - start);

    cout << "\nSorted Transactions:\n";
    display();
    cout << "Sorting completed in " << duration.count() << " microseconds.\n";
}

void TransactionList::measureSortingPerformance() {
    cout << "Measuring sorting performance for Linked List implementation...\n";
    
    // Create copies of the list for different sorting algorithms
    TransactionList list1, list2, list3;
    NodeTransaction* current = head;
    
    while (current != nullptr) {
        list1.insert(current->data);
        list2.insert(current->data);
        list3.insert(current->data);
        current = current->next;
    }
    
    // Measure bubble sort
    auto start1 = high_resolution_clock::now();
    list1.bubbleSort();
    auto end1 = high_resolution_clock::now();
    auto bubble_time = duration_cast<microseconds>(end1 - start1).count();
    
    // Measure selection sort
    auto start2 = high_resolution_clock::now();
    list2.selectionSort();
    auto end2 = high_resolution_clock::now();
    auto selection_time = duration_cast<microseconds>(end2 - start2).count();
    
    // Measure insertion sort
    auto start3 = high_resolution_clock::now();
    list3.insertionSort();
    auto end3 = high_resolution_clock::now();
    auto insertion_time = duration_cast<microseconds>(end3 - start3).count();
    
    // Display results
    cout << "\nLinked List Sorting Performance:\n";
    cout << "--------------------------------\n";
    cout << "Bubble Sort:    " << bubble_time << " microseconds\n";
    cout << "Selection Sort: " << selection_time << " microseconds\n";
    cout << "Insertion Sort: " << insertion_time << " microseconds\n";
    cout << "--------------------------------\n";
}

// ReviewList implementation
ReviewList::ReviewList() : head(nullptr), size(0) {}

ReviewList::~ReviewList() {
    clear();
}

void ReviewList::clear() {
    NodeReview* current = head;
    while (current != nullptr) {
        NodeReview* temp = current;
        current = current->next;
        delete temp;
    }
    head = nullptr;
    size = 0;
}

void ReviewList::insert(CustomerReview review) {
    NodeReview* newNode = new NodeReview;
    newNode->data = review;
    newNode->next = head;
    head = newNode;
    size++;
}

void ReviewList::display() {
    NodeReview* current = head;
    cout << "-----------------------------------------\n";
    cout << "| Product ID | Customer ID | Rating | Review Text |\n";
    cout << "-----------------------------------------\n";
    
    while (current != nullptr) {
        cout << "| " << current->data.productID 
             << " | " << current->data.customerID
             << " | " << current->data.rating
             << " | " << current->data.reviewText << " |\n";
        current = current->next;
    }
    cout << "-----------------------------------------\n";
}

int ReviewList::getSize() const {
    return size;
}

NodeReview* ReviewList::getHead() const {
    return head;
}

void ReviewList::loadFromCSV(const char* filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return;
    }

    string line;
    getline(file, line);  // Skip header row
    
    // Read through the CSV
    while (getline(file, line)) {
        CustomerReview review;
        stringstream ss(line);
        
        ss.getline(review.productID, MAX_STRING_LENGTH, ',');
        ss.getline(review.customerID, MAX_STRING_LENGTH, ',');
        string ratingStr;
        getline(ss, ratingStr, ',');
        review.rating = stoi(ratingStr);
        ss.getline(review.reviewText, MAX_STRING_LENGTH, ',');
        
        insert(review);
    }
    
    file.close();
    cout << "Loaded " << size << " reviews into linked list." << endl;
}

void ReviewList::processReviews() {
    int chosenRating;
    cout << "Enter rating to analyze (1-5): ";
    cin >> chosenRating;

    if (chosenRating < 1 || chosenRating > 5) {
        cout << "Invalid input. Please enter a rating between 1 and 5.\n";
        return;
    }

    // Create a linked list for word frequency
    NodeWordFreq* wordListHead = nullptr;
    int wordCount = 0;

    // Process all reviews with the chosen rating
    NodeReview* current = head;
    while (current != nullptr) {
        if (current->data.rating == chosenRating) {
            stringstream ss(current->data.reviewText);
            string word;
            
            while (ss >> word) {
                cleanWord(word);  // Clean punctuation and convert to lowercase
                
                // Check if the word already exists in the list
                bool wordExists = false;
                NodeWordFreq* wordNode = wordListHead;
                
                while (wordNode != nullptr) {
                    if (strcmp(wordNode->word, word.c_str()) == 0) {
                        wordNode->frequency++;
                        wordExists = true;
                        break;
                    }
                    wordNode = wordNode->next;
                }
                
                // If word is new, add it to the list
                if (!wordExists && !word.empty()) {
                    NodeWordFreq* newWord = new NodeWordFreq;
                    strcpy(newWord->word, word.c_str());
                    newWord->frequency = 1;
                    newWord->next = wordListHead;
                    wordListHead = newWord;
                    wordCount++;
                }
            }
        }
        current = current->next;
    }
    
    // Sort word list by frequency (using bubble sort)
    // This is a DIY (do-it-yourself) implementation of bubble sort
    // for the word frequency linked list, sorting by frequency in descending order
    
    // If list is empty or has only one node, it's already sorted
    if (wordListHead == nullptr || wordListHead->next == nullptr) {
        // Already sorted (empty or single node)
    } else {
        bool swapped;
        NodeWordFreq* ptr1;
        NodeWordFreq* lptr = nullptr;
        
        do {
            swapped = false;
            ptr1 = wordListHead;
            
            while (ptr1->next != lptr) {
                // Compare frequencies - we want descending order
                if (ptr1->frequency < ptr1->next->frequency) {
                    // Manually swap the data between nodes
                    
                    // Temporary storage for the current node's data
                    int tempFreq = ptr1->frequency;
                    char tempWord[MAX_STRING_LENGTH];
                    strcpy(tempWord, ptr1->word);
                    
                    // Copy next node's data to current node
                    ptr1->frequency = ptr1->next->frequency;
                    strcpy(ptr1->word, ptr1->next->word);
                    
                    // Copy saved data to next node
                    ptr1->next->frequency = tempFreq;
                    strcpy(ptr1->next->word, tempWord);
                    
                    swapped = true;
                }
                ptr1 = ptr1->next;
            }
            lptr = ptr1;
        } while (swapped);
    }
    
    // Display top words
    cout << "\nTop 10 words in " << chosenRating << "-star reviews:\n";
    cout << "-------------------------------\n";
    cout << "|   Word      |   Frequency   |\n";
    cout << "-------------------------------\n";
    
    NodeWordFreq* wordNode = wordListHead;
    int displayCount = 0;
    
    while (wordNode != nullptr && displayCount < 10) {
        cout << "| " << setw(12) << left << wordNode->word
             << "| " << setw(13) << wordNode->frequency << " |\n";
        wordNode = wordNode->next;
        displayCount++;
    }
    cout << "-------------------------------\n";
    
    // Clean up word frequency list
    while (wordListHead != nullptr) {
        NodeWordFreq* temp = wordListHead;
        wordListHead = wordListHead->next;
        delete temp;
    }
}

// Helper functions implementation
Date parseDate(const char* dateStr) {
    Date date;
    // Manually parse the date string in format "DD/MM/YYYY"
    sscanf(dateStr, "%d/%d/%d", &date.day, &date.month, &date.year);
    return date;
}

// Function to manually compare two dates
int compareDates(const char* date1, const char* date2) {
    Date d1 = parseDate(date1);
    Date d2 = parseDate(date2);

    // Compare year first
    if (d1.year != d2.year) return d1.year - d2.year;
    
    // If years are equal, compare month
    if (d1.month != d2.month) return d1.month - d2.month;
    
    // If months are equal, compare day
    return d1.day - d2.day;
}

// Function to check if a date is within a range
bool isDateInRange(const char* transactionDate, const char* startDate, const char* endDate) {
    // If either bound is not specified, consider it as not restricted
    if (strcmp(startDate, "-") == 0 || strcmp(endDate, "-") == 0) return true;

    // Check if date is between start and end dates
    return compareDates(transactionDate, startDate) >= 0 && 
           compareDates(transactionDate, endDate) <= 0;
}

// Function to clean a word (remove punctuation, convert to lowercase)
void cleanWord(std::string& word) {
    // Convert to lowercase
    for (int i = 0; i < word.length(); i++) {
        if (word[i] >= 'A' && word[i] <= 'Z') {
            word[i] = word[i] + ('a' - 'A');
        }
    }
    
    // Remove punctuation (manually)
    std::string result = "";
    for (int i = 0; i < word.length(); i++) {
        if (!ispunct(word[i])) {
            result += word[i];
        }
    }
    word = result;
}

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