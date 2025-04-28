#include "array.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>  // For string operations
#include <cstdlib>  // For dynamic memory allocation
#include <chrono>   // For performance measurement
#include <cstring>
#include <iomanip>  // For formatting output

using namespace std;
using namespace std::chrono; 

// Read transactions from CSV into manually allocated array
void readcsv(const char* filename, Transaction transactions[], int& count) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return;
    }

    string line;
    count = 0;

    while (getline(file, line) && count < MAX_TRANSACTIONS) {
        stringstream ss(line);
        ss.getline(transactions[count].customerID, MAX_STRING_LENGTH, ',');
        ss.getline(transactions[count].product, MAX_STRING_LENGTH, ',');
        ss.getline(transactions[count].category, MAX_STRING_LENGTH, ',');
        ss >> transactions[count].price;
        ss.ignore();  // Ignore comma
        ss.getline(transactions[count].date, MAX_STRING_LENGTH, ',');
        ss.getline(transactions[count].paymentMethod, MAX_STRING_LENGTH, ',');
        
        count++;
    }
    file.close();
}
// Search transactions by Customer ID
void searchdata(Transaction transactions[], int count, const char* customerID) {
    bool found = false;
    for (int i = 0; i < count; i++) {
        if (strcmp(transactions[i].customerID, customerID) == 0) {
            found = true;
            cout << "--------------------------------------" << endl;
            cout << "Customer ID       : " << transactions[i].customerID << endl;
            cout << "Product           : " << transactions[i].product << endl;
            cout << "Category          : " << transactions[i].category << endl;
            cout << "Price             : " << transactions[i].price << endl;
            cout << "Date              : " << transactions[i].date << endl;
            cout << "Payment Method    : " << transactions[i].paymentMethod << endl;
            cout << "--------------------------------------" << endl;
        }
    }
    if (!found) {
        cout << "Customer ID not found." << endl;
    }
}

Date parseDate(const char* dateStr) {
    Date date;
    sscanf(dateStr, "%d/%d/%d", &date.day, &date.month, &date.year);
    return date;
}
// Function to compare two dates numerically
int compareDates(const char* date1, const char* date2) {
    Date d1 = parseDate(date1);
    Date d2 = parseDate(date2);

    if (d1.year != d2.year) return d1.year - d2.year;
    if (d1.month != d2.month) return d1.month - d2.month;
    return d1.day - d2.day;
}
// Function to check if a transaction's date falls within a range
bool isDateInRange(const char* transactionDate, const char* startDate, const char* endDate) {
    if (strcmp(startDate, "-") == 0 || strcmp(endDate, "-") == 0) return true;

    return compareDates(transactionDate, startDate) >= 0 && compareDates(transactionDate, endDate) <= 0;
}

