#ifndef HELPER_HPP
#define HELPER_HPP

#include <iostream>
#include "array.hpp"

//Helper functions declarations
Date parseDate(const char* dateStr);
int compareDates(const char* date1, const char* date2);
bool isDateInRange(const char* transactionDate, const char* startDate, const char* endDate);
void cleanWord(std::string& word);
void displayLinkedListMenu();

#endif