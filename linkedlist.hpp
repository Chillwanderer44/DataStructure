#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

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


#endif