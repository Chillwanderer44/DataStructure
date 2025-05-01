#include "helpers.hpp"
#include "array.hpp"
#include "linkedlist.hpp"
#include <iostream>

using namespace std;

// Function to display the main menu
void displayMainMenu() {
    cout << "\n===== MAIN MENU =====\n";
    cout << "1. Use Array\n";
    cout << "2. Use Linked List\n";
    cout << "3. Compare Performance\n";
    cout << "4. Exit\n";
    cout << "Enter your choice: ";
}

// Display menu for array operations
void displayArrayMenu() {
    cout << "\n===== ARRAY OPERATIONS =====\n";
    cout << "1. Search by Customer ID\n";
    cout << "2. Filter Transactions\n";
    cout << "3. Sort Transactions by Date\n";
    cout << "4. Analyze Reviews\n";
    cout << "5. Compare Sorting Performance\n";
    cout << "6. Return to Main Menu\n";
    cout << "Enter your choice: ";
}

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

// Display menu for performance comparison
void displayPerformanceMenu() {
    cout << "\n===== PERFORMANCE COMPARISON MENU =====\n";
    cout << "1. Compare Sorting Performance\n";
    cout << "2. Compare Search Performance\n";
    cout << "3. Return to Main Menu\n";
    cout << "Enter your choice: ";
}

