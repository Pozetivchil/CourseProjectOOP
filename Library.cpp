#include "Library.h"
#include "LibraryException.h"
#include <fstream>
using namespace std;
Library::Library() : nextItemId(1), nextReaderId(1), nextLoanId(1) {}

Library::~Library() {
	clearItems();
}
void Library::clearItems() {
	for (auto p : items) delete p;
	items.clear();
}

void Library::addBook(const string& title, const string& author, int year, int copies) {
	Book* b = new Book(nextItemId, title, author, year, copies);
	items.push_back(b);
	++nextItemId;
}
void Library::addJournal(const string& title, int year, int copies,
	int issue, const string& periodicity) {
	Journal* j = new Journal(nextItemId, title, year, copies, issue, periodicity);
	items.push_back(j);
	++nextItemId;
}
bool Library::removeItem(int id) {
	for (auto it = items.begin(); it != items.end(); ++it) {
		if ((*it)->getId() == id) {
			if ((*it)->getAvailableCopies() < (*it)->getTotalCopies()) {
				throw LibraryException("Нельзя удалить: есть невозвращённые экземпляры");
			}
			delete* it;
			items.erase(it);
			return true;
		}
	}
	return false;
}
LibraryItem* Library::findItem(int id) {
	for (auto p : items) if (p->getId() == id) return p;
	return nullptr;
}
const LibraryItem* Library::findItem(int id) const {
	for (auto p : items) if (p->getId() == id) return p;
	return nullptr;
}

void Library::registerReader(const string& ln, const string& fn,
	const string& mn, const string& ticket) {
	for (const auto& r : readers) {
		if (r.getTicketNumber() == ticket) {
			throw LibraryException("Читатель с таким билетом уже существует");
		}
	}
	Reader r(nextReaderId, ln, fn, mn, ticket, Date::today());
	readers.push_back(r);
	++nextReaderId;
}
bool Library::removeReader(int id) {
	for (auto it = readers.begin(); it != readers.end(); ++it) {
		if (it->getId() == id) {
			if (it->getActiveLoansCount() > 0) {
				throw LibraryException("Нельзя удалить читателя с активными выдачами");
			}
			readers.erase(it);
			return true;
		}
	}
	return false;
}
Reader* Library::findReader(int id) {
	for (auto& r : readers) if (r.getId() == id) return &r;
	return nullptr;
}
const Reader* Library::findReader(int id) const {
	for (const auto& r : readers) if (r.getId() == id) return &r;
	return nullptr;
}
Reader* Library::findReaderByTicket(const string& ticket) {
	for (auto& r : readers) if (r.getTicketNumber() == ticket) return &r;
	return nullptr;
}

int Library::issueLoan(int itemId, const string& ticket, const Date& today) {
	LibraryItem* item = findItem(itemId);
	Reader* r = findReaderByTicket(ticket);
	if (item == nullptr) throw LibraryException("Издание с указанным id не найдено");
	if (r == nullptr) throw LibraryException("Читатель с указанным билетом не найден");
	item->issueOne();
	try {
		r->incrementLoans();
	}
	catch (const LibraryException&) {
		item->returnOne();
		throw;
	}
	Date due = today.addDays(Loan::LOAN_PERIOD_DAYS);
	Loan loan(nextLoanId, itemId, r->getId(), today, due);
	loans.push_back(loan);
	int newId = nextLoanId;
	++nextLoanId;
	return newId;
}
bool Library::returnLoan(int loanId, const Date& today) {
	for (auto& loan : loans) {
		if (loan.getId() == loanId) {
			if (loan.getStatus() == LoanStatus::Returned) return false;
			LibraryItem* item = findItem(loan.getItemId());
			Reader* r = findReader(loan.getReaderId());
			if (item != nullptr) item->returnOne();
			if (r != nullptr) r->decrementLoans();
			loan.close(today);
			return true;
		}
	}
	return false;
}

vector<const LibraryItem*> Library::searchByTitle(const string& q) const {
	vector<const LibraryItem*> result;
	for (auto p : items) {
		if (p->getTitle().find(q) != string::npos) result.push_back(p);
	}
	return result;
}
vector<const Book*> Library::searchByAuthor(const string& q) const {
	vector<const Book*> result;
	for (auto p : items) {
		const Book* b = dynamic_cast<const Book*>(p);
		if (b != nullptr && b->getAuthor().find(q) != string::npos) {
			result.push_back(b);
		}
	}
	return result;
}
vector<const LibraryItem*> Library::searchByYear(int year) const {
	vector<const LibraryItem*> result;
	for (auto p : items) if (p->getYear() == year) result.push_back(p);
	return result;
}

vector<const Loan*> Library::activeLoans() const {
	vector<const Loan*> result;
	for (const auto& l : loans) {
		if (l.getStatus() == LoanStatus::Active) result.push_back(&l);
	}
	return result;
}
vector<const Loan*> Library::overdueLoans(const Date& today) const {
	vector<const Loan*> result;
	for (const auto& l : loans) {
		if (l.isOverdue(today)) result.push_back(&l);
	}
	return result;
}

void Library::saveAll() const {
	ofstream fb("books.txt");
	int booksCount = 0;
	for (auto p : items) if (dynamic_cast<const Book*>(p)) ++booksCount;
	fb << booksCount << "\n";
	for (auto p : items) {
		const Book* b = dynamic_cast<const Book*>(p);
		if (b != nullptr) b->writeTo(fb);
	}
	ofstream fj("journals.txt");
	int journalsCount = 0;
	for (auto p : items) if (dynamic_cast<const Journal*>(p)) ++journalsCount;
	fj << journalsCount << "\n";
	for (auto p : items) {
		const Journal* j = dynamic_cast<const Journal*>(p);
		if (j != nullptr) j->writeTo(fj);
	}
	ofstream fr("readers.txt");
	fr << readers.size() << "\n";
	for (const auto& r : readers) r.writeTo(fr);
	ofstream fl("loans.txt");
	fl << loans.size() << "\n";
	for (const auto& l : loans) l.writeTo(fl);
	ofstream fc("counters.txt");
	fc << nextItemId << " " << nextReaderId << " " << nextLoanId << "\n";
}
void Library::loadAll() {
	clearItems();
	readers.clear();
	loans.clear();
	ifstream fb("books.txt");
	if (fb) {
		int n; fb >> n; fb.get();
		for (int i = 0; i < n; ++i) {
			Book* b = new Book();
			b->readFrom(fb);
			items.push_back(b);
		}
	}
	ifstream fj("journals.txt");
	if (fj) {
		int n; fj >> n; fj.get();
		for (int i = 0; i < n; ++i) {
			Journal* j = new Journal();
			j->readFrom(fj);
			items.push_back(j);
		}
	}
	ifstream fr("readers.txt");
	if (fr) {
		int n; fr >> n; fr.get();
		for (int i = 0; i < n; ++i) {
			Reader r;
			r.readFrom(fr);
			readers.push_back(r);
		}
	}
	ifstream fl("loans.txt");
	if (fl) {
		int n; fl >> n; fl.get();
		for (int i = 0; i < n; ++i) {
			Loan l;
			l.readFrom(fl);
			loans.push_back(l);
		}
	}
	ifstream fc("counters.txt");
	if (fc) {
		fc >> nextItemId >> nextReaderId >> nextLoanId;
	}
}
