#include "LibraryException.h"
#include "Loan.h"
using namespace std;
Loan::Loan()
	: id(0), itemId(0), readerId(0),
	issueDate(), dueDate(), returnDate(), status(LoanStatus::Active) {
}
Loan::Loan(int id, int itemId, int readerId, const Date& issueDate, const Date& dueDate)
	: id(id), itemId(itemId), readerId(readerId),
	issueDate(issueDate), dueDate(dueDate), returnDate(),
	status(LoanStatus::Active) {
}
void Loan::close(const Date& today) {
	if (status == LoanStatus::Returned) {
		throw LibraryException("Запись о выдаче уже закрыта");
	}
	returnDate = today;
	status = LoanStatus::Returned;
}
bool Loan::isOverdue(const Date& today) const {
	if (status == LoanStatus::Returned) return false;
	return today > dueDate;
}
int Loan::overdueDays(const Date& today) const {
	if (!isOverdue(today)) return 0;
	return dueDate.daysUntil(today);
}
void Loan::writeTo(ofstream& out) const {
	out << id << " " << itemId << " " << readerId << "\n";
	issueDate.writeTo(out); out << "\n";
	dueDate.writeTo(out);   out << "\n";
	returnDate.writeTo(out); out << "\n";
	out << (status == LoanStatus::Active ? 0 : 1) << "\n";
}
void Loan::readFrom(ifstream& in) {
	in >> id >> itemId >> readerId;
	issueDate.readFrom(in);
	dueDate.readFrom(in);
	returnDate.readFrom(in);
	int s;
	in >> s;
	status = (s == 0) ? LoanStatus::Active : LoanStatus::Returned;
	in.get();
}
