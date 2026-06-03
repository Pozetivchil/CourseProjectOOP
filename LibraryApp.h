#pragma once
#include "Library.h"
#include <string>

class LibraryApp {
private:
	Library library;
	bool running;
	Date currentDate;

	void menuBooks();
	void menuReaders();
	void menuLoans();
	void menuSearch();
	void menuReports();
	void doSave();
	void doExit();
	void printAllItems() const;
	void printAllReaders() const;
	void printLoan(const Loan& l) const;

	static std::string readLine(const std::string& prompt);
	static std::string readNonEmpty(const std::string& prompt);
	static int         readInt(const std::string& prompt);
	static int         readPositiveInt(const std::string& prompt);
	static bool        isBlank(const std::string& s);
public:
	LibraryApp();
	void run();
};
