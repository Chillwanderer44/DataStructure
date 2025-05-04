#include "helpers.hpp"
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

    // Skip header row
    getline(file, line);

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
    int choice;
    cout << "Choose search algorithm:" << endl;
    cout << "1. Linear Search" << endl;
    cout << "2. Binary Search" << endl;
    cout << "Enter choice: ";
    cin >> choice;

    switch (choice) {
        case 1:
            linearSearchByCustomerID(transactions, count, customerID);
            break;
        case 2:
            binarySearchByCustomerID(transactions, count, customerID);
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
            break;
    }
}
// Seach transacations Algorithms
// Linear and Binary Search

// Linear search by customer ID
void linearSearchByCustomerID(Transaction transactions[], int count, const char* customerID) {
    auto start = high_resolution_clock::now();  // Start timing
    
    bool found = false;
    int comparisons = 0;
    
    for (int i = 0; i < count; i++) {
        comparisons++;
        if (strcmp(transactions[i].customerID, customerID) == 0) {
            found = true;
            cout << "--------------------------------------" << endl;
            cout << "Customer ID       : " << transactions[i].customerID << endl;
            cout << "Product           : " << transactions[i].product << endl;
            cout << "Category          : " << transactions[i].category << endl;
            cout << "Price             : " << fixed << setprecision(2) << transactions[i].price << endl;
            cout << "Date              : " << transactions[i].date << endl;
            cout << "Payment Method    : " << transactions[i].paymentMethod << endl;
            cout << "--------------------------------------" << endl;
        }
    }
    
    auto end = high_resolution_clock::now();  // End timing
    auto duration = duration_cast<microseconds>(end - start);
    
    if (!found) {
        cout << "Customer ID not found." << endl;
    }
    
    cout << "Linear Search completed." << endl;
    cout << "Comparisons: " << comparisons << endl;
    cout << "Time taken: " << duration.count() << " microseconds." << endl;
}

// Sort transactions by customer ID (required for binary search)
void sortByCustomerID(Transaction transactions[], int count) {
    auto start = high_resolution_clock::now();  // Start timing
    
    // Using insertion sort for simplicity
    for (int i = 1; i < count; i++) {
        Transaction key = transactions[i];
        int j = i - 1;
        
        while (j >= 0 && strcmp(transactions[j].customerID, key.customerID) > 0) {
            transactions[j + 1] = transactions[j];
            j--;
        }
        transactions[j + 1] = key;
    }
    
    auto end = high_resolution_clock::now();  // End timing
    auto duration = duration_cast<microseconds>(end - start);
    
    cout << "Sorted Customer IDs using Insertion Sort" << endl;
    cout << "Time taken: " << duration.count() << " microseconds." << endl;
}

