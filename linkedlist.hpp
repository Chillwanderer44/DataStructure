#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>  // For string operations
#include <cstdlib>  // For dynamic memory allocation
#include <chrono>   // For performance measurement
#include "array.hpp" 

// Transaction node structure for linked list
struct NodeTransaction {
    Transaction data;
    NodeTransaction* next;
};

// Review node structure for linked list
struct NodeReview {
    CustomerReview data;
    NodeReview* next;
};

// Word frequency node for linked list
struct NodeWordFreq {
    char word[MAX_STRING_LENGTH];
    int frequency;
    NodeWordFreq* next;
};

// Linked list class for transactions
class TransactionList {
private:
    NodeTransaction* head;
    int size;

public:
    TransactionList();
    ~TransactionList();
    
    // Basic operations
    void insert(Transaction transaction);
    void display();
    int getSize() const;
    NodeTransaction* getHead() const;
    void clear();
    
    // Core functionality - parallel to array implementation
    void loadFromCSV(const char* filename);
    void searchByCustomerID(const char* customerID);
    void filterTransactions();
    
    // Sorting algorithms
    void bubbleSort();
    void selectionSort();
    void insertionSort();
    void sortByDate();
    
    // Performance measurement
    void measureSortingPerformance();
};

// Linked list class for reviews
class ReviewList {
private:
    NodeReview* head;
    int size;

public:
    ReviewList();
    ~ReviewList();
    
    // Basic operations
    void insert(CustomerReview review);
    void display();
    int getSize() const;
    NodeReview* getHead() const;
    void clear();
    
    // Core functionality
    void loadFromCSV(const char* filename);
    void processReviews();
};

// Helper functions
Date parseDate(const char* dateStr);
int compareDates(const char* date1, const char* date2);
bool isDateInRange(const char* transactionDate, const char* startDate, const char* endDate);
void cleanWord(std::string& word);

// Menu function for linked list operations
void displayLinkedListMenu();

#endif // LINKED_LIST_HPP