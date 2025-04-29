#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

#include "array.hpp"
#include <cstring>

// Node Structure for Transaction
struct NodeTransaction {
    Transaction data;
    NodeTransaction* next;
};

// Node Structure for Review
struct NodeReview {
    CustomerReview data;
    NodeReview* next;
};

// Node Structure for Word Frequency
struct NodeWordFreq {
    char word[MAX_STRING_LENGTH];
    int frequency;
    NodeWordFreq* next;
};

// Class Declarations 
//Linked list class for transactions
class TransactionList {
    private:
        NodeTransaction* head;
        int size;    
    public:
        //Destructor and Constructor
        TransactionList();
        ~TransactionList();

        //Basic operations
        void insert(Transaction transaction);
        void display();
        int getSize() const;
        NodeTransaction* getHead() const;
        void clear();

        //Core functionality
        void loadFromCSV(const char* filename);
        void filterTransactions();
        void searchByCustomerID(const char* customerID);

        //sorting algorithms
        void bubbleSort();
        void insertionSort();
        void mergeSort();

        //performance measurement
        void measureSortingPerformance();
};

#endif