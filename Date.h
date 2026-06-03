#pragma once
#include <fstream>
#include <iostream>

class Date {
private:
	int year;
	int month;
	int day;
	static bool isLeap(int y);
	static int daysInMonth(int y, int m);
	int daysSince1900() const;
public:
	Date();
	Date(int y, int m, int d);
	int getYear()  const { return year; }
	int getMonth() const { return month; }
	int getDay()   const { return day; }
	static Date today();
	Date addDays(int delta) const;
	int  daysUntil(const Date& other) const;
	bool isValid() const;
	void writeTo(std::ofstream& out) const;
	void readFrom(std::ifstream& in);
	void print(std::ostream& out) const;
	bool operator==(const Date& other) const;
	bool operator< (const Date& other) const;
	bool operator> (const Date& other) const;
};