int main() {
    // Relative file paths for transactions and reviews
    // Ensure these files are in the same directory as the executable
    const char* transactionFile = "transactions_cleaned.csv";
    const char* reviewFile = "reviews_cleaned.csv";

    Transaction transactions[MAX_TRANSACTIONS];
    CustomerReview reviews[MAX_REVIEWS];
    int transactionCount = 0, reviewCount = 0;

    // Load transactions and reviews from CSV
    readcsv(transactionFile, transactions, transactionCount);
    if (transactionCount == 0) {
        std::cout << "No transactions found or failed to load the file " << endl; 
        std::cout << "Please check if the file exists in the current directory." << endl;
        return 1;
    }
    std::cout << "Successfully loaded " << transactionCount << " transactions " << endl;

    readReviews(reviewFile, reviews, reviewCount);
    if (reviewCount == 0) {
        std::cout << "No reviews found or failed to load the file " << endl;
        std::cout << "Please check if the file exists in the current directory." << endl;
        return 1;
    }
    std::cout << "Successfully loaded " << reviewCount << " reviews " << endl;

    // Main menu loop
    int choice = 0;

    while (true) {
        displayMainMenu();
        cin >> choice;

        if (cin.fail() || choice < 1 || choice > 4) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number between 1 and 4.\n";
            continue;
        }

        if (choice == 1) { 
            // Run array-based menu
            int arrayChoice = 0;
            while (true) {
                displayArrayMenu();
                cin >> arrayChoice;
        
                if (arrayChoice == 1) {
                    char customerID[MAX_STRING_LENGTH];
                    do {
                        cout << "Enter Customer ID to search: ";
                        cin >> customerID;
                        searchdata(transactions, transactionCount, customerID);
        
                        cout << "\nDo you want to search again? (Yes = 1 / No = 0): ";
                        cin >> arrayChoice;
                    } while (arrayChoice == 1);
                } 
        
                else if (arrayChoice == 2) {
                    do {
                        filterDataset(transactions, transactionCount);
                        cout << "\nDo you want to filter again? (Yes = 1 / No = 0): ";
                        cin >> arrayChoice;
                    } while (arrayChoice == 1);
                }
        
                else if (arrayChoice == 3) {
                    do {
                        sortByDate(transactions, transactionCount);
                        cout << "\nDo you want to sort again? (Yes = 1 / No = 0): ";
                        cin >> arrayChoice;
                    } while (arrayChoice == 1);
                }
        
                else if (arrayChoice == 4) {
                    do {
                        processReviews(reviews, reviewCount);
                        cout << "\nDo you want to analyze reviews again? (Yes = 1 / No = 0): ";
                        cin >> arrayChoice;
                    } while (arrayChoice == 1);
                }
        
                else if (arrayChoice == 5) {
                    do {
                        measureSortingPerformance(transactions, transactionCount);
                        cout << "\nDo you want to compare sorting performance again? (Yes = 1 / No = 0): ";
                        cin >> arrayChoice;
                    } while (arrayChoice == 1);
                }
        
                else if (arrayChoice == 6) {
                    cout << "Returning to main menu...\n";
                    break;
                }
            }
        }        
        
        else if (choice == 2) {
            // Linked List structure initialized
            TransactionList transactionlist;
            ReviewList reviewlist;

            // Load data
            transactionlist.loadFromCSV(transactionFile);
            reviewlist.loadFromCSV(reviewFile);

            int linkedListChoice = 0;
            while (true) {
                displayLinkedListMenu();
                cin >> linkedListChoice;

                if (linkedListChoice == 1) {
                    char customerID[MAX_STRING_LENGTH];
                    do {
                        cout << "Enter Customer ID to search: ";
                        cin >> customerID;
                        transactionlist.searchByCustomerID(customerID);
        
                        cout << "\nDo you want to search again? (Yes = 1 / No = 0): ";
                        cin >> linkedListChoice;
                    } while (linkedListChoice == 1);
                } 
        
                else if (linkedListChoice == 2) {
                    do {
                        transactionlist.filterTransactions();
                        cout << "\nDo you want to filter again? (Yes = 1 / No = 0): ";
                        cin >> linkedListChoice;
                    } while (linkedListChoice == 1);
                }
        
                else if (linkedListChoice == 3) {
                    do {
                        transactionlist.sortByDate();
                        cout << "\nDo you want to sort again? (Yes = 1 / No = 0): ";
                        cin >> linkedListChoice;
                    } while (linkedListChoice == 1);
                }
        
                else if (linkedListChoice == 4) {
                    do {
                        reviewlist.processReviews();
                        cout << "\nDo you want to analyze reviews again? (Yes = 1 / No = 0): ";
                        cin >> linkedListChoice;
                    } while (linkedListChoice == 1);
                }
        
                else if (linkedListChoice == 5) {
                    do {
                        transactionlist.measureSortingPerformance();
                        cout << "\nDo you want to compare sorting performance again? (Yes = 1 / No = 0): ";
                        cin >> linkedListChoice;
                    } while (linkedListChoice == 1);
                }
        
                else if (linkedListChoice == 6) {
                    cout << "Returning to main menu...\n";
                    break;
                }
            }
        }
        // Performance comparison between arrays and linked lists 
        else if (choice == 3) {
            int performanceChoice = 0;
            while (true) {
                displayPerformanceMenu();
                cin >> performanceChoice;
                
                if (performanceChoice == 1) {
                    cout << "Comparing sorting performance of Arrays vs Linked Lists...\n";
                    cout << "\nArray-based implementation:\n";
                    measureSortingPerformance(transactions, transactionCount);
                    
                    cout << "\nLinked List-based implementation:\n";
                    // Create a linked list for comparison
                    TransactionList transactionlist;
                    transactionlist.loadFromCSV(transactionFile);
                    transactionlist.measureSortingPerformance();
                    
                    cout << "\nPress Enter to continue...";
                    cin.ignore();
                    cin.get();
                }
                else if (performanceChoice == 2) {
                    char customerID[MAX_STRING_LENGTH];
                    cout << "Enter Customer ID to search: ";
                    cin >> customerID;
                    
                    cout << "\nComparing search algorithms performance:\n";
                    
                    cout << "\nArray-based implementation:\n";
                    compareSearchAlgorithms(transactions, transactionCount, customerID);
                    
                    cout << "\nLinked List-based implementation:\n";
                    // Create a linked list for comparison
                    TransactionList transactionlist;
                    transactionlist.loadFromCSV(transactionFile);
                    transactionlist.compareSearchAlgorithms(customerID);
                    
                    cout << "\nPress Enter to continue...";
                    cin.ignore();
                    cin.get();
                }
                else if (performanceChoice == 3) {
                    cout << "Returning to main menu...\n";
                    break;
                }
                else {
                    cout << "Invalid choice. Please enter a number between 1 and 3.\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            }
        }
        else if (choice == 4) {
            cout << "Exiting Program...\n";
            break;
        }
    }
    return 0;
}