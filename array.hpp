#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>  // For string operations
#include <cstdlib>  // For dynamic memory allocation

#define MAX_TRANSACTIONS 10000
#define MAX_REVIEWS 10000
#define MAX_WORDS 50000
#define MAX_STRING_LENGTH 50  // Adjust as needed

using namespace std;

struct Date {
    int day, month, year;
};

struct WordFrequency {
    char word[MAX_STRING_LENGTH];
    int frequency;
};

// Structure for transactions (2D array replacement)
struct Transaction {
    char customerID[MAX_STRING_LENGTH];
    char product[MAX_STRING_LENGTH];
    char category[MAX_STRING_LENGTH];
    double price;
    char date[MAX_STRING_LENGTH];
    char paymentMethod[MAX_STRING_LENGTH];
};

// Structure for customer reviews
struct CustomerReview {
    char productID[MAX_STRING_LENGTH];
    char customerID[MAX_STRING_LENGTH];
    int rating;
    char reviewText[MAX_STRING_LENGTH];
};

// Function declarations
void readcsv(const char* filename, Transaction transactions[], int& count);
void searchdata(Transaction transactions[], int count, const char* customerID);
void filterDataset(Transaction transactions[], int count);
void sortByDate(Transaction transactions[], int count);
void measureSortingPerformance(Transaction transactions[], int count);
void readReviews(const char* filename, CustomerReview reviews[], int& count);
void processReviews(CustomerReview reviews[], int count);
void displayMenu();

// searching algorithms
void linearSearchByCustomerID(Transaction transaction[], int count, const char* customerID);
void binarySearchByCustomerID(Transaction transaction[], int count, const char* customerID);
void sortByCustomerID(Transaction transaction[], int count);
void compareSearchAlgorithms(Transaction transaction[], int count, const char* customerID);

#endif
