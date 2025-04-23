#include <iostream>
#include <fstream>

using namespace std;
// Linked-List Implementation

// Transaction node structure for linked list
struct NodeTransaction 
{
    Transaction data;
    Node* next;
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