void trim(char* str) {
    // Trim leading spaces
    while (isspace(*str)) memmove(str, str + 1, strlen(str));

    // Trim trailing spaces
    int len = strlen(str);
    while (len > 0 && isspace(str[len - 1])) str[--len] = '\0';
}
// Function to filter transactions based on user input
void filterDataset(Transaction transactions[], int count) {
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

    int matchCount = 0;
    for (int i = 0; i < count; i++) {

        bool matchesProduct = (strcmp(productFilter, "-") == 0 || strcmp(transactions[i].product, productFilter) == 0);
        bool matchesCategory = (strcmp(categoryFilter, "-") == 0 || strcmp(transactions[i].category, categoryFilter) == 0);
        bool matchesPrice = (minPrice == -1 || (transactions[i].price >= minPrice && transactions[i].price <= maxPrice));
        trim(transactions[i].paymentMethod); //must do this
        trim(paymentMethodFilter); //must do this
        bool matchesPaymentMethod = (strcmp(paymentMethodFilter, "-") == 0 || strcmp(transactions[i].paymentMethod, paymentMethodFilter) == 0);
        bool matchesDate = isDateInRange(transactions[i].date, startDate, endDate);

        if (matchesProduct && matchesCategory && matchesPrice && matchesDate && matchesPaymentMethod) {
            matchCount++;
            cout << transactions[i].customerID << " " << transactions[i].product << " "
                 << transactions[i].category << " " << transactions[i].price << " "
                 << transactions[i].date << " " << transactions[i].paymentMethod << endl;
        }
    }

    cout << "\nTotal: " << matchCount << " transactions match the filters.\n";
}
// Selection Sort for sorting transactions by date
void sortByDate(Transaction transactions[], int count) {
    int choice;
    cout << "Choose sorting algorithm:\n";
    cout << "1. Bubble Sort\n";
    cout << "2. Selection Sort\n";
    cout << "3. Insertion Sort\n";
    cout << "Enter choice: ";
    cin >> choice;

    auto start = high_resolution_clock::now();  // Start timing

    switch (choice) {
        case 1: // Bubble Sort
            for (int i = 0; i < count - 1; i++) {
                for (int j = 0; j < count - i - 1; j++) {
                    if (strcmp(transactions[j].date, transactions[j + 1].date) > 0) {
                        swap(transactions[j], transactions[j + 1]);
                    }
                }
            }
            cout << "Sorted using Bubble Sort.\n";
            break;

        case 2: // Selection Sort
            for (int i = 0; i < count - 1; i++) {
                int minIndex = i;
                for (int j = i + 1; j < count; j++) {
                    if (strcmp(transactions[j].date, transactions[minIndex].date) < 0) {
                        minIndex = j;
                    }
                }
                swap(transactions[i], transactions[minIndex]);
            }
            cout << "Sorted using Selection Sort.\n";
            break;

        case 3: // Insertion Sort
            for (int i = 1; i < count; i++) {
                Transaction key = transactions[i];
                int j = i - 1;
                while (j >= 0 && strcmp(transactions[j].date, key.date) > 0) {
                    transactions[j + 1] = transactions[j];
                    j--;
                }
                transactions[j + 1] = key;
            }
            cout << "Sorted using Insertion Sort.\n";
            break;

        default:
            cout << "Invalid choice. No sorting applied.\n";
            return;
    }

    auto end = high_resolution_clock::now();  // End timing
    auto duration = duration_cast<microseconds>(end - start);

    cout << "\nSorted Transactions:\n";
    cout << "-------------------------------------------------------------------------------------------\n";
    cout << "|     Date     | Customer ID |     Product     |   Category   |  Price  |   Payment Method  |\n";
    cout << "-------------------------------------------------------------------------------------------\n";

    for (int i = 0; i < count; i++) {
        cout << setw(11) << transactions[i].date
             << " | " << setw(11) << transactions[i].customerID
             << " | " << setw(14) << transactions[i].product
             << " | " << setw(15) << transactions[i].category
             << " | " << setw(7) << fixed << setprecision(2) << transactions[i].price
             << " | " << setw(16) << transactions[i].paymentMethod << "\n";
    }

   

    cout << "Sorting completed in " << duration.count() << " microseconds.\n";
}
// Measure sorting performance
void measureSortingPerformance(Transaction transactions[], int count) {
    auto start = chrono::high_resolution_clock::now();
    sortByDate(transactions, count);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    cout << "Sorting completed in " << duration.count() << " microseconds.\n";
}
// Read and store reviews manually using an array
void cleanWord(string& word) {
    // Convert to lowercase
    transform(word.begin(), word.end(), word.begin(), ::tolower);
    // Remove punctuation
    word.erase(remove_if(word.begin(), word.end(), ::ispunct), word.end());
}
// Function to read customer reviews from CSV
void readReviews(const char* filename, CustomerReview reviews[], int& count) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return;
    }

    string line;
    getline(file, line);  // Skip header row
    count = 0;

    while (getline(file, line) && count < MAX_REVIEWS) {
        stringstream ss(line);
        ss.getline(reviews[count].productID, MAX_STRING_LENGTH, ',');
        ss.getline(reviews[count].customerID, MAX_STRING_LENGTH, ',');
        string ratingStr;
        getline(ss, ratingStr, ',');
        reviews[count].rating = stoi(ratingStr);
        ss.getline(reviews[count].reviewText, MAX_STRING_LENGTH, ',');

        count++;
    }

    file.close();
}
// Function to process reviews and count word frequency
void processReviews(CustomerReview reviews[], int count) {
    int chosenRating;
    cout << "Enter rating to analyze (1-5): ";
    cin >> chosenRating;

    if (chosenRating < 1 || chosenRating > 5) {
        cout << "Invalid input. Please enter a rating between 1 and 5.\n";
        return;
    }

    WordFrequency wordArray[MAX_WORDS];  
    int wordCount = 0;

    for (int i = 0; i < count; i++) {
        if (reviews[i].rating == chosenRating) {
            stringstream ss(reviews[i].reviewText);
            string word;
            while (ss >> word) {
                cleanWord(word);  // Clean punctuation and convert to lowercase
                
                // Check if the word already exists in the array
                bool wordExists = false;
                for (int j = 0; j < wordCount; j++) {
                    if (strcmp(wordArray[j].word, word.c_str()) == 0) {
                        wordArray[j].frequency++;  // Increase count if word exists
                        wordExists = true;
                        break;
                    }
                }

                // If the word is new, add it to the array
                if (!wordExists) {
                    strcpy(wordArray[wordCount].word, word.c_str());
                    wordArray[wordCount].frequency = 1;
                    wordCount++;
                }
            }
        }
    }

    // Sort words by frequency (Descending order)
    for (int i = 0; i < wordCount - 1; i++) {
        int maxIndex = i;
        for (int j = i + 1; j < wordCount; j++) {
            if (wordArray[j].frequency > wordArray[maxIndex].frequency) {
                maxIndex = j;
            }
        }
        swap(wordArray[i], wordArray[maxIndex]);
    }

    // Display top words in chosen rating reviews
    cout << "\nTop 10 words in " << chosenRating << "-star reviews:\n";
    cout << "-------------------------------\n";
    cout << "|   Word      |   Frequency   |\n";
    cout << "-------------------------------\n";
    for (int i = 0; i < min(10, wordCount); i++) {
        cout << "| " << setw(12) << left << wordArray[i].word
             << "| " << setw(13) << wordArray[i].frequency << " |\n";
    }
    cout << "-------------------------------\n";
}