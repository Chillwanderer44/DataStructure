#include "helpers.hpp"
#include <cstring>
#include <iostream>
#include <sstream>

using namespace std;

// Helper function to parse data
Date parseDate(const char* dateStr) {
    Date date;
    sscanf(dateStr, "%d/%d/%d", &date.day, &date.month, &date.year);
    return date;
}

// Helper function to compare two dates
int compareDates(const char* date1, const char* date2) {
    Date d1 = parseDate(date1);
    Date d2 = parseDate(date2);

    //compare first year, then month, then day
    if(d1.year != d2.year) return d1.year - d2.year;
    if(d1.month != d2.month) return d1.month - d2.month;
    return d1.day - d2.day;
}

// Helper function check if date is within a range
bool isDateInRange(const char* transactionDate, const char* startDate, const char* endDate) {
    if(strcmp(startDate, "-") == 0 || strcmp(endDate, "-") == 0) return true;

    //check if date is between start and end dates
    return compareDates(transactionDate, startDate) >= 0 && 
           compareDates(transactionDate, endDate) <= 0;
}

// Clean a word (remove punctuation, convert to lowercase)
void cleanWord(string& word) {
    // Convert to lowercase
    for (int i = 0; i < word.length(); i++) {
        if (isupper(word[i])) {
            word[i] = tolower(word[i]);
        }
    }
    
    // Remove punctuation
    string result;
    for (int i = 0; i < word.length(); i++) {
        if (!ispunct(word[i])) {
            result += word[i];
        }
    }
    word = result;
}
