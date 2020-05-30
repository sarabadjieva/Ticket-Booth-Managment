#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <iterator>
#include <ctime>

const int DATE_LENGTH = 11;

const char UNKNOWN[] = "Unknown";
const char DELIMITER = ' ';
const char DELIMITER2 = '-';
const char DELIMITER3 = '.';
const char DELIMITER4 = '$';
const char DELIMITER5 = '|';
const char DELIMITER_ = '_';


class Date
{
private:
	size_t year;
	size_t month;
	size_t day;
	bool validDate;
public:

	//the default values are 1, because I set the date to today's date if no parameters are given
	//and they are not zero because of the second if-function, which would set validDate to false
	Date(size_t y = 1, size_t m = 1, size_t d = 1);
	Date(const Date&);
	Date& operator=(const Date&);
	~Date() {};

	const size_t getYear() const { return this->year; }
	const size_t getMonth() const { return this->month; }
	const size_t getDay() const { return this->day; }
	bool isValidDate() const { return this->validDate; };

	void setYear(const size_t);
	void setMonth(const size_t);
	void setDay(const size_t);

	size_t daysInMonth(size_t, size_t) const;
	Date& nextDate() const;

	bool operator==(const Date&) const;
	bool operator<(const Date&) const;
	bool operator>(const Date&) const;

	friend std::ostream& operator<<(std::ostream&, const Date&);
	friend std::istream& operator>>(std::istream&, Date&);
	friend std::vector<Date> getDatesBetween(const Date&, const Date&);
};
