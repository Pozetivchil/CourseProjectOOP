#pragma once
#include "Date.h"
#include "LibraryItem.h"
#include "Loan.h"
#include "Person.h"
#include <string>
#include <vector>

class Library {
private:
	std::vector<LibraryItem*> items;
	std::vector<Reader>       readers;
	std::vector<Loan>         loans;
	int nextItemId;
	int nextReaderId;
	int nextLoanId;
public:
	Library();
	~Library();

	Library(const Library&);
	Library& operator=(const Library&);

	void addBook(const std::string& title, const std::string& author,
		int year, int copies);
	void addJournal(const std::string& title, int year, int copies,
		int issue, const std::string& periodicity);
	bool removeItem(int id);

	LibraryItem* findItem(int id);
	const LibraryItem* findItem(int id) const;

	void registerReader(const std::string& ln, const std::string& fn,
		const std::string& mn, const std::string& ticket);
	bool removeReader(int id);
	Reader* findReader(int id);
	const Reader* findReader(int id) const;
	Reader* findReaderByTicket(const std::string& ticket);

	int  issueLoan(int itemId, const std::string& ticket, const Date& today);
	bool returnLoan(int loanId, const Date& today);

	std::vector<const LibraryItem*> searchByTitle(const std::string& q) const;
	std::vector<const Book*>        searchByAuthor(const std::string& q) const;
	std::vector<const LibraryItem*> searchByYear(int year) const;

	std::vector<const Loan*> activeLoans() const;
	std::vector<const Loan*> overdueLoans(const Date& today) const;

	const std::vector<LibraryItem*>& getItems()   const { return items; }
	const std::vector<Reader>& getReaders() const { return readers; }
	const std::vector<Loan>& getLoans()   const { return loans; }

	void saveAll() const;
	void loadAll();
private:
	void clearItems();
};
