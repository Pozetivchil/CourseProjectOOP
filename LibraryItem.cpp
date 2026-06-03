#include "LibraryException.h"
#include "LibraryItem.h"

using namespace std;

LibraryItem::LibraryItem()
	: id(0), title(""), year(0), totalCopies(0), availableCopies(0) {
}
LibraryItem::LibraryItem(int id, const string& title, int year, int copies)
	: id(id), title(title), year(year), totalCopies(copies), availableCopies(copies) {
	if (title.empty()) {
		throw LibraryException("Название не может быть пустым");
	}
	if (copies < 0) {
		throw LibraryException("Количество экземпляров не может быть отрицательным");
	}
}
void LibraryItem::issueOne() {
	if (availableCopies <= 0) {
		throw LibraryException("Нет доступных экземпляров для выдачи");
	}
	--availableCopies;
}
void LibraryItem::returnOne() {
	if (availableCopies >= totalCopies) {
		throw LibraryException("Все экземпляры уже находятся в фонде");
	}
	++availableCopies;
}

Book::Book() : LibraryItem(), author("") {}
Book::Book(int id, const string& title, const string& author, int year, int copies)
	: LibraryItem(id, title, year, copies), author(author) {
	if (author.empty()) {
		throw LibraryException("Автор книги не может быть пустым");
	}
}
void Book::display() const {
	cout << "[Книга] #" << id << "  " << title
		<< " / " << author
		<< "  (" << year << ")"
		<< "  доступно: " << availableCopies << " из " << totalCopies << "\n";
}
void Book::writeTo(ofstream& out) const {
	out << id << "\n";
	out << title << "\n";
	out << author << "\n";
	out << year << " " << totalCopies << " " << availableCopies << "\n";
}
void Book::readFrom(ifstream& in) {
	in >> id;
	in.get();
	getline(in, title);
	getline(in, author);
	in >> year >> totalCopies >> availableCopies;
	in.get();
}

Journal::Journal() : LibraryItem(), issueNumber(0), periodicity("") {}
Journal::Journal(int id, const string& title, int year, int copies,
	int issue, const string& periodicity)
	: LibraryItem(id, title, year, copies),
	issueNumber(issue), periodicity(periodicity) {
}
void Journal::display() const {
	cout << "[Журнал] #" << id << "  " << title
		<< "  №" << issueNumber
		<< "  (" << year << ", " << periodicity << ")"
		<< "  доступно: " << availableCopies << " из " << totalCopies << "\n";
}
void Journal::writeTo(ofstream& out) const {
	out << id << "\n";
	out << title << "\n";
	out << year << " " << totalCopies << " " << availableCopies << "\n";
	out << issueNumber << "\n";
	out << (periodicity.empty() ? "-" : periodicity) << "\n";
}
void Journal::readFrom(ifstream& in) {
	in >> id;
	in.get();
	getline(in, title);
	in >> year >> totalCopies >> availableCopies;
	in.get();
	in >> issueNumber;
	in.get();
	getline(in, periodicity);
	if (periodicity == "-") periodicity = "";
}
