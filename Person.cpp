#include "LibraryException.h"
#include "Person.h"
using namespace std;
Person::Person() : lastName(""), firstName(""), middleName("") {}
Person::Person(const string& ln, const string& fn, const string& mn)
	: lastName(ln), firstName(fn), middleName(mn) {
	if (ln.empty() || fn.empty()) {
		throw LibraryException("Фамилия и имя не могут быть пустыми");
	}
}
string Person::getFullName() const {
	string result = lastName + " " + firstName;
	if (!middleName.empty()) result += " " + middleName;
	return result;
}

Reader::Reader()
	: Person(), id(0), ticketNumber(""), registrationDate(), activeLoansCount(0) {
}
Reader::Reader(int id, const string& ln, const string& fn, const string& mn,
	const string& ticket, const Date& regDate)
	: Person(ln, fn, mn), id(id), ticketNumber(ticket),
	registrationDate(regDate), activeLoansCount(0) {
	if (ticket.empty()) {
		throw LibraryException("Номер читательского билета не может быть пустым");
	}
}
void Reader::incrementLoans() {
	if (activeLoansCount >= MAX_ACTIVE_LOANS)
		throw LibraryException("Превышен лимит одновременно выданных книг");
	++activeLoansCount;
}
void Reader::decrementLoans() {
	if (activeLoansCount > 0) --activeLoansCount;
}
void Reader::writeTo(ofstream& out) const {
	out << id << "\n";
	out << lastName << "\n";
	out << firstName << "\n";
	out << (middleName.empty() ? "-" : middleName) << "\n";
	out << ticketNumber << "\n";
	registrationDate.writeTo(out);
	out << "\n";
	out << activeLoansCount << "\n";
}
void Reader::readFrom(ifstream& in) {
	in >> id;
	in.get();
	getline(in, lastName);
	getline(in, firstName);
	getline(in, middleName);
	if (middleName == "-") middleName = "";
	getline(in, ticketNumber);
	registrationDate.readFrom(in);
	in >> activeLoansCount;
	in.get();
}
