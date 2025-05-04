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
        void bubbleSort(bool showTiming = true);
        void insertionSort(bool showTiming = true);
        void mergeSort(bool showTiming = true);
        void sortByDate();

        //performance measurement
        void measureSortingPerformance();

        //Searching algorthms
        void linearSearchByCustomerID(const char* customerID);
        void binarySearchByCustomerID(const char* customerID);
        void sortByCustomerID(); // Sort before binary search
        void compareSearchAlgorithms(const char* customerID);

        //memory usage
        size_t calculateSearchMemoryUsage(bool isBinary) const;
        size_t calculateSortMemoryUsage(const char* algorithm = "general") const;

        //debugging
        bool hasCustomerID(const char* customerID);
        void inspectCSVFormat(const char* filename);

};

//linked list class for reviews
class ReviewList {
    private:
        NodeReview* head;
        int size;
    public:
        //Destructor and constructor
        ReviewList();
        ~ReviewList();

        //Basic operations
        void insert(CustomerReview review);
        void display();
        int getSize() const;
        NodeReview* getHead() const;
        void clear();

        //Core functionality
        void loadFromCSV(const char* filename);
        void processReviews();
};
//Helper function to clean a word
void cleanWord(string& word);

#endif