#pragma once
#include "Date.h"
#include <fstream>
enum LoanStatus { Active, Returned };

class Loan {
private:
	int id;
	int itemId;
	int readerId;
	Date issueDate;
	Date dueDate;
	Date returnDate;
	LoanStatus status;
public:
	static const int LOAN_PERIOD_DAYS = 14;
	Loan();
	Loan(int id, int itemId, int readerId, const Date& issueDate, const Date& dueDate);
	int getId()       const { return id; }
	int getItemId()   const { return itemId; }
	int getReaderId() const { return readerId; }
	const Date& getIssueDate()  const { return issueDate; }
	const Date& getDueDate()    const { return dueDate; }
	const Date& getReturnDate() const { return returnDate; }
	LoanStatus  getStatus()     const { return status; }
	void close(const Date& today);
	bool isOverdue(const Date& today) const;
	int  overdueDays(const Date& today) const;
	void writeTo(std::ofstream& out) const;
	void readFrom(std::ifstream& in);
};
