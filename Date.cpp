#define _CRT_SECURE_NO_WARNINGS
#include "Date.h"
#include "LibraryException.h"
#include <ctime>

using namespace std;
Date::Date() : year(1900), month(1), day(1) {}
Date::Date(int y, int m, int d) : year(y), month(m), day(d) {
	if (!isValid()) {
		throw LibraryException("Некорректная дата");
	}
}
bool Date::isLeap(int y) {
	if (y % 400 == 0) return true;
	if (y % 100 == 0) return false;
	return y % 4 == 0;
}
int Date::daysInMonth(int y, int m) {
	int days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	if (m < 1 || m > 12) return 0;
	if (m == 2 && isLeap(y)) return 29;
	return days[m - 1];
}
bool Date::isValid() const {
	if (year < 1900 || year > 2100) return false;
	if (month < 1 || month > 12) return false;
	if (day < 1 || day > daysInMonth(year, month)) return false;
	return true;
}
int Date::daysSince1900() const {
	int total = 0;
	for (int y = 1900; y < year; ++y) {
		total += isLeap(y) ? 366 : 365;
	}
	for (int m = 1; m < month; ++m) {
		total += daysInMonth(year, m);
	}
	total += day - 1;
	return total;
}
Date Date::today() {
	time_t t = time(nullptr);
	tm* lt = localtime(&t);
	return Date(lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday);
}
Date Date::addDays(int delta) const {
	int total = daysSince1900() + delta;
	if (total < 0) {
		throw LibraryException("Дата вышла за допустимый диапазон");
	}
	int y = 1900;
	while (true) {
		int yd = isLeap(y) ? 366 : 365;
		if (total < yd) break;
		total -= yd;
		++y;
	}
	int m = 1;
	while (total >= daysInMonth(y, m)) {
		total -= daysInMonth(y, m);
		++m;
	}
	return Date(y, m, total + 1);
}
int Date::daysUntil(const Date& other) const {
	return other.daysSince1900() - daysSince1900();
}
void Date::writeTo(ofstream& out) const {
	out << year << " " << month << " " << day;
}
void Date::readFrom(ifstream& in) {
	in >> year >> month >> day;
}
void Date::print(ostream& out) const {
	out << year << "-";
	if (month < 10) out << "0";
	out << month << "-";
	if (day < 10) out << "0";
	out << day;
}
bool Date::operator==(const Date& o) const {
	return year == o.year && month == o.month && day == o.day;
}
bool Date::operator<(const Date& o) const {
	if (year != o.year)  return year < o.year;
	if (month != o.month) return month < o.month;
	return day < o.day;
}
bool Date::operator>(const Date& o) const {
	return o < *this;
}
