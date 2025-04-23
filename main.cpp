#include "array.hpp"
#include <iostream>

using namespace std;

// Function to display the main menu
void displayMainMenu() {
    cout << "Choose Data Structure:\n";
    cout << "1. Use Array\n";
    cout << "2. Use Linked List\n";
    cout << "3. Compare Performance\n";
    cout << "4. Exit\n";
    cout << "Enter your choice: ";
}

void displayArrayMenu() {
    cout << "Array Operations:\n";
    cout << "1. Search by Customer ID\n";
    cout << "2. Filter Transactions\n";
    cout << "3. Sort Transactions by Date\n";
    cout << "4. Analyze Reviews\n";
    cout << "5. Compare Sorting Performance\n";
    cout << "6. Return to Main Menu\n";
    cout << "Enter your choice: ";
}

int main() {
    Transaction transactions[MAX_TRANSACTIONS];
    CustomerReview reviews[MAX_REVIEWS];
    int transactionCount = 0, reviewCount = 0;

    // Load transaction data from CSV
    readcsv("/Users/amiryusof/Documents/Data Structures/Assignment/implementation/transactions_cleaned.csv", transactions, transactionCount);

    // Load customer reviews from CSV
    readReviews("/Users/amiryusof/Documents/Data Structures/Assignment/implementation/reviews_cleaned.csv", reviews, reviewCount);

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
            cout << "Linked list functionality is not implemented yet.\n";
            // Here, you'd later integrate linked list operations.
        } 
        
        else if (choice == 3) {
            cout << "Comparing performance of Arrays vs Linked Lists...\n";
            measureSortingPerformance(transactions, transactionCount);
            cout << "Linked List performance comparison is not implemented yet.\n";
        } 
        
        else if (choice == 4) {
            cout << "Exiting Program...\n";
            break;
        }
    }

    return 0;
}
