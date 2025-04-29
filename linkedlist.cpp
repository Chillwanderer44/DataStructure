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

            out << "--------------------------------------" << endl;
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