// Binary search by customer ID
void binarySearchByCustomerID(Transaction transactions[], int count, const char* customerID) {
    // First check if sorted by customer ID
    bool isSorted = true;
    for (int i = 0; i < count - 1; i++) {
        if (strcmp(transactions[i].customerID, transactions[i + 1].customerID) > 0) {
            isSorted = false;
            break;
        }
    }
    
    if (!isSorted) {
        cout << "Data is not sorted by Customer ID. Sorting now..." << endl;
        sortByCustomerID(transactions, count);
    }
    
    auto start = high_resolution_clock::now();  // Start timing
    
    int left = 0;
    int right = count - 1;
    bool found = false;
    int comparisons = 0;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        comparisons++;
        
        int comparison = strcmp(transactions[mid].customerID, customerID);
        
        if (comparison == 0) {
            found = true;
            
            cout << "--------------------------------------" << endl;
            cout << "Customer ID       : " << transactions[mid].customerID << endl;
            cout << "Product           : " << transactions[mid].product << endl;
            cout << "Category          : " << transactions[mid].category << endl;
            cout << "Price             : " << fixed << setprecision(2) << transactions[mid].price << endl;
            cout << "Date              : " << transactions[mid].date << endl;
            cout << "Payment Method    : " << transactions[mid].paymentMethod << endl;
            cout << "--------------------------------------" << endl;
            
            // Check for other instances with the same customer ID
            // Check left
            int temp = mid - 1;
            while (temp >= 0 && strcmp(transactions[temp].customerID, customerID) == 0) {
                comparisons++;
                cout << "--------------------------------------" << endl;
                cout << "Customer ID       : " << transactions[temp].customerID << endl;
                cout << "Product           : " << transactions[temp].product << endl;
                cout << "Category          : " << transactions[temp].category << endl;
                cout << "Price             : " << fixed << setprecision(2) << transactions[temp].price << endl;
                cout << "Date              : " << transactions[temp].date << endl;
                cout << "Payment Method    : " << transactions[temp].paymentMethod << endl;
                cout << "--------------------------------------" << endl;
                temp--;
            }
            
            // Check right
            temp = mid + 1;
            while (temp < count && strcmp(transactions[temp].customerID, customerID) == 0) {
                comparisons++;
                cout << "--------------------------------------" << endl;
                cout << "Customer ID       : " << transactions[temp].customerID << endl;
                cout << "Product           : " << transactions[temp].product << endl;
                cout << "Category          : " << transactions[temp].category << endl;
                cout << "Price             : " << fixed << setprecision(2) << transactions[temp].price << endl;
                cout << "Date              : " << transactions[temp].date << endl;
                cout << "Payment Method    : " << transactions[temp].paymentMethod << endl;
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
    
    auto end = high_resolution_clock::now();  // End timing
    auto duration = duration_cast<microseconds>(end - start);
    
    if (!found) {
        cout << "Customer ID not found." << endl;
    }
    
    cout << "Binary Search completed." << endl;
    cout << "Comparisons: " << comparisons << endl;
    cout << "Time taken: " << duration.count() << " microseconds." << endl;
}

// Function to compare search algorithms
void compareSearchAlgorithms(Transaction transactions[], int count, const char* customerID) {
    cout << "\n===== ARRAY-BASED SEARCH ALGORITHM COMPARISON =====\n";

    // Add memory usage calculation
    size_t linearMemory = calculateSearchMemoryUsage(count, false);
    size_t binaryMemory = calculateSearchMemoryUsage(count, true);
    
    cout << "Memory usage:\n";
    cout << "- Linear Search: " << linearMemory / 1024 << " KB\n";
    cout << "- Binary Search: " << binaryMemory / 1024 << " KB\n\n";
    
    // Measure linear search
    auto start1 = high_resolution_clock::now();
    bool found1 = false;
    int linearComparisons = 0;
    
    for (int i = 0; i < count; i++) {
        linearComparisons++;
        if (strcmp(transactions[i].customerID, customerID) == 0) {
            found1 = true;
            // In real search we would display the transaction details here
            // but for timing purposes, we just set the found flag
        }
    }
    auto end1 = high_resolution_clock::now();
    auto duration1 = duration_cast<microseconds>(end1 - start1);
    
    // Check if sorted by customerID
    bool isSorted = true;
    for (int i = 0; i < count - 1; i++) {
        if (strcmp(transactions[i].customerID, transactions[i + 1].customerID) > 0) {
            isSorted = false;
            break;
        }
    }
    
    if (!isSorted) {
        cout << "Sorting array by Customer ID for binary search...\n";
        sortByCustomerID(transactions, count);
    }
    
    // Measure binary search
    auto start2 = high_resolution_clock::now();
    
    // Binary search implementation
    int left = 0;
    int right = count - 1;
    bool found2 = false;
    int binaryComparisons = 0;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        binaryComparisons++;
        
        int comparison = strcmp(transactions[mid].customerID, customerID);
        
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
    
    auto end2 = high_resolution_clock::now();
    auto duration2 = duration_cast<microseconds>(end2 - start2);
    
    // Display results
    cout << "\n===== SEARCH RESULTS =====\n";
    cout << "Searching for Customer ID: " << customerID << endl;
    cout << "Customer found: " << (found1 ? "Yes" : "No") << endl << endl;
    
    cout << "Linear Search:\n";
    cout << "- Comparisons: " << linearComparisons << endl;
    cout << "- Time taken: " << duration1.count() << " microseconds\n";
    cout << "- Memory used: " << linearMemory / 1024 << " KB\n\n";
    
    cout << "Binary Search:\n";
    cout << "- Comparisons: " << binaryComparisons << endl;
    cout << "- Time taken: " << duration2.count() << " microseconds\n";
    cout << "- Memory used: " << binaryMemory / 1024 << " KB\n\n";
    
    if (duration1.count() < duration2.count()) {
        cout << "For this dataset and search key, Linear Search was faster.\n";
    } else if (duration2.count() < duration1.count()) {
        cout << "For this dataset and search key, Binary Search was faster.\n";
    } else {
        cout << "Both algorithms performed equally fast for this search.\n";
    }
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
// Helper function for merge sort
void merge(Transaction arr[], int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    // Create temporary arrays
    Transaction* L = new Transaction[n1];
    Transaction* R = new Transaction[n2];
    
    // Copy data to temporary arrays
    for (i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];
    
    // Merge the temporary arrays back into arr[left..right]
    i = 0;    // Initial index of first subarray
    j = 0;    // Initial index of second subarray
    k = left; // Initial index of merged subarray
    
    while (i < n1 && j < n2) {
        if (strcmp(L[i].date, R[j].date) <= 0) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    
    // Copy the remaining elements of L[], if there are any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    
    // Copy the remaining elements of R[], if there are any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
    
    // Free temporary arrays
    delete[] L;
    delete[] R;
}

// Merge sort implementation
void mergeSortByDate(Transaction arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        
        // Sort first and second halves
        mergeSortByDate(arr, left, mid);
        mergeSortByDate(arr, mid + 1, right);
        
        // Merge the sorted halves
        merge(arr, left, mid, right);
    }
}

// Selection Sort for sorting transactions by date
void sortByDate(Transaction transactions[], int count) {
    int choice;
    cout << "Choose sorting algorithm:\n";
    cout << "1. Bubble Sort\n";
    cout << "2. Insertion Sort\n";
    cout << "3. Merge Sort\n";
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

        case 2: // Insertion Sort
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

        case 3: // Merge Sort
            mergeSortByDate(transactions, 0, count - 1);
            cout << "Sorted using Merge Sort.\n";
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
        cout << "| " << setw(11) << transactions[i].date
             << " | " << setw(11) << transactions[i].customerID
             << " | " << setw(14) << transactions[i].product
             << " | " << setw(12) << transactions[i].category
             << " | " << setw(7) << fixed << setprecision(2) << transactions[i].price
             << " | " << setw(16) << transactions[i].paymentMethod << " |\n";
    }

    cout << "Sorting completed in " << duration.count() << " microseconds.\n";
}
//function to calculate memory usage
size_t calculateArrayMemoryUsage(int count) {
    size_t memoryArray = count * sizeof(Transaction);
    
    return memoryArray;
    }

// Measure sorting performance
// merge sort, insertion sort, bubble sort
void measureSortingPerformance(Transaction transactions[], int count) {
    if (count == 0) {
        cout << "No transactions to sort, please check your data" << endl;
        return;
    }
    // Calculate memory usage for each algorithm
    size_t bubbleMemory = calculateArrayMemoryUsage(count, "bubble");
    size_t insertionMemory = calculateArrayMemoryUsage(count, "insertion");
    size_t mergeMemory = calculateArrayMemoryUsage(count, "merge");
    
    cout << "Memory usage:" << endl;
    cout << "Bubble Sort: " << bubbleMemory / 1024.0 << " KB" << endl;
    cout << "Insertion Sort: " << insertionMemory / 1024.0 << " KB" << endl;
    cout << "Merge Sort: " << mergeMemory / 1024.0 << " KB" << endl;

    cout << "Measuring sorting performance for " << count << " transactions:" << endl;
    cout << "------------------------------------------------------" << endl;

    // Create copies of the original array to test each algorithm
    Transaction* bubbleArray = new Transaction[count];
    Transaction* insertionArray = new Transaction[count];
    Transaction* mergeArray = new Transaction[count];

    // Copy original array to test arrays
    for (int i = 0; i < count; i++) {
        bubbleArray[i] = transactions[i];
        insertionArray[i] = transactions[i];
        mergeArray[i] = transactions[i];
    }

    // Measure bubble sort
    auto start1 = high_resolution_clock::now();
    // Bubble Sort implementation
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (strcmp(bubbleArray[j].date, bubbleArray[j + 1].date) > 0) {
                Transaction temp = bubbleArray[j];
                bubbleArray[j] = bubbleArray[j + 1];
                bubbleArray[j + 1] = temp;
            }
        }
    }
    auto end1 = high_resolution_clock::now();
    auto duration1 = duration_cast<microseconds>(end1 - start1);

    // Measure insertion sort
    auto start2 = high_resolution_clock::now();
    // Insertion Sort implementation
    for (int i = 1; i < count; i++) {
        Transaction key = insertionArray[i];
        int j = i - 1;
        while (j >= 0 && strcmp(insertionArray[j].date, key.date) > 0) {
            insertionArray[j + 1] = insertionArray[j];
            j--;
        }
        insertionArray[j + 1] = key;
    }
    auto end2 = high_resolution_clock::now();
    auto duration2 = duration_cast<microseconds>(end2 - start2);

    // Measure merge sort
    auto start3 = high_resolution_clock::now();
    // Merge Sort implementation
    mergeSortByDate(mergeArray, 0, count - 1);
    auto end3 = high_resolution_clock::now();
    auto duration3 = duration_cast<microseconds>(end3 - start3);

    // Display results
    cout << "Bubble Sort:    " << duration1.count() << " microseconds" << endl;
    cout << "Insertion Sort: " << duration2.count() << " microseconds" << endl;
    cout << "Merge Sort:     " << duration3.count() << " microseconds" << endl;
    cout << "------------------------------------------------------" << endl;

    // Show the best method
    if (duration1.count() <= duration2.count() && duration1.count() <= duration3.count()) {
        cout << "Bubble Sort was fastest for this dataset." << endl;
    } else if (duration2.count() <= duration1.count() && duration2.count() <= duration3.count()) {
        cout << "Insertion Sort was fastest for this dataset." << endl;
    } else {
        cout << "Merge Sort was fastest for this dataset." << endl;
    }

    // Free memory
    delete[] bubbleArray;
    delete[] insertionArray;
    delete[] mergeArray;
}


// Read and store reviews manually using an array
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

// funciton to calculate memory usage
size_t calculateSearchMemoryUsage(int count, bool isBinary) {
    size_t memoryUsage = count * sizeof(Transaction); // Base array memory
    
    if (isBinary) {
        // For binary search, we need a sorted array
        memoryUsage += sizeof(int) * 3; // left, right, mid variables
    } else {
        // For linear search, we only need a counter variable
        memoryUsage += sizeof(int); // loop counter
    }
    
    return memoryUsage;
}

size_t calculateArrayMemoryUsage(int count, const char* algorithm = "general") {
    size_t memoryArray = count * sizeof(Transaction);
    size_t additionalMemory = 0;
    
    if (strcmp(algorithm, "bubble") == 0) {
        // Bubble sort uses a temp variable for swapping
        additionalMemory = sizeof(Transaction); // One temp variable
    } 
    else if (strcmp(algorithm, "insertion") == 0) {
        // Insertion sort uses a key variable
        additionalMemory = sizeof(Transaction); // Key variable
    }
    else if (strcmp(algorithm, "merge") == 0) {
        // Merge sort uses temporary arrays
        additionalMemory = count * sizeof(Transaction); // Temporary arrays
    }
    
    return memoryArray + additionalMemory;
}